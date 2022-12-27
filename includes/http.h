#ifndef HTTP_H
#define HTTP_H

#include "servtypes.h"
#include <stdio.h>

#define RESPONSE_SIZE 16000
#define HTTP_404 "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n"

RawHttpRequest* parse_request(char*);

// Default options
RawHttpResponse* render(char* filename);
RawHttpResponse* render_404(char* filename);

RawHttpResponse* http_response(char* text);

int create_response(char* buf, const char* status, FILE*);


#endif
