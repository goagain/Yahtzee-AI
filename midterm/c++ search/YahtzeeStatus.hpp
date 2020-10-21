#pragma once

#include <vector>
#include <map>

typedef std::vector<int> Dices;
typedef uint32_t hash_t;
typedef uint16_t dicehash_t;
typedef uint16_t compressed_dicehash_t;

enum class ScoreType : uint16_t
{
    Ones = 1 << 0,
    Twos = 1 << 1,
    Threes = 1 << 2,
    Fours = 1 << 3,
    Fives = 1 << 4,
    Sixes = 1 << 5,
    Chance = 1 << 6,
    ThreeOfAKind = 1 << 7,
    FourOfAKind = 1 << 8,
    FullHouse = 1 << 9,
    SmallStraight = 1 << 10,
    LargeStraight = 1 << 11,
    Yahtzee = 1 << 12,
    Bonus = 1 << 13,
    ALL = 0x1fff
};

struct YahtzeeStatusCode
{
    hash_t state;
    compressed_dicehash_t diceHash;
    int8_t upperSum;
    int8_t rerollTimes;

    static YahtzeeStatusCode ParseHashCode(hash_t code);
    hash_t getHash();

    static const hash_t MAX = 1 << 13 << 9 << 6 << 2;
};

class YahtzeeStatus
{
public:
    static std::vector<ScoreType> AllScoreType;

    Dices dices;
    std::map<ScoreType, int> scoreMap;
    int reroll_times = 0;
    int score = 0;

    int getUpperSum() const;

    int getLowerSum() const;

    int getScoreByType(ScoreType type) const;

    bool end() const;

    hash_t getStatusHash() const;

    static int getDecDice(const Dices& dices);

    static YahtzeeStatusCode parseStatusCode(hash_t code);

    static compressed_dicehash_t composeDices(const Dices &dices);

    static compressed_dicehash_t composeDicehash(hash_t hash);

    static Dices parseDices(compressed_dicehash_t compressedDiceHash);

    static void init();

    static YahtzeeStatus getInitialState();
private:
    static std::map<dicehash_t, compressed_dicehash_t> dicemap;
    static std::map<compressed_dicehash_t, dicehash_t> reverse_dicemap;
};
