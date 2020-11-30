#pragma once

#include <vector>
#include <map>
#include <algorithm>
struct Dices
{
    uint8_t dices[5];

    inline bool ready() const&
    {
        return dices[0] > 0 &&
            dices[1] > 0 &&
            dices[2] > 0 &&
            dices[3] > 0 &&
            dices[4] > 0;
    }

    inline bool valid() const&
    {
        return dices[0] >= 0 && dices[0] <= 6 &&
            dices[1] >= 0 && dices[1] <= 6 &&
            dices[2] >= 0 && dices[2] <= 6 &&
            dices[3] >= 0 && dices[3] <= 6 &&
            dices[4] >= 0 && dices[4] <= 6;
    }

    inline uint8_t& operator[](int index)
    {
        return dices[index];
    }

    inline const uint8_t& operator[](int index) const
    {
        return dices[index];
    }

    inline size_t size() const&
    {
        return 5;
    }

    void clear()
    {
        for (size_t i = 0; i < 5; i++)
        {
            dices[i] = 0;
        }
    }

    void sort()
    {
        std::sort(dices, dices + 5);
    }
};
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
    static compressed_dicehash_t dicemap[65535];
    static dicehash_t reverse_dicemap[65535];

    //static std::map<dicehash_t, compressed_dicehash_t> dicemap;
    //static std::map<compressed_dicehash_t, dicehash_t> reverse_dicemap;
};
