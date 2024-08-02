#ifndef SERVER104_H
#define SERVER104_H


class Server104
{
    enum TagType : unsigned char {SP = 0, MV = 1, FL = 2};
    enum TagState {VALID = 100, INVALID = 200};
    enum Column { IP = 0, Port = 1, ASDU = 2, IOT = 3, TAGTYPE = 4, VALUE = 5, TIME = 6, STATE = 7 };

public:
    Server104(){}
    //
    Server104(unsigned int *address, unsigned char tagT, float tagV, char* tagTime, bool tagState)
    {
        for (int i = 0; i < 7; i++)
        {
            if (i < 4) ip[i] = address[i];
            else if (i < 5) port = address[i];
            else if (i < 6) asdu = address[i];
            else if (i < 7) iot = address[i];
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

            time = tagTime;
        }
    }

    Server104(const Server104 &other)
    {
        for (int i = 0; i < 4; i++)
        {
            ip[i] = other.ip[i];
        }
        port = other.port;
        asdu = other.asdu;
        iot = other.iot;
        tagType = other.tagType;
        value = other.value;
        state = other.state;
        time = other.time;

    }
    ~Server104(){}

    unsigned int* getIP() { return ip; }
    unsigned int getPort() const { return port; }
    unsigned int getAsdu() const { return asdu; }
    unsigned int getIOT() const { return iot; }
    TagType getTagType() const { return tagType; }
    float getValue() const { return value; }
    char* getTime() const { return time; }
    TagState getState() const { return state; }

private:
    unsigned int ip[4] = {127, 0, 0, 1};
    unsigned int port = 2404;
    unsigned int asdu = 1;
    unsigned int iot = 1;
    TagType tagType = TagType::SP;
    float value = 0;
    char* time = {};
    TagState state = TagState(0);

};




#endif // SERVER104_H
