#ifndef ROUTES_H
#define ROUTES_H

#include "servtypes.h"
#include <glib.h>

typedef GHashTable RouteMap;

RawHttpResponse* route_connection(RawHttpRequest*, RouteMap*);

void route(Server*, char* path, RawHttpResponse* (*view)(RawHttpRequest*));

#endif
