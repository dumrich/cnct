#include "routes.h"
#include "http.h"
#include "servtypes.h"
#include <string.h>
#include <glib.h>
#include <stdio.h>
#include <ctype.h>

RawHttpResponse* route_connection(RawHttpRequest* request, RouteMap* r, struct Settings* opts) {
    // Add Regex handler
    RawHttpResponse* (*root)(void*) = g_hash_table_lookup(r, request->path);


    // 404 Error
    if(root == NULL) {
        printf("File: %s\n", opts->not_found_path);
        return render_404(opts->not_found_path);
    }
    
    // Header Already Added
    RawHttpResponse* response = root(request);

    return response;
}

// TODO: Add Regex support and make into a tree structure
void route(Server* s, char* path, RawHttpResponse*(*view)(RawHttpRequest*)) {
    g_hash_table_insert(s->rmp, path, view);
}
