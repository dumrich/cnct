#include <cnct.h>
#include <routes.h>

int main(void) {
    Server* server = new_server();

    /* Start TCP Server */
    return cnct(server, 8000);
}
