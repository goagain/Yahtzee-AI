#pragma once

#include "YahtzeeStatus.h"
#include "YahtzeeController.h"
#include "Action.h"
#include "IAgent.h"

#include <vector>

class SimpleAgent : public IAgent
{
public:
	SimpleAgent();
	SimpleAgent(FILE* f);
	virtual ~SimpleAgent();

	float train();
	float getExpect(YahtzeeStatus* status);
	float getExpectByHash(const hash_t hash, Action* next = nullptr);

	Action getAction(YahtzeeStatus* status);

	void save(FILE* f);

	float* rewardTable;
};