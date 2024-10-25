#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define GetCurrentDir getcwd


typedef enum {
    OPEN,
    CLOSE
} WindowState;

typedef enum {
    ON,
    OFF
} LigthsState;


FILE *f;

char* getText(WindowState window, LigthsState lights);
void sendEmail(WindowState window, LigthsState ligths);
void writeFile(WindowState window, LigthsState ligths);



int main(void) {

    WindowState window = CLOSE;
    LigthsState ligths = OFF;

    

    f = fopen("data.txt", "w");
    writeFile(window, ligths);
    sendEmail(window, ligths);


    fclose(f);

    return 0;
}

void writeFile(WindowState window, LigthsState ligths) {
    char* text = getText(window, ligths);

    time_t t = time(NULL);
    char * time_str = ctime(&t);
    time_str[strlen(time_str)-1] = '\0';



    fprintf(f, "%s: %s\n", time_str, text);
}





void sendEmail(WindowState window, LigthsState ligths) {

    //const char *recipients = "burhennemalte1@gmail.com Axmed141102@gmail.com";
    const char *recipients = "burhennemalte1@gmail.com";

    char command[1024];
    snprintf(command, sizeof(command),
             "echo \"%s\" | ssmtp -v %s", getText(window, ligths), recipients);


    printf("Executing: %s\n", command);

    int result = system(command);

    if (result == 0) {
        printf("Command executed successfully.\n");
    } else {
        printf("Command execution failed with code: %d\n", result);
    }
}

char* getText(WindowState window, LigthsState lights) {
    if (window == OPEN && lights == ON) {
        return "The window is open and the lights are on.";

    } else if (window == OPEN && lights == OFF) {
        return "The window is open and the lights are off.";

    } else if (window == CLOSE && lights == ON) {
        return "The window is closed and the lights are on.";

    } else if (window == CLOSE && lights == OFF) {
        return "The window is closed and the lights are off.";
    }

    return "Unknown state.";
}
