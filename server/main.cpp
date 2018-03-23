
#include "Server.h"
#include <thread>
#include <vector>
using namespace std;

void startRead(Server server, int commSocket){
    server.userLogging(commSocket);
    while (true){
    /*while(*/server.serverRead(commSocket)/*<=0)*/;

    server.serverSend();
    }

}

void startSend(Server server){
    while (true) {
        //
    }

}



int main(int argc, char const *argv[]) {

    int commSocket;
    Server server;
    server.init();
    std::cout << server.port << '\n';
    server.serverListen();
    vector<thread*> threads;
    while(true){
    commSocket=server.serverAccept();
    threads.push_back(new thread(startRead, server, commSocket));
    }
    //readAndSend.join();
    server.end();

}
