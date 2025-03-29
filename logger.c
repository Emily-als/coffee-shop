//Simon Sherwood 318597036
//Emily Alshtein 207796533
//Ifat Vider 206865941

#define _CRT_SECURE_NO_WARNINGS
#include "logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void log_message(const char* filename, const char* message) {
    FILE* log_file = fopen(filename, "a"); // Open file in append mode
    if (log_file == NULL) {
        printf("Error opening log file.\n");
        return;
    }

    // Get the current timestamp
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    // Write timestamp and log message to file
    fprintf(log_file, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
            local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
            local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
            message);

    fclose(log_file); // Close the file
}
