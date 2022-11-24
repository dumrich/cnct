#ifndef CNCT_H
#define CNCT_H

#include <pthread.h>
#include <glib.h>
#include "routes.h"
#include "servtypes.h"

/* Must be greater than 0 */
#define NUM_THREADS 4

/* Default size */
/* TODO: Make growable */
#define REQUEST_SIZE 8096

typedef struct {
    struct RouteMap* rmp;

    /* Mutex for only the queue. Other data is read only */
    pthread_mutex_t _req_queue_mutex;
    pthread_cond_t _req_queue_cond_var;
    GQueue* _req_queue;

    pthread_t tids[NUM_THREADS];
    

    int socket_descriptor;
} Server;

Server* new_server(void);
int cnct(Server* s, short int port);

void* handle_requests(void*);

#endif
