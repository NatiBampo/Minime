#ifndef SERVER104_H
#define SERVER104_H

#include <QObject>
#include <QThread>


class Server104 : public QThread
{
    Q_OBJECT
    //enum TagType : unsigned char {SP = 0, MV = 1, FL = 2};
    //enum TagState {VALID = 100, INVALID = 200};
    enum Column { IP = 0, Port = 1, ASDU = 2, State = 3 };//, IO = 3, TAGTYPE = 4, VALUE = 5, TIME = 6, STATE = 7 };
    static int m_server_count;
public:


    Server104()
    {
        ip[0] = 127;
        ip[1] = 0;
        ip[2] = 0;
        ip[3] = 1;
        port = 2404;
        asdu = 1;
        state = false;

    }
    //
    Server104(const unsigned int *address)//unsigned char tagT, float tagV, char* tagTime, bool tagState)
    {
        //m_server_count++;
        for (int i = 0; i < 7; i++)
        {
            if (i < 4) ip[i] = address[i];
            else if (i < 5) port = address[i];
            else if (i < 6) asdu = address[i];
            state = false;
            /*else if (i < 7) io = address[i];
            switch (static_cast<unsigned char>(tagT))
            {
                case 0 : tagType = TagType::SP; break;
                case 1 : tagType = TagType::MV; break;
                case 2 : tagType = TagType::FL; break;
                default: tagType = TagType::FL;
            }
            value = tagV;
            if (tagState) { state = TagState::VALID; }
            else { state = TagState::INVALID; }

            time = tagTime;*/
        }
    }

    Server104& operator=(const Server104& other)
    {
        /*this->~Server104();
        return *new(this) Server104(std::move(other));*/
        for (int i = 0; i < 4; i++)
        {
            ip[i] = other.ip[i];
        }
        port = other.port;
        asdu = other.asdu;
        state = other.state;
    }

    Server104(const Server104 &other)
    {
        for (int i = 0; i < 4; i++)
        {
            ip[i] = other.ip[i];
        }
        port = other.port;
        asdu = other.asdu;
        state = other.state;
        /*io = other.io;
        tagType = other.tagType;
        value = other.value;
        state = other.state;
        time = other.time;*/
    }
    ~Server104(){}

    unsigned int* getIP() { return ip; }
    unsigned int getPort() { return port; }
    unsigned int getAsdu()  { return asdu; }
    bool getState() { return state; }
    void setIP(unsigned int* address)  { for (int i=0; i<4; i++) ip[i] = address[i]; }
    void setPort(unsigned int value )  { port = value; }
    void setAsdu(unsigned int value )  { asdu = value; }
    void setState(bool value) {state = value;}
    /*unsigned int getIO() const { return io; }
    TagType getTagType() const { return tagType; }
    float getValue() const { return value; }
    char* getTime() const { return time; }
    TagState getState() const { return state; }*/
    void startServer();
    void createASDU(int num, float* values, int* iots, int* types);

public:
    unsigned int ip[4];
    unsigned int port;
    unsigned int asdu;
    bool state;
    /*unsigned int io = 1;
    TagType tagType = TagType::SP;
    float value = 0;
    char* time = {};
    TagState state = TagState(0);*/
};
#endif // SERVER104_H
