class IMessage {

};

class IMessagable {
public:
    virtual void accept(int message);
};

class Ghost : IMessagable {

};

class Player : IMessagable {

};

class Tree : IMessagable {

};

class MessagePasser {

};