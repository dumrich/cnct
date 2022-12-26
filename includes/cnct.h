#ifndef CNCT_H
#define CNCT_H

#include "servtypes.h"

extern int gsfd;

/* Default size for request buffer */
/* TODO: Make growable request buffer */
#define REQUEST_SIZE 16000

Server* new_server(void);
int cnct(Server* s, short int port);

void* handle_requests(void*);

#endif
