#pragma once

#include "YahtzeeStatus.hpp"

#include <map>
#include <functional>
#include <string>

class Judger
{
public:
	Judger();
	int Judge(const YahtzeeStatus& status, ScoreType type);
	int JudgeByDicehash(compressed_dicehash_t hash, ScoreType type);

	int Ones(const Dices& dices);
	int Twos(const Dices& dices);
	int Threes(const Dices& dices);
	int Fours(const Dices& dices);
	int Fives(const Dices& dices);
	int Sixes(const Dices& dices);

	int Chance(const Dices& dices);
	int ThreeOfAKind(const Dices& dices);
	int FourOfAKind(const Dices& dices);
	int FullHouse(const Dices& dices);
	int SmallStraight(const Dices& dices);
	int LargeStraight(const Dices& dices);
	int Yahtzee(const Dices& dices);

private:
	std::map<ScoreType, std::function<int(const Dices&)>> methods;
	std::map<std::pair<compressed_dicehash_t, ScoreType>, int> result;
};