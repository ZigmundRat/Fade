#pragma once

#include "Supervisor.h"

class SupervisorTest
{
	static void TestInitRunningCode()
	{
		Supervisor supervisor;
		LedDeviceSimulator ledDevice(500);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
        int pins[16];
        pins[0] = 555;

        ledManager.Configure(0, "Test", 1, 1, pins);

		Settings settings;
		settings.SaveProgramText("A(55)");
		settings.SaveShouldExecuteCode(true);

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings, 0, 0);
		Serial.SetOutput(true);

		Assert::AreEqual("A(55)", supervisor.GetCurrentProgram());
		Assert::AreEqual(1, supervisor.GetExecutingProgramState());

		CommandSource* pCommandSource = supervisor.GetCommandSource();
		Command* pCommand = pCommandSource->GetCommand(0);
		Assert::AreEqual("A(55)", pCommand->GetString());

		Assert::AreEqual(0, supervisor.GetExecutionCount());
		supervisor.Execute();
		Assert::AreEqual(55, supervisor.GetExecutionCount());
	}

	static void TestInitNonRunningCode()
	{
		Supervisor supervisor;
		LedDeviceSimulator ledDevice(100);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
        int pins[16];
        pins[0] = 555;

        ledManager.Configure(0, "Test", 6, 1, pins);

		Settings settings;
		settings.SaveProgramText("A(55)");
		settings.SaveShouldExecuteCode(false);

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings, 0, 0);
		Serial.SetOutput(true);

		Assert::AreEqual("A(55)", supervisor.GetCurrentProgram());
		Assert::AreEqual(0, supervisor.GetExecutingProgramState());

		CommandSource* pCommandSource = supervisor.GetCommandSource();
		Command* pCommand = pCommandSource->GetCommand(0);
		Assert::AreEqual(0, pCommand);

		Assert::AreEqual(0, supervisor.GetExecutionCount());
		supervisor.Execute();
		Assert::AreEqual(0, supervisor.GetExecutionCount());
	}

	static void TestNoProgram()
	{
		Supervisor supervisor;
		LedDeviceSimulator ledDevice(100);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
        int pins[16];
        pins[0] = 555;

        ledManager.Configure(0, "Test", 16, 1, pins);

		Settings settings;
		settings.SaveShouldExecuteCode(false);

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings, 0, 0);
		Serial.SetOutput(true);

		Assert::AreEqual("", supervisor.GetCurrentProgram());
		Assert::AreEqual(0, supervisor.GetExecutingProgramState());

		CommandSource* pCommandSource = supervisor.GetCommandSource();
		Command* pCommand = pCommandSource->GetCommand(0);
		Assert::AreEqual(0, pCommand);
	}

  static void TestNoProgramShouldExecute()
  {
    Supervisor supervisor;
    LedDeviceSimulator ledDevice(100);
    LedDeviceCreatorSimulator ledCreator(&ledDevice);

    LedManager ledManager(&ledCreator);
    int pins[16];
    pins[0] = 555;

    ledManager.Configure(0, "Test", 16, 1, pins);

    Settings settings;
    settings.SaveShouldExecuteCode(true);

    Serial.SetOutput(false);
    supervisor.Init(&ledManager, &settings, 0, 0);
    Serial.SetOutput(true);

    Assert::AreEqual("", supervisor.GetCurrentProgram());
    Assert::AreEqual(1, supervisor.GetExecutingProgramState());

    CommandSource* pCommandSource = supervisor.GetCommandSource();
    Command* pCommand = pCommandSource->GetCommand(0);
    Assert::AreEqual(0, pCommand);
  }

  static void TestUpdatedProgram()
	{
		Supervisor supervisor;
		LedDeviceSimulator ledDevice(500);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
        int pins[16];
        pins[0] = 555;

        ledManager.Configure(0, "Test", 1, 1, pins);

		Settings settings;
		settings.SaveProgramText("A(55)");
		settings.SaveShouldExecuteCode(false);

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings, 0, 0);

		Assert::AreEqual("A(55)", supervisor.GetCurrentProgram());
		Assert::AreEqual(0, supervisor.GetExecutingProgramState());

		supervisor.Execute();
		Assert::AreEqual(0, supervisor.GetExecutionCount());

		supervisor.UpdateProgram("A(333)");
		Serial.SetOutput(true);

		Assert::AreEqual("A(333)", supervisor.GetCurrentProgram());
		Assert::AreEqual(1, supervisor.GetExecutingProgramState());

    Assert::AreEqual(1, EspFunctions::RestartCalled);
	}

  // removed after change to restart on update...
	static void TestUpdateCodeSavesShouldExecuteAfterSuccessfulLoop()
	{
		Supervisor supervisor;
		LedDeviceSimulator ledDevice(1000);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
        int pins[16];
        pins[0] = 555;

        ledManager.Configure(0, "Test", 1, 1, pins);

		Settings settings;

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings, 0, 0);

		supervisor.UpdateProgram("A(333)");

		// initially, we are executing locally but have not saved that state...
		Assert::AreEqual(1, supervisor.GetExecutingProgramState());
		Assert::AreEqual(0, settings.LoadShouldExecuteCode());

		supervisor.Execute();
		Assert::AreEqual(1, settings.LoadShouldExecuteCode());

		Serial.SetOutput(true);
	}

	static void TestCodeWithErrorsDisablesExecution()
	{
		Supervisor supervisor;
		LedDeviceSimulator ledDevice(1000);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
        int pins[16];
        pins[0] = 555;

        ledManager.Configure(0, "Test", 1, 1, pins);
        
		Settings settings;

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings, 0, 0);

		supervisor.UpdateProgram("B(333)");
		Assert::AreEqual(1, supervisor.GetExecutingProgramState());

		supervisor.Execute();
		Assert::AreEqual(0, supervisor.GetExecutingProgramState());
		Assert::AreEqual("Unrecognized function: B\n", supervisor.GetCurrentErrors());

		Serial.SetOutput(true);
	}

	static void TestSavedRunningProgramDoesNotUpdateSettingsAfter500Loops()
	{
		Supervisor supervisor;
		LedDeviceSimulator ledDevice(1000);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
        int pins[16];
        pins[0] = 555;

        ledManager.Configure(0, "Test", 1, 1, pins);

		Settings settings;
		settings.SaveProgramText("A(3)");
		settings.SaveShouldExecuteCode(1);

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings, 0, 0);

		settings.SaveShouldExecuteCode(false);	// change value to ensure no update

		for (int i = 0; i < 501; i++)
		{
			supervisor.Execute();
		}
		Assert::AreEqual(0, settings.LoadShouldExecuteCode());

		Serial.SetOutput(true);
	}

	static void TestUpdatedNodeName()
	{
		Supervisor supervisor;
		LedDeviceSimulator ledDevice(100);
		LedDeviceCreatorSimulator ledCreator(&ledDevice);

		LedManager ledManager(&ledCreator);
        int pins[16];
        pins[0] = 555;

        ledManager.Configure(0, "", 1, 1, pins);

		Settings settings;

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings, 0, 0);

		supervisor.UpdateNodeName("MyNameIsFred");

		char value[128];
		settings.LoadNodeName(value, 128);
		Assert::AreEqual("MyNameIsFred", value);

		Assert::AreEqual("MyNameIsFred", supervisor.GetNodeName());
	}


public:
	static void Run()
	{
		TestInitRunningCode();
		TestInitNonRunningCode();
		TestNoProgram();
    TestNoProgramShouldExecute();
		TestUpdatedProgram();

		TestCodeWithErrorsDisablesExecution();
		TestSavedRunningProgramDoesNotUpdateSettingsAfter500Loops();

		TestUpdatedNodeName();
	}
};
