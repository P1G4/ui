#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STATES 100


typedef enum {
    OPEN,
    CLOSE
} WindowState;

typedef enum {
    ON,
    OFF
} LigthsState;

typedef struct {
    WindowState window;
    LigthsState ligths;
} States;


FILE *f1;


char *getText(States states);

void sendEmail(States *states);

void writeFile(States *states);

States *getData(States *states);


int main(void) {

    States states[MAX_STATES];

    getData(states);


    f1 = fopen("data.txt", "w");

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

    if (states.window == OPEN && states.ligths == ON) {
        return "The window is open and the lights are on.";

    } else if (states.window == OPEN && states.ligths == OFF) {
        return "The window is open and the lights are off.";

    } else if (states.window == CLOSE && states.ligths == ON) {
        return "The window is closed and the lights are on.";

    } else if (states.window == CLOSE && states.ligths == OFF) {
        return "The window is closed and the lights are off.";
    }

    return "Unknown state.";
}

States *getData(States *states) {
    States state;
    int index = 0;
    FILE *f2 = fopen("C:\\Users\\burhe\\CLionProjects\\p1\\ui\\input.txt", "r");
    char *window = NULL;
    char *ligths = NULL;
    while (fscanf(f2, "%*s %*s %s %s", window, ligths) == 1) {

        // Process window state
        if (strcmp(window, "open") == 0) {
            state.window = OPEN;
        } else if (strcmp(window, "closed") == 0) {
            state.window = CLOSE;
        }

        // Process light state
        if (strcmp(ligths, "on") == 0) {
            state.ligths = ON;
        } else if (strcmp(ligths, "off") == 0) {
            state.ligths = OFF;
        }

        printf("Window: %s, Ligths: %s\n", window, ligths);


        // Store the state in the array
        states[index] = state;
        index++;
    }


    fclose(f2);


    return states;
}

