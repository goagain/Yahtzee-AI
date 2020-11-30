
#include "Judger.h"

#include <map>
#include <numeric>
#include <set>
#include <algorithm>
#include <cassert>

int Judger::result[14][65536];


Judger::Judger()
{
	methods[ScoreType::Ones] = std::bind(&Judger::Ones, this, std::placeholders::_1);
	methods[ScoreType::Twos] = std::bind(&Judger::Twos, this, std::placeholders::_1);
	methods[ScoreType::Threes] = std::bind(&Judger::Threes, this, std::placeholders::_1);
	methods[ScoreType::Fours] = std::bind(&Judger::Fours, this, std::placeholders::_1);
	methods[ScoreType::Fives] = std::bind(&Judger::Fives, this, std::placeholders::_1);
	methods[ScoreType::Sixes] = std::bind(&Judger::Sixes, this, std::placeholders::_1);

	methods[ScoreType::Chance] = std::bind(&Judger::Chance, this, std::placeholders::_1);
	methods[ScoreType::ThreeOfAKind] = std::bind(&Judger::ThreeOfAKind, this, std::placeholders::_1);
	methods[ScoreType::FourOfAKind] = std::bind(&Judger::FourOfAKind, this, std::placeholders::_1);
	methods[ScoreType::FullHouse] = std::bind(&Judger::FullHouse, this, std::placeholders::_1);
	methods[ScoreType::SmallStraight] = std::bind(&Judger::SmallStraight, this, std::placeholders::_1);
	methods[ScoreType::LargeStraight] = std::bind(&Judger::LargeStraight, this, std::placeholders::_1);
	methods[ScoreType::Yahtzee] = std::bind(&Judger::Yahtzee, this, std::placeholders::_1);

	static bool inited = false;
	if (!inited)
		memset(result, -1, sizeof(result));
	inited = true;
}

int Judger::Judge(const YahtzeeStatus& status, ScoreType type)
{
	assert(status.dices.ready());

	compressed_dicehash_t dicehash = YahtzeeStatus::composeDices(status.dices);

	return JudgeByDicehash(dicehash, type);
}

int Judger::JudgeByDicehash(compressed_dicehash_t hash, ScoreType type)
{
	int bit = 0;
	while (((int)type & (1 << bit)) == 0 && bit < 14)
	{
		bit++;
	}
	if (result[bit][hash] != -1)
	{
		return result[bit][hash];
	}

	Dices dices = YahtzeeStatus::parseDices(hash);
	int score = methods[type](dices);
	return result[bit][hash] = score;
}

int Judger::Ones(const Dices& dices)
{
	int score = 0;
	for (int dice : dices.dices)
	{
		if (dice == 1)
		{
			score += dice;
		}
	}
	return score;
}

int Judger::Twos(const Dices& dices)
{
	int score = 0;
	for (int dice : dices.dices)
	{
		if (dice == 2)
		{
			score += dice;
		}
	}
	return score;
}

int Judger::Threes(const Dices& dices)
{
	int score = 0;
	for (int dice : dices.dices)
	{
		if (dice == 3)
		{
			score += dice;
		}
	}
	return score;
}

int Judger::Fours(const Dices& dices)
{
	int score = 0;
	for (int dice : dices.dices)
	{
		if (dice == 4)
		{
			score += dice;
		}
	}
	return score;
}

int Judger::Fives(const Dices& dices)
{
	int score = 0;
	for (int dice : dices.dices)
	{
		if (dice == 5)
		{
			score += dice;
		}
	}
	return score;
}

int Judger::Sixes(const Dices& dices)
{
	int score = 0;
	for (int dice : dices.dices)
	{
		if (dice == 6)
		{
			score += dice;
		}
	}
	return score;
}

int Judger::Chance(const Dices& dices)
{
	int score = 0;
	for (int dice : dices.dices)
	{
		score += dice;
	}
	return score;
}

int Judger::ThreeOfAKind(const Dices& dices)
{
	std::map<int, int> m;
	for (int dice : dices.dices)
	{
		m[dice]++;
	}

	for (auto count : m)
	{
		if (count.second >= 3)
		{
			int sum = 0;
			for (int dice : dices.dices)
			{
				sum += dice;
			}
		}
	}
	return 0;
}

int Judger::FourOfAKind(const Dices& dices)
{
	std::map<int, int> m;
	for (int dice : dices.dices)
	{
		m[dice]++;
	}

	for (auto count : m)
	{
		if (count.second >= 4)
		{
			int sum = 0;
			for (int dice : dices.dices)
			{
				sum += dice;
			}
		}
	}
	return 0;
}

int Judger::FullHouse(const Dices& dices)
{
	std::map<int, int> m;
	for (int dice : dices.dices)
	{
		m[dice]++;
	}

	if (m.size() == 2 && m.begin()->second == 2 || m.begin()->second == 3)
	{
		return 25;
	}
	return 0;
}

int Judger::SmallStraight(const Dices& dices)
{
	std::set<int> s;
	for (int dice : dices.dices)
	{
		s.insert(dice);
	}
	std::vector<std::set<int>> prefabs =
	{
		{1, 2, 3, 4},
		{2, 3, 4, 5},
		{3, 4, 5, 6}
	};
	for (std::set<int>& prefab : prefabs)
	{
		bool matched = true;
		for (int dice : prefab)
		{
			if (s.find(dice) == s.end())
			{
				matched = false;
				break;
			}
		}

		if (matched)
		{
			return 30;
		}
	}
	return 0;
}

int Judger::LargeStraight(const Dices& dices)
{
	std::set<int> s;
	for (int dice : dices.dices)
	{
		s.insert(dice);
	}
	std::vector<std::set<int>> prefabs =
	{
		{1, 2, 3, 4, 5},
		{2, 3, 4, 5, 6},
	};
	for (std::set<int>& prefab : prefabs)
	{
		bool matched = true;
		for (int dice : prefab)
		{
			if (s.find(dice) == s.end())
			{
				matched = false;
				break;
			}
		}

		if (matched)
		{
			return 40;
		}
	}
	return 0;
}

int Judger::Yahtzee(const Dices& dices)
{
	std::map<int, int> m;
	for (int dice : dices.dices)
	{
		m[dice]++;
	}

	if (m.begin()->second == 5)
	{
		return 50;
	}

	return 0;
}
