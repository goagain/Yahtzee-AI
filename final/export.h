#pragma once
#include <stdlib.h>
#include <stdint.h>

#include "YahtzeeStatus.h"
#include "QAgent.h"
#include "IAgent.h"
#include "SimpleAgent.h"

extern "C" {
	__declspec(dllexport) int32_t __stdcall TestAdd(int32_t a, int32_t b);

	__declspec(dllexport) void __stdcall YahtzeeStatus_init();

	__declspec(dllexport) QAgent* __stdcall QAgent_New();

	__declspec(dllexport) QAgent* __stdcall QAgent_NewFromFile(FILE* f);

	__declspec(dllexport) SimpleAgent* __stdcall SimpleAgent_New();

	__declspec(dllexport) SimpleAgent* __stdcall SimpleAgent_NewFromFile(FILE* f);

	__declspec(dllexport) void __stdcall IAgent_save(IAgent* q, FILE* f);

	__declspec(dllexport) QAgent* __stdcall QAgent_train(QAgent* q, float learningRate = 0.9f);

	__declspec(dllexport) float __stdcall IAgent_simulate(IAgent* q, int n, FILE* output = nullptr);

	__declspec(dllexport) Action __stdcall IAgent_getAction(IAgent* q, YahtzeeStatus* status);

}