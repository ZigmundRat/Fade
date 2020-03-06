#pragma once

#include "ExecutionFlow.h"

#define ResultStorageSize 100

class ExecutionFlowTest
{
	static CommandResult _commandResults[ResultStorageSize];
	static int _commandResultCount;

	static void CommandResultCallback(CommandResult* pCommandResult)
	{
		_commandResults[_commandResultCount] = *pCommandResult;
		_commandResultCount++;
	}

	static void ResetResultStore()
	{
		for (int i = 0; i < _commandResultCount; i++)
		{
			_commandResults[i] = CommandResult();
		}
			
		_commandResultCount = 0;
	}

	static void RunProgram(CommandSourceSimulator* pCommandSource)
	{
		ParseErrors parseErrors;
		ExecutionFlow executionFlow(pCommandSource, &parseErrors, CommandResultCallback);

		ResetResultStore();
		executionFlow.RunProgram(1);
		Assert::AreEqual(0, parseErrors.GetErrorCount());
	}

	static void Test()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;

		commandSource.AddCommand("D(10,0,10.0)");
		commandSource.AddCommand("A(10)");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);
		AssertResult(_commandResults[0], 10, 0, 10.0F);
	}

	static void TestAutoRestart()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;

		commandSource.AddCommand("DI(10,0,10.0)");

		ExecutionFlow executionFlow(&commandSource, &parseErrors, CommandResultCallback);

		ResetResultStore();
		executionFlow.RunProgram(2);

		Assert::AreEqual(2, _commandResultCount);
		AssertResult(_commandResults[0], 10, 0, 10.0F);
		AssertResult(_commandResults[1], 10, 0, 10.0F);
	}

	static void AssertResult(CommandResult commandResult, int cycleCount, int channel, float brightness)
	{
		Assert::AreEqual(cycleCount, commandResult.GetCycleCount());
		LedState ledState = commandResult.GetTarget(0);
		Assert::AreEqual(channel, ledState.GetChannel());
		Assert::AreEqual(brightness, ledState.GetBrightness()->GetValueFloat(0));
	}

	static void AssertResult(LedCommand ledCommand, int cycleCount, int channel, float brightness)
	{
		Assert::AreEqual(cycleCount, ledCommand._commandResult.GetCycleCount());
		LedState ledState = ledCommand._commandResult.GetTarget(0);
		Assert::AreEqual(channel, ledState.GetChannel());
		Assert::AreEqual(brightness, ledState.GetBrightness()->GetValueFloat(0));
	}

	static void TestLoop()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR B 0:7");
		commandSource.AddCommand("D(7,B,10.0)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		RunProgram(&commandSource);

		Assert::AreEqual(9, _commandResultCount);

		for (int i = 0; i <= 7; i++)
		{
			AssertResult(_commandResults[i], 7, i, 10.0F);
		}

		// should be command outside of loop
		AssertResult(_commandResults[8], 15, 15, 15.0F);
	}

	static void TestNestedLoop()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR B 0:1");
		commandSource.AddCommand("FOR D 3:4");
		commandSource.AddCommand("D(7,B,D)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		RunProgram(&commandSource);

		Assert::AreEqual(5, _commandResultCount);

		AssertResult(_commandResults[0], 7, 0, 3.0F);
		AssertResult(_commandResults[1], 7, 0, 4.0F);
		AssertResult(_commandResults[2], 7, 1, 3.0F);
		AssertResult(_commandResults[3], 7, 1, 4.0F);

		// should be command outside of loop
		AssertResult(_commandResults[4], 15, 15, 15.0F);
	}

	static void TestLoopWithIncrement()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR D 0:1:0.5");
		commandSource.AddCommand("D(7,1,D)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		RunProgram(&commandSource);

		Assert::AreEqual(4, _commandResultCount);

		AssertResult(_commandResults[0], 7, 1, 0.0F);
		AssertResult(_commandResults[1], 7, 1, 0.5F);
		AssertResult(_commandResults[2], 7, 1, 1.0F);

		// should be command outside of loop
		AssertResult(_commandResults[3], 15, 15, 15.0F);
	}

	static void TestLoopWithVariableCount()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR B 10:20:10");
		commandSource.AddCommand("D(B,0,10.0)");
		commandSource.AddCommand("A(B)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		RunProgram(&commandSource);

		Assert::AreEqual(3, _commandResultCount);

		AssertResult(_commandResults[0], 10, 0, 10.0F);
		AssertResult(_commandResults[1], 20, 0, 10.0F);

		// should be command outside of loop
		AssertResult(_commandResults[2], 15, 15, 15.0F);
	}

	static void TestLoopWithFunctionCall()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC GetValue");
		commandSource.AddCommand("RETURN 7");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("FOR B 0:GetValue()");
		commandSource.AddCommand("D(7,B,10.0)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		RunProgram(&commandSource);

		Assert::AreEqual(9, _commandResultCount);

		for (int i = 0; i <= 7; i++)
		{
			AssertResult(_commandResults[i], 7, i, 10.0F);
		}

		// should be command outside of loop
		AssertResult(_commandResults[8], 15, 15, 15.0F);
	}


	static void TestLoopDown()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR B 6:0:-3");
		commandSource.AddCommand("D(7,B,10.0)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		RunProgram(&commandSource);

		Assert::AreEqual(4, _commandResultCount);

		AssertResult(_commandResults[0], 7, 6, 10.0F);
		AssertResult(_commandResults[1], 7, 3, 10.0F);
		AssertResult(_commandResults[2], 7, 0, 10.0F);

		// should be command outside of loop
		AssertResult(_commandResults[3], 15, 15, 15.0F);
	}

	static void TestLoopDownWithVariables()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Start=6");
		commandSource.AddCommand("End=0");
		commandSource.AddCommand("Inc=-3");
		commandSource.AddCommand("FOR B Start:End:Inc");
		commandSource.AddCommand("D(7,B,10.0)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		RunProgram(&commandSource);

		Assert::AreEqual(4, _commandResultCount);

		AssertResult(_commandResults[0], 7, 6, 10.0F);
		AssertResult(_commandResults[1], 7, 3, 10.0F);
		AssertResult(_commandResults[2], 7, 0, 10.0F);

		// should be command outside of loop
		AssertResult(_commandResults[3], 15, 15, 15.0F);
	}

	static void TestLoopImmediate()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR B 6:0:-3");
		commandSource.AddCommand("DI(7,B,10.0)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("DI(15,15,15.0)");

		RunProgram(&commandSource);

		Assert::AreEqual(4, _commandResultCount);

		AssertResult(_commandResults[0], 7, 6, 10.0F);
		AssertResult(_commandResults[1], 7, 3, 10.0F);
		AssertResult(_commandResults[2], 7, 0, 10.0F);

		// should be command outside of loop
		AssertResult(_commandResults[3], 15, 15, 15.0F);
	}

	static void TestNestedLoopWithLongNames()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR Bravo 0:1");
		commandSource.AddCommand("FOR Delta 3:4");
		commandSource.AddCommand("D(7,Bravo,Delta)");
		commandSource.AddCommand("A(7)");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("D(15,15,15.0)");
		commandSource.AddCommand("A(15)");

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, CommandResultCallback);
	}

	static void TestOverlappingAnimation()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("D(10,1,1.0)");
		commandSource.AddCommand("A(5)");
		commandSource.AddCommand("D(5,2,1.0)");
		commandSource.AddCommand("A(5)");

		RunProgram(&commandSource);

		Assert::AreEqual(2, _commandResultCount);

		AssertResult(_commandResults[0], 5, 1, 1.0F);
		AssertResult(_commandResults[1], 5, 2, 1.0F);
	}

	static void TestMissingAnimation()
	{
		CommandSourceSimulator commandSource;
		commandSource.AddCommand("D(10,1,1.0)");

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, CommandResultCallback);

		ResetResultStore();
		CommandResultStatus commandResultStatus = executionFlow.RunProgram();

		Assert::AreEqual((int) CommandResultStatus::CommandTargetCountExceeded, (int) commandResultStatus);
	}

	static void TestMissingEndFor()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR B 0:7");

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, CommandResultCallback);

		ResetResultStore();
		executionFlow.RunProgram(1);

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		ParseError parseError = parseErrors.GetError(0);
		Assert::AreEqual("Missing loop end", parseError._errorText);
	}

	static void TestInvalidStatement()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("guar");

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, CommandResultCallback);

		ResetResultStore();
		executionFlow.RunProgram(1);

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		ParseError parseError = parseErrors.GetError(0);
		Assert::AreEqual("Unrecognized identifier: guar", parseError._errorText);
	}

	static void TestDoubleLoop()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FOR A 0:6");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("FOR A 7:1:-1");
		commandSource.AddCommand("ENDFOR");
		commandSource.AddCommand("A(1)");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);
	}

	static void TestPrint()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("PL(\"Hello\")");
		commandSource.AddCommand("DI(1, 2, 1.0)");

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, CommandResultCallback);

		Serial.SetOutput(false);
		executionFlow.RunProgram(1);
		Serial.SetOutput(true);

		Assert::AreEqual("Hello\n", Serial.GetLastString());
	}

	static void TestFunctionDef()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("DI(1, 2, 1.0)");

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, CommandResultCallback);

		executionFlow.RunProgram(1);
		Assert::AreEqual(1, executionFlow.GetExecutionContext()._functionStore.GetCount());
		FunctionDefinition* pFunction = executionFlow.GetExecutionContext()._functionStore.Lookup("Function");
		Assert::AreEqual("Function", pFunction->Name);
		Assert::AreEqual(0, pFunction->SerialNumberStart);
		Assert::AreEqual(1, pFunction->SerialNumberEnd);

		ExecutionContext executionContext = executionFlow.GetExecutionContext();
	}

	static void TestFunctionCall()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function");
		commandSource.AddCommand("RETURN 10.0");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("A=Function()");
		commandSource.AddCommand("DI(A, 2, 1.0)");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 10, 2, 1.0F);
	}

	static void TestFunctionCallNested()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function");
		commandSource.AddCommand("RETURN 11.0");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("FUNC FunctionOuter");
		commandSource.AddCommand("A=Function()");
		commandSource.AddCommand("RETURN A");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("B=FunctionOuter()");
		commandSource.AddCommand("DI(B, 2, 1.0)");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 11, 2, 1.0F);
	}

	static void TestFunctionCallAsArgument()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function(A, B)");
		commandSource.AddCommand("RETURN A");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("FUNC Function2(X, Y, Z)");
		commandSource.AddCommand("RETURN X + Y + Z");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("B=Function2(3, Function(1, 3), 6)");
		commandSource.AddCommand("DI(B, 2, 1.0)");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 10, 2, 1.0F);
	}

	static void TestFunctionCallRandomParameter()
	{
		CommandSourceSimulator commandSource;

		MyRandom::SetFirstValue(13);

		commandSource.AddCommand("FUNC Function(A)");
		commandSource.AddCommand("RETURN A");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("B=Function(RAND(0,10))");
		commandSource.AddCommand("DI(B, 2, 1.0)");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 13, 2, 1.0F);
	}

	static void TestIgnoredFunctionResult()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function");
		commandSource.AddCommand("RETURN 10.0");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function()");
		commandSource.AddCommand("DI(5, 2, 1.0)");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 5, 2, 1.0F);
	}

	static void TestMethodCall()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function");
		commandSource.AddCommand("PL(\"Hello\")");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function()");
		commandSource.AddCommand("DI(1, 2, 1.0)");

		Serial.SetOutput(false);
		RunProgram(&commandSource);
		Serial.SetOutput(true);

		Assert::AreEqual("Hello\n", Serial.GetLastString());
	}

	static void TestMethodCallWithParameter()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function(V)");
		commandSource.AddCommand("PL(V)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function(13)");

		Serial.SetOutput(false);
		RunProgram(&commandSource);
		Serial.SetOutput(true);

		Assert::AreEqual("13.000000\n", Serial.GetLastString());
	}

	static void TestMethodCallWithTwoParameters()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function(V, W)"); // ignore leading whitespace... - add it to the ListParser 
		commandSource.AddCommand("PL(W)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function(1, 12)");

		Serial.SetOutput(false);
		RunProgram(&commandSource);
		Serial.SetOutput(true);

		Assert::AreEqual("12.000000\n", Serial.GetLastString());
	}

	static void TestMethodCallWithParameterDuplicateName()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("V=15.0");
		commandSource.AddCommand("FUNC Function(V)");
		commandSource.AddCommand("PL(V)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function(13)");

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, CommandResultCallback);

		Serial.SetOutput(false);
		executionFlow.RunProgram(1);
		Serial.SetOutput(true);

		Assert::AreEqual("13.000000\n", Serial.GetLastString());
		Assert::AreEqual(15.0F, executionFlow.GetExecutionContext()._variables.GetWithoutErrorCheck("V", 1)->GetValueFloat(0));
	}

	static void TestMethodCannotAccessParentVariables()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("V=15.0");
		commandSource.AddCommand("FUNC Function()");
		commandSource.AddCommand("PL(V)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function()");

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, CommandResultCallback);

		Serial.SetOutput(false);
		executionFlow.RunProgram(1);
		Serial.SetOutput(true);

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		ParseError parseError = parseErrors.GetError(0);
		Assert::AreEqual("Unrecognized identifier: V", parseError._errorText);
	}

	static void TestMethodCallWithWrongArgumentCount()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function(X)");
		commandSource.AddCommand("PL(X)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function()");

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, CommandResultCallback);

		Serial.SetOutput(false);
		executionFlow.RunProgram(1);
		Serial.SetOutput(true);

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		ParseError parseError = parseErrors.GetError(0);
		Assert::AreEqual("Mismatched Function Function requires 1 parameter(s) but was called with 0 argument(s).", parseError._errorText);
		Assert::AreEqual(3, parseError._lineNumber);
	}

	static void TestMethodCallWithWrongArgumentCount2()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("FUNC Function(X)");
		commandSource.AddCommand("PL(X)");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("Function(15.0, 35.0)");

		ParseErrors parseErrors;
		ExecutionFlow executionFlow(&commandSource, &parseErrors, CommandResultCallback);

		Serial.SetOutput(false);
		executionFlow.RunProgram(1);
		Serial.SetOutput(true);

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		ParseError parseError = parseErrors.GetError(0);
		Assert::AreEqual("Mismatched Function Function requires 1 parameter(s) but was called with 2 argument(s).", parseError._errorText);
		Assert::AreEqual(3, parseError._lineNumber);
	}

	static void TestDirectWithFunctionCall()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;

		commandSource.AddCommand("FUNC F(X)");
		commandSource.AddCommand("RETURN X");
		commandSource.AddCommand("ENDFUNC");
		commandSource.AddCommand("DI(F(1), F(2), F(3))");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);
		AssertResult(_commandResults[0], 1, 2, 3.0F);
	}

	static void TestIfTrue()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=3");
		commandSource.AddCommand("IF Value==3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ENDIF");
		commandSource.AddCommand("DI(15,15,15.0)");

		RunProgram(&commandSource);

		Assert::AreEqual(2, _commandResultCount);

		AssertResult(_commandResults[0], 7, 7, 10.0F);
		AssertResult(_commandResults[1], 15, 15, 15.0F);
	}

	static void TestIfFalse()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=2");
		commandSource.AddCommand("IF Value==3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ENDIF");
		commandSource.AddCommand("DI(15,15,15.0)");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 15, 15, 15.0F);
	}

	static void TestIfElseTrue()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=3");
		commandSource.AddCommand("IF Value==3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("DI(15,15,15.0)");
		commandSource.AddCommand("ENDIF");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 7, 7, 10.0F);
	}

	static void TestIfElseFalse()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=2");
		commandSource.AddCommand("IF Value>=3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("DI(15,15,15.0)");
		commandSource.AddCommand("ENDIF");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 15, 15, 15.0F);
	}

	static void TestIfElseIfElseOne()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=3");
		commandSource.AddCommand("IF Value==3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ELSEIF Value==4");
		commandSource.AddCommand("DI(10,10,10.0)");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("DI(15,15,15.0)");
		commandSource.AddCommand("ENDIF");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 7, 7, 10.0F);
	}

	static void TestIfElseIfElseTwo()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=4");
		commandSource.AddCommand("IF Value==3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ELSEIF Value==4");
		commandSource.AddCommand("DI(10,10,10.0)");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("DI(15,15,15.0)");
		commandSource.AddCommand("ENDIF");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 10, 10, 10.0F);
	}

	static void TestIfElseIfElseThree()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=5");
		commandSource.AddCommand("IF Value==3");
		commandSource.AddCommand("DI(7,7,10.0)");
		commandSource.AddCommand("ELSEIF Value==4");
		commandSource.AddCommand("DI(10,10,10.0)");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("DI(15,15,15.0)");
		commandSource.AddCommand("ENDIF");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 15, 15, 15.0F);
	}

	static void AddCommands(CommandSourceSimulator& commandSource)
	{

		commandSource.AddCommand("IF A==3");
		commandSource.AddCommand("  IF B==3");
		commandSource.AddCommand("    DI(1,1,1.0)");
		commandSource.AddCommand("  ELSE");
		commandSource.AddCommand("    DI(2,2,2.0)");
		commandSource.AddCommand("  ENDIF");
		commandSource.AddCommand("ELSE");
		commandSource.AddCommand("  IF B==3");
		commandSource.AddCommand("    DI(3,3,3.0)");
		commandSource.AddCommand("  ELSE");
		commandSource.AddCommand("    DI(4,4,4.0)");
		commandSource.AddCommand("  ENDIF");
		commandSource.AddCommand("ENDIF");
	}

	static void TestNestedIfElse1()
	{
		CommandSourceSimulator commandSource;
		commandSource.AddCommand("A=3");
		commandSource.AddCommand("B=3");
		AddCommands(commandSource);

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 1, 1, 1.0F);
	}

	static void TestNestedIfElse2()
	{
		CommandSourceSimulator commandSource;
		commandSource.AddCommand("A=3");
		commandSource.AddCommand("B=4");
		AddCommands(commandSource);

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 2, 2, 2.0F);
	}

	static void TestNestedIfElse3()
	{
		CommandSourceSimulator commandSource;
		commandSource.AddCommand("A=1");
		commandSource.AddCommand("B=3");
		AddCommands(commandSource);

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 3, 3, 3.0F);
	}

	static void TestNestedIfElse4()
	{
		CommandSourceSimulator commandSource;
		commandSource.AddCommand("A=1");
		commandSource.AddCommand("B=2");
		AddCommands(commandSource);

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);

		AssertResult(_commandResults[0], 4, 4, 4.0F);
	}

	static void TestIncrement()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=3");
		commandSource.AddCommand("Value++");
		commandSource.AddCommand("DI(Value, Value, Value)");
		commandSource.AddCommand("Value++");
		commandSource.AddCommand("DI(Value, Value, Value)");

		RunProgram(&commandSource);

		Assert::AreEqual(2, _commandResultCount);

		AssertResult(_commandResults[0], 4, 4, 4.0F);
		AssertResult(_commandResults[1], 5, 5, 5.0F);
	}

	static void TestDecrement()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=3");
		commandSource.AddCommand("Value--");
		commandSource.AddCommand("DI(Value, Value, Value)");
		commandSource.AddCommand("Value--");
		commandSource.AddCommand("DI(Value, Value, Value)");

		RunProgram(&commandSource);

		Assert::AreEqual(2, _commandResultCount);

		AssertResult(_commandResults[0], 2, 2, 2.0F);
		AssertResult(_commandResults[1], 1, 1, 1.0F);
	}

	static void TestIncrementReference()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("Value=3");
		commandSource.AddCommand("Test=Value++");
		commandSource.AddCommand("DI(Test, Test, Test)");
		commandSource.AddCommand("DI(Value, Value, Value)");

		RunProgram(&commandSource);

		Assert::AreEqual(2, _commandResultCount);

		AssertResult(_commandResults[0], 3, 3, 3.0F);
		AssertResult(_commandResults[1], 4, 4, 4.0F);
	}

	static void TestDirectWithLists()
	{
		CommandSourceSimulator commandSource;
		ParseErrors parseErrors;

		commandSource.AddCommand("DI(10, 15, {0.5, 1.0, 0.4})");

		RunProgram(&commandSource);

		Assert::AreEqual(1, _commandResultCount);
		LedState ledState = _commandResults[0].GetTarget(0);
		Assert::AreEqual(15, ledState.GetChannel());
		Assert::AreEqual(0.5F, ledState.GetBrightness()->GetValueFloat(0));
		Assert::AreEqual(1.0F, ledState.GetBrightness()->GetValueFloat(1));
		Assert::AreEqual(0.4F, ledState.GetBrightness()->GetValueFloat(2));
	}


public:

	static int Run()
	{
		TestDirectWithLists();

		Test();
		TestLoop();
		TestLoopDown();
		TestLoopDownWithVariables();
		TestNestedLoop();
		TestLoopWithIncrement();
		TestLoopWithVariableCount();
		TestNestedLoopWithLongNames();

		TestOverlappingAnimation();
		TestMissingAnimation();

		TestLoopImmediate();

		TestAutoRestart();
		TestMissingEndFor();
		TestInvalidStatement();

		TestDoubleLoop();
		TestPrint();

		TestMethodCallWithParameter();
		TestMethodCallWithTwoParameters();
		TestFunctionDef();
		TestFunctionCall();
		TestFunctionCallNested();
		TestFunctionCallAsArgument();
		TestFunctionCallRandomParameter();
		TestMethodCall();
		TestIgnoredFunctionResult();
		TestMethodCallWithParameterDuplicateName();
		TestMethodCannotAccessParentVariables();
		TestMethodCallWithWrongArgumentCount();
		TestMethodCallWithWrongArgumentCount2();

		TestLoopWithFunctionCall();
		TestDirectWithFunctionCall();

		TestNestedIfElse1();
		TestNestedIfElse2();
		TestNestedIfElse3();
		TestNestedIfElse4();

		TestIfElseTrue();
		TestIfElseFalse();
		TestIfFalse();
		TestIfTrue();

		TestIfElseIfElseOne();
		TestIfElseIfElseTwo();
		TestIfElseIfElseThree();

		TestIncrement();
		TestDecrement();
		TestIncrementReference();


		return 0;
	}
};

CommandResult ExecutionFlowTest::_commandResults[ResultStorageSize];
int ExecutionFlowTest::_commandResultCount;
