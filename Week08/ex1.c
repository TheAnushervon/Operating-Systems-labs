#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>

char generatePrintableChar() {
    const char printableChars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+";
    const int charCount = strlen(printableChars);
    int randomIndex;

    int random_fd = open("/dev/urandom", O_RDONLY);
    if (random_fd == -1) {
        perror("Failed to open /dev/urandom");
        exit(1);
    }

    if (read(random_fd, &randomIndex, sizeof(randomIndex)) == -1) {
        perror("Failed to read from /dev/urandom");
        exit(1);
    }
    close(random_fd);

    return printableChars[randomIndex % charCount];
}

int main() {
    FILE *pid_file = fopen("/tmp/ex1.pid", "w");
    if (pid_file) {
        fprintf(pid_file, "%d", getpid());
        fclose(pid_file);
    }

    char password[9];  
    strcpy(password, "pass:");
    for (int i = 5; i < 9; i++) {
        password[i] = generatePrintableChar();
    }
    password[9] = '\0';

    char *password_ptr = (char *)mmap(NULL, sizeof(password), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (password_ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    strcpy(password_ptr, password);

    while (1) {
        sleep(1);
    }
}
