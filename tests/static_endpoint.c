#include <cnct.h>
#include <routes.h>
#include <http.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define PORT 8000

RawHttpResponse* home(RawHttpRequest* r) {
    return render("tests/templates/index.html");
}

RawHttpResponse* about(RawHttpRequest* r) {
    return render("tests/templates/about.html");
}

RawHttpResponse* blog(RawHttpRequest* r) {
    return render("tests/templates/blog.html");
}

int main(void) {

    Server* server = new_server();
    server->opts.not_found_path = "tests/templates/404.html";
    server->opts.static_dir = "tests/templates/assets"; 
    
    /* Setup HTTP Routes */
    route(server, "/", home);
    route(server, "/about", about);
    route(server, "/blog", blog);


    /* Start TCP Server */
    return cnct(server, PORT);
}
