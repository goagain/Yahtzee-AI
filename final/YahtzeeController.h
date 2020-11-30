#pragma once

#include "YahtzeeStatus.h"
#include "Judger.h"

#include <random>

class YahtzeeController
{
public:
	YahtzeeController(unsigned int randomSeed = 0);

	Judger judger;

	YahtzeeStatus startNewGame();

	YahtzeeStatus roll(const YahtzeeStatus& status);
	YahtzeeStatus reroll(const YahtzeeStatus& status, Dices kept);
	YahtzeeStatus write(const YahtzeeStatus& status, ScoreType type, int* scoreChange = nullptr, int * bonus = nullptr);

	hash_t rollByHash(hash_t hash);
	hash_t writeByHash(hash_t hash, ScoreType type, int* scoreChange = nullptr);
	int random();
protected:

	uint32_t x, y, z, w;
};