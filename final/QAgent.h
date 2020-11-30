#pragma once

#include "YahtzeeController.h"
#include "YahtzeeGame.h"
#include "IAgent.h"

class QAgent : public IAgent
{
public:
	float* qTable;

	void train(float learningRate);

	QAgent();
	QAgent(FILE* f);
	virtual ~QAgent();

	void save(FILE* f);
	void load(FILE* f);

	Action getAction(YahtzeeStatus* status);
	float getExpect(YahtzeeStatus* status);
private:
	float __train(YahtzeeController* controller, hash_t index, Action* bestAction = nullptr);
};

