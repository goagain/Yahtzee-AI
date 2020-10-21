#include <cstdio>
#include <iostream>
#include <ctime>

#include "Agent.hpp"
#include "YahtzeeStatus.hpp"

using namespace std;

int main()
{
	clock_t start, end;
	start = clock();

	YahtzeeStatus::init();

	YahtzeeController* controller = new YahtzeeController(time(nullptr));

	FILE* model = fopen("model.dat", "rb");
	Agent *agent;
	if (model != nullptr)
	{
		agent = new Agent(model);
		fclose(model);
		end = clock();
		cout << ((float)(end - start)) / CLOCKS_PER_SEC << " s" << endl;
		puts("load model complete");
	}
	else
	{
		agent = new Agent();
		end = clock();
		cout << ((float)(end - start)) / CLOCKS_PER_SEC << " s" << endl;
		puts("train complete");
	}
	cout << agent->getExpect(YahtzeeStatus::getInitialState()) << endl;


	YahtzeeStatus status;
	status.dices = Dices{ 2,5,6,6,2 };
	status.scoreMap[ScoreType::Chance] = 23;
	status.scoreMap[ScoreType::FourOfAKind] = 23;
	status.scoreMap[ScoreType::SmallStraight] = 40;

	status.reroll_times = 0;
	status.score = 76;
	Action bestAction = agent->getBestAction(status);
	auto result = controller->write(status, bestAction.written);
	
	char c;
	puts("Enter y to save the model");

	if (cin >> c, c == 'y')
	{
		puts("start writing...");
		start = clock();
		FILE* f = fopen("model.dat", "wb");
		fwrite(agent->rewardTable, sizeof(int), YahtzeeStatusCode::MAX, f);
		fclose(f);
		end = clock();
		cout << ((float)(end - start)) / CLOCKS_PER_SEC << " s" << endl;
	}
	//puts("Enter y to simulate");
	//if (cin >> c, c == 'y')
	//{
		FILE* f = fopen("simulation-1000.csv", "wb");
		agent->simulate(1000, f);
		fclose(f);

		f = fopen("simulation-100000.csv", "wb");
		agent->simulate(100000, f);
		fclose(f);
	/*}*/

}