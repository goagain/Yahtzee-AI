#pragma once

#include "YahtzeeStatus.h"
#include "YahtzeeController.h"
#include "Action.h"

class Recorder
{

};

struct GameRecord
{
	std::vector<Action> actions;
	std::vector<int> scores;
};

class YahtzeeGame
{
public:
	YahtzeeStatus status;

	GameRecord* record;

	YahtzeeController* controller;

	YahtzeeGame();

	virtual ~YahtzeeGame();

	void newGame();

	void roll();

	void reroll(Dices dices);
	
	int write(ScoreType type);

};

