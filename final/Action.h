#pragma once

#include "YahtzeeStatus.h"

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
	Dices kept;
	Action();
};
