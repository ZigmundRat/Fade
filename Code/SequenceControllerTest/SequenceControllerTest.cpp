// SequenceControllerTest.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

//void delayMicroseconds(int delay) {}
//void vTaskDelay(int delay) {}
//int micros() { return 0; }

// Callback methods for WinFade Integration

void LedRenderCycleDone() {}
void ParseErrorFound(const char* pError, int lineNumber) {}


#include "Environment.h"

#include "Assert.h"
#include "SafeStringTest.h"
#include "Serial.h"
#include "TimeServices.h"
#include "EspFunctions.h"
#include "MyRandom.h"
#include "StackWatcher.h"
#include "Instrumentation.h"
#include "Profiler.h"
#include "VectorTest.h"
#include "DelayerTest.h"
#include "SystemCallback.h"
#include "InternedStringsTest.h"
#include "CommandTest.h"
#include "ParseErrorsTest.h"
#include "CommandSourceTest.h"
#include "CommandFormatterTest.h"
#include "VariableDataTest.h"
#include "VariableStoreChunkTest.h"
#include "VariableStoreTest.h"
#include "VariableTest.h"
#include "VariableCollectionTest.h"
#include "ExpressionResult.h"
#include "LedState.h"
#include "LedType.h"
#include "CommandResult.h"
#include "ILedManager.h"
#include "ILedDevice.h"
#include "ILedDeviceCreator.h"
#include "FunctionStoreTest.h"
#include "StackTest.h"
#include "IExecutionFlow.h"
#include "IExecutionContext.h"
#include "ExpressionTokenSourceTest.h"
#include "UdpLogger.h"
#include "BuiltInFunctions.h"
#include "RDEvaluater.h"
#include "ExecutionContextTest.h"
#include "RDEvaluaterTest.h"
#include "LedGroupsTest.h"
#include "LedManagerTest.h"
#include "CommandSourceSimulatorTest.h"
#include "ILedMessageHandler.h"
#include "IButton.h"
#include "IButtonCreator.h"
#include "ExecutionFlowTest.h"
#include "TimebaseTest.h"
#include "IntegrationTest.h"
#include "Settings.h"
#include "SupervisorTest.h"
#include "BuiltInFunctionsTest.h"
#include "ExpressionResultTest.h"


int main()
{
  InternedStringsTest::Run();
  
  ExpressionResultTest::Run();

  VectorTest::Run();

  VariableDataTest::Run();
  VariableCollectionTest::Run();
  VariableTest::Run();
  VariableStoreChunkTest::Run();
  VariableStoreTest::Run();

  ExecutionContextTest::Run();

  SafeStringTest::Run();

  ExpressionTokenSourceTest::Run();
  RDEvaluaterTest::Run();

  DelayerTest::Run();

  ExecutionFlowTest::Run();
  IntegrationTest::Run();

  CommandSourceSimulatorTest::Test();

  LedGroupsTest::Run();
  LedManagerTest::Run();
  TimebaseTest::Run();
  CommandSourceTest::Run();

  //VariableTest::Run();

  StackTest::Run();
  FunctionStoreTest::Run();

  CommandTest::Run();

  CommandFormatterTest::Run();

  ParseErrorsTest::Run();

  SupervisorTest::Run();

  BuildInFunctionsTest::Run();

  printf("%d assertions checked\n", Assert::GetAssertCount());

  ExpressionTokenSource expressionTokenSource("Test");

  return 0;

  Serial.SetOutput(1);
  for (int i = 0; i < 256; i++)
  {
    char c = i;
    int isDigit = expressionTokenSource.IsDigit(c);
    int isAlpha = expressionTokenSource.IsAlpha(c);
    int isUnderscore = c == '_';
    int isDot = c == '.';

    int full = isDigit | isAlpha << 1 | isUnderscore << 2 | isDot << 3;

    Serial.print(full);
    Serial.print(", ");

    Serial.print("/* ");
    if (i > 32 && i < 128)
    {
      char v[2];
      v[0] = i;
      v[1] = 0;
      Serial.print(v);
    }

    else
    {
      Serial.print(i);
    }
    Serial.println(" */");
  }

  return 0;
}

