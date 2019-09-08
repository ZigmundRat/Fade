// SequenceControllerTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Assert.h"
#include "Serial.h"
#include "CommandSourceTest.h"
#include "LedState.h"
#include "LedPwm.h"
#include "BrightnessTarget.h"
#include "LedCommand.h"
#include "VariableTest.h"
#include "StackTest.h"
#include "CommandDecoderTest.h"
#include "LedManagerTest.h"
#include "CommandSourceSimulatorTest.h"
#include "ExecutionFlowTest.h"
#include "TimebaseTest.h"
#include "IntegrationTest.h"
#include "LoopTest.h"


int main()
{
	CommandSourceSimulatorTest::Test();
	
	ExecutionFlowTest::Run();
	TimebaseTest::Run();
	CommandSourceTest::Run();
	LedManagerTest::Run();
	CommandDecoderTest::Run();
	
	IntegrationTest::Run();

	LoopTest::Run();

	VariableTest::Run();

	StackTest::Run();

    return 0;
}

