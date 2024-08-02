#ifndef SIMPLE_SERVER_H
#define SIMPLE_SERVER_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "cs104_slave.h"

#include "hal_thread.h"
#include "hal_time.h"

static bool running = true;


void sigint_handler(int signalId);
void printCP56Time2a(CP56Time2a time);
static void rawMessageHandler(void* parameter, IMasterConnection conneciton, uint8_t* msg, int msgSize, bool sent);
static bool clockSyncHandler (void* parameter, IMasterConnection connection, CS101_ASDU asdu, CP56Time2a newTime);
static bool interrogationHandler(void* parameter, IMasterConnection connection, CS101_ASDU asdu, uint8_t qoi);
static bool asduHandler(void* parameter, IMasterConnection connection, CS101_ASDU asdu);
static bool connectionRequestHandler(void* parameter, const char* ipAddress);
int init_iec_server();



#endif // SIMPLE_SERVER_H
