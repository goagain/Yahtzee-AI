import numpy as np
import server.game_pb2 as game__pb2
from server.game_pb2 import ScoreType
from server.game_pb2 import Action
from server.YahtzeeGameInterface import YahtzeeGameInterface
from server.Judger import Judger


class YahtzeeGame(YahtzeeGameInterface):
    def __init__(self, seed):
        self.seed = seed
        self.random = np.random.RandomState(seed=seed)
        self.status = game__pb2.GameStatus(
            available_actions=[Action.Roll]
        )
        self.judger = Judger()

    def get_available_actions(self):
        return self.status.available_actions

    def get_available_score_type(self):
        types = [ScoreType.Ones,
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
                 ScoreType.Yahtzee
                 ]

        return [x for x in types if self.get_score_by_type(x) is None]

    def roll(self):
        status = self.status
        if game__pb2.Action.Roll in status.available_actions:
            result = self.random.randint(1, 7, 5)
            status.dices[:] = result
            status.available_actions[:] = [
                game__pb2.Action.Reroll, game__pb2.Action.Write]
            status.reroll_times = 2
        else:
            raise Exception(
                "invailed action")

    def reroll(self, indexes):
        status = self.status
        if game__pb2.Action.Reroll in status.available_actions:
            result = [self.random.randint(1, 7) if x not in indexes
                      else status.dices[x]
                      for x in range(5)]
            status.dices[:] = result
            status.available_actions[:] = [
                game__pb2.Action.Reroll, game__pb2.Action.Write]

            status.reroll_times -= 1
            if status.reroll_times > 0:
                status.available_actions[:] = [
                    game__pb2.Action.Reroll, game__pb2.Action.Write]
            else:
                status.available_actions[:] = [game__pb2.Action.Write]
        else:
            raise Exception(
                "invailed action")

    def write(self, score_type):
        status = self.status
        if score_type == ScoreType.SCORETYPE_INVALID:
            raise Exception(
                "invailed selection")

        if game__pb2.Action.Write in status.available_actions:
            if self.get_score_by_type(score_type) is not None:
                raise Exception(
                    "already selected")
            else:
                selected_score = self.judger.judge(status.dices, score_type)
                status.dices[:] = []
                status.scores.append(game__pb2.PartScore(
                    type=score_type, score=selected_score))
                status.score += selected_score
                status.turns += 1
                if self.is_bonus_available():
                    status.score += 35  # TODO: hard code
                    status.scores.append(game__pb2.PartScore(
                        type=ScoreType.Bonus, score=35))

                if status.turns == 12:
                    status.available_actions[:] = [Action.GameOver]
                else:
                    status.available_actions[:] = [Action.Roll]
        else:
            raise Exception(
                "invailed action")

    def get_score_by_type(self, score_type, default_zero=False):
        status = self.status
        for score in status.scores:
            if score.type == score_type:
                return score.score
        else:
            if default_zero:
                return 0
            return None

    def is_bonus_available(self):
        if self.get_score_by_type(ScoreType.Bonus) is not None:
            return False
        numerics = [ScoreType.Ones, ScoreType.Twos, ScoreType.Threes,
                    ScoreType.Fours, ScoreType.Fives, ScoreType.Sixes]
        total_numeric_score = sum(self.get_score_by_type(
            x, default_zero=True) for x in numerics)
        if total_numeric_score >= 63:  # TODO: hard code
            return True

    def get_status(self):
        return self.status
