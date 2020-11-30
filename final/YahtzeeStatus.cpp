#include "YahtzeeStatus.h"

#include <algorithm>
int YahtzeeStatus::getUpperSum() const
{
	static const auto list = std::vector<ScoreType>{
	ScoreType::Ones,
	ScoreType::Twos ,
	ScoreType::Threes ,
	ScoreType::Fours ,
	ScoreType::Fives,
	ScoreType::Sixes 
	};

	int score = 0;
	for (auto iter : list)
	{
		score += getScoreByType(iter);
	}
	return score;
}

int YahtzeeStatus::getLowerSum() const
{
	static const auto list = std::vector<ScoreType>{
	ScoreType::Chance,
	ScoreType::ThreeOfAKind ,
	ScoreType::FourOfAKind,
	ScoreType::FullHouse,
	ScoreType::SmallStraight,
	ScoreType::LargeStraight ,
	ScoreType::Yahtzee
	};

	int score = 0;
	for (auto iter : list)
	{
		score += getScoreByType(iter);
	}
	return score;
}

int YahtzeeStatus::getScoreByType(ScoreType type) const
{
	if (scoreMap.find(type) != scoreMap.end())
	{
		return scoreMap.at(type);
	}
	return 0;
}

bool YahtzeeStatus::end() const
{
	for (int i = 1; i < 13; i++)
	{
		ScoreType type = (ScoreType)(1 << i);
		if (scoreMap.find(type) == scoreMap.end())
		{
			return false;
		}
	}
	return true;
}

hash_t YahtzeeStatus::getStatusHash() const
{
	hash_t state = 0;

	for (auto pair : scoreMap)
	{
		if (pair.first != ScoreType::Bonus)
		{
			state |= (hash_t) pair.first;
		}
	}

	compressed_dicehash_t dicehash = composeDices(dices);

	int8_t upperSum = (int8_t)getUpperSum();
	if (upperSum > 63)
	{
		upperSum = 63;
	}
	return YahtzeeStatusCode{ state, dicehash, upperSum, (int8_t)reroll_times }.getHash();
}

int YahtzeeStatus::getDecDice(const Dices& dices)
{
	int res = 0;
	for (int i = 0; i < 5; i++)
	{
		res = res * 10 + dices.dices[i];
	}
	return res;
}

YahtzeeStatusCode YahtzeeStatus::parseStatusCode(hash_t code)
{
	return YahtzeeStatusCode::ParseHashCode(code);
}


compressed_dicehash_t YahtzeeStatus::composeDices(const Dices& dices)
{
	dicehash_t hash = 0;

	for (int range = 0; range < 5; range++)
	{
		hash = (hash << 3) | dices.dices[range];
	}

	return dicemap[hash];
}

compressed_dicehash_t YahtzeeStatus::composeDicehash(hash_t hash)
{
	return dicemap[hash];
}

Dices YahtzeeStatus::parseDices(compressed_dicehash_t compressedDiceHash)
{
	dicehash_t dicehash = reverse_dicemap[compressedDiceHash];
	Dices dices;
	bool valid = true;
	for (int range = 0; range < 5; range++)
	{
		uint8_t d = (dicehash >> (range * 3)) & 0x7;
		dices[range] = d;
	}

	return dices;
}

void YahtzeeStatus::init()
{
	static bool inited = false;

	if (inited)
	{
		return;
	}
	memset(dicemap, -1, sizeof(dicemap));
	memset(reverse_dicemap, -1, sizeof(reverse_dicemap));

	compressed_dicehash_t index = 0;
	for (dicehash_t dicehash = 0; dicehash < (1 << 15); dicehash++)
	{
		Dices dices;
		bool valid = true;
		for (int range = 0; range < 5; range++)
		{
			int d = (dicehash >> (range * 3)) & 0x7;
			dices.dices[range] = d;
			if (d == 7)
			{
				valid = false;
				break;
			}
		}

		if (!valid)
		{
			continue;
		}

		dices.sort();

		dicehash_t sortedDicehash = 0;
		for (int range = 0; range < 5; range++)
		{
			sortedDicehash = (sortedDicehash << 3) | dices[range];
		}

		if (dicemap[sortedDicehash] != UINT16_MAX)
		{
			dicemap[dicehash] = dicemap[sortedDicehash];
		}
		else
		{
			dicemap[sortedDicehash] = index;
			dicemap[dicehash] = index;
			reverse_dicemap[index] = sortedDicehash;
			index++;
		}
	}
	inited = true;
}

YahtzeeStatus YahtzeeStatus::getInitialState()
{
	YahtzeeStatus state;
	state.reroll_times = 3;
	state.dices.clear();
	return state;
}

std::vector<ScoreType> YahtzeeStatus::AllScoreType{
	ScoreType::Ones,
	ScoreType::Twos,
	ScoreType::Threes,
	ScoreType::Fours,
	ScoreType::Fives,
	ScoreType::Sixes,
	ScoreType::Chance,
	ScoreType::FourOfAKind,
	ScoreType::FullHouse,
	ScoreType::SmallStraight,
	ScoreType::LargeStraight,
	ScoreType::Yahtzee,
	ScoreType::Bonus
};

YahtzeeStatusCode YahtzeeStatusCode::ParseHashCode(hash_t code)
{
	YahtzeeStatusCode status;
	status.rerollTimes = code & 3;
	status.upperSum = (code >> 2) & 63;
	status.diceHash = (code >> 6 >> 2) & 0x1ff;
	status.state = (code >> 9 >> 6 >> 2);
	return status;
}

hash_t YahtzeeStatusCode::getHash()
{
	return state << 9 << 6 << 2 // 13 bits
		| diceHash << 6 << 2 // 9 bits
		| upperSum << 2 // 6 bits
		| rerollTimes; //2 bits
}

compressed_dicehash_t YahtzeeStatus::dicemap[65535];
dicehash_t YahtzeeStatus::reverse_dicemap[65535];