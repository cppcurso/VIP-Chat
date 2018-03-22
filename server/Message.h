#include <string>

using namespace std;
class Message{
    string text;
    string destination;
    string origin;
public:
    Message(){};
    Message(string t, string d, string o);
    string getText() {return text;};
    string getDestination() {return destination;};
    string getOrigin() {return origin;};
    void setText(string text);
    void setDestination(string destination);
    void setOrigin(string origin);
};
