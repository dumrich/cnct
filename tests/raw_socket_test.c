#include <stdio.h>
#include <cnct.h>

int main(int argc, char** argv) {
    Server* server = new_server();

    /* Start TCP Server */
    return cnct(server, 8000);
}
