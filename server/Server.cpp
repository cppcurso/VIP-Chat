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

int Server::serverAccept(){
    commSocket = accept(listeningSocket, (struct sockaddr *)&clientAddress, &addrlen);
    if (commSocket < 0) {
        std::cerr << "Error: accept fallido" << '\n';
        return -1;
    }
    std::cout << "Recibida petición de " << inet_ntoa(clientAddress.sin_addr) << '\n';
    return commSocket;

}

void Server::userLogging(int commSocket){
    char buffer[bufferSize] = {0};
    int n = read(commSocket, buffer, bufferSize);
    socketUsers[buffer] = commSocket;
}

int Server::serverRead(int commSocket){
    std::string destination = "";
    std::string text = "";
    Message* messageEmitter = new Message;
    bool fromHere = false;
    char buffer[bufferSize] = {0};
    int n = read(commSocket, buffer, bufferSize);
    for (size_t i = 0; i < buffer.size(); i++) {
        if (buffer[i] == "·") {
            messageEmitter->setDestination(destination);
            break;
        }
        destination += buffer[i];
    }
    for (size_t i = 0; i < buffer.size(); i++) {
        if (buffer[i] == "·" && !fromHere) {
            fromHere = true;
        } else if (fromHere) {
            text += buffer[i];
        }
    }
    messageEmitter->setText(text);
    messageEmitter->setOrigin(inet_ntoa(clientAddress.sin_addr));
    if (n > 0) {
        std::cout << "Mensaje recibido: " << messageEmitter->getText() << '\n';
    } else {
        return -1;
    }

    messageQueue.push(messageEmitter);
    std::cout << "Mensaje COLA: " << messageQueue.back()->getText() << '\n';
    message = "Hola, estoy aquí para servirte";
    return n;
}

bool Server::serverSend(){
    struct sockaddr_in destinationAddress;
    commSocketReception = accept(socketUsers[messageEmitter->getDestination()], (struct sockaddr *)&clientAddress, &addrlen);
    // if (listeningSocket < 0) {
    //     std::cerr << "Error: creación de socket" << '\n';
    //     return false;
    // }
    //
    // int enable = 1;
    // if (setsockopt(commSocket1, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(enable))) {
    //     std::cerr << "Error: puerto en uso" << '\n';
    //     return false;
    // }
    // destinationAddress.sin_family = AF_INET; // Address Family IPv4
    // destinationAddress.sin_addr.s_addr = INADDR_ANY; // IP 0.0.0.0 (todas las interfaces de red)
    // destinationAddress.sin_port = htons(8888); // Puerto de Endian de Host a Endian de Network
    inet_aton(messageQueue.back()->getDestination().c_str(), &destinationAddress.sin_addr);

    // if (bind(commSocket1, (struct sockaddr *)&destinationAddress, sizeof(destinationAddress)) < 0) {
    //     std::cerr << "Error: bind fallido" << '\n';
    //     return false;
    // }
    // //commSocket = accept(listeningSocket, (struct sockaddr *)&destinationAddress, &addrlen);
    message = messageQueue.back()->getText().c_str();
    messageQueue.pop();
    send(commSocket1, message, strlen(message), 0);
    std::cout << "Mensaje enviado: " << message << '\n';

    return true;

}

void Server::end(){
    close(listeningSocket);
    close(commSocket);
    close(commSocket1);
}
