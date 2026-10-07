#ifndef UTILS_LOGGER_H
#define UTILS_LOGGER_H

void log_request(const char *method, const char *path, int status);

void log_info(const char *format, ...);

void log_error(const char *format, ...);

#endif // UTILS_LOGGER_H

