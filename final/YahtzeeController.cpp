#include "YahtzeeController.h"

#include <random>
#include <algorithm>
#include <cassert>

YahtzeeController::YahtzeeController(unsigned int randomSeed)
{
	this->x = randomSeed;
	y = 15123564126;
	z = 6716431;
	w = 1137773;
}

YahtzeeStatus YahtzeeController::startNewGame()
{
	return YahtzeeStatus::getInitialState();
}

YahtzeeStatus YahtzeeController::roll(const YahtzeeStatus& status)
{
	assert(!status.end());

	for (int index = 0; index < status.dices.size(); index++)
	{
		assert(status.dices[index] == 0);
	}

	YahtzeeStatus newStatus = status;

	newStatus.dices.clear();

	for (int index = 0; index < 5; index++)
	{
		newStatus.dices[index] = random();
	}
	newStatus.reroll_times--;
	return newStatus;
}
YahtzeeStatus YahtzeeController::reroll(const YahtzeeStatus& status, Dices kept)
{
	assert(status.reroll_times > 0);
	assert(!status.end());

	YahtzeeStatus newStatus = status;
	newStatus.reroll_times--;

	for (int index = 0; index < 5; index++)
	{
		bool found = false;
		for (int j = 0; j < 5; j++)
		{
			if (kept[j] == newStatus.dices[index])
			{
				kept[j] = 0;
				found = true;
				break;
			}
		}

		if (newStatus.dices[index] == 0 || !found)
		{
			newStatus.dices[index] = random();
		}
	}
	return newStatus;
}

YahtzeeStatus YahtzeeController::write(const YahtzeeStatus& status, ScoreType type, int *scoreChange, int *bonus)
{
	if (scoreChange != nullptr)
	{
		*scoreChange = 0;
	}
	if (bonus != nullptr)
	{
		*bonus = 0;
	}
	assert(status.dices.ready());

	YahtzeeStatus newStatus = status;

	assert(status.scoreMap.find(type) == status.scoreMap.end());

	int score = judger.Judge(status, type);

	newStatus.score += score;
	newStatus.scoreMap[type] = score;

	if (status.getUpperSum() < 63 && newStatus.getUpperSum() >= 63)
	{
		score += 35;
		newStatus.score += 35;
		newStatus.scoreMap[ScoreType::Bonus] = 35;
		if (bonus != nullptr)
		{
			*bonus = 35;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		newStatus.dices[i] = 0;
	}

	if (scoreChange != nullptr)
	{
		*scoreChange = score;
	}
	newStatus.reroll_times = 3;
	return newStatus;
}

hash_t YahtzeeController::rollByHash(hash_t hash)
{
	YahtzeeStatusCode code = YahtzeeStatusCode::ParseHashCode(hash);
	assert(code.rerollTimes > 0);

	Dices dices = YahtzeeStatus::parseDices(code.diceHash);

	for (int i = 0; i <= 5; i++)
	{
		if (dices[i] == 0)
		{
			if (dices[i] == 0)
			{
				dices[i] = random();
			}
		}
	}
	dicehash_t dicehash = YahtzeeStatus::composeDices(dices);
	code.diceHash = dicehash;
	code.rerollTimes--;

	return code.getHash();
}

int YahtzeeController::random()
{
	uint32_t t = x ^ (x << 11);

	x = y; y = z; z = w;

	w = w ^ (w >> 19) ^ t ^ (t >> 8);

	return w % 6 + 1;
}

hash_t YahtzeeController::writeByHash(hash_t hash, ScoreType type, int* scoreChange)
{
	YahtzeeStatusCode code = YahtzeeStatusCode::ParseHashCode(hash);

	assert(code.state != 0x1fff);
	assert((code.state & (int)type) == 0);

	int score = judger.JudgeByDicehash(code.diceHash, type);

	if (type >= ScoreType::Ones && type <= ScoreType::Sixes)
	{
		if (code.upperSum < 63 && code.upperSum + score >= 63)
		{
			score += 35;
		}
		code.upperSum += score;

		if (code.upperSum > 63)
		{
			code.upperSum = 63;
		}
	}
	code.state |= (int)type;
	code.diceHash = 0;
	code.rerollTimes = 3;
	if (scoreChange != nullptr)
	{
		*scoreChange = score;
	}

	return code.getHash();
}
