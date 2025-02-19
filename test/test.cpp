

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <assert.h>
/*
#include "netwrapper.h"
#include "AddressInterceptPass.h"
#include "exports.h"
#include "logger.h"
#include "assertion.h"
#include "obusfaction.h"
#include "logofun.h"
*/
#include "Ir.h"
#include "remcu.h"


#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wextra"
#pragma clang diagnostic ignored "-Wmacro-redefined"

using namespace std;

static bool error = false;

static void callback(){
    cout << "callback()" << endl;
    error = true;
}

#ifdef NDEBUG
    #define assert
#endif

static const uint16_t PORT_TCL = 6666;
static const uint16_t PORT_GDB = 3333;
static const int DEFAULT_TIMEOUT = 1;

#define _STRING_ "123456789abc"

void assertErrorTest(uint32_t address){
    std::cout << "\n----------------------- Test Error -----------------------\n" << endl;

    //remcu_setErrorSignalFunc(callback);
    assert(remcu_getErrorCount() == 0);
    assert(error == false);
    simpleTest(reinterpret_cast<int*>(address));
    assert(remcu_getErrorCount() > 0);
    //assert(error == true);
    error = false;
    remcu_clearErrorCount();

    //remcu_setErrorSignalFunc(NULL);
    simpleTest(reinterpret_cast<int*>(address));
    assert(error == false);
    remcu_clearErrorCount();
}

void standartTestAddr(uintptr_t address){
    int sanbox[111] = {0};
    int ret = 0;

    assert(remcu_debuggerTest() == NULL);

    assert(remcu_isConnected());

    const char* error = remcu_debuggerTest();

    assert(error == NULL);

    #define _SIZE 30
    uint8_t testMessage[_SIZE];
    const size_t qty_write = _SIZE - 1;
    for(int i =0; i < qty_write; i++)
        testMessage[i] = '0' + i;

    testMessage[_SIZE - 1] = '\0';

    uint8_t dist[100] = {'\0'};

    assert(remcu_store2mem(address, testMessage, _SIZE));

    assert(remcu_loadFrMem(address, _SIZE, dist));

    ret = strncmp((char*)testMessage, (char*)dist, _SIZE);

    assert(ret == 0);

    assert(remcu_store2mem(address, testMessage, MAX_MEM_OPERATION_SIZE) == true);
    assert(remcu_store2mem(address, testMessage, MAX_MEM_OPERATION_SIZE + 1) == false);


    assert(remcu_store2mem(address, testMessage, MIN_MEM_OPERATION_SIZE) == true);
    assert(remcu_store2mem(address, testMessage, MIN_MEM_OPERATION_SIZE - 1) == false);

    remcu_disconnect();

    assertErrorTest(address);

    assert(remcu_isConnected() == false);
}

int main(int argc, char** argv)
{
#define _SIZE_VERSION 100

    assert(remcu_getVersion());
    std::cout << "version : " << remcu_getVersion() << endl;

    if(argc < 3){
        printf("test requare 2 arguments: host and verbose level\n");
        printf("optional 3-d arg: testOpenocd(bool)\n");
        return -1;
    }

    int ret = 0;

    const string host(argv[1]);
    const uintptr_t address = 0x20000000;
    const LevelDebug level = static_cast<LevelDebug>(atoi(argv[2]) & 0xF);
    bool testOpenocd = true;

    printf("argc: %d '%s'\n", argc, argv[3]);

    if(argc > 3){
        if(string(argv[3]).compare("no") == 0){
            testOpenocd = false;
        }
    }

    cout << "host: " << host << endl;
    cout << "address: 0x" << hex << address << endl;
    cout << "Verbose Level: " << level << endl;
    remcu_setVerboseLevel(level);

    assertErrorTest(address);

    assert(remcu_isConnected() == false);

    if(testOpenocd){
        std::cout << "\n----------------------- Test OpenOCD client -----------------------\n" << endl;

        assert(remcu_connect2OpenOCD(host.c_str(), PORT_TCL, DEFAULT_TIMEOUT));

        assert(remcu_resetRemoteUnit(__RUN));

        assert(remcu_resetRemoteUnit(__HALT));

        standartTestAddr(address);
    }

    std::cout << "\n----------------------- Test RSP GDB client -----------------------\n" << endl;

    assert(remcu_connect2GDB(host.c_str(), PORT_GDB, DEFAULT_TIMEOUT));

    assert(remcu_resetRemoteUnit(__HALT));

    assert(remcu_resetRemoteUnit(__RUN) == false);

    standartTestAddr(address);

    std::cout << "\n----------------------- SUCCESS TEST -----------------------\n" << endl;

    
}

