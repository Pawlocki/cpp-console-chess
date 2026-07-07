#pragma once

#include <iostream>
#include <string>
#include "Move.hpp"

using namespace std;

class Board
{
private:
    int whoseTurn = 0; // 0 -> white (Uppercase); 1 -> black (Lowercase)

    string board[8][8] =
        {
            {"r", "n", "b", "q", "k", "b", "n", "r"}, 
            {"p", "p", "p", "p", "p", "p", "p", "p"},
            {".", ".", ".", ".", ".", ".", ".", "."},
            {".", ".", ".", ".", ".", ".", ".", "."},
            {".", ".", ".", ".", ".", ".", ".", "."},
            {".", ".", ".", ".", ".", ".", ".", "."},
            {"P", "P", "P", "P", "P", "P", "P", "P"},
            {"R", "N", "B", "Q", "K", "B", "N", "R"}};

    int haveRooksMovedDuringTheGame[8][8] = {};
    int haveKingsMovedDuringTheGame[2] = {}; // [0] white king, [1] black king

    int whiteWasLastMovementPawnPush[8][8] = {};
    int blackWasLastMovementPawnPush[8][8] = {};

public:

    int getCurrentPlayer();
    string getCurrentPlayerName();
    void setCurrentPlayer(int a);
    void changeTurn();

    bool isValidSquare(string check);
    bool isWhitePiece(string piece);
    bool isBlackPiece(string piece);
    bool isCurrentPlayersPiece(string piece);
    bool isOCCupiedByEnemyPiece(string check);
    bool isOccupiedByOwnPiece(string check);
    bool isNotOccupied(string check);

    int convertColumnToNumber(string check);
    int convertRowtoNumber(string check);
    string convertPositionToSquare(int row, int column);
    void clearLastPawnPushTableWhite();
    void clearLastPawnPushTableBlack();

    string getPiece(int row, int column);
    void setPiece(int row, int column, string piece);
    string getPieceFromSquare(string square);
    void setPieceOnSquare(string square, string piece);

    MoveResult tryMovePiece(string fromSquare, string toSquare, bool shouldMove = true, string promotionPiece = "");
    bool isPawnMovementValid(string fromSquare, string toSquare);
    bool isRookMovementValid(string fromSquare, string toSquare);
    bool isBishopMovementValid(string fromSquare, string toSquare);
    bool isKnightMovementValid(string fromSquare, string toSquare);
    bool isQueenMovementValid(string fromSquare, string toSquare);
    bool isKingMovementValid(string fromSquare, string toSquare);
    bool isCastlingMoveValid(string fromSquare, string toSquare);

    void promotePawn(string square, string promotionPiece);
    bool isPawnPromotionMove(string fromSquare, string toSquare);

    string findKingPosition();
    bool isSquareAttackedByPawn(string squareToCheck);
    bool isSquareAttackedByRook(string squareToCheck);
    bool isSquareAttackedByBishop(string squareToCheck);
    bool isSquareAttackedByKnight(string squareToCheck);
    bool isSquareAttackedByQueen(string squareToCheck);
    bool isSquareAttackedByKing(string squareToCheck);
    bool isSquareAttacked(string squareToCheck);

    bool playerHasAnyLegalMove();
    bool isCheckMate();
    bool isStaleMate();
};