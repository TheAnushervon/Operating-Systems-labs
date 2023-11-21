#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

struct inode
{
    char name[16];
    int size;
    int blockPointers[8];
    int used;
};

// Global variables
struct inode inodes[16];
char freeBlockList[128];

void readFreeBlockList(int fd)
{
    lseek(fd, 0, SEEK_SET);
    read(fd, freeBlockList, sizeof(freeBlockList));
}

void writeFreeBlockList(int fd)
{
    lseek(fd, 0, SEEK_SET);
    write(fd, freeBlockList, sizeof(freeBlockList));
}

void readInode(int fd, int index)
{
    lseek(fd, 128 + index * sizeof(struct inode), SEEK_SET);
    read(fd, &inodes[index], sizeof(struct inode));
}

void writeInode(int fd, int index)
{
    lseek(fd, 128 + index * sizeof(struct inode), SEEK_SET);
    write(fd, &inodes[index], sizeof(struct inode));
}

int findFreeInode()
{
    for (int i = 0; i < 16; i++)
    {
        if (inodes[i].used == 0)
        {
            return i;
        }
    }
    return -1; // No free inode found
}

int findFreeBlock()
{
    for (int i = 1; i < 128; i++)
    {
        if (freeBlockList[i] == 0)
        {
            return i;
        }
    }
    return -1; // No free block found
}

int create(char name[16], int size, int fd)
{
    // Step 1: Check free space
    readFreeBlockList(fd);
    int freeBlocks = 0;
    for (int i = 1; i < 128; i++)
    {
        if (freeBlockList[i] == 0)
        {
            freeBlocks++;
        }
    }

    if (freeBlocks < size)
    {
        printf("Error: Not enough free space on disk.\n");
        return -1;
    }

    // Step 2: Find a free inode
    int inodeIndex = findFreeInode();
    if (inodeIndex == -1)
    {
        printf("Error: No free inode available.\n");
        return -1;
    }

    // Set inode values
    strncpy(inodes[inodeIndex].name, name, 16);
    inodes[inodeIndex].size = size;
    inodes[inodeIndex].used = 1;

    // Step 3: Allocate data blocks
    for (int i = 0; i < size; i++)
    {
        int freeBlock = findFreeBlock();
        if (freeBlock == -1)
        {
            printf("Error: No free block available.\n");
            return -1;
        }
        inodes[inodeIndex].blockPointers[i] = freeBlock;
        freeBlockList[freeBlock] = 1;
    }

    // Step 4: Write out the inode and free block list to disk
    writeInode(fd, inodeIndex);
    writeFreeBlockList(fd);

    return 0; // Success
}

int delete(char name[16], int fd)
{
    // Step 1: Locate the inode for this file
    for (int i = 0; i < 16; i++)
    {
        readInode(fd, i);
        if (inodes[i].used && strcmp(inodes[i].name, name) == 0)
        {
            // Step 2: Free blocks
            readFreeBlockList(fd);
            for (int j = 0; j < inodes[i].size; j++)
            {
                freeBlockList[inodes[i].blockPointers[j]] = 0;
            }
            writeFreeBlockList(fd);

            // Step 3: Mark inode as free
            inodes[i].used = 0;

            // Step 4: Write out the inode and free block list to disk
            writeInode(fd, i);
            return 0; // Success
        }
    }

    printf("Error: File not found.\n");
    return -1;
}

void ls(int fd)
{
    // Step 1: Read in each inode and print
    for (int i = 0; i < 16; i++)
    {
        readInode(fd, i);
        if (inodes[i].used)
        {
            printf("File Name: %s, Size: %d blocks\n", inodes[i].name, inodes[i].size);
        }
    }
}

void read_block(char name[16], int blockNum, char buf[1024], int fd)
{
    // Step 1: Locate the inode for this file
    for (int i = 0; i < 16; i++)
    {
        readInode(fd, i);
        if (inodes[i].used && strcmp(inodes[i].name, name) == 0)
        {
            // Step 2: Read the specified block
            if (blockNum < inodes[i].size)
            {
                int addr = inodes[i].blockPointers[blockNum];
                lseek(fd, addr * 1024, SEEK_SET);
                read(fd, buf, 1024);
                return; // Success
            }
            else
            {
                printf("Error: Invalid block number.\n");
                return;
            }
        }
    }

    printf("Error: File not found.\n");
}

void write_block(char name[16], int blockNum, char buf[1024], int fd)
{
    // Step 1: Locate the inode for this file
    for (int i = 0; i < 16; i++)
    {
        readInode(fd, i);
        if (inodes[i].used && strcmp(inodes[i].name, name) == 0)
        {
            // Step 2: Write to the specified block
            if (blockNum < inodes[i].size)
            {
                int addr = inodes[i].blockPointers[blockNum];
                lseek(fd, addr * 1024, SEEK_SET);
                write(fd, buf, 1024);
                return; // Success
            }
            else
            {
                printf("Error: Invalid block number.\n");
                return;
            }
        }
    }

    printf("Error: File not found.\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        exit(1);
    }

    char *inputFileName = argv[1];
    int diskFile = open("disk0", O_RDWR);

    if (diskFile == -1)
    {
        printf("Error: Could not open the disk file.\n");
        exit(1);
    }

    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL)
    {
        printf("Error: Could not open the input file.\n");
        exit(1);
    }

    char operation;
    char name[16];
    int size, blockNum;
    char buf[1024];

    while (fscanf(inputFile, " %c", &operation) != EOF)
    {
        switch (operation)
        {
        case 'C':
            fscanf(inputFile, " %s %d", name, &size);
            create(name, size, diskFile);
            break;
        case 'D':
            fscanf(inputFile, " %s", name);
            delete (name, diskFile);
            break;
        case 'L':
            ls(diskFile);
            break;
        case 'R':
            fscanf(inputFile, " %s %d", name, &blockNum);
            read_block(name, blockNum, buf, diskFile);
            break;
        case 'W':
            fscanf(inputFile, " %s %d", name, &blockNum);
            for (int i = 0; i < 1024; i++)
            {
                buf[i] = 'x'; 
            }
            write_block(name, blockNum, buf, diskFile);
            break;
        default:
            printf("Error: Invalid operation.\n");
            break;
        }
    }

    close(diskFile);
    fclose(inputFile);

    return 0;
}