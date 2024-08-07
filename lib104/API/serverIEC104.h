#ifndef SERVERIEC104_H
#define SERVERIEC104_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "cs104_slave.h"

#include "hal_thread.h"
#include "hal_time.h"

static bool running = true;

void someshit_handler();
void sigint_handler(int signalId);
void printCP56Time2a(CP56Time2a time);
static void rawMessageHandler(void* parameter, IMasterConnection conneciton, uint8_t* msg, int msgSize, bool sent);
static bool clockSyncHandler (void* parameter, IMasterConnection connection, CS101_ASDU asdu, CP56Time2a newTime);
static bool interrogationHandler(void* parameter, IMasterConnection connection, CS101_ASDU asdu, uint8_t qoi);
static bool asduHandler(void* parameter, IMasterConnection connection, CS101_ASDU asdu);
static bool connectionRequestHandler(void* parameter, const char* ipAddress);
void init_iec_server(const char* address);
//void init_iec_serverTable(const char* address, ServerVariant* server);

#endif // SERVERIEC104_H
