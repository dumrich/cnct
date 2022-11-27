#include "routes.h"
#include "http.h"
#include "servtypes.h"
#include <string.h>
#include <glib.h>
#include <stdio.h>

RawHttpResponse* route_connection(RawHttpRequest* request, RouteMap* r) {
    // Add Regex handler
    RawHttpResponse* (*root)(void*) = g_hash_table_lookup(r, request->path);

    // 404 Error
    if(root == NULL) {
        RawHttpResponse* responser = malloc(sizeof(RawHttpResponse));
        responser->response = malloc(strlen(HTTP_404) + 1);
        strcpy(responser->response, HTTP_404);
        return responser;
    }

    // Header Already Added
    RawHttpResponse* response = root(request);

    return response;
}

// TODO: Add Regex support and make into a tree structure
void route(Server* s, char* path, RawHttpResponse*(*view)(RawHttpRequest*)) {
    g_hash_table_insert(s->rmp, path, view);
}
