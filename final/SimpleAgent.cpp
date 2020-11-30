#include "SimpleAgent.h"
#include "YahtzeeController.h"

SimpleAgent::SimpleAgent() : IAgent()
{
	rewardTable = new float[YahtzeeStatusCode::MAX];
	//memset(rewardTable, -1, sizeof(float) * YahtzeeStatusCode::MAX);
	//rewardTable = std::vector<fixed>(YahtzeeStatusCode::MAX, -1);
	for (size_t index = 0; index < YahtzeeStatusCode::MAX; index++)
	{
		rewardTable[index] = -1;
	}
}

SimpleAgent::SimpleAgent(FILE* f) : IAgent()
{
	rewardTable = new float[YahtzeeStatusCode::MAX];
	fread(rewardTable, sizeof(int), YahtzeeStatusCode::MAX, f);

}

SimpleAgent::~SimpleAgent()
{
	delete[] rewardTable;
}

float SimpleAgent::train()
{
	return getExpect(&YahtzeeStatus::getInitialState());
}

float SimpleAgent::getExpect(YahtzeeStatus* status)
{
	return getExpectByHash(status->getStatusHash());
}

float SimpleAgent::getExpectByHash(const hash_t hash, Action* next)
{
	if (hash > YahtzeeStatusCode::MAX)
	{
		return 0;
	}
	if (rewardTable[hash] >= 0 && next == nullptr)
	{
		return rewardTable[hash];
	}
	YahtzeeStatusCode code = YahtzeeStatusCode::ParseHashCode(hash);

	if (code.state == 0x1fff)
	{
		return rewardTable[hash] = 0;
	}

	Dices dices = YahtzeeStatus::parseDices(code.diceHash);
	bool dicesReady = dices.ready();

	bool dicesValid = dices.valid();
	if (!dicesValid)
	{
		return 0;
	}

	float expect = -1;
	if (dicesReady)
	{
		for (ScoreType iter = ScoreType::Ones; iter <= ScoreType::Yahtzee; iter = (ScoreType)((int)iter << 1)) // Try to write
		{
			if (code.state & (int)iter)
			{
				continue;
			}
			int scoreChange = 0;
		    hash_t newHash = controller->writeByHash(hash, iter, &scoreChange);
			float newExpect = getExpectByHash(newHash) + scoreChange;
			if (newExpect > expect)
			{
				expect = newExpect;
				if (next != nullptr)
				{
					next->type = ActionType::Write;
					next->written = iter;
				}
			}
		}
		if (code.rerollTimes > 0)
		{
			for (int bits = 0; bits < 31; bits++)
			{
				YahtzeeStatusCode newCode = code;

				hash_t hash = 0;
				for (int bit = 0; bit < 5; bit++)
				{
					if ((1 << bit) & bits)
					{
						hash = (hash << 3) | dices[bit];
					}
				}

				newCode.diceHash = YahtzeeStatus::composeDicehash(hash);

				float newExpect = getExpectByHash(newCode.getHash());
				if (newExpect > expect)
				{
					expect = newExpect;
					if (next != nullptr)
					{
						next->type = ActionType::Reroll;
						next->kept.clear();

						int c = 0;
						for (int bit = 0; bit < 5; bit++)
						{
							if ((1 << bit) & bits)
							{
								next->kept[c++] = dices[bit];
							}
						}
					}
				}
			}
		}
	}
	else
	{
		expect = 0;
		int firstZeroIndex = 0;
		for (size_t i = 0; i < 5; i++)
		{
			if (dices[i] == 0)
			{
				firstZeroIndex = i;
			}
		}
		for (int d = 1; d <= 6; d++)
		{
			dices[firstZeroIndex] = d;
			compressed_dicehash_t dicehash = YahtzeeStatus::composeDices(dices);
			YahtzeeStatusCode newCode = code;

			if (dices.ready())
			{
				newCode.rerollTimes--;
			}

			newCode.diceHash = dicehash;
			float newExpect = getExpectByHash(newCode.getHash()) / 6.0;
			expect += newExpect;
		}
		dices[firstZeroIndex] = 0;
	}
	static int count = 0;

	count++;
	if ((count & 0xfffff)==0)
	{
		printf("training... No.%d : %u -> %f\n", count, hash, (float)expect);
	}
	return rewardTable[hash] = expect;
}

Action SimpleAgent::getAction(YahtzeeStatus* status)
{
	hash_t hash = status->getStatusHash();
	Action best;
	getExpectByHash(hash, &best);

	return best;
}


void SimpleAgent::save(FILE* f)
{
	fwrite(rewardTable, sizeof(float), YahtzeeStatusCode::MAX, f);
}