#include <stdio.h>
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


char *getText(States states);

void sendEmail(States states);

States *getData(States *states);


int main(void) {
    States states[MAX_STATES] = {0}; // Initialize array

    if (getData(states) == NULL) {
        fprintf(stderr, "Error: Unable to read data.\n");
        return 1;
    }

    sendEmail(states[0]);

    return 0;
}

void sendEmail(States states) {
    char command[1024];
    const char *recipients = "maltetestemail@gmail.com";

    snprintf(command, sizeof(command),
             "echo \"%s\" | ssmtp -v %s", getText(states), recipients);


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

    FILE *f2 = fopen("input.txt", "r");
    if (f2 == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char window[20], lights[20];

    while (fscanf(f2, "%*[^ ] %19s %19s", window, lights) == 2) {
        printf("Window: %s, Lights: %s\n", window, lights);

        if (strcmp(window, "open") == 0) {
            state.window = OPEN;
        } else if (strcmp(window, "closed") == 0) {
            state.window = CLOSE;
        }

        if (strcmp(lights, "on") == 0) {
            state.lights = ON;
        } else if (strcmp(lights, "off") == 0) {
            state.lights = OFF;
        }

        if (index < MAX_STATES) {
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


