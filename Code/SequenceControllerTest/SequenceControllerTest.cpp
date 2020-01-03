// SequenceControllerTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Assert.h"
#include "Serial.h"
#include "MyRandom.h"
#include "TickSource.h"
#include "DelayerTest.h"
#include "SystemCallback.h"
#include "CommandTest.h"
#include "ListParserTest.h"
#include "ParseErrorsTest.h"
#include "CommandSourceTest.h"
#include "CommandFormatterTest.h"
#include "LedState.h"
#include "LedPwm.h"
#include "CommandResult.h"
#include "LedCommand.h"
#include "VariableTest.h"
#include "ExpressionTokenizerTest.h"
#include "FunctionStoreTest.h"
#include "StackTest.h"
#include "IExecutionFlow.h"
#include "ExpressionTest.h"
#include "CommandDecoderTest.h"
#include "LedManagerTest.h"
#include "CommandSourceSimulatorTest.h"
#include "ExecutionFlowTest.h"
#include "TimebaseTest.h"
#include "IntegrationTest.h"
#include "LoopTest.h"
#include "Settings.h"
#include "SupervisorTest.h"


int main()
{
	ExpressionTokenizerTest::Run();
	DelayerTest::Run();

	ExecutionFlowTest::Run();
	IntegrationTest::Run();

	CommandSourceSimulatorTest::Test();
	
	TimebaseTest::Run();
	CommandSourceTest::Run();
	LedManagerTest::Run();
	CommandDecoderTest::Run();

	LoopTest::Run();

	VariableTest::Run();
	ExpressionTest::Run();

	StackTest::Run();
	FunctionStoreTest::Run();

	ListParserTest::Run();
	CommandTest::Run();

	CommandFormatterTest::Run();

	ParseErrorsTest::Run();

	SupervisorTest::Run();

	printf("%d assertions checked\n", Assert::GetAssertCount());

    return 0;
}

