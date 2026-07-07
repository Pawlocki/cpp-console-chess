#pragma once

#include "Board.hpp"
#include "../ui/ConsoleUI.hpp"

class Game
{
private:
    Board board;
    ConsoleUI ui;

    int moveNumber = 1;
    bool isGameFinished = false;

public:
    void startGame();
    void makeTurn();
};