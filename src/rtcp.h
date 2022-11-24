#ifndef RTCP_H
#define RTCP_H

#include "servtypes.h"
#include <glib.h>

raw_sock create_socket(short int port);
int listen_at_addr(raw_sock, GQueue*, pthread_cond_t*, pthread_mutex_t*);

#endif
