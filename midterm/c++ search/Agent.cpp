#include "Agent.hpp"
#include "YahtzeeController.hpp"


Agent::Agent()
{
	rewardTable = new float[YahtzeeStatusCode::MAX];
	//memset(rewardTable, -1, sizeof(float) * YahtzeeStatusCode::MAX);
	//rewardTable = std::vector<fixed>(YahtzeeStatusCode::MAX, -1);
	for (size_t index = 0; index < YahtzeeStatusCode::MAX; index++)
	{
		rewardTable[index] = -1;
	}

	controller = new YahtzeeController();
}

Agent::Agent(FILE* f)
{
	rewardTable = new float[YahtzeeStatusCode::MAX];
	fread(rewardTable, sizeof(int), YahtzeeStatusCode::MAX, f);

	controller = new YahtzeeController();
}

Agent::~Agent()
{
	delete[] rewardTable;
	delete controller;
}

float Agent::getExpect(const YahtzeeStatus& status)
{
	return getExpectByHash(status.getStatusHash());
}

float Agent::getExpectByHash(const hash_t hash, Action* next)
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
	bool dicesReady = std::all_of(dices.begin(), dices.end(), [](int i) { return i > 0; });

	bool dicesValid = std::all_of(dices.begin(), dices.end(), [](int i) { return i >= 0 && i <= 6; });
	if (!dicesValid || code.rerollTimes > 2)
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
			for (int bits = 0; bits <= 31; bits++)
			{
				YahtzeeStatusCode newCode = code;
				newCode.rerollTimes -= 1;

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
						for (int bit = 0; bit < 5; bit++)
						{
							if ((1 << bit) & bits)
							{
								next->kept.push_back(dices[bit]);
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
		for (size_t i = 4; i >= 1; i--)
		{
			if (dices[i] == 0)
			{
				firstZeroIndex = i;
				break;
			}
		}
		for (int d = 1; d <= 6; d++)
		{
			dices[firstZeroIndex] = d;
			compressed_dicehash_t dicehash = YahtzeeStatus::composeDices(dices);
			YahtzeeStatusCode newCode = code;
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

Action Agent::getBestAction(const YahtzeeStatus& status)
{
	hash_t hash = status.getStatusHash();
	Action best;
	getExpectByHash(hash, &best);

	return best;
}

void Agent::simulate(int n, FILE* output)
{
	if (output == nullptr)
	{
		printf("output is null!\n");
		return;
	}
	fprintf(output, "game id,turn,dice,action,written,kept,score change,score,expect\r\n");

	for (int no = 0; no < n; no++)
	{
		//printf("Start game: No. %d\n", no);

		YahtzeeStatus game = controller->startNewGame();

		int turn = 0;
		while (!game.end())
		{
			bool zero = false;
			bool allzero = true;

			for (int id = 0; id < game.dices.size(); id++)
			{
				if (game.dices[id] == 0)
				{
					zero = true;
				}
				else
				{
					allzero = false;
				}
			}

			if (allzero)
			{
				game = controller->roll(game);
				turn++;
				if (output != nullptr)
				{
					fprintf(output, "%d,%d,%05d,%s,%s,,,%d,%f\r\n", no, turn, YahtzeeStatus::getDecDice(game.dices), "Roll", "", game.score, getExpect(game)+ (double)game.score);
				}
			}
			else if (zero)
			{
				printf("shouldn't ...");
			}
			else
			{
				Action bestAction = getBestAction(game);
				if (bestAction.type == ActionType::Write)
				{
					int scoreChange = 0, bonus = 0;
					game = controller->write(game, bestAction.written, &scoreChange, &bonus);

					fprintf(output, "%d,%d,%05d,%s,%d,,%d,%d,%f\r\n", no, turn, YahtzeeStatus::getDecDice(game.dices), "Write", bestAction.written,scoreChange - bonus, game.score - bonus, getExpect(game) + (double)game.score);
					if (bonus > 0)
					{
						fprintf(output, "%d,%d,%05d,%s,%d,,%d,%d,%f\r\n", no, turn, YahtzeeStatus::getDecDice(game.dices), "Write", ScoreType::Bonus,bonus, game.score, getExpect(game) + (double)game.score);
					}
				}
				else if (bestAction.type == ActionType::Reroll)
				{
					game = controller->reroll(game, bestAction.kept);
					fprintf(output, "%d,%d,%05d,%s,%s,%d,,%d,%f\r\n", no, turn, YahtzeeStatus::getDecDice(game.dices), "Reroll", "", YahtzeeStatus::getDecDice(bestAction.kept), game.score, getExpect(game) + (double)game.score);
				}
			}
		}
		if (game.getScoreByType(ScoreType::Bonus) == 0)
		{
			fprintf(output, "%d,%d,%05d,%s,%d,,%d,%d,%f\r\n", no, turn, YahtzeeStatus::getDecDice(game.dices), "Write", ScoreType::Bonus, 0, game.score, getExpect(game) + (double)game.score);
		}
		//printf("No. %d : %d\n", no, game.score);
	}
}
