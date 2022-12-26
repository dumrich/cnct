#include <cnct.h>
#include <routes.h>
#include <http.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


int gsfd;

// Ctrl-C Handler
void sigterm_handler(int signum) {
    printf("Ctrl-C Reveived...\n");
    close(gsfd);
    exit(0);
}

RawHttpResponse* home(RawHttpRequest* r) {
    return render("tests/templates/index.html");
}

int main(void) {

    Server* server = new_server();
    
    /* Setup HTTP Routes */
    route(server, "/home", home);

    /* Other Code */
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &sigterm_handler;
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);


    /* Start TCP Server */
    return cnct(server, 8001);
}
