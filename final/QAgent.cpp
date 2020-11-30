#include "QAgent.h"
#include "YahtzeeStatus.h"
#include "YahtzeeController.h"

#include <stdlib.h>
#include <ctime>
#include <thread>
#include <iostream>

void QAgent::train(float learningRate)
{
	clock_t start, end;

	start = clock();

	int threadNum = 12;
	std::vector<std::thread> threads;
	for (size_t threadID = 0; threadID < threadNum; threadID++)
	{

		hash_t start = YahtzeeStatusCode::MAX / threadNum * threadID;
		hash_t end = YahtzeeStatusCode::MAX / threadNum * (threadID + 1);
		if (end > YahtzeeStatusCode::MAX)
		{
			end = YahtzeeStatusCode::MAX;
		}
		threads.push_back(std::thread([=]() {
			YahtzeeController controller(std::time(nullptr));

			printf("%lld Running...\n", threadID);
			for (hash_t index = start; index < end; index++)
			{
				float newExpect = __train(&controller, index);
				qTable[index] = qTable[index] * (1 - learningRate) + newExpect * learningRate;

			//	if ((index - start) % (1<<26) == 0)
			//	{
			//		printf("%lld %d/%d...\n", threadID, index - start, end - start);
			//	}
			}
			printf("%lld Exiting...\n", threadID);

		}
		));

	}
	for (auto& thread : threads)
	{
		thread.join();
	}
	end = clock();
	std::cout << ((float)(end - start)) / CLOCKS_PER_SEC << " s" << std::endl;
}

QAgent::QAgent()
{
	qTable = new float[YahtzeeStatusCode::MAX];

	for (size_t i = 0; i < YahtzeeStatusCode::MAX; i++)
	{
		qTable[i] = 0;
	}
}

QAgent::~QAgent()
{
	if (qTable != nullptr)
	{
		delete[] qTable;
	}
}

Action QAgent::getAction(YahtzeeStatus* status)
{
	hash_t hash = status->getStatusHash();
	Action best;
	best.kept.clear();
	__train(this->controller, hash, &best);

	return best;
}

float QAgent::getExpect(YahtzeeStatus* status)
{
	hash_t hash = status->getStatusHash();
	return qTable[hash];
}

float QAgent::__train(YahtzeeController *controller, hash_t hash, Action *bestAction)
{
	if (hash > YahtzeeStatusCode::MAX)
	{
		return 0;
	}
	YahtzeeStatusCode code = YahtzeeStatusCode::ParseHashCode(hash);

	if (code.state == 0x1fff)
	{
		return 0;
	}
	Dices dices = YahtzeeStatus::parseDices(code.diceHash);
	bool dicesReady = dices.ready();

	bool dicesValid = dices.valid();
	if (!dicesValid)
	{
		return 0;
	}

	if (!dicesReady && code.rerollTimes > 0)
	{
		int batch = 16;
		float sum = 0;
		for (int i = 0; i < batch; i++)
		{
			hash_t newHash = controller->rollByHash(hash);
			sum += qTable[newHash];
		}
		if (bestAction != nullptr)
		{
			bestAction->type = ActionType::Roll;
		}
		return sum / batch;
	}
	else
	{
		float expect = -1;
		for (ScoreType iter = ScoreType::Ones; iter <= ScoreType::Yahtzee; iter = (ScoreType)((int)iter << 1)) // Try to write
		{
			if (code.state & (int)iter)
			{
				continue;
			}
			int scoreChange = 0;
			hash_t newHash = controller->writeByHash(hash, iter, &scoreChange);
			float newExpect = qTable[newHash] + scoreChange;
			if (newExpect > expect)
			{
				expect = newExpect;
				if (bestAction != nullptr)
				{
					bestAction->type = ActionType::Write;
					bestAction->written = iter;
					bestAction->kept.clear();
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

				float newExpect = qTable[newCode.getHash()];
				if (newExpect > expect)
				{
					expect = newExpect;
					if (bestAction != nullptr)
					{
						bestAction->type = ActionType::Reroll;
						bestAction->kept.clear();
						for (int bit = 0; bit < 5; bit++)
						{
							if ((1 << bit) & bits)
							{
								bestAction->kept[bit] = dices[bit];
							}
							else
							{
								bestAction->kept[bit] = 0;
							}
						}
					}
				}
			}
		}
		return expect;
	}
}

QAgent::QAgent(FILE* f) : IAgent()
{
	qTable = new float[YahtzeeStatusCode::MAX];
	fread(qTable, sizeof(float), YahtzeeStatusCode::MAX, f);
}

void QAgent::save(FILE* f)
{
	fwrite(qTable, sizeof(float), YahtzeeStatusCode::MAX, f);
}

void QAgent::load(FILE* f)
{
	fread(qTable, sizeof(float), YahtzeeStatusCode::MAX, f);
}
