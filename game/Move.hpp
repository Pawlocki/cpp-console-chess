#pragma once

#include <string>

using namespace std;

struct Move
{
    string fromSquare;
    string toSquare;
    string promotionPiece = "";
    bool wantsToExit = false;
};

enum class MoveError
{
    None,
    InvalidPieceMovement,
    CannotCaptureKing,
    KingWouldBeInCheck
};

struct MoveResult
{
    bool wasMoveDone = false;
    MoveError error = MoveError::None;
};