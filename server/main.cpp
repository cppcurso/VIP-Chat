
#include "Server.h"
#include <thread>
using namespace std;

void startRead(Server server){
    server.serverRead();
}

void startSend(Server server){
    server.serverSend();
}



int main(int argc, char const *argv[]) {
    // if (argc> 2){
    //     Server server(atoi(argv[1]));
    // } else{
    //     Server server;
    // }
Server server;
    server.init();
    std::cout << server.port << '\n';
    server.serverListen();

    while(true){
    server.serverAccept();
    thread serverRead(startRead, server);
    thread serverSend(startSend, server);
    serverRead.join();
    serverSend.join();
    }
    server.end();

}
