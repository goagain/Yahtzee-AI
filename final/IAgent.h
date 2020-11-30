#pragma once
#include "YahtzeeStatus.h"
#include "YahtzeeGame.h"
class IAgent
{
public:
	virtual Action getAction(YahtzeeStatus* status) = 0;

	YahtzeeController* controller;

	virtual float simulate(int n = 1000, FILE* output = nullptr);

	virtual float getExpect(YahtzeeStatus *status) = 0;

	virtual void save(FILE* f) = 0;

	IAgent();
	virtual ~IAgent();
};

