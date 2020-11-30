#include <stdlib.h>
#include <stdint.h>

#include "YahtzeeStatus.h"
#include "QAgent.h"
#include "SimpleAgent.h"
extern "C" {
	__declspec(dllexport) int32_t __stdcall TestAdd(int32_t a, int32_t b) {
		return a + b;
	}

	__declspec(dllexport) void __stdcall YahtzeeStatus_init() {
		YahtzeeStatus::init();
	}

	__declspec(dllexport) QAgent* __stdcall QAgent_New() {
		return new QAgent();
	}

	__declspec(dllexport) QAgent* __stdcall QAgent_NewFromFile(FILE* f) {
		return new QAgent(f);
	}

	__declspec(dllexport) SimpleAgent* __stdcall SimpleAgent_New() {
		return new SimpleAgent();
	}

	__declspec(dllexport) SimpleAgent* __stdcall SimpleAgent_NewFromFile(FILE* f) {
		return new SimpleAgent(f);
	}

	__declspec(dllexport) void __stdcall IAgent_save(IAgent* q, FILE* f) {
		q->save(f);
	}

	__declspec(dllexport) QAgent* __stdcall QAgent_train(QAgent* q, float learningRate) {
		q->train(learningRate);
		return q;
	}

	__declspec(dllexport) float __stdcall IAgent_simulate(IAgent* q, int n, FILE* output) {
		return q->simulate(n, output);
	}

	__declspec(dllexport) Action __stdcall IAgent_getAction(IAgent* q, YahtzeeStatus* status) {
		return q->getAction(status);
	}

	__declspec(dllexport) void __stdcall Release(IAgent* q) {
		delete q;
	}

}