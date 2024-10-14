#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#ifdef _WIN32
    #include <direct.h>   // For _getcwd() on Windows
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>   // For getcwd() on UNIX-like systems
    #define GetCurrentDir getcwd
#endif


FILE *f;


void Scribere();

int main(void) {
    char buffer[1024];
    if (GetCurrentDir(buffer, sizeof(buffer)) != NULL) {
        printf("Current working directory: %s\n", buffer);
    } else {
        perror("getcwd() error");
        return 1;
    }

    char *fileName = "data.txt";


    printf("Full path to the file: %s/%s\n", buffer, fileName);

    f = fopen(strcat(buffer, fileName), "w");
    for (int i = 1; i <= 10; i++) {
        Scribere();
        sleep(1);
    }


    fclose(f);

    return 0;
}

void Scribere() {
    time_t t = time(NULL);
    char * time_str = ctime(&t);
    time_str[strlen(time_str)-1] = '\0';


    fprintf(f, "%s\n", time_str);
}
