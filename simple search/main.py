# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'
# %%
import Controller
from YahtzeeStatus import YahtzeeStatus
import itertools

controller = Controller.Controller()
status = YahtzeeStatus()


# %%
RewardTable = {}


# %%


class sequence_generator:
    def __init__(self):
        self.results = {}

    def _gen(self, n):
        l = list(itertools.product(range(1, 7), repeat=n))
        m = {}
        for dices in l:
            dd = list(dices)
            dd.sort()
            tup = tuple(dd)
            if tup in m:
                m[tup] += 1
            else:
                m[tup] = 1
        return m, len(l)

    def gen_sequence(self, n=5):
        if n not in self.results:
            self.results[n] = self._gen(n)

        return self.results[n]


seq_gen = sequence_generator()


def CountBits(n):
    n = (n & 0x5555555555555555) + ((n & 0xAAAAAAAAAAAAAAAA) >> 1)
    n = (n & 0x3333333333333333) + ((n & 0xCCCCCCCCCCCCCCCC) >> 2)
    n = (n & 0x0F0F0F0F0F0F0F0F) + ((n & 0xF0F0F0F0F0F0F0F0) >> 4)
    n = (n & 0x00FF00FF00FF00FF) + ((n & 0xFF00FF00FF00FF00) >> 8)
    n = (n & 0x0000FFFF0000FFFF) + ((n & 0xFFFF0000FFFF0000) >> 16)
    # This last & isn't strictly necessary.
    n = (n & 0x00000000FFFFFFFF) + ((n & 0xFFFFFFFF00000000) >> 32)
    return n


# %%
def get_expect(status_code) -> float:
    if status_code in RewardTable:
        return RewardTable[status_code]

    state, dice_hash, numerical, reroll_times = YahtzeeStatus.parse_status_code(
        status_code)
    # print(state, dice_hash, numerical, reroll_times)
    dices = YahtzeeStatus.parse_dices(dice_hash)

    if CountBits(state) == len(controller.basic_scores):
        return 0
    if dice_hash > 10000:
        writables = controller.get_writable_by_statuscode(status_code)

        expect = 0
        for w in writables:
            new_status = controller.write_by_statuscode(status_code, w)

            new_expect = get_expect(
                new_status) + controller.judge_by_statuscode(status_code, w)
            if new_expect > expect:
                expect = new_expect
        if reroll_times > 0:
            for kept in range(5):
                new_dices_set = set(itertools.combinations(dices, kept))
                for dices in new_dices_set:
                    new_status = YahtzeeStatus.compose_status_code(state, YahtzeeStatus.compose_dices(dices),
                                                                   numerical, reroll_times - 1)
                    if get_expect(new_status) > expect:
                        expect = get_expect(new_status)
    else:
        kept = len(dices)
        comings, total = seq_gen.gen_sequence(5-kept)
        expect = 0
        for coming_dices in comings:
            count = comings[coming_dices]

            probability = count / len(comings)
            new_dices = list(dices) + list(coming_dices)
            new_status = YahtzeeStatus.compose_status_code(state, YahtzeeStatus.compose_dices(new_dices),
                                                           numerical, reroll_times)
            # print(new_status)
            expect += get_expect(new_status) * probability

    RewardTable[status_code] = expect
    if(len(RewardTable) % 10000 == 0):
        print(len(RewardTable))
    return expect


# %%
print(len(RewardTable))
print(RewardTable)


# %%
init_status = 2
print(get_expect(init_status))


# %%
