#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

const char *device_path = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";

#define HISTORY_SIZE 4

struct input_event_history {
    struct input_event events[HISTORY_SIZE];
    int index;
};

void print_custom_message(const char *message) {
    printf("Shortcut: %s\n", message);
}

int main() {
    // Open the keyboard device with sudo permissions
    int fd = open(device_path, O_RDONLY);

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

    // Initialize the input event history
    struct input_event_history history = {0};

    // Main event handling loop
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
            if (ev.value == 1||ev.value == 2 || ev.value == 0) {
                // Print and save the output events in the specified format
                fprintf(output_file, "%s 0x%04X (%d)\n",
                        (ev.value == 1) ? "PRESSED" : ((ev.value == 2) ? "REPEATED" : "RELEASED"),
                        ev.code, ev.code);
                fflush(output_file);

                // Print to stdout as well
                printf("%s 0x%04X (%d)\n",
                       (ev.value == 1) ? "PRESSED" : ((ev.value == 2) ? "REPEATED" : "RELEASED"),
                       ev.code, ev.code);

                // Update the input event history
                history.events[history.index] = ev;
                history.index = (history.index + 1) % HISTORY_SIZE;

                // Check for termination condition ("E+X" sequence)
                if (history.events[0].code == KEY_E && history.events[1].code == KEY_X &&
                    history.events[0].value == 1 && history.events[1].value == 1) {
                    break; // Terminate the loop on "E+X"
                }

                // Check for specific shortcuts
                if (history.events[0].code == KEY_P && history.events[1].code == KEY_E &&
                    history.events[0].value == 1 && history.events[1].value == 1 &&
                    history.events[2].code == KEY_P && history.events[2].value == 0) {
                    print_custom_message("I passed the Exam!");
                    // Reset the history
                    history.index = 0;
                }

                if (history.events[0].code == KEY_C && history.events[1].code == KEY_A &&
                    history.events[2].code == KEY_P && history.events[3].code == KEY_P &&
                    history.events[0].value == 1 && history.events[1].value == 1 &&
                    history.events[2].value == 1 && history.events[3].value == 0) {
                    print_custom_message("Get some cappuccino!");
                    // Reset the history
                    history.index = 0;
                }// Check for custom shortcut
                if (history.events[0].code == KEY_LEFTCTRL &&
                    history.events[1].code == KEY_LEFTALT &&
                    history.events[2].code == KEY_LEFTSHIFT &&
                    history.events[3].code == KEY_F12 &&
                    history.events[0].value == 1 &&
                    history.events[1].value == 1 &&
                    history.events[2].value == 1 &&
                    history.events[3].value == 0) {
                    print_custom_message("Custom shortcut activated: Ctrl+Alt+Shift+F12");
                    // Reset the history
                    history.index = 0;
                }
            }
        }
    }

    // Close the keyboard device and output file
    close(fd);
    fclose(output_file);

    return EXIT_SUCCESS;
}