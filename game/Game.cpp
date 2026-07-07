#include "Game.hpp"

void Game::startGame()
{
    ui.printIntro();

    while (isGameFinished == false)
    {
        ui.printMoveNumber(moveNumber);
        ui.printBoard(board);

        if (board.isCheckMate() == true)
        {
            ui.printCheckMate(board);
            isGameFinished = true;
        }
        else if (board.isStaleMate() == true)
        {
            ui.printStaleMate();
            isGameFinished = true;
        }
        else
        {
            if (board.isSquareAttacked(board.findKingPosition()) == true)
                ui.printCheck(board);

            makeTurn();
        }
    }
}

void Game::makeTurn()
{
    bool wasMoveDone = false;

    while (wasMoveDone == false && isGameFinished == false)
    {
        Move move = ui.askMove(board);

        if (move.wantsToExit == true)
        {
            isGameFinished = true;
            return;
        }

        if (board.isPawnPromotionMove(move.fromSquare, move.toSquare) == true)
            move.promotionPiece = ui.askPromotionPiece(board);

        MoveResult moveResult = board.tryMovePiece(move.fromSquare, move.toSquare, true, move.promotionPiece);

        wasMoveDone = moveResult.wasMoveDone;

        if (wasMoveDone == false)
            ui.printMoveError(moveResult);
    }

    if (isGameFinished == false)
    {
        if (board.getCurrentPlayer() == 0)
        {
            board.clearLastPawnPushTableBlack();
            board.changeTurn();
        }
        else
        {
            board.clearLastPawnPushTableWhite();
            board.changeTurn();
            moveNumber++;
        }
    }
}
