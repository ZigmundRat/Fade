class BuiltInFunctions
{
	static bool HandleBuiltInRand(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "Rand") == 0)
		{
			Variable *pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");
			if (pArgumentCount->GetValueInt() == 2)
			{
				// Serial.println("    found random: ");
				Variable *pMinValue = pExecutionContext->GetVariableWithoutErrorCheck("#A0");
				Variable *pMaxValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");

				pExpressionResult->_variable.SetValue(0, (float)MyRandom::GetValue(pMinValue->GetValueInt(), pMaxValue->GetValueInt()));
				return true;
			}
			else if (pArgumentCount->GetValueInt() == 0)
			{
				pExpressionResult->_variable.SetValue(0, (float)MyRandom::GetFloatValue());
				return true;
			}

			pParseErrors->AddError("Rand requires zero or two parameters", "", pExpressionTokenSource->GetLineNumber());
			return true;
		}

		return false;
	}

	static bool HandleBuiltInDirect(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if ((strcmp(pFunctionName, "D") == 0 || strcmp(pFunctionName, "Di") == 0))
		{
			bool immediateMode;
			if (*(pFunctionName + 1) == 'i')
			{
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandExecute);
				immediateMode = true;
			}
			else
			{
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);
				immediateMode = false;
			}

			Variable *pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");

			Variable *pCycleCount = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			if (immediateMode)
			{
				pExecutionFlow->GetCommandResult()->SetCycleCount(pCycleCount->GetValueInt());
			}

			// even number of arguments is a problem...
			if (pArgumentCount->GetValueInt() % 2 == 0)
			{
				pParseErrors->AddError("Invalid D command: ", "missing brightness target", pExpressionTokenSource->GetLineNumber());
				return true;
			}

			for (int i = 1; i < pArgumentCount->GetValueInt(); i += 2)
			{
				char argumentName[10];

				snprintf(argumentName, sizeof(argumentName) / sizeof(char), "#A%d", i);
				Variable *pChannel = pExecutionContext->GetVariableWithoutErrorCheck(argumentName);

				snprintf(argumentName, sizeof(argumentName) / sizeof(char), "#A%d", i + 1);
				Variable *pBrightness = pExecutionContext->GetVariableWithoutErrorCheck(argumentName);

				pExecutionFlow->GetCommandResult()->AddTarget(LedState(pChannel->GetValueInt(), pBrightness, pCycleCount->GetValueInt()));
			}

			if (immediateMode)
			{
				pExecutionFlow->ExecuteLedCommand(pExecutionFlow->GetCommandResult());
			}

			return true;
		}

		return false;
	}

	static bool HandleBuiltInSequential(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if ((strcmp(pFunctionName, "S") == 0 || strcmp(pFunctionName, "Si") == 0))
		{
			bool immediateMode;
			if (*(pFunctionName + 1) == 'i')
			{
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandExecute);
				immediateMode = true;
			}
			else
			{
				pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandNone);
				immediateMode = false;
			}

			Variable *pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");
			if (pArgumentCount->GetValueInt() == 0)
			{
				pParseErrors->AddError("Invalid S command: ", "expected cycle count after (", pExpressionTokenSource->GetLineNumber());
				return true;
			}

			Variable *pCycleCount = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			if (immediateMode)
			{
				pExecutionFlow->GetCommandResult()->SetCycleCount(pCycleCount->GetValueInt());
			}

			for (int channel = 1; channel < pArgumentCount->GetValueInt(); channel++)
			{
				char argumentName[10];

				snprintf(argumentName, sizeof(argumentName) / sizeof(char), "#A%d", channel);
				Variable *pBrightness = pExecutionContext->GetVariableWithoutErrorCheck(argumentName);

				pExecutionFlow->GetCommandResult()->AddTarget(LedState(channel - 1, pBrightness, pCycleCount->GetValueInt()));
			}

			if (immediateMode)
			{
				pExecutionFlow->ExecuteLedCommand(pExecutionFlow->GetCommandResult());
			}

			return true;
		}

		return false;
	}

	static bool HandleBuiltInAnimate(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "A") == 0)
		{
			Variable *pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");
			if (pArgumentCount->GetValueInt() == 0)
			{
				pParseErrors->AddError("Invalid A command: ", "expected cycle count", pExpressionTokenSource->GetLineNumber());
				return true;
			}

			Variable *pCycleCount = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			pExecutionFlow->GetCommandResult()->SetCycleCount(pCycleCount->GetValueInt());
			pExecutionFlow->GetCommandResult()->SetStatus(CommandResultStatus::CommandExecute);

			pExecutionFlow->ExecuteLedCommand(pExecutionFlow->GetCommandResult());

			return true;
		}

		return false;
	}

	static bool HandleBuiltInPrint(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if ((strcmp(pFunctionName, "P") == 0 || strcmp(pFunctionName, "Pl") == 0))
		{
			char outputString[64];
			outputString[0] = '\0';

			bool newLine = strcmp(pFunctionName, "Pl") == 0;

			Variable *pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");

			char buffer[16];
			for (int argument = 0; argument < pArgumentCount->GetValueInt(); argument++)
			{
				snprintf(buffer, sizeof(buffer), "#A%d", argument);

				Variable *pArgument = pExecutionContext->GetVariableWithoutErrorCheck(buffer);

				if (pArgument->GetValueCount() > 1)
				{
					strcat(outputString, "{");

					for (int i = 0; i < pArgument->GetValueCount(); i++)
					{
						if (i != 0)
						{
							strcat(outputString, ", ");
						}
						snprintf(buffer, sizeof(buffer), "%f", pArgument->GetValueFloat(i));
						strcat(outputString, buffer);
					}
					strcat(outputString, "}");
				}
				else if (pArgument->GetValueCount() == 1)
				{
					snprintf(buffer, sizeof(outputString), "%f", pArgument->GetValueFloat(0));
					strcat(outputString, buffer);
				}
				else
				{
					strcat(outputString, pArgument->GetValueString());
				}
			}

			if (newLine)
			{
				strcat(outputString, "\n");
			}
			Serial.print(outputString);

			if (Environment.DebugPrintOverUdp)
			{
				UdpLogger.print(outputString);
			}

			return true;
		}

		return false;
	}

	static bool HandleBuiltInAbort(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "Abort") == 0)
		{
			pExecutionFlow->AbortExecution();
			pParseErrors->AddError("Aborting: ", "Abort", pExpressionTokenSource->GetLineNumber());

			return true;
		}

		return false;
	}

	static int GetVariableOrNegativeOne(IExecutionContext *pExecutionContext, const char *pVariableName)
	{
		Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck(pVariableName);

		if (pValue != 0)
		{
			return pValue->GetValueInt();
		}
		else
		{
			return -1;
		}
	}

	static bool HandleBuiltInConfigLed(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "ConfigLed") == 0)
		{
			Variable *parameterCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");
			Variable *pLedGroupNumber = pExecutionContext->GetVariableWithoutErrorCheck("#A0");
			Variable *pLedType = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
			Variable *pLedCount = pExecutionContext->GetVariableWithoutErrorCheck("#A2");

			int pins[18];
			int pinCount;

			pinCount = parameterCount->GetValueInt() - 3;

			char buffer[8];
			for (int i = 0; i < pinCount; i++)
			{
				sprintf(buffer, "#A%d", i + 3);

				pins[i] = GetVariableOrNegativeOne(pExecutionContext, buffer);
			}

			if (!pExecutionFlow->ConfigureLeds(pLedGroupNumber->GetValueInt(), pLedType->GetValueString(), pLedCount->GetValueInt(), pinCount, pins))
			{
				pParseErrors->AddError("ConfigLed error", "", lineNumber);
			}

			return true;
		}

		return false;
	}

	static bool HandleBuiltInConfigButton(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "ConfigButton") == 0)
		{
			Variable *pButtonNumber = pExecutionContext->GetVariableWithoutErrorCheck("#A0");
			Variable *pButtonType = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
			Variable *pPin = pExecutionContext->GetVariableWithoutErrorCheck("#A2");
			Variable *pParameter1 = pExecutionContext->GetVariableWithoutErrorCheck("#A3");

			pExecutionFlow->ConfigureButton(pButtonNumber->GetValueInt(), pButtonType->GetValueString(), pPin->GetValueInt(), pParameter1->GetValueInt());

			return true;
		}

		return false;
	}

	static bool HandleBuiltInReadButton(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "ReadButton") == 0)
		{
			Variable *pButtonNumber = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			int buttonNumber = pButtonNumber->GetValueInt();

			if (buttonNumber < 0)
			{
				pParseErrors->AddError("Invalid Button Number: ", "button numbers must positive", pExpressionTokenSource->GetLineNumber());
			}
			else if (buttonNumber >= pExecutionFlow->GetButtonCount())
			{
				pParseErrors->AddError("Invalid Button Number: ", "button number is not defined", pExpressionTokenSource->GetLineNumber());
			}
			else
			{
				bool buttonState = pExecutionFlow->GetButtonState(buttonNumber);
				// Serial.print("ReadButton: ");
				// Serial.println(buttonState);

				pExpressionResult->_variable.SetValue(0, (float)buttonState);
			}
			return true;
		}

		return false;
	}

	static bool HandleBuiltInDebug(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "Debug") == 0)
		{
			Variable *pIdentifier = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			if (strcmp(pIdentifier->GetValueString(), "LogStatements") == 0)
			{
				Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
				Environment.DebugLogStatements = pValue->GetValueInt();
				return true;
			}
			else if (strcmp(pIdentifier->GetValueString(), "LogHeapFreeOnAllocation") == 0)
			{
				Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
				Environment.DebugLogHeapFreeOnAllocation = pValue->GetValueInt();
				return true;
			}
			else if (strcmp(pIdentifier->GetValueString(), "DebugLogTouchButtonValues") == 0)
			{
				Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
				Environment.DebugLogTouchButtonValues = pValue->GetValueInt();
				return true;
			}
			else if (strcmp(pIdentifier->GetValueString(), "DebugLogCycleDeltas") == 0)
			{
				Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
				Environment.DebugLogCycleDeltas = pValue->GetValueInt();
				return true;
			}
			else if (strcmp(pIdentifier->GetValueString(), "DebugLogLedUpdates") == 0)
			{
				Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
				Environment.DebugLogLedUpdates = pValue->GetValueInt();
				return true;
			}
			else if (strcmp(pIdentifier->GetValueString(), "DebugPrintOverUdp") == 0)
			{
				Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
				Environment.DebugPrintOverUdp = pValue->GetValueInt();
				UdpLogger.init(Environment.DebugPrintOverUdp);
				return true;
			}
		}

		return false;
	}

	static bool HandleBuiltInLogValue(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
        if (strcmp(pFunctionName, "LogValue") == 0)
        {
            Variable *pIdentifier = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

            char buffer[64];
            if (strcmp(pIdentifier->GetValueString(), "VectorDataItemProviderInUseCount") == 0)
            {
                int inUseCount = Vector::GetDataItemProviderInUseCount();

                snprintf(buffer, sizeof(buffer), "%s: %d\n", pIdentifier->GetValueString(), inUseCount);
            }
            else if (strcmp(pIdentifier->GetValueString(), "VariableStoreInUseCount") == 0)
            {
                int inUseCount = VariableStore::VariableStoreInstance.GetInUseCount();

                snprintf(buffer, sizeof(buffer), "%s: %d\n", pIdentifier->GetValueString(), inUseCount);
            }
            else
            {
                return false;
            }

            Serial.print(buffer);

            if (Environment.DebugPrintOverUdp)
            {
                UdpLogger.print(buffer);
            }
            return true;
        }

		return false;
	}

	static bool HandleBuiltInConfigEnvironment(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "ConfigEnvironment") == 0)
		{
			Variable *pIdentifier = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			if (strcmp(pIdentifier->GetValueString(), "VectorItemChunkSize") == 0)
			{
				Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
				Environment.VectorItemChunkSize = pValue->GetValueInt();
				return true;
			}
			else if (strcmp(pIdentifier->GetValueString(), "VariableStoreChunkSize") == 0)
			{
				Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
				Environment.VariableStoreChunkSize = pValue->GetValueInt();
				return true;
			}
		}

		return false;
	}

	static void HSVtoRGB(float h, float s, float v, Variable *pResult)
	{
		float C = s * v;
		float temp = (float)(fmod(h / 60, 2.0F) - 1.0F);
		float tempAbs = temp > 0 ? temp : -temp;

		float X = C * (1 - tempAbs);
		float m = v - C;

		float r, g, b;

		if (h >= 0 && h < 60)
		{
			r = C, g = X, b = 0;
		}
		else if (h >= 60 && h < 120)
		{
			r = X, g = C, b = 0;
		}
		else if (h >= 120 && h < 180)
		{
			r = 0, g = C, b = X;
		}
		else if (h >= 180 && h < 240)
		{
			r = 0, g = X, b = C;
		}
		else if (h >= 240 && h < 300)
		{
			r = X, g = 0, b = C;
		}
		else
		{
			r = C, g = 0, b = X;
		}

		pResult->SetValue(0, r + m);
		pResult->SetValue(1, g + m);
		pResult->SetValue(2, b + m);
	}

	static bool HandleBuiltInHsvToRGB(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "HsvToRgb") == 0)
		{
			// Variable *pIdentifier = pExecutionContext->GetVariableWithoutErrorCheck("#A0");

			Variable *pH = pExecutionContext->GetVariableWithoutErrorCheck("#A0");
			Variable *pS = pExecutionContext->GetVariableWithoutErrorCheck("#A1");
			Variable *pV = pExecutionContext->GetVariableWithoutErrorCheck("#A2");

			float hue = pH->GetValueFloat(0);
			if (hue < 0)
			{
				hue += 360;
			}
			else if (hue > 360)
			{
				hue -= 360;
			}

			HSVtoRGB(hue, pS->GetValueFloat(0), pV->GetValueFloat(0), &pExpressionResult->_variable);
			return true;
		}

		return false;
	}

	static bool HandleBuiltInMax(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "Max") == 0)
		{
			Variable *pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");

			float max = pExecutionContext->GetVariableWithoutErrorCheck("#A0")->GetValueFloat(0);

			char buffer[10];
			for (int argument = 1; argument < pArgumentCount->GetValueInt(); argument++)
			{
				sprintf(buffer, "#A%d", argument);
				Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck(buffer);
				if (pValue->GetValueFloat(0) > max)
				{
					max = pValue->GetValueFloat(0);
				}
			}

			pExpressionResult->_variable.SetValue(0, max);
			return true;
		}

		return false;
	}

	static bool HandleBuiltInMin(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (strcmp(pFunctionName, "Min") == 0)
		{
			Variable *pArgumentCount = pExecutionContext->GetVariableWithoutErrorCheck("#A");

			float min = pExecutionContext->GetVariableWithoutErrorCheck("#A0")->GetValueFloat(0);

			char buffer[10];
			for (int argument = 1; argument < pArgumentCount->GetValueInt(); argument++)
			{
				sprintf(buffer, "#A%d", argument);
				Variable *pValue = pExecutionContext->GetVariableWithoutErrorCheck(buffer);
				if (pValue->GetValueFloat(0) < min)
				{
					min = pValue->GetValueFloat(0);
				}
			}

			pExpressionResult->_variable.SetValue(0, min);
			return true;
		}

		return false;
	}

public:
	static bool HandleBuiltInFunctions(const char *pFunctionName, IExecutionContext *pExecutionContext, ParseErrors *pParseErrors, int lineNumber, ExpressionTokenSource *pExpressionTokenSource, IExecutionFlow *pExecutionFlow, ExpressionResult *pExpressionResult)
	{
		if (HandleBuiltInRand(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInDirect(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInSequential(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInAnimate(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInPrint(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInAbort(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInConfigLed(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInConfigButton(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInReadButton(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInDebug(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInHsvToRGB(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInConfigEnvironment(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInMax(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInMin(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		if (HandleBuiltInLogValue(pFunctionName, pExecutionContext, pParseErrors, lineNumber, pExpressionTokenSource, pExecutionFlow, pExpressionResult))
		{
			return true;
		}

		return false;
	}
};
