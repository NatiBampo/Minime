#ifndef SERVERVARIANT_H
#define SERVERVARIANT_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QVariant>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "cs104_slave.h"
#include "serverIEC104.h"
#include "hal_thread.h"
#include "hal_time.h"


class ServerVariant : public QObject
{
    Q_OBJECT
public:
    enum TagType : unsigned char {SP = 0, MV = 1, FL = 2};
    enum TagState {VALID = 100, INVALID = 200};
    enum Column { IP = 0, Port = 1, ASDU = 2, State = 3 };//, IO = 3, TAGTYPE = 4, VALUE = 5, TIME = 6, STATE = 7 };
    static int m_server_count;


    class TagVar
    {
    public:
        TagVar (int addr)
        {
            iot = addr;
        }
        int iot;
        TagType tagType;

    };
    class FLTag : public TagVar
    {
    public:
        FLTag(int addr, float val) : TagVar(addr)
        {
            value = val;
            tagType = FL;
        }
        float value;
    };
    class MVTag : public TagVar
    {
    public:
        MVTag(int addr, int val) : TagVar(addr)
        {
            value = val;
            tagType = MV;
        }
        int value;
    };
    class SPTag: public TagVar
    {
    public:
        SPTag(int addr, bool val) : TagVar(addr)
        {
            value = val;
            tagType = SP;
        }
        bool value;
    };

public:
    SPTag a1 {601,  true};
    SPTag a2 {602,  true};
    SPTag a3 {603, false};


    MVTag b1 {701, 100500};
    MVTag b2 {702, 666};
    MVTag b3 {703, 69};

    FLTag c1 {801, 1.5};
    FLTag c2 {802, 6.67};
    FLTag c3 {803, 69.25};

    QList<SPTag> m_tags_SP = {a1, a2, a3};
    QList<MVTag> m_tags_MV = {b1, b2, b3};
    QList<FLTag> m_tags_FL = {c1, c2, c3};

public:

    ServerVariant()
    {
        ip[0] = 127;
        ip[1] = 0;
        ip[2] = 0;
        ip[3] = 1;
        /*ip[0] = 172;
        ip[1] = 18;
        ip[2] = 192;
        ip[3] = 1;*/
        port = 2404;
        asdu = 1;
        state = false;
    }
    //
    ServerVariant(const unsigned int *address)//unsigned char tagT, float tagV, char* tagTime, bool tagState)
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

    ServerVariant& operator=(const ServerVariant& other)
    {
        /*this->~ServerVariant();
        return *new(this) ServerVariant(std::move(other));*/
        for (int i = 0; i < 4; i++)
        {
            ip[i] = other.ip[i];
        }
        port = other.port;
        asdu = other.asdu;
        state = other.state;
    }

    ServerVariant(const ServerVariant &other)
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
    ~ServerVariant(){}

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

    void createASDU(int num, float* values, int* iots, int* types);

public:
    unsigned int ip[4];
    unsigned int port;
    unsigned int asdu;
    bool state;
    bool state2;
    /*unsigned int io = 1;
    TagType tagType = TagType::SP;
    float value = 0;
    char* time = {};
    TagState state = TagState(0);*/

private :
    void init_ServerTable(const char* address)
    {
        CS104_Slave slave = CS104_Slave_create(10, 10);
        CS104_Slave_setLocalAddress(slave, address);//"0.0.0.0"
        CS104_Slave_setServerMode(slave, CS104_MODE_SINGLE_REDUNDANCY_GROUP);
        CS101_AppLayerParameters alParams = CS104_Slave_getAppLayerParameters(slave);
        CS104_APCIParameters apciParams = CS104_Slave_getConnectionParameters(slave);

        qDebug() << "APCI parameters:" ;
        qDebug() << QString("  t0: %0").arg(apciParams->t0)
         << QString("  t1: %0").arg(apciParams->t1)
         << QString("  t2: %0").arg(apciParams->t2)
         << QString("  t3: %0").arg(apciParams->t3)
         << QString("  k: %0").arg(apciParams->k)
         << QString("  w: %0").arg(apciParams->w);

        //CS104_Slave_setClockSyncHandler(slave, clockSyncHandler, NULL);
        //CS104_Slave_setConnectionRequestHandler(slave, connectionRequestHandler, NULL);
        //CS104_Slave_setConnectionEventHandler(slave, connectionEventHandler, NULL);
        CS104_Slave_start(slave);

        int16_t scaledValue = 0;

        while (state)
        {
            Thread_sleep(1000);

            CS101_ASDU newAsdu = CS101_ASDU_create(alParams, false, CS101_COT_PERIODIC, 0, 1, false, false);
            //InformationObject io = (InformationObject) MeasuredValueScaled_create(NULL, 110, scaledValue, IEC60870_QUALITY_GOOD);
            InformationObject io;
            for (int i = 0; i < m_tags_SP.length(); i++)
            {
                if (i == 0)
                    io = (InformationObject) SinglePointInformation_create(NULL, m_tags_SP[i].iot,
                                                                           m_tags_SP[i].value, IEC60870_QUALITY_GOOD);

                CS101_ASDU_addInformationObject(newAsdu,
                                                (InformationObject) SinglePointInformation_create((SinglePointInformation) io,
                                                m_tags_SP[i].iot, m_tags_SP[i].value, IEC60870_QUALITY_GOOD));

            }
            CS101_ASDU_addInformationObject(newAsdu, io);
            InformationObject_destroy(io);
            CS104_Slave_enqueueASDU(slave, newAsdu);
            CS101_ASDU_destroy(newAsdu);


            newAsdu = CS101_ASDU_create(alParams, false, CS101_COT_PERIODIC, 0, 1, false, false);
            //InformationObject io =

            for (int i = 0; i < m_tags_MV.length(); i++)
            {
                if (i == 0)
                    io = (InformationObject) MeasuredValueScaled_create(NULL, m_tags_MV[i].iot,
                                                                           m_tags_MV[i].value, IEC60870_QUALITY_GOOD);

                CS101_ASDU_addInformationObject(newAsdu,
                                                (InformationObject) MeasuredValueScaled_create((MeasuredValueScaled) io,
                                                m_tags_MV[i].iot, m_tags_MV[i].value, IEC60870_QUALITY_GOOD));

            }
            CS101_ASDU_addInformationObject(newAsdu,
                                            (InformationObject) MeasuredValueScaled_create((MeasuredValueScaled) io,
                                            5001, scaledValue, IEC60870_QUALITY_GOOD));

            CS101_ASDU_addInformationObject(newAsdu, io);
            InformationObject_destroy(io);
            CS104_Slave_enqueueASDU(slave, newAsdu);
            CS101_ASDU_destroy(newAsdu);

            scaledValue++;
        }

        qDebug() << "stopped in init";

        CS104_Slave_stop(slave);
    //exit_program:
        CS104_Slave_destroy(slave);
        Thread_sleep(500);

    }


public slots:
    void startServer()
    {
        if (!state)
        {
           qDebug() << QString("Starting %1 server").arg(0);
           QString str1 = QString("%1.%2.%3.%4").arg(ip[0]).arg(ip[1]).arg(ip[2]).arg(ip[3]);
           qDebug() << str1;
           QByteArray ba = str1.toLocal8Bit();
           const char *c_str2 = ba.data();
           init_iec_server(c_str2);
            state = false;
        }
        else
        {
            state = true;
            qDebug() << "i'm out server1";
        }


    }

    void startServer2()
    {
        if (!state2)
        {
            qDebug() << QString("Starting %1 server").arg(1); //index.row());
            //init_iec_server("172.18.192.1");

            QString str1 = QString("%1.%2.%3.%4").arg(ip[0]).arg(ip[1]).arg(ip[2]).arg(ip[3]);
            qDebug() << str1;
            QByteArray ba = str1.toLocal8Bit();
            const char *c_str2 = ba.data();
            init_ServerTable(c_str2);
            state = false;
        }
        else
        {
            state = true;
            qDebug() << "i'm out server2";
        }
    }
};

Q_DECLARE_METATYPE(ServerVariant)

#endif // SERVERVARIANT_H
