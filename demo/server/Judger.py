from server.game_pb2 import ScoreType


class Judger(object):
    def __init__(self):
        self.func = {
            ScoreType.Ones: self.Ones,
            ScoreType.Twos: self.Twos,
            ScoreType.Threes: self.Threes,
            ScoreType.Fours: self.Fours,
            ScoreType.Fives: self.Fives,
            ScoreType.Sixes: self.Sixes,
            ScoreType.Chance: self.Chance,
            ScoreType.FourOfAKind: self.FourOfAKind,
            ScoreType.FullHouse: self.FullHouse,
            ScoreType.SmallStraight: self.SmallStraight,
            ScoreType.LargeStraight: self.LargeStraight,
            ScoreType.Yahtzee: self.Yahtzee,
        }

    def judge(self, dices, selected_type):
        if len(dices) != 5 or not all(x >= 1 and x <= 6 for x in dices):
            raise Exception(
                "dices must be a 5-len array and all elements must be between 1-6")
        if selected_type not in self.func:
            raise Exception(
                "Invailed Choice")
        return self.func[selected_type](dices)

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

    def FourOfAKind(self, dices):  # For AAABB
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
