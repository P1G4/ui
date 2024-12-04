#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STATES 50


typedef enum {
    OPEN,
    CLOSE
} WindowState;

typedef enum {
    ON,
    OFF
} lightsState;

typedef struct {
    WindowState window;
    lightsState lights;
} States;


FILE *f1;


char *getText(States states);

void sendEmail(States *states);

void writeFile(States *states);

States *getData(States *states);


int main(void) {
        States states[MAX_STATES] = {0}; // Initialize array

        if (getData(states) == NULL) {
            fprintf(stderr, "Error: Unable to read data.\n");
            return 1;
        }

        FILE *f1 = fopen("data.txt", "w");
        if (!f1) {
            perror("Error opening file for writing");
            return 1;
        }

        writeFile(&states[0]);
        sendEmail(states);

        fclose(f1);
        return 0;
}

void writeFile(States *states) {
    char *text = getText(*states);

    time_t t = time(NULL);
    char *time_str = ctime(&t);
    time_str[strlen(time_str) - 1] = '\0';


    fprintf(f1, "%s: %s\n", time_str, text);
}


void sendEmail(States *states) {

    const char *recipients = "burhennemalte1@gmail.com";

    char command[1024];
    snprintf(command, sizeof(command),
             "echo \"%s\" | ssmtp -v %s", getText(*states), recipients);


    printf("Executing: %s\n", command);

    int result = system(command);

    if (result == 0) {
        printf("Command executed successfully.\n");
    } else {
        printf("Command execution failed with code: %d\n", result);
    }
}

char *getText(States states) {

    if (states.window == OPEN && states.lights == ON) {
        return "The window is open and the lights are on.";

    } else if (states.window == OPEN && states.lights == OFF) {
        return "The window is open and the lights are off.";

    } else if (states.window == CLOSE && states.lights == ON) {
        return "The window is closed and the lights are on.";

    } else if (states.window == CLOSE && states.lights == OFF) {
        return "The window is closed and the lights are off.";
    }

    return "Unknown state.";
}

States *getData(States *states) {
    States state;
    int index = 0;

    // Open the file
    FILE *f2 = fopen("input.txt", "r");
    if (f2 == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Allocate buffers for strings
    char window[20], lights[20]; // Adjust buffer size if needed

    // Read data from the file
    while (fscanf(f2, "%*[^ ] %19s %19s", window, lights) == 2) { // Limit input size
        printf("Window: %s, Lights: %s\n", window, lights);

        // Process window state
        if (strcmp(window, "open") == 0) {
            state.window = OPEN;
        } else if (strcmp(window, "closed") == 0) {
            state.window = CLOSE;
        }

        // Process light state
        if (strcmp(lights, "on") == 0) {
            state.lights = ON;
        } else if (strcmp(lights, "off") == 0) {
            state.lights = OFF;
        }

        // Store the state in the array
        if (index < MAX_STATES) { // Prevent overflow
            states[index] = state;
            index++;
        } else {
            fprintf(stderr, "Error: Exceeded maximum states array size.\n");
            break;
        }
    }

    fclose(f2);

    return states;
}


