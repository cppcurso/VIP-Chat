#include <iostream> // cout
#include <sys/socket.h> // sockaddr, socklen_t
#include <stdlib.h> // atoi
#include <netinet/in.h> // sockaddr_in
#include <string.h> // strlen
#include <unistd.h> // read, close
#include <arpa/inet.h> // inet_ntoa
#include <queue>
#include "Message.h"
#include <cstdlib>

using namespace std;

class Server{
public:
    int listeningSocket;
    struct sockaddr_in address;
    struct sockaddr_in clientAddress;
    socklen_t addrlen;
    int commSocket;
    const char *message = "Hola, estoy aquí para servirte";
    const int bufferSize = 1024;
    std::queue<Message*> messageQueue;
    int port;
    Server(int port = 8888):port(port){}
    bool init();
    bool serverListen();
    bool serverAccept();
    bool serverRead();
    bool serverSend();
    void end();
};
