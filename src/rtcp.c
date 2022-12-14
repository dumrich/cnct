#include "rtcp.h"
#include <bits/pthreadtypes.h>
#include <glib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

raw_sock create_socket(short int port) {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
        fprintf(stderr, "setsockopt(SO_REUSEADDR) failed");
    
    if(sfd == -1) {
        return (raw_sock){.port = -1, .sfd = -1};
    }
    
    struct sockaddr_in addr;
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    int e = bind(sfd, (struct sockaddr *)&addr, sizeof addr);

    raw_sock s = {.port = port, .sfd = sfd};

    return s;
}

int listen_at_addr(raw_sock socket, GQueue* queue, pthread_cond_t* socketcondvar, pthread_mutex_t* socketmutex) {
    int e = listen(socket.sfd, SOMAXCONN);

    if(e == 0) {
        while(1) {
            int* client_fd = malloc(sizeof(int));
            *client_fd = accept(socket.sfd, NULL, NULL);

            pthread_mutex_lock(socketmutex);
            g_queue_push_tail(queue, client_fd);

            pthread_mutex_unlock(socketmutex);

            pthread_cond_signal(socketcondvar);

        }
    }
    
    return e;
}
