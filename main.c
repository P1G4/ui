#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>


#ifdef _WIN32
    #include <direct.h>   // For _getcwd() on Windows
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>   // For getcwd() on UNIX-like systems
    #define GetCurrentDir getcwd
#endif


typedef enum {
    OPEN,
    CLOSE
} WindowState;

typedef enum {
    ON,
    OFF
} LigthsState;


FILE *f;

#define FROM    "burhennemalte5@gmail.com"

//#define ENV_FILE ".env"
#define ENV_FILE "C:\\Users\\burhe\\CLionProjects\\p1\\ui\\.env"

char* load_app_password(const char* env_file_path);
const char* getText(WindowState window, LigthsState lights);
void sendEmail(WindowState window, LigthsState ligths);
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
    sendEmail(window, ligths);


    fclose(f);

    return 0;
}

void Scribere(WindowState window, LigthsState ligths) {
    char* text = getText(window, ligths);

    time_t t = time(NULL);
    char * time_str = ctime(&t);
    time_str[strlen(time_str)-1] = '\0';



    fprintf(f, "%s: %s\n", time_str, text);
}



// Function to load the App Password from .env file
char* load_app_password(const char* env_file_path) {
    FILE *file = fopen(env_file_path, "r");
    if (!file) {
        perror("Could not open .env file");
        return NULL;
    }

    char line[256];
    char *app_password = NULL;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "APP_PASSWORD=", 13) == 0) {
            // Allocate memory for the password and extract it from the line
            app_password = strdup(line + 13); // Skip "APP_PASSWORD=" part

            // Remove newline character from the end if present
            app_password[strcspn(app_password, "\r\n")] = 0;

            break;
        }
    }

    fclose(file);
    return app_password;
}

void sendEmail(WindowState window, LigthsState ligths) {
    // Load App Password from the .env file
    char *app_password = load_app_password(ENV_FILE);
    if (!app_password) {
        fprintf(stderr, "Failed to load APP_PASSWORD from .env file.\n");

    }

    // Create the email body content
    char email_body[512];
    snprintf(email_body, sizeof(email_body),
             "%s", getText(window, ligths));

    //const char *recipients = "<burhennemalte1@gmail.com>, <Axmed141102@gmail.com>";
    const char *recipients = "<burhennemalte1@gmail.com>";
    
    char powershell_command[1024];
    snprintf(powershell_command, sizeof(powershell_command),
             "powershell -Command \"Send-MailMessage -To '%s' -From '%s' -Subject 'Close window or turn off lights' "
             "-Body '%s' -SmtpServer 'smtp.gmail.com' -UseSsl -Port 587 "
             "-Credential (New-Object -TypeName PSCredential -ArgumentList '%s', (ConvertTo-SecureString '%s' -AsPlainText -Force))\"",
             recipients, FROM, email_body, FROM, app_password);


    printf("Executing: %s\n", powershell_command);

    // Use system() to send the command to PowerShell
    int result = system(powershell_command);

    if (result == 0) {
        printf("Command executed successfully.\n");
    } else {
        printf("Command execution failed with code: %d\n", result);
    }

    // Free allocated memory for the App Password
    free(app_password);
}

const char* getText(WindowState window, LigthsState lights) {
    if (window == OPEN && lights == ON) {
        return "The window is open and the lights are on.";

    } else if (window == OPEN && lights == OFF) {
        return "The window is open and the lights are off.";

    } else if (window == CLOSE && lights == ON) {
        return "The window is closed and the lights are on.";

    } else if (window == CLOSE && lights == OFF) {
        return "The window is closed and the lights are off.";
    }

    // Optional: Handle unexpected values
    return "Unknown state.";
}

