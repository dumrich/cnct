#include "servtypes.h"
#include "http.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

RawHttpRequest* parse_request(char* buf) {
    RawHttpRequest* r = malloc(sizeof(RawHttpRequest));


    /* TODO: Make larger */
    r->path = malloc(100);
    sscanf(buf, "%*s %s HTTP/", r->path);

    char method[3];
    memcpy(method, buf, 3);
    if(strcmp("GET", method) == 0) {
        r->r = GET;
    } else if (strcmp("POS", method) == 0) {
        r->r = POST;
    } else if (strcmp("PUT", method) == 0) {
        r->r = PUT;
    }

    return r;
    
}

static size_t file_size(FILE* f) {
    fseek(f, 0, SEEK_END);
    int size = ftell(f);

    fseek(f, 0, SEEK_SET);

    return size;
}

RawHttpResponse* render(char* filename) {
    RawHttpResponse* response = malloc(sizeof(RawHttpResponse));

    response->status_code = 200;

    FILE* f = fopen(filename, "r");

    if(f == NULL) {
        fprintf(stderr, "Requested file not found\n");
        response->status_code = 404;
        return NULL;
    } 

    int length = file_size(f);
    response->response = calloc(1, RESPONSE_SIZE);

    response->response_len = create_response(response->response, response->status_code, f);

    fclose(f);

    return response;
}


char date[1000];
// TODO: Add status code and HTTP version
int create_response(char* buf, int status, FILE* f) {
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(date, sizeof date, "%a, %d %b %Y %H:%M:%S %Z", &tm);

    // HTTP Version
    strcat(buf, "HTTP/1.1");
    // HTTP Status Code
    strcat(buf, " 200 OK\n");
    // HTTP Return Type
    strcat(buf, "Content-Type: text/html\n");
    // HTTP Date
    strcat(buf, "Connection: Closed\nDate: ");
    strcat(buf, date);
    strcat(buf, "\n\n");
    unsigned int offset = strlen(buf);

    size_t bytes_read = fread(buf + offset, 1, RESPONSE_SIZE - offset, f);
    return offset + bytes_read;
}
