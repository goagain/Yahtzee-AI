# Yahtzee Final Project Report

## Rui Tang

## Yingshu Wang


## Executive Summary

```
In order to show the far-reaching significance of artificial intelligence that can
```
solve complex problems, our team decided to train AI to execute and train a North

American dice game called Yahtzee. The reason for using this game is because its

winning strategy does not come from a single element, but requires a combination of

multiple factors to win the final victory. This is a good performance for training AI's

ability to solve problems.

```
Out of personal interest in this game and recognition of AI's problem-solving
```
ability, we will let AI play several consecutive games according to the rules and

continue to let it learn, develop and refine strategies. The way to improve AI strategy

comes from data collection and processing. Then the AI will continue to learn and

improve its strategy in the Yahtzee game based on the data. We hope to finally see

an objective win rate in this game.

```
In the report, we will introduce our AI algorithm design, AI's requirements for
```
collected data, and the strategy of how to strengthen AI through data. Finally, the

results are used to explain the feasible methods and advantages of AI for solving

complex problems.

## Introduction

```
Yahtzee is a popular dice game in North America. The game is a
```
development of earlier dice games such as Poker Dice, Yacht and Generala. It is

also similar to Yatzy, which is popular in Scandinavia. [1]

```
The objective of the game is to score points by rolling five dice to make
```
certain combinations. The dice can be rolled up to three times in a turn to try to make

various scoring combinations and dice must remain in the box. A game consists of

thirteen rounds. After each round the player chooses which scoring category is to be

used for that round. Once a category has been used in the game, it cannot be used

again. The scoring categories have varying point values, some of which are fixed

values and others for which the score depends on the value of the dice. A Yahtzee is

five-of-a-kind and scores 50 points, the highest of any category. The winner is the

player who scores the most points.


```
A classic Yahtzee game has 13 turns and 13 types of scores plus a bonus
```
score. Each type of score must and only be selected once. In each turn, players can

roll five dice and reroll up to two times. When rerolling, the player can keep any of

the dice at that moment and reroll the others. If players want to write the score onto

the scoreboard (or the reroll times used out), they should select one of the score

types and calculate the corresponding score then record it. If the numerical score is

higher than 63, a 35-score bonus will be earned.

```
In 2016, AlphaGo defeated the world Go champion Lee Sedol and shocked
```
the whole world. But the more far-reaching significance lies in it shows the possibility

that Artificial Intelligence(AI) can solve complex problems. [2] Inspired by AlphaGo

and based on our personal interests, our team decided to train a game AI. The

widely-used models can be optimized and implemented into various types of games.

```
Many people regard Yahtzee as a lucky game. Yahtzee’s optimal winning
```
strategy is not immediately obvious. Few reports indicate there are not

straightforward relationships between each factor. For example, Glenn, J. in his

report named “An optimal strategy for Yahtzee” (2006) found that “the best strategy

does not depend on what has been scored in each category” by simulating an

extreme environment that has only one category, Yahtzee. They keep track of the

expected score for remaining turns, finding the average score of the whole game is

the average score of the rest game at the beginning’s state. He has identified an

optimal strategy that maximizes the expected score. However, it’s not about

enhancing the probability of winning. In this report, our solutions to maximum

Yahtzee winning probability will be discussed.

## Rational Statement

```
In the previous version, we built an AI based on simple search, which can get
```
the ideal expected value. This time we want to get the result with a self-learning

function: QLearning, rather than a certain algorithm.

## Problems

We found and solved these problems in this project:


1. We used many C++ STL features in the previous version. However, the usage

```
of STL is always with memory allocation and deallocation, which makes our
```
```
program very slow. We abandoned STL features like map and vector, using C
```
```
basic array structure instead.
```
2. To accelerate training speed, we used multi-thread to calculate. In

```
multi-thread programming, we found cache conflict influences the speed very
```
```
much, so that we allocate independent memory for each thread.
```
3. The choice of coefficients in continuous training is very important. Too large

```
coefficients will cause the results to not converge, and too small coefficients
```
```
will converge too slowly. In the end, we chose the exponential decay method.
```
## Data Requirements

## Data

```
This section is a list of your data sources and data sets used for the
```
project. It also includes descriptions of what they are and where they’re located.

```
In this project, our models are self-driven. Like most reinforcement learning
```
projects, we programmed a Yahtzee game simulation, allowing the computer against

the computer. All the computer’s operations are recorded, including the game ID, the

number of turns, the player’s action, randomly generated five dice' numbers, the dice

player kept, the player existing scores and the player’s expectation. We extract

information from the environment and self-iterate to an optimal solution.

```
Assumption Constraint
```
```
Detailed (Tracing all steps) can not 100% correctly simulate human
```
```
behavior
```
```
All data consist of the same range
```
```
Data collected at one Standard
```

```
Below is our data list:
```
- ​ **Game ID:** ​ the number of games
- ​ **turn:** ​ the number of turns in one game. min 1, max 13
- ​ **Dice:** ​random dices computer generated. For example,123 means having three

dice, each dice have 1, 2, 3 points respectively

- ​ **Action:** ​player's behavior, computer-generated
- ​ **Written:** ​At the end of each turn, the category player chosen
- ​ **Kept:** ​ The dice player keep
- ​ **Score:** ​The player existing score
- ​ **Expect:** ​It's computer-generated data. If the player feels lucky (has positive

feedback from previous play), the expectation tends to be high.

```
● For blank dices: E=Average ({E(p)| p in all dices possibly})
```
```
● Expected value for end game: 0, because we can do nothing
```
```
● For each step needs to decide: E = max(E(next) + score in this turn| next
```
```
in all possible action))
```

## Model/Architecture Approach.

```
In this project, our algorithm is taking advantage of QLearning. In the
```
previous project version, we turned out that the state space is very huge. Thus,

we use a different way to calculate the QTable. Like the previous version, we

defined the same data structure and initialized the QTable to 0(randomly

initialization is also possible). After that, the algorithm can be presented as

following:

```
a. For states whose dice have been rolled, find all the feasibility of the next
```
```
state, and get the expected value. The expected value of this state is the
```
```
maximum value from all the next states where the score gained plus the
```
```
expected value of the next state.
```
```
b. For states whose dice have not been rolled(where zeros inside), we
```
```
randomly roll and get a new state. The current state’s expected value
```
```
equals the next. To accelerate the calculation process and improve the
```
```
precision, each time we roll multiple times as a batch. In this project, 16 is
```
```
considered as a suitable batch size which is acceptable for stability and
```
```
not too slow.
```
```
c. Same as the previous project, the expected value of the final state is 0.
```
```
After the above calculation process, we can get the new expected value of
```
each state. Although neither the old or new expected value is the ideal value, we

use the following iteration to approach the ideal value:

```
Q ′= Q 1 earningRate ) earningRate
*
```
```
( − l + S
*
```
```
l
```
```
Q is the last value in QTable
```
```
Q’ is the renewed value in QTable
```
```
S is the expected value in the current iteration
```
```
A too-large learning rate will cause instability so that in the first epoch we
```
set the learning rate to 1 and each following epoch we set the learning rate to

0.99 times the previous learning rate.


```
After 500 iterations, we get the model and it is very close to the ideal
```
QTable which we got in the previous project.

```
*The ideal expectation in each game is approximately 224.61.
```
## Project Work Breakdown

**Rui:**

```
● Responsible for the design and writing of logic and algorithms in the Yahtzee
```
```
game’s code
```
```
● Responsible for the interpretation and result reporting of most algorithms in
```
```
the report
```
```
● Run the game to report the results and check the algorithm and comment
```
**Sue:**

```
● Run the program, report and collect the results, and perform data processing
```
```
● Performing EDA based on the data given after running the program
```
```
● Report most of the results and conclusions related to the data in the report
```
```
● Assist game algorithm and programming work
```

## EDA

1. Blue zones mean at that turn this score type was given up


2. FullHouse, Yahtzee and LongStraight have a decreasing tendency as the

```
number of turns increases. The player tends to give up Yahtzee and
```
```
LongStraight in the later turns.
```
**Others:**

## Data Preprocessing pipeline

```
● importing data
```

```
● data cleaning
```
Separating dive data For example, from 34612 to dice one is 3, dice 2 is 4 ...

from string to int


Originally, if the player hasn't chosen any dice to keep, the dice data shows zero.

Now, we replace any zero value to NaN except the game id.

## ​Algorithm Evaluation

```
In this project, we have used two types of algorithms.
```
```
a) Memorized Simple Search
```
```
b) QLearning
```
```
Search QLearning
```

Pros 1. High precision

2. Fast
    1. easy to use multi thread
    2. Algorithm is easy to

```
understand
```
Cons 1. Need to know the exact

```
process of the game,
```
```
including the random
```
```
process
```
2. Hard to pararell
3. Model is very hude (4 GB)
    1. Very slow. For 500

```
times iteration, 5 hours
```
```
has been spent
```
2. Model is very hude

### (4GB)


## Final Inference

```
See in the C++ project. SimpleSearchAgent.cpp is for the memorized search
```
algorithm, and QAgent is for the QLearning algorithm.

## Appendices

[1] Yahtzee. (2020, November 16). Retrieved November 25, 2020, from

https://en.wikipedia.org/wiki/Yahtzee

[2] Vincent, J. (2019, November 27). Former Go champion beaten by DeepMind

retires after declaring AI invincible. Retrieved November 25, 2020, from

https://www.theverge.com/2019/11/27/20985260/ai-go-alphago-lee-se-dol-retired-de

epmind-defea

[3] Glenn, J. 2006. An optimal strategy for Yahtzee. Loyola College in Maryland,

Tech. Rep. CS-TR-0002.


