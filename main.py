#!/usr/bin/python3
from server.YahtzeeServer import YahtzeeServer
from server.game_pb2 import ScoreType
from server.game_pb2 import Action
from server.Judger import Judger
import sys


server = YahtzeeServer()
game = server.new_game()
judger = Judger()
while Action.GameOver not in game.get_available_actions():
    status = game.get_status()
    print(f"Current dices: {status.dices}")
    print(f"Current score: {status.score}")
    print(f"Current detail scores: {status.scores}")
    print(f"Current remain reroll times: {status.reroll_times}")
    actions = game.get_available_actions()

    prompts = []
    for i in range(len(actions)):
        prompts.append(f"{i}.{Action.Name(actions[i])}\n")

    selection = input(''.join(prompts))
    try:
        selected_action = actions[int(selection)]
        if selected_action == Action.Roll:
            game.roll()
        if selected_action == Action.Reroll:
            kept_str = input("input dices you want to keep\n")
            kept = [int(x) for x in kept_str.split()]
            game.reroll(kept)
        if selected_action == Action.Write:
            writable = game.get_available_score_type()
            for i in range(len(writable)):
                prompts.append(f"{i}.{ScoreType.Name(writable[i])} : {judger.judge(status.dices, writable[i])}\n")

            write_str = input(
                "input index you want to write:\n" + ''.join(prompts))
            write = writable[int(write_str)]
            game.write(write)

    except Exception as ex:
        print(ex, file=sys.stderr)

status = game.get_status()
print("Final score is ", status.score)
