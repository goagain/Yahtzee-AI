#include "YahtzeeGame.h"

YahtzeeGame::YahtzeeGame()
{
	controller = new YahtzeeController();

}

YahtzeeGame::~YahtzeeGame()
{
	if (controller != nullptr)
	{
		delete controller;
	}
}

void YahtzeeGame::roll()
{
	this->status = controller->roll(this->status);
}

void YahtzeeGame::reroll(Dices dices)
{
	this->status = controller->reroll(this->status, dices);
}

int YahtzeeGame::write(ScoreType type)
{
	int scoreChange = 0;
	this->status = this->controller->write(this->status, type, &scoreChange);

	if (this->status.end())
	{
		record->scores.push_back(this->status.score);
	}

	return scoreChange;
}

void YahtzeeGame::newGame()
{
	this->status = YahtzeeStatus::getInitialState();
}

