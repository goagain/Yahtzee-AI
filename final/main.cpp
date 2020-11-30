#include <cstdio>
#include <iostream>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <process.h>
#include <direct.h>
#include "YahtzeeStatus.h"
#include "QAgent.h"
#include "SimpleAgent.h"
using namespace std;

int main()
{
	YahtzeeStatus::init();

	clock_t start, end;	
	srand(time(0));

	//SimpleAgent agent;
	//cout << agent.train() << endl;
	//FILE* f = fopen("model.dat", "wb");
	//agent.save(f);
	//fclose(f);

	//FILE* f = fopen("model.dat", "rb");

	//QAgent qAgent(f);
	//fclose(f);

	//f = fopen("record.csv", "wb");
	//float result = qAgent.simulate(1000, f);
	//printf("Average: %.2f\n", result);

	QAgent qAgent;

	uint32_t t = time(0);
	char s[200];
	sprintf(s, "%u", t);

	_mkdir(s);

	sprintf(s, "%u/avg.csv", t);
	FILE* avg = fopen(s, "wb");
	fprintf(avg, "generation,learningRate,avg\n");
	float learningRate = 1;
	for (int i = 0; i < 500; i++)
	{
		qAgent.train(learningRate);

		sprintf(s, "%u/simulation_%d.csv", t, i);
		FILE* f = fopen(s, "wb");
		float result = qAgent.simulate(1000, f);
		fclose(f);
		printf("Average: %.2f\n", result);
		fprintf(avg, "%d,%.5f,%.4f\n", i, learningRate, result);

		learningRate *= 0.99;
	}
	sprintf(s, "%u/model.dat", t);

	FILE* fmodel = fopen(s, "wb");
	qAgent.save(fmodel);
	fclose(fmodel);

	//while (true)
	//{
	//	printf("t to training\n s to simulation\n f to save model\n l to load model\n");
	//	printf("input command:");

	//	char c;
	//	cin >> c;
	//	if (c == 't')
	//	{
	//		float learningRate;
	//		puts("input learning rate:");
	//		cin >> learningRate;
	//		qAgent.train(learningRate);
	//	}
	//	if (c == 's')
	//	{
	//		char s[20];
	//		sprintf(s, "simulation_%d.csv", no++);
	//		FILE* f = fopen(s, "wb");
	//		float result = qAgent.simulate(1000, f);
	//		fclose(f);
	//		printf("Average: %.2f\n", result);
	//	}
	//	if (c == 'f')
	//	{
	//		FILE* f = fopen("model.dat", "wb");
	//		qAgent.save(f);
	//		fclose(f);
	//	}
	//	if (c == 'l')
	//	{
	//		FILE* f = fopen("model.dat", "wb");
	//		qAgent.load(f);
	//		fclose(f);
	//	}
	//}
	system("pause");
}