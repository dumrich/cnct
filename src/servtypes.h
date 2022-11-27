#ifndef SERV_TYPES_H
#define SERV_TYPES_H

#define NUM_THREADS 4
// TODO: Calculate lowest value
#define RESPONSE_HEADER_LEN 40

#include <glib.h>
#include <pthread.h>


typedef struct {
    short int port;
    int sfd;
} raw_sock;

enum REQUEST_METHOD {
    GET,
    POST,
    PUT,
};

typedef struct {
    enum REQUEST_METHOD r;
    char* path;
    // TODO: Add query arguements
} RawHttpRequest;

typedef struct {
    short int status_code;

    /* Response Buffer */
    size_t response_len;
    char* response;
} RawHttpResponse;

typedef GHashTable RouteMap;

typedef struct {
    RouteMap* rmp;

    /* Mutex for only the queue. Other data is read only */
    pthread_mutex_t _req_queue_mutex;
    pthread_cond_t _req_queue_cond_var;
    GQueue* _req_queue;

    pthread_t tids[NUM_THREADS];
    

    int socket_descriptor;
} Server;

#endif
