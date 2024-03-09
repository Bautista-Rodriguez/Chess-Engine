#20,999,266 chess positions evaluated with Stockfish. (Lichess Database)
'''
import torch
import torch.nn as nn
import torch.nn.functional as F


class Model(nn.Module):
    def __init__(self,input = 790,h1 = 512,h2 = 128,h3 = 32,h4 = 32,output = 1):
        super().__init__()
        self.fc1 = nn.Linear(input,h1)
        self.fc2 = nn.Linear(h1,h2)
        self.fc3 = nn.Linear(h2,h3)
        self.fc4 = nn.Linear(h3,h4)
        self.fc5 = nn.Linear(h4,output)
    def forward(self,x):
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = F.relu(self.fc3(x))
        x = F.relu(self.fc4(x))
        x = F.relu(self.fc5(x))
        x = self.out(x)
        return x
'''
import json
import chess

board = chess.Board('rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1')



with open('C:/Users/bauty/Desktop/Bau/ChessNNDataset/lichess_db_eval.jsonl', 'r') as dataSet:
    for i, line in enumerate(dataSet):
        if i == 3:
            break
        strFile = json.loads(line)
        WP = board.pieces(chess.PAWN, chess.WHITE)
        print(int(WP))
        board.set_fen(strFile['fen'])
        cp = strFile['evals'][0]['pvs'][0]['cp']
        print(strFile['fen'],"| cp:",cp)
