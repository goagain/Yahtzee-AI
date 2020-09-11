from abc import abstractmethod


class YahtzeeGameInterface(object):
    @abstractmethod
    def get_available_actions(self):
        raise NotImplementedError

    @abstractmethod
    def roll(self):
        raise NotImplementedError

    @abstractmethod
    def reroll(self, indexes):
        raise NotImplementedError

    @abstractmethod
    def write(self, score_type):
        raise NotImplementedError

    @abstractmethod
    def get_score_by_type(self, score_type, default_zero=False):
        raise NotImplementedError

    @abstractmethod
    def get_status(self):
        raise NotImplementedError

    @abstractmethod
    def is_bonus_available(self):
        raise NotImplementedError
