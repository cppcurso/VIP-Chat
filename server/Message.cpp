#include "Message.h"

Message::Message(string t, string d, string o){
    text = t;
    destination = d;
    origin = o;
}

void Message::setText(string text){
    this->text = text;
}
void Message::setDestination(string destination){
    this->destination = destination;
}
void Message::setOrigin(string origin){
    this->origin = origin;
}
