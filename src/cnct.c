#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <stdbool.h>
#include <glib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "cnct.h"
#include "rtcp.h"
#include "http.h"
#include "routes.h"
#include "servtypes.h"

int gsfd;

Server* new_server(void) {
    Server* s = malloc(sizeof(Server));

    // Initialize Routemap
    s->rmp = g_hash_table_new(g_str_hash, g_str_equal);

    if(NUM_THREADS > 0) {
        pthread_mutex_init(&s->_req_queue_mutex, NULL);
        pthread_cond_init(&s->_req_queue_cond_var, NULL);

        for(int i = 0; i < NUM_THREADS; i++) {
            s->tids[i] = (pthread_t)0;
        }
    }

    s->_req_queue = g_queue_new();
    g_queue_clear(s->_req_queue);

    return s;
}

int cnct(Server* s, short int port) {

    printf("Starting web server on port %hu...", port);
    fflush(stdout);
    /* Start TCP Server */
    raw_sock socket = create_socket(port);
    gsfd = socket.sfd;

    if(socket.sfd < 0) {
        perror("Could not start socket");
        exit(-1);
    }
    
    s->socket_descriptor = socket.sfd;
    
    /* Start Threads */
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&s->tids[i], NULL, handle_requests, s);
    }

    printf(" Started\n");
    return listen_at_addr(socket, s->_req_queue, &s->_req_queue_cond_var, &s->_req_queue_mutex);
}


static void print_request(RawHttpRequest* r) {
    switch(r->r) {
    case GET:
        printf(": GET ");
        break;
    }

    // Resource
    printf("%s HTTP/1.1\n\n", r->path);
    
}

char rdate[120];
void* handle_requests(void* v) {
    Server* s = (Server*) v;

    char* request_header = calloc(sizeof(char), REQUEST_SIZE);
    
    while(true) {
        pthread_mutex_lock(&s->_req_queue_mutex);

        if(g_queue_peek_head(s->_req_queue) == NULL) {
            pthread_cond_wait(&s->_req_queue_cond_var, &s->_req_queue_mutex);
        }

        /* Handle connection code */
        int* cfd_pointer = (int*)g_queue_pop_head(s->_req_queue);

        pthread_mutex_unlock(&s->_req_queue_mutex);

        printf("Connection Accepted on thread %d!\n", syscall(__NR_gettid));

        recv(*cfd_pointer, request_header, REQUEST_SIZE, 0);

        time_t now = time(0);
        struct tm tm = *gmtime(&now);
        strftime(rdate, sizeof rdate, "%a, %d %b %Y %H:%M:%S %Z", &tm);
        printf("[%s]", rdate);
        
        /* Get request header */
        RawHttpRequest* http_request = parse_request(request_header);
        print_request(http_request);


        RawHttpResponse* response = route_connection(http_request, s->rmp, &s->opts); 
        
        send(*cfd_pointer, response->response, response->response_len, 0);
        
        free(response->response);
        free(response);

        free(http_request->path);
        free(http_request);

        close(*cfd_pointer);
        free(cfd_pointer);
        /* End Handle Connection */
        

    };
}
