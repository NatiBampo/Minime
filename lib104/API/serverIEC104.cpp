#include <serverIEC104.h>

#include <QDebug>

void
sigint_handler(int signalId)
{
    qDebug() << "sigint_handler: " << signalId ;
    running = false;
}

void someshit_handler()
{
    qDebug() << "someshit_handler";
    running = false;
}


void
printCP56Time2a(CP56Time2a time)
{
    printf("%02i:%02i:%02i %02i/%02i/%04i", CP56Time2a_getHour(time),
                             CP56Time2a_getMinute(time),
                             CP56Time2a_getSecond(time),
                             CP56Time2a_getDayOfMonth(time),
                             CP56Time2a_getMonth(time),
                             CP56Time2a_getYear(time) + 2000);
}

/* Callback handler to log sent or received messages (optional) */
static void
rawMessageHandler(void* parameter, IMasterConnection conneciton, uint8_t* msg, int msgSize, bool sent)
{
    if (sent)
        printf("SEND: ");
    else
        printf("RCVD: ");

    int i;
    for (i = 0; i < msgSize; i++) {
        printf("%02x ", msg[i]);
    }

    printf("\n");
}

static bool
clockSyncHandler (void* parameter, IMasterConnection connection, CS101_ASDU asdu, CP56Time2a newTime)
{
    printf("Process time sync command with time "); printCP56Time2a(newTime); printf("\n");

    uint64_t newSystemTimeInMs = CP56Time2a_toMsTimestamp(newTime);

    /* Set time for ACT_CON message */
    CP56Time2a_setFromMsTimestamp(newTime, Hal_getTimeInMs());

    /* update system time here */

    return true;
}

static bool
interrogationHandler(void* parameter, IMasterConnection connection, CS101_ASDU asdu, uint8_t qoi)
{
    printf("Received interrogation for group %i\n", qoi);

    if (qoi == 20) { /* only handle station interrogation */

        CS101_AppLayerParameters alParams = IMasterConnection_getApplicationLayerParameters(connection);

        IMasterConnection_sendACT_CON(connection, asdu, false);

        /* The CS101 specification only allows information objects without timestamp in GI responses */

        CS101_ASDU newAsdu = CS101_ASDU_create(alParams, false, CS101_COT_INTERROGATED_BY_STATION,
                0, 1, false, false);

        InformationObject io = (InformationObject) MeasuredValueScaled_create(NULL, 100, -150, IEC60870_QUALITY_GOOD);

        CS101_ASDU_addInformationObject(newAsdu, io);

        CS101_ASDU_addInformationObject(newAsdu, (InformationObject)
            MeasuredValueScaled_create((MeasuredValueScaled) io, 101, 23, IEC60870_QUALITY_GOOD));

        CS101_ASDU_addInformationObject(newAsdu, (InformationObject)
            MeasuredValueScaled_create((MeasuredValueScaled) io, 102, 2300, IEC60870_QUALITY_GOOD));

        InformationObject_destroy(io);

        IMasterConnection_sendASDU(connection, newAsdu);

        CS101_ASDU_destroy(newAsdu);

        newAsdu = CS101_ASDU_create(alParams, false, CS101_COT_INTERROGATED_BY_STATION,
                    0, 1, false, false);

        io = (InformationObject) SinglePointInformation_create(NULL, 104, true, IEC60870_QUALITY_GOOD);

        CS101_ASDU_addInformationObject(newAsdu, io);

        CS101_ASDU_addInformationObject(newAsdu, (InformationObject)
            SinglePointInformation_create((SinglePointInformation) io, 105, false, IEC60870_QUALITY_GOOD));

        InformationObject_destroy(io);

        IMasterConnection_sendASDU(connection, newAsdu);

        CS101_ASDU_destroy(newAsdu);

        newAsdu = CS101_ASDU_create(alParams, true, CS101_COT_INTERROGATED_BY_STATION,
                0, 1, false, false);

        CS101_ASDU_addInformationObject(newAsdu, io = (InformationObject) SinglePointInformation_create(NULL, 300, true, IEC60870_QUALITY_GOOD));
        CS101_ASDU_addInformationObject(newAsdu, (InformationObject) SinglePointInformation_create((SinglePointInformation) io, 301, false, IEC60870_QUALITY_GOOD));
        CS101_ASDU_addInformationObject(newAsdu, (InformationObject) SinglePointInformation_create((SinglePointInformation) io, 302, false, IEC60870_QUALITY_GOOD));
        CS101_ASDU_addInformationObject(newAsdu, (InformationObject) SinglePointInformation_create((SinglePointInformation) io, 303, false, IEC60870_QUALITY_GOOD));
        CS101_ASDU_addInformationObject(newAsdu, (InformationObject) SinglePointInformation_create((SinglePointInformation) io, 304, true, IEC60870_QUALITY_GOOD));
        CS101_ASDU_addInformationObject(newAsdu, (InformationObject) SinglePointInformation_create((SinglePointInformation) io, 305, true, IEC60870_QUALITY_GOOD));
        CS101_ASDU_addInformationObject(newAsdu, (InformationObject) SinglePointInformation_create((SinglePointInformation) io, 306, true, IEC60870_QUALITY_GOOD));
        CS101_ASDU_addInformationObject(newAsdu, (InformationObject) SinglePointInformation_create((SinglePointInformation) io, 307, false, IEC60870_QUALITY_GOOD));

        InformationObject_destroy(io);

        IMasterConnection_sendASDU(connection, newAsdu);

        CS101_ASDU_destroy(newAsdu);

        newAsdu = CS101_ASDU_create(alParams, false, CS101_COT_INTERROGATED_BY_STATION,
                        0, 1, false, false);

        io = (InformationObject) BitString32_create(NULL, 500, 0xaaaa);

        CS101_ASDU_addInformationObject(newAsdu, io);

        InformationObject_destroy(io);

        IMasterConnection_sendASDU(connection, newAsdu);

        CS101_ASDU_destroy(newAsdu);

        IMasterConnection_sendACT_TERM(connection, asdu);
    }
    else {
        IMasterConnection_sendACT_CON(connection, asdu, true);
    }

    return true;
}

static bool
asduHandler(void* parameter, IMasterConnection connection, CS101_ASDU asdu)
{
    if (CS101_ASDU_getTypeID(asdu) == C_SC_NA_1) {
        printf("received single command\n");
        qDebug() << "received single command";
        if  (CS101_ASDU_getCOT(asdu) == CS101_COT_ACTIVATION) {
            InformationObject io = CS101_ASDU_getElement(asdu, 0);

            if (io) {
                if (InformationObject_getObjectAddress(io) == 5000) {
                    SingleCommand sc = (SingleCommand) io;

                    printf("IOA: %i switch to %i\n", InformationObject_getObjectAddress(io),
                            SingleCommand_getState(sc));
                    qDebug() << QString("IOA: %1 switch to %2\n").arg(InformationObject_getObjectAddress(io))
                                .arg(SingleCommand_getState(sc));


                    CS101_ASDU_setCOT(asdu, CS101_COT_ACTIVATION_CON);
                }
                else
                    CS101_ASDU_setCOT(asdu, CS101_COT_UNKNOWN_IOA);

                InformationObject_destroy(io);
            }
            else {
                qDebug() << "ERROR: message has no valid information object";
                printf("ERROR: message has no valid information object\n");
                return true;
            }
        }
        else
            CS101_ASDU_setCOT(asdu, CS101_COT_UNKNOWN_COT);

        IMasterConnection_sendASDU(connection, asdu);

        return true;
    }

    return false;
}

static bool
connectionRequestHandler(void* parameter, const char* ipAddress)
{
    printf("New connection request from %s\n", ipAddress);
    qDebug() << "New connection request from " << ipAddress;

/*#if 0
    if (strcmp(ipAddress, "127.0.0.1") == 0) {
        printf("Accept connection\n");
        return true;
    }
    else {
        printf("Deny connection\n");
        return false;
    }
#else*/
    return true;
//#endif
}

static void
connectionEventHandler(void* parameter, IMasterConnection con, CS104_PeerConnectionEvent event)
{
    if (event == CS104_CON_EVENT_CONNECTION_OPENED) {
        printf("Connection opened (%p)\n", con);
        qDebug() << QString("Connection opened ");
    }
    else if (event == CS104_CON_EVENT_CONNECTION_CLOSED) {
        printf("Connection closed (%p)\n", con);
        qDebug() << QString("Connection closed ");
    }
    else if (event == CS104_CON_EVENT_ACTIVATED) {
        printf("Connection activated (%p)\n", con);
        qDebug() << QString("Connection activated ");

    }
    else if (event == CS104_CON_EVENT_DEACTIVATED) {
        printf("Connection deactivated (%p)\n", con);
        qDebug() << QString("Connection deactivated ");

    }
}

void init_iec_server(const char* address)
{
    /* Add Ctrl-C handler */
    signal(SIGINT, sigint_handler);

    /* create a new slave/server instance with default connection parameters and
     * default message queue size */
    CS104_Slave slave = CS104_Slave_create(10, 10);

    CS104_Slave_setLocalAddress(slave, address);//"0.0.0.0"

    /* Set mode to a single redundancy group
     * NOTE: library has to be compiled with CONFIG_CS104_SUPPORT_SERVER_MODE_SINGLE_REDUNDANCY_GROUP enabled (=1)
     */
    CS104_Slave_setServerMode(slave, CS104_MODE_SINGLE_REDUNDANCY_GROUP);

    /* get the connection parameters - we need them to create correct ASDUs -
     * you can also modify the parameters here when default parameters are not to be used */
    CS101_AppLayerParameters alParams = CS104_Slave_getAppLayerParameters(slave);

    /* when you have to tweak the APCI parameters (t0-t3, k, w) you can access them here */
    CS104_APCIParameters apciParams = CS104_Slave_getConnectionParameters(slave);

    qDebug() << "APCI parameters:" ;
    qDebug() << QString("  t0: %0").arg(apciParams->t0)
     << QString("  t1: %0").arg(apciParams->t1)
     << QString("  t2: %0").arg(apciParams->t2)
     << QString("  t3: %0").arg(apciParams->t3)
     << QString("  k: %0").arg(apciParams->k)
     << QString("  w: %0").arg(apciParams->w);

    /* set the callback handler for the clock synchronization command */
    CS104_Slave_setClockSyncHandler(slave, clockSyncHandler, NULL);

    /* set the callback handler for the interrogation command */
    CS104_Slave_setInterrogationHandler(slave, interrogationHandler, NULL);

    /* set handler for other message types */
    CS104_Slave_setASDUHandler(slave, asduHandler, NULL);

    /* set handler to handle connection requests (optional) */
    CS104_Slave_setConnectionRequestHandler(slave, connectionRequestHandler, NULL);

    /* set handler to track connection events (optional) */
    CS104_Slave_setConnectionEventHandler(slave, connectionEventHandler, NULL);

    /* uncomment to log messages */
    //CS104_Slave_setRawMessageHandler(slave, rawMessageHandler, NULL);

    CS104_Slave_start(slave);

    /*if (CS104_Slave_isRunning(slave) == false) {
        printf("Starting server failed!\n");
        goto exit_program;
    }*/

    int16_t scaledValue = 0;
    running = true;
    while (running)
    {
        Thread_sleep(1000);

        CS101_ASDU newAsdu = CS101_ASDU_create(alParams, false, CS101_COT_PERIODIC, 0, 1, false, false);

        InformationObject io = (InformationObject) MeasuredValueScaled_create(NULL, 110, scaledValue, IEC60870_QUALITY_GOOD);

        scaledValue++;

        CS101_ASDU_addInformationObject(newAsdu, io);

        InformationObject_destroy(io);

        /* Add ASDU to slave event queue */
        CS104_Slave_enqueueASDU(slave, newAsdu);

        CS101_ASDU_destroy(newAsdu);
    }

    qDebug() << "stopped in init";

    CS104_Slave_stop(slave);

//exit_program:
    CS104_Slave_destroy(slave);

    Thread_sleep(500);
}

/*
void init_iec_serverTable(const char* address, ServerVariant* server)
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

    CS104_Slave_setClockSyncHandler(slave, clockSyncHandler, NULL);
    CS104_Slave_setConnectionRequestHandler(slave, connectionRequestHandler, NULL);
    CS104_Slave_setConnectionEventHandler(slave, connectionEventHandler, NULL);
    CS104_Slave_start(slave);

    int16_t scaledValue = 0;

    while (server->state)
    {
        Thread_sleep(1000);

        CS101_ASDU newAsdu = CS101_ASDU_create(alParams, false, CS101_COT_PERIODIC, 0, 1, false, false);
        //InformationObject io = (InformationObject) MeasuredValueScaled_create(NULL, 110, scaledValue, IEC60870_QUALITY_GOOD);
        InformationObject io;
        for (int i = 0; i < server->m_tags_SP.length(); i++)
        {
            if (i == 0)
                io = (InformationObject) SinglePointInformation_create(NULL, server->m_tags_SP[i].iot,
                                                                       server->m_tags_SP[i].value, IEC60870_QUALITY_GOOD);

            CS101_ASDU_addInformationObject(newAsdu,
                                            (InformationObject) SinglePointInformation_create((SinglePointInformation) io,
                                            server->m_tags_SP[i].iot, server->m_tags_SP[i].value, IEC60870_QUALITY_GOOD));

        }
        CS101_ASDU_addInformationObject(newAsdu, io);
        InformationObject_destroy(io);
        CS104_Slave_enqueueASDU(slave, newAsdu);
        CS101_ASDU_destroy(newAsdu);


        newAsdu = CS101_ASDU_create(alParams, false, CS101_COT_PERIODIC, 0, 1, false, false);
        //InformationObject io =

        for (int i = 0; i < server->m_tags_MV.length(); i++)
        {
            if (i == 0)
                io = (InformationObject) MeasuredValueScaled_create(NULL, server->m_tags_MV[i].iot,
                                                                       server->m_tags_MV[i].value, IEC60870_QUALITY_GOOD);

            CS101_ASDU_addInformationObject(newAsdu,
                                            (InformationObject) MeasuredValueScaled_create((MeasuredValueScaled) io,
                                            server->m_tags_MV[i].iot, server->m_tags_MV[i].value, IEC60870_QUALITY_GOOD));

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
*/
