#include <cnct.h>
#include <routes.h>
#include <http.h>

RawHttpResponse* home(RawHttpRequest* r) {
    return render("tests/index.html");
}

int main(void) {
    Server* server = new_server();
    
    /* Setup HTTP Routes */
    route(server, "/home", home);

    /* Start TCP Server */
    return cnct(server, 8048);
}
