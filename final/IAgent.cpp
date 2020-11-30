#include "IAgent.h"

IAgent::IAgent()
{
	controller = new YahtzeeController();
}

IAgent::~IAgent()
{
	delete this->controller;
}

float IAgent::simulate(int n, FILE* output)
{
	if (output != nullptr)
	{
		fprintf(output, "game id,turn,dice,action,written,kept,score change,score,expect\r\n");
	}
	float sum = 0;

	for (int no = 0; no < n; no++)
	{
		//printf("Start game: No. %d\n", no);

		YahtzeeStatus game = controller->startNewGame();

		int turn = 0;
		while (!game.end())
		{
			bool zero = false;
			bool allzero = true;

			for (int id = 0; id < 5; id++)
			{
				if (game.dices.dices[id] == 0)
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
					fprintf(output, "%d,%d,%05d,%s,%s,,,%d,%f\r\n", no, turn, YahtzeeStatus::getDecDice(game.dices), "Roll", "", game.score, getExpect(&game) + (double)game.score);
				}
			}
			else if (zero)
			{
				printf("shouldn't ...");
			}
			else
			{
				Action bestAction = getAction(&game);
				if (bestAction.type == ActionType::Roll)
				{
					game = controller->roll(game);
					if (output != nullptr)
					{
						fprintf(output, "%d,%d,%05d,%s,%s,%d,,%d,%f\r\n", no, turn, YahtzeeStatus::getDecDice(game.dices), "Reroll", "", YahtzeeStatus::getDecDice(bestAction.kept), game.score, getExpect(&game) + (double)game.score);
					}
				}
				else if (bestAction.type == ActionType::Write)
				{
					int scoreChange = 0, bonus = 0;
					game = controller->write(game, bestAction.written, &scoreChange, &bonus);
					if (output != nullptr)
					{
						fprintf(output, "%d,%d,%05d,%s,%d,,%d,%d,%f\r\n", no, turn, YahtzeeStatus::getDecDice(game.dices), "Write", bestAction.written, scoreChange - bonus, game.score - bonus, getExpect(&game) + (double)game.score);
						if (bonus > 0)
						{
							fprintf(output, "%d,%d,%05d,%s,%d,,%d,%d,%f\r\n", no, turn, YahtzeeStatus::getDecDice(game.dices), "Write", ScoreType::Bonus, bonus, game.score, getExpect(&game) + (double)game.score);
						}
					}
				}
				else if (bestAction.type == ActionType::Reroll)
				{
					game = controller->reroll(game, bestAction.kept);
					if (output != nullptr)
					{
						fprintf(output, "%d,%d,%05d,%s,%s,%d,,%d,%f\r\n", no, turn, YahtzeeStatus::getDecDice(game.dices), "Reroll", "", YahtzeeStatus::getDecDice(bestAction.kept), game.score, getExpect(&game) + (double)game.score);
					}
				}
			}
		}
		if (game.getScoreByType(ScoreType::Bonus) == 0 && output != nullptr)
		{
			fprintf(output, "%d,%d,%05d,%s,%d,,%d,%d,%f\r\n", no, turn, YahtzeeStatus::getDecDice(game.dices), "Write", ScoreType::Bonus, 0, game.score, getExpect(&game) + (double)game.score);
		}
		sum += game.score;
		printf("No. %d : %d\n", no, game.score);
	}
	return sum / (float)n;
}
