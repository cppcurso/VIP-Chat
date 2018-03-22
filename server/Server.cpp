#include "Server.h"


bool Server::init(){

    listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listeningSocket < 0) {
        std::cerr << "Error: creación de socket" << '\n';
        return false;
    }

    int enable = 1;

    if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(enable))) {
        std::cerr << "Error: puerto en uso" << '\n';
        return false;
    }

    address.sin_family = AF_INET; // Address Family IPv4
    address.sin_addr.s_addr = INADDR_ANY; // IP 0.0.0.0 (todas las interfaces de red)
    address.sin_port = htons(port); // Puerto de Endian de Host a Endian de Network

    if (bind(listeningSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Error: bind fallido" << '\n';
        return false;
    }

}

bool Server::serverListen(){

    if (listen(listeningSocket, 5) < 0) {
        std::cerr << "Error: listen fallido" << '\n';
        return false;
    }

    std::cout << "Esperando peticiones..." << '\n';
    addrlen = sizeof(address);

}

bool Server::serverAccept(){
    struct sockaddr_in clientAddress;
    commSocket = accept(listeningSocket, (struct sockaddr *)&clientAddress, &addrlen);
    if (commSocket < 0) {
        std::cerr << "Error: accept fallido" << '\n';
        return false;
    }
    std::cout << "Recibida petición de " << inet_ntoa(clientAddress.sin_addr) << '\n';

}

bool Server::serverRead(){
    char buffer[bufferSize] = {0};
    int n = read(commSocket, buffer, bufferSize);
    if (n > 0) {
        std::cout << "Mensaje recibido: " << buffer << '\n';
    } else {
        std::cerr << "Error: recepción del mensaje" << '\n';
    }

    const char *message = "Hola, estoy aquí para servirte";
}

bool Server::serverSend(){
    send(commSocket, message, strlen(message), 0);
    std::cout << "Mensaje enviado: " << message << '\n';
}

void Server::end(){
    close(listeningSocket);
    close(commSocket);
}
