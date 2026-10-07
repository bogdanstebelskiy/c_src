#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static void log_with_timestamp(const char *level, const char *format, va_list args) {
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    printf("[%s] [%s] ", timestamp, level);
    vprintf(format, args);
    printf("\n");
}

void log_request(const char *method, const char *path, int status) {
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    printf("[%s] %s %s - %d\n", timestamp, method, path, status);
}

void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_with_timestamp("INFO", format, args);
    va_end(args);
}

void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_with_timestamp("ERROR", format, args);
    va_end(args);
}

