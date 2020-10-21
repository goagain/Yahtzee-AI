#pragma once

#include "YahtzeeStatus.hpp"
#include "YahtzeeController.hpp"

#include <vector>

enum class ActionType
{
	Roll,
	Reroll,
	Write,
};

struct Action
{
	ActionType type;
	ScoreType written;
	std::vector<int> kept;
};

class Agent
{
public:
	Agent();
	Agent(FILE* f);
	virtual ~Agent();

	float getExpect(const YahtzeeStatus& status);
	float getExpectByHash(const hash_t hash, Action* next = nullptr);

	Action getBestAction(const YahtzeeStatus& status);

	void simulate(int n = 1000, FILE* output = nullptr);

	float* rewardTable;
protected:
	//std::vector<fixed> rewardTable;
	YahtzeeController* controller;
};