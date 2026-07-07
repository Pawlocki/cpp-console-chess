#pragma once

#include "../game/Board.hpp"
#include "../game/Move.hpp"

class ConsoleUI
{
public:
    void printIntro();
    void printHelp();
    void printBoard(Board &board);
    void printMoveNumber(int moveNumber);
    void printCheck(Board &board);
    void printCheckMate(Board &board);
    void printStaleMate();
    void printMoveError(MoveResult moveResult);

    Move askMove(Board &board);
    string askPromotionPiece(Board &board);

private:
    bool didPlayerTypeExit(string input);
    bool didPlayerTypeHelp(string input);
};