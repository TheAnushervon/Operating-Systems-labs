#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#define HISTORY_SIZE 2 
const char *device_path = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
void check_input (signed int value){
    if (value == 1){
        printf("Pressed"); 
    }
    else if (value == 2){
        printf("REPEATED"); 
    }
    else printf("RELEASED"); 
}
struct input_event_history {
    struct input_event events[HISTORY_SIZE];
    int index;
};
int main() {
    // Open the keyboard device with sudo permissions
    
    int fd = open(device_path, O_RDONLY);
    char saver[3] = {'z', 'z', 'z'}; 
    if (fd == -1) {
        perror("Error opening keyboard device");
        return EXIT_FAILURE;
    }

    struct input_event ev;

    // Open the output file for writing
    FILE *output_file = fopen("ex1.txt", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        close(fd);
        return EXIT_FAILURE;
    }
    else printf("opened file"); 

    // Main event handling loop
    struct input_event_history history = {0};
    while (1) {
        // Read the keyboard event
        ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));

        if (bytesRead == -1) {
            perror("Error reading from keyboard device");
            break; // Terminate the loop on read error
        }

        // Check if it's an EV_KEY event (key press or release)
        if (ev.type == EV_KEY) {
            // Check for PRESSED, REPEATED, or RELEASED events
            if (ev.value == 1 || ev.value == 2 || ev.value == 0) {
                // Print and save the output events in the specified format
                fprintf(output_file, "%s 0x%04X (%d)\n",
                        (ev.value == 1) ? "PRESSED" : ((ev.value == 0) ? "RELEASED" : "REPEATED"),
                        ev.code, ev.code);
                fflush(output_file);

                // Print to stdout as well
                printf("%s 0x%04X (%d)\n",
                       (ev.value == 1) ? "PRESSED" : ((ev.value == 0) ?  "RELEASED":"REPEATED"),
                       ev.code, ev.code);
                       
            }
            history.events[history.index] = ev;
                history.index = (history.index + 1) % HISTORY_SIZE;

                // Check for termination condition ("E+X" sequence)
                if (history.events[0].code == KEY_E && history.events[1].code == KEY_X) {
                    break; // Terminate the loop on "E+X"
                }
        }

        // Check for termination condition (E+X)
        /*if (ev.type == EV_KEY && ev.value == 1 && ev.code == KEY_E) {
            // Read the next event
            bytesRead = read(fd, &ev, sizeof(struct input_event));

            // Check if the next event is 'X'
            if (bytesRead != -1 && ev.type == EV_KEY && ev.value == 1 && ev.code == KEY_X) {
                break; // Terminate the loop on 'E+X'
            }
        }*/
    }

    // Close the keyboard device and output file
    close(fd);
    fclose(output_file);

    return EXIT_SUCCESS;
}