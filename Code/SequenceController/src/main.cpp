#include <Arduino.h>
#include <arduinonvs.h>
#include <stdio.h>
#include <stdlib.h>

#include <Preferences.h>
#include <WiFi.h>

#define strncpy_s strncpy
#define strncmp_s strncmp

#include "SafeString.h"

#include "Profiler.h"
#include "StackWatcher.h"
#include "Instrumentation.h"
#include "SystemCallback.h"
#include "MyRandom.h"
#include "TimeServices.h"
#include "Delayer.h"
#include "Command.h"
#include "ListParser.h"
#include "CommandSource.h"
#include "CommandFormatter.h"
#include "ParseErrors.h"
#include "Variable.h"
#include "LedState.h"
#include "CommandResult.h"
#include "IExecutionFlow.h"
#include "ILedDevice.h"
#include "LedPwmEsp32.h"
#include "LedRGB.h"
#include "ILedDeviceCreator.h"
#include "LedDeviceCreator.h"
#include "LedManager.h"
#include "FunctionStore.h"
#include "Stack.h"
#include "ExpressionNode.h"
#include "ExpressionTokenSource.h"
#include "IExecutionContext.h"
#include "FunctionCaller.h"
#include "RDEvaluater.h"
#include "ExecutionContext.h"
#include "CommandDecoder.h"
#include "ILedMessageHandler.h"
#include "ExecutionFlow.h"
#include "Timebase.h"
#include <WiFi.h>

#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <UdpLogger.h>

#include <ArduinoNvs.h>
#include <Settings.h>
#include <Supervisor.h>

#include "MyWebServer.h"

LedDeviceCreator *_pLedDeviceCreator;
ILedDevice *_pLedDevice;
LedManager *_pLedManager;

Supervisor *_pSupervisor;
Settings *_pSettings;

MyWebServer *pMyWebServer;

void Callback()
{
    //Serial.println("Callback");
    //pMyWebServer->HandleClient();

    //TrackMemory();

    //delay(10);
}

void RunDim(void *parameter)
{
    Serial.println("RunDim Start");
    StackWatcher::Init();
    //StackWatcher::Enable();

    StackWatcher::Log("Task Start");
    _pSupervisor->ExecuteLoop();
}

void HandleWebClient(void *parameter)
{
    Serial.println("HandleWedClient Start");
    while (true)
    {
        pMyWebServer->HandleClient();
        delay(25);
    }
}

void setup()
{

    Serial.begin(115200);
    Serial.println("setup");
    Serial.flush();
    StackWatcher::Log("setup");
    //StackWatcher::Enable();
    _pSupervisor = new Supervisor();
    _pSettings = new Settings();
    WiFi.setHostname(_pSupervisor->GetNodeName());

    //_pLedPwm = new LedPwmEsp32();
    //_pLedManager = new LedManager(_pLedPwm, 16);

    _pLedDeviceCreator = new LedDeviceCreator();
    //_pLedDevice = new LedRGB(33, 13);

    WiFiManager wifiManager;

    wifiManager.autoConnect("SequenceController", "12345678");
    //wifiManager.startConfigPortal("SequenceController", "12345678");
    Serial.print("after autoconnect: ");
    Serial.println(WiFi.localIP());

    pMyWebServer = new MyWebServer(_pSupervisor, WiFi.localIP());

    _pSettings->Init();
    _pLedManager = new LedManager(_pLedDeviceCreator);
    _pSupervisor->Init(_pLedManager, _pSettings, Callback);

    Serial.println("Setup completed");

    xTaskCreate(
        RunDim, /* Task function. */
        "Dim",  /* String with name of task. */
        40000,  /* Stack size in bytes. */
        NULL,   /* Parameter passed as input of the task */
        5,      /* Priority of the task. */
        NULL);  /* Task handle. */

    xTaskCreate(
        HandleWebClient,   /* Task function. */
        "HandleWebClient", /* String with name of task. */
        10000,             /* Stack size in bytes. */
        NULL,              /* Parameter passed as input of the task */
        5,                 /* Priority of the task. */
        NULL);             /* Task handle. */
}

int iterations = 0;

void TrackMemory()
{
    if (iterations % 100 == 0)
    {
        Serial.print(iterations);
        Serial.print(" = ");
        Serial.println(ESP.getFreeHeap());
    }
    iterations++;
}

void loop()
{
    //pMyWebServer->HandleClient();
    vTaskDelay(10);

    //StackWatcher::Log("loop");
    //_pSupervisor->ExecuteLoop();

    //pMyWebServer->HandleClient();

    //supervisor.Execute();

    //TrackMemory();

    //delay(10);
}
