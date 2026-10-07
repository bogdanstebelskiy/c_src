#include "mime.h"

#include <string.h>

const char* mime_type_from_extension(const char *ext) {
    if (!ext) return "application/octet-stream";
    
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".txt") == 0) return "text/plain";
    if (strcmp(ext, ".xml") == 0) return "text/xml";
    
    if (strcmp(ext, ".js") == 0) return "application/javascript";
    if (strcmp(ext, ".json") == 0) return "application/json";
    if (strcmp(ext, ".pdf") == 0) return "application/pdf";
    if (strcmp(ext, ".zip") == 0) return "application/zip";
    
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    if (strcmp(ext, ".svg") == 0) return "image/svg+xml";
    if (strcmp(ext, ".ico") == 0) return "image/x-icon";
    if (strcmp(ext, ".webp") == 0) return "image/webp";
    
    if (strcmp(ext, ".mp3") == 0) return "audio/mpeg";
    if (strcmp(ext, ".mp4") == 0) return "video/mp4";
    if (strcmp(ext, ".webm") == 0) return "video/webm";
    
    return "application/octet-stream";
}

const char* mime_type_from_path(const char *path) {
    if (!path) return "application/octet-stream";

    const char *ext = strrchr(path, '.');

    return mime_type_from_extension(ext);
}
