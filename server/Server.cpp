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
    return true;

}

bool Server::serverListen(){

    if (listen(listeningSocket, 5) < 0) {
        std::cerr << "Error: listen fallido" << '\n';
        return false;
    }

    std::cout << "Esperando peticiones..." << '\n';
    addrlen = sizeof(address);
    return true;
}

bool Server::serverAccept(){
    commSocket = accept(listeningSocket, (struct sockaddr *)&clientAddress, &addrlen);
    if (commSocket < 0) {
        std::cerr << "Error: accept fallido" << '\n';
        return false;
    }
    std::cout << "Recibida petición de " << inet_ntoa(clientAddress.sin_addr) << '\n';
    return true;

}

bool Server::serverRead(){
    char buffer[bufferSize] = {0};
    int n = read(commSocket, buffer, bufferSize);
    Message* messageEmitter = new Message;
    messageEmitter->setText(buffer);
    messageEmitter->setOrigin(inet_ntoa(clientAddress.sin_addr));
    messageEmitter->setDestination("172.17.3.91")

    if (n > 0) {
        std::cout << "Mensaje recibido: " << messageEmitter->getText() << '\n';
    } else {

        std::cerr << "Error: recepción del mensaje" << '\n';
        return false;
    }

    messageQueue.push(messageEmitter);
    std::cout << "Mensaje COLA: " << messageQueue.back()->getText() << '\n';
    message = "Hola, estoy aquí para servirte";
    return true;
}

bool Server::serverSend(){
    struct sockaddr_in destinationAddress;
    inet_aton(messageQueue.back()->getDestination(), &destinationAddress.sin_addr);
    commSocket = accept(listeningSocket, (struct sockaddr *)&destinationAddress, &addrlen);
    message = messageQueue.back()->getText();
    messageQueue.pop();
    send(commSocket, message, strlen(message), 0);
    std::cout << "Mensaje enviado: " << message << '\n';
    return true;

}

void Server::end(){
    close(listeningSocket);
    close(commSocket);
}
