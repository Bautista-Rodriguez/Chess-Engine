#20,999,266 chess positions evaluated with Stockfish. (Lichess Database)

import torch
import torch.nn as nn
import torch.nn.functional as F
import matplotlib.pyplot as plt
from stockfish import Stockfish
import chess
import json
import numpy as np


class Model(nn.Module):
    def __init__(self,inputFeatures = 790,h1 = 512,h2 = 128,h3 = 32,h4 = 32,outputFeatures = 1):
        super().__init__()
        self.fc1 = nn.Linear(inputFeatures,h1)
        self.fc2 = nn.Linear(h1,h2)
        self.fc3 = nn.Linear(h2,h3)
        self.fc4 = nn.Linear(h3,h4)
        self.out = nn.Linear(h4,outputFeatures)
    def forward(self,x):
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = F.relu(self.fc3(x))
        x = F.relu(self.fc4(x))
        x = self.out(x)
        return x
    def boardToTensor(self,fen):
        board.set_fen(fen)
        whiteP = int(board.pieces(chess.PAWN, chess.WHITE))
        whiteN = int(board.pieces(chess.KNIGHT, chess.WHITE))
        whiteB = int(board.pieces(chess.BISHOP, chess.WHITE))
        whiteR = int(board.pieces(chess.ROOK, chess.WHITE))
        whiteQ = int(board.pieces(chess.QUEEN, chess.WHITE))
        whiteK = int(board.pieces(chess.KING, chess.WHITE))
        blackP = int(board.pieces(chess.PAWN, chess.BLACK))
        blackN = int(board.pieces(chess.KNIGHT, chess.BLACK))
        blackB = int(board.pieces(chess.BISHOP, chess.BLACK))
        blackR = int(board.pieces(chess.ROOK, chess.BLACK))
        blackQ = int(board.pieces(chess.QUEEN, chess.BLACK))
        blackK = int(board.pieces(chess.KING, chess.BLACK))
        castleRights = (
                ((board.castling_rights & (1 << chess.A1)) != 0) +
                (((board.castling_rights & (1 << chess.H1)) != 0) << 1) +
                (((board.castling_rights & (1 << chess.A8)) != 0) << 2) +
                (((board.castling_rights & (1 << chess.H8)) != 0) << 3))
        side = 1
        enPassant = 0
        if board.ep_square:
            enPassant = (1 << (board.ep_square % 8)) << 16
        if "w" in fen:
            side += 1
            if board.ep_square:
                enPassant >>= 8
        bitboardsArray = np.array([whiteP, whiteN, whiteB, whiteR, whiteQ, whiteK,
                                   blackP, blackN, blackB, blackR, blackQ, blackK], dtype=np.uint64)
        bitboardsArray = np.unpackbits(bitboardsArray.view(np.uint8))
        leint = ((side << 4) | (castleRights) | (enPassant)) << 2
        miscArray = np.array([leint], dtype=np.uint32)
        miscArray = np.unpackbits(miscArray.view(np.uint8))
        for i in range(10):
            miscArray = np.delete(miscArray, 22)
        tensorArray = torch.FloatTensor(np.append(bitboardsArray, miscArray))
        #print(tensorArray)
        return tensorArray


lossesArray = []
myModel = Model()

board = chess.Board('rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3')
stockfish = Stockfish(path="C:/Users/bauty/Desktop/Bau/Nueva carpeta (2)/stockfish/stockfish-windows-x86-64-sse41-popcnt.exe",depth = 1)

torch.manual_seed(17)
lossCriteria = nn.MSELoss()
optimizer = torch.optim.Adam(myModel.parameters(),lr = 0.001)
epochs = 1


for j in range(epochs):
  print("epochs:", j)
  with (open('C:/Users/bauty/Desktop/Bau/ChessNNDataset/lichess_db_eval.jsonl', 'r') as dataSet):
    for i, line in enumerate(dataSet):
               #1,000,000
        if i >= 1000000:
            break
        if i == 25318:
            continue
        if i == 92341:
            continue
        if i == 92352:
            continue
        if i == 124516:
            continue
        if i == 169343:
            continue
        if i == 185615:
            continue
        if i == 185617:
            continue
        if i == 224970:
            continue
        if i == 402200:
            continue
        if i == 620006:
            continue
        if i == 678677:
            continue
        if i == 791830:
            continue
        if i == 816045:
            continue
        if i == 816047:
            continue
        if i == 819823:
            continue
        if i == 868682:
            continue
        if i == 963155:
            continue
        strFile = json.loads(line)
        tensorArray = myModel.boardToTensor(strFile['fen'])
        stockfish.set_fen_position(strFile['fen'])
        cp = stockfish.get_evaluation()
        if cp['type'] == 'mate':
            if cp['value'] > 0:
                cp = 1500
            else:
                cp = -1500
        else:
            cp = max(cp['value'], -1500)
            cp = min(cp, 1500)
        cp = torch.FloatTensor([cp])
        yPred = myModel.forward(tensorArray)
        loss = lossCriteria(yPred,cp)
        lossesArray.append(loss.detach().numpy())
        if i % 100000 == 0:
           print("loss value:",float(loss),"- prediction:",float(yPred),"- eval:",float(cp),"- n iteration:",i)
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
print("\n-------\nended training!\n-------------\n")

#torch.save(myModel.state_dict(),'CEM 11 3 2024.pt')
#newModel = Model()
#newModel.load_state_dict(torch.load('CEM.pt'))

'''
plt.plot(range(epochs),lossesArray)
plt.xlabel('epoch')
plt.ylabel('loss/error')
'''