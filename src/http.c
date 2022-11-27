#include "servtypes.h"
#include "http.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

RawHttpRequest* parse_request(const char* buf) {
    RawHttpRequest* r = malloc(sizeof(RawHttpRequest));

    char method[3];
    memcpy(method, buf, 3);
    if(strcmp("GET", method) == 0) {
        r->r = GET;
    } else if (strcmp("POS", method) == 0) {
        r->r = POST;
    } else if (strcmp("PUT", method) == 0) {
        r->r = PUT;
    }

    int lower_bound;
    int upper_bound;
    for(lower_bound = 3; buf[lower_bound] != ' '; lower_bound++);
    for(upper_bound = lower_bound + 2; buf[upper_bound] != ' '; upper_bound++);

    r->path = malloc(upper_bound - lower_bound + 2);
    memcpy(r->path, buf + lower_bound + 1, upper_bound - lower_bound - 1);

    return r;
    
}

RawHttpResponse* render(char* filename) {
    RawHttpResponse* response = malloc(sizeof(RawHttpResponse));

    response->status_code = 200;

    int fd = open(filename, O_RDONLY);
    if(fd < 0) {
        printf("Requested file not found\n");
        response->status_code = 404;
    } 

    int length = file_size(fd);
    response->response = malloc(length + RESPONSE_HEADER_LEN + 1);

    int offset = create_header(response->response, response->status_code);  

    printf("%d\n", offset);
    printf("%d\n", length);

    read(fd, response->response + offset, length);
    
    close(fd);

    response->response_len = offset + length;

    return response;
}

size_t file_size(int fd) {
    size_t size = lseek(fd, 0, SEEK_END);

    lseek(fd, 0, SEEK_SET);

    return size;
}

// TODO: Add status code and HTTP version
int create_header(char* buf, int status) {
    char date[1000];
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
    return strlen(buf);
}
