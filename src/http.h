#ifndef HTTP_H
#define HTTP_H

#include "servtypes.h"

#define HTTP_404 "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n"

RawHttpRequest* parse_request(const char*);

// Default options
RawHttpResponse* render(char* filename);
RawHttpResponse* http_response(char* text);

int create_header(char* buf, int status);
size_t file_size(int fd);


#endif
