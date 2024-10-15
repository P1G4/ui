#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
    #include <direct.h>   // For _getcwd() on Windows
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>   // For getcwd() on UNIX-like systems
    #define GetCurrentDir getcwd
#endif


FILE *f;

typedef enum {
    OPEN,
    CLOSE
} WindowState;

typedef enum {
    ON,
    OFF
} LigthsState;

void Scribere(WindowState window, LigthsState ligths);

int main(void) {

    WindowState window = CLOSE;
    LigthsState ligths = OFF;

    char buffer[1024];
    if (GetCurrentDir(buffer, sizeof(buffer)) != NULL) {
        printf("Current working directory: %s\n", buffer);
    } else {
        perror("getcwd() error");
        return 1;
    }

    char *fileName = "data.txt";


    printf("Full path to the file: %s/%s\n", buffer, fileName);

    //f = fopen(strcat(buffer, fileName), "w");
    f = fopen("C:\\Users\\burhe\\CLionProjects\\p1\\ui\\data.txt", "w");
    Scribere(window, ligths);


    fclose(f);

    return 0;
}

void Scribere(WindowState window, LigthsState ligths) {
    char text[50];

    if (window == OPEN && ligths == ON) {
        strcpy(text, "The window is open and the lights are on.");

    } else if (window == OPEN && ligths == OFF) {
        strcpy(text, "The window is open and the lights are off.");

    } else if (window == CLOSE && ligths == ON) {
        strcpy(text, "The window is closed and the lights are on.");

    } else if (window == CLOSE && ligths == OFF) {
        strcpy(text, "The window is closed and the lights are off.");
    }


    time_t t = time(NULL);
    char * time_str = ctime(&t);
    time_str[strlen(time_str)-1] = '\0';



    fprintf(f, "%s: %s\n", time_str, text);
}
