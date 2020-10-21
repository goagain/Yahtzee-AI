class ScoreType(int):
    Ones = 1 << 0
    Twos = 1 << 1
    Threes = 1 << 2
    Fours = 1 << 3
    Fives = 1 << 4
    Sixes = 1 << 5
    Chance = 1 << 6
    ThreeOfAKind = 1 << 7
    FourOfAKind = 1 << 8
    FullHouse = 1 << 9
    SmallStraight = 1 << 10
    LargeStraight = 1 << 11
    Yahtzee = 1 << 12
    Bonus = 1 << 13


class YahtzeeStatus():
    ALL_SCORE_TYPE = [ScoreType.Ones,
                      ScoreType.Twos,
                      ScoreType.Threes,
                      ScoreType.Fours,
                      ScoreType.Fives,
                      ScoreType.Sixes,
                      ScoreType.Chance,
                      ScoreType.FourOfAKind,
                      ScoreType.FullHouse,
                      ScoreType.SmallStraight,
                      ScoreType.LargeStraight,
                      ScoreType.Yahtzee,
                      ScoreType.Bonus
                      ]

    def __init__(self):
        self.dices = []
        self.score_map = {}
        self.reroll_times = 0
        self.score = 0

    @property
    def get_available_actions(self):
        return None

    def get_available_score_type(self):
        types = YahtzeeStatus.ALL_SCORE_TYPE

        return [x for x in types if self.get_score_by_type(x) is None]

    def get_score_by_type(self, score_type):
        if score_type in self.status_map:
            return self.score_map[score_type]

    @property
    def upper_sum(self):
        numerical = 0
        for x in [ScoreType.Ones,
                  ScoreType.Twos,
                  ScoreType.Threes,
                  ScoreType.Fours,
                  ScoreType.Fives,
                  ScoreType.Sixes]:
            if x in self.score_map:
                numerical += self.score_map[x]
        return numerical

    @property
    def status_code(self):
        state = 0
        # print(YahtzeeStatus.ALL_SCORE_TYPE, file=sys.stderr)

        for x in YahtzeeStatus.ALL_SCORE_TYPE[:-1]:
            if x in self.score_map:
                state += x

        self.dices.sort()
        dice_hash = 0
        for index, dice in enumerate(self.dices):
            dice_hash = dice_hash * 10 + dice

        numerical = self.upper_sum
        if numerical > 63:
            numerical = 63

        return state*100000*100*10 + dice_hash * 100 * 10 + numerical * 10 + self.reroll_times

    @classmethod
    def parse_status_code(cls, code):
        reroll_times = code % 10
        numerical = code // 10 % 100
        dice_hash = code // (10 * 100) % 100000
        state = code // (10 * 100 * 100000)
        return (state, dice_hash, numerical, reroll_times)

    @classmethod
    def compose_status_code(cls, state, dice_hash, numerical, reroll_times):
        return state*100000*100*10 + dice_hash * 100 * 10 + numerical * 10 + reroll_times

    @classmethod
    def compose_dices(cls, dices):
        dices = list(dices)
        dices.sort()
        dice_hash = 0
        for index, dice in enumerate(dices):
            dice_hash = dice_hash * 10 + dice
        return dice_hash

    _dices_hashmap = {}

    @classmethod
    def parse_dices(cls, dice_hash):
        if dice_hash in cls._dices_hashmap:
            return cls._dices_hashmap[dice_hash]

        dices = []
        t_dice_hash = dice_hash
        while t_dice_hash > 0:
            dices.append(t_dice_hash % 10)
            t_dice_hash //= 10
        dices.sort()
        cls._dices_hashmap[dice_hash] = dices
        return dices

    def copy(self):
        cloned = YahtzeeStatus()
        cloned.dices = self.dices.copy()
        cloned.score_map = self.score_map.copy()
        cloned.reroll_times = self.reroll_times
        cloned.turn = self.turn
        cloned.score = self.score
        return cloned
