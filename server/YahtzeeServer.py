from server.YahtzeeGame import YahtzeeGame


class YahtzeeServer(object):
    def __init__(self):
        pass

    def new_game(self, seed=None):
        return YahtzeeGame(seed)
