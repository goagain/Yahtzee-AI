from YahtzeeStatus import ScoreType
from YahtzeeStatus import YahtzeeStatus


class Controller(object):
    def __init__(self):
        self.basic_scores = {
            ScoreType.Ones: self.Ones,
            ScoreType.Twos: self.Twos,
            ScoreType.Threes: self.Threes,
            ScoreType.Fours: self.Fours,
            ScoreType.Fives: self.Fives,
            ScoreType.Sixes: self.Sixes,
            ScoreType.Chance: self.Chance,
            ScoreType.ThreeOfAKind: self.ThreeOfAKind,
            ScoreType.FourOfAKind: self.FourOfAKind,
            ScoreType.FullHouse: self.FullHouse,
            ScoreType.SmallStraight: self.SmallStraight,
            ScoreType.LargeStraight: self.LargeStraight,
            ScoreType.Yahtzee: self.Yahtzee,
        }
        self._judger_cache = {}

    @property
    def actions(self):
        return self.basic_scores.keys()

    def get_writable(self, status):
        return [x for x in self.basic_scores if x not in status.score_map]

    def get_writable_by_statuscode(self, status_code):
        state, dice_hash, numerical, reroll_times = YahtzeeStatus.parse_status_code(
            status_code)

        return [x for x in self.basic_scores if (x & state) == 0]

    def judge(self, status, selected_type):
        if len(status.dices) != 5 or\
                not all(x >= 1 and x <= 6 for x in status.dices):
            raise Exception(
                "dices must be a 5-len array "
                + " and all elements must be between 1-6")
        if selected_type not in self.basic_scores:
            raise Exception(
                "Invailed Choice")
        score = self.basic_scores[selected_type](status.dices)
        return score

    def judger_dicehash_cache(self, dice_hash, selected_type):
        if (dice_hash, selected_type) not in self._judger_cache:
            dices = YahtzeeStatus.parse_dices(dice_hash)
            score = self.basic_scores[selected_type](dices)
            self._judger_cache[(dice_hash, selected_type)] = score

        return self._judger_cache[(dice_hash, selected_type)]

    def judge_by_statuscode(self, status_code, selected_type):
        state, dice_hash, numerical, reroll_times = YahtzeeStatus.parse_status_code(
            status_code)

        dices = YahtzeeStatus.parse_dices(dice_hash)

        if len(dices) != 5 or\
                not all(x >= 1 and x <= 6 for x in dices):
            raise Exception(
                "dices must be a 5-len array "
                + " and all elements must be between 1-6")
        if selected_type & state:
            raise Exception(
                "Invailed Choice")

        score = self.judger_dicehash_cache(dice_hash, selected_type)
        if selected_type <= ScoreType.Sixes and numerical < 63 and numerical + score >= 63:
            score += 35
        return score

    def roll(self, status):
        if status.turn < len(self.basic_scores) and not status.dices:
            result = self.random.randint(1, 7, 5)
            status.dices[:] = result
            status.reroll_times = 2
        else:
            raise Exception(
                "invailed action")

    def reroll(self, status, indexes):
        if len(status.dices) < 5:
            result = [self.random.randint(1, 7) if x not in indexes
                      else status.dices[x]
                      for x in range(5)]
            status.dices[:] = result
            status.reroll_times -= 1
        else:
            raise Exception(
                "invailed action")

    def write(self, status, score_type):
        if len(status.dices) == 5:
            if score_type in status.score_map:
                raise Exception(
                    "already selected")
            else:
                selected_score = self.judge(status, score_type)
                status.dices[:] = []
                status.score_map[score_type] = selected_score
                status.score += selected_score
                status.turn += 1
                if status.upper_sum >= 63 and ScoreType.Bonus not in status.score_map:
                    status.score += 35  # TODO: hard code
                    status.score_map[ScoreType.Bonus] = 35

        else:
            raise Exception(
                "invailed action")

    def write_by_statuscode(self, status_code, score_type):
        state, dice_hash, numerical, reroll_times = YahtzeeStatus.parse_status_code(
            status_code)

        dices = YahtzeeStatus.parse_dices(dice_hash)

        if len(dices) == 5:
            if score_type & state:
                raise Exception(
                    "already selected")
            else:
                selected_score = self.judge_by_statuscode(
                    status_code, score_type)

                if score_type <= ScoreType.Sixes:
                    numerical += selected_score
                    if numerical >= 63:
                        numerical = 63
                return YahtzeeStatus.compose_status_code(state | score_type, 0,
                                                         numerical, 2)
        else:
            raise Exception(
                "invailed action")

    def Ones(self, dices):
        return sum(x for x in dices if x == 1)

    def Twos(self, dices):
        return sum(x for x in dices if x == 2)

    def Threes(self, dices):
        return sum(x for x in dices if x == 3)

    def Fours(self, dices):
        return sum(x for x in dices if x == 4)

    def Fives(self, dices):
        return sum(x for x in dices if x == 5)

    def Sixes(self, dices):
        return sum(x for x in dices if x == 6)

    def Chance(self, dices):
        return sum(dices)

    def ThreeOfAKind(self, dices):  # For AAABC
        m = {}
        for x in dices:
            if x in m:
                m[x] += 1
            else:
                m[x] = 1
        for key in m:
            if m[key] >= 3:
                return sum(dices)
        return 0

    def FourOfAKind(self, dices):  # For AAAAB
        m = {}
        for x in dices:
            if x in m:
                m[x] += 1
            else:
                m[x] = 1
        for key in m:
            if m[key] >= 4:
                return sum(dices)
        return 0

    def FullHouse(self, dices):  # For AAABB
        m = {}
        for x in dices:
            if x in m:
                m[x] += 1
            else:
                m[x] = 1
        if len(m) == 1:  # Yahtzee is also FullHouse
            return sum(dices)
        if len(m) == 2:
            first_count = next(iter(m.values()))
            if first_count == 2 or first_count == 3:
                return sum(dices)
        return 0

    def SmallStraight(self, dices):  # For 1234,2345,3456
        patterns = [[1, 2, 3, 4], [2, 3, 4, 5], [3, 4, 5, 6]]
        for pattern in patterns:
            for x in pattern:
                if x not in dices:
                    break
            else:
                return 15  # TODO: hard code
        return 0

    def LargeStraight(self, dices):  # For 12345,23456
        patterns = [[1, 2, 3, 4, 5], [2, 3, 4, 5, 6]]
        for pattern in patterns:
            for x in pattern:
                if x not in dices:
                    break
            else:
                return 30  # TODO: hard code
        return 0

    def Yahtzee(self, dices):  # For AAAAA
        m = {}
        for x in dices:
            if x in m:
                m[x] += 1
            else:
                m[x] = 1
        if len(m) == 1:
            return 50  # TODO: hard code
        return 0
