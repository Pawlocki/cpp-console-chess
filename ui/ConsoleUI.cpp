#include "ConsoleUI.hpp"
#include <iostream>

using namespace std;

void ConsoleUI::printIntro()
{
    cout << "Welcome to console chess!" << endl;
    cout << endl;

    cout << "Enter moves using chess coordinates." << endl;
    cout << "Example move: from e2 to e4." << endl;
    cout << endl;

    cout << "Type help or h at any prompt to show help." << endl;
    cout << "Type exit or q to quit the game." << endl;
    cout << endl;
}
void ConsoleUI::printHelp()
{
    cout << endl;
    cout << "HELP" << endl;
    cout << endl;

    cout << "How to move:" << endl;
    cout << "  Enter the starting square, for example: e2" << endl;
    cout << "  Then enter the target square, for example: e4" << endl;
    cout << endl;

    cout << "Pieces:" << endl;
    cout << "  White pieces use uppercase letters:" << endl;
    cout << "    P - pawn      R - rook      N - knight" << endl;
    cout << "    B - bishop    Q - queen     K - king" << endl;
    cout << endl;

    cout << "  Black pieces use lowercase letters:" << endl;
    cout << "    p - pawn      r - rook      n - knight" << endl;
    cout << "    b - bishop    q - queen     k - king" << endl;
    cout << endl;

    cout << "Commands:" << endl;
    cout << "  help / h   - show this help message" << endl;
    cout << "  exit / q   - quit the game" << endl;
    cout << endl;
}
void ConsoleUI::printBoard(Board &board)
{
    cout << "    a   b   c   d   e   f   g   h" << endl;

    for (int row = 0; row < 8; row++)
    {
        cout << "  ";

        for (int col = 0; col < 8; col++)
        {
            cout << "+---";
        }

        cout << "+" << endl;

        cout << 8 - row << " ";

        for (int col = 0; col < 8; col++)
        {
            cout << "| " << board.getPiece(row, col) << " ";
        }

        cout << "| " << 8 - row << endl;
    }

    cout << "  ";

    for (int col = 0; col < 8; col++)
    {
        cout << "+---";
    }

    cout << "+" << endl;

    cout << "    a   b   c   d   e   f   g   h" << endl;
}
void ConsoleUI::printMoveNumber(int moveNumber)
{
    cout << endl
         << "Move number: " << moveNumber << endl;
}
void ConsoleUI::printCheck(Board &board)
{
    cout << board.getCurrentPlayerName() << " is in check." << endl;
}
void ConsoleUI::printCheckMate(Board &board)
{
    if (board.getCurrentPlayer() == 0)
        cout << "Checkmate. BLACK wins." << endl;
    else
        cout << "Checkmate. WHITE wins." << endl;
}
void ConsoleUI::printStaleMate()
{
    cout << "Stalemate. The game is a draw." << endl;
}
Move ConsoleUI::askMove(Board &board)
{
    Move move;

    while (true)
    {
        cout << board.getCurrentPlayerName()
             << ": Enter the starting square. Type help for help or exit to quit."
             << endl;

        cin >> move.fromSquare;

        if (didPlayerTypeExit(move.fromSquare))
        {
            move.wantsToExit = true;
            return move;
        }

        if (didPlayerTypeHelp(move.fromSquare))
        {
            printHelp();
            continue;
        }

        if (board.isValidSquare(move.fromSquare) == false)
        {
            cout << "Invalid square, please try again." << endl;
            continue;
        }

        if (board.isOccupiedByOwnPiece(move.fromSquare) == false)
        {
            cout << "Selected square does not contain your piece, please try again." << endl;
            continue;
        }

        break;
    }

    while (true)
    {
        cout << board.getCurrentPlayerName()
             << ": Enter the target square. Type help for help or exit to quit."
             << endl;

        cin >> move.toSquare;

        if (didPlayerTypeExit(move.toSquare))
        {
            move.wantsToExit = true;
            return move;
        }

        if (didPlayerTypeHelp(move.toSquare))
        {
            printHelp();
            continue;
        }

        if (board.isValidSquare(move.toSquare) == false)
        {
            cout << "Invalid target square, please try again." << endl;
            continue;
        }

        if (board.isOccupiedByOwnPiece(move.toSquare) == true)
        {
            cout << "Target square contains your own piece, please try again." << endl;
            continue;
        }

        break;
    }

    return move;
}
string ConsoleUI::askPromotionPiece(Board &board)
{
    string piece;

    if (board.getCurrentPlayer() == 0)
        cout << "White pawn promotion. Choose Q, R, B or N." << endl;
    else
        cout << "Black pawn promotion. Choose q, r, b or n." << endl;

    cin >> piece;

    while (piece != "Q" && piece != "R" && piece != "B" && piece != "N" &&
           piece != "q" && piece != "r" && piece != "b" && piece != "n")
    {
        if (board.getCurrentPlayer() == 0)
            cout << "Invalid promotion piece. Choose Q, R, B or N." << endl;
        else
            cout << "Invalid promotion piece. Choose q, r, b or n." << endl;

        cin >> piece;
    }

    if (board.getCurrentPlayer() == 0)
    {
        if (piece == "q" || piece == "Q")
            return "Q";
        else if (piece == "r" || piece == "R")
            return "R";
        else if (piece == "b" || piece == "B")
            return "B";
        else
            return "N";
    }
    else
    {
        if (piece == "q" || piece == "Q")
            return "q";
        else if (piece == "r" || piece == "R")
            return "r";
        else if (piece == "b" || piece == "B")
            return "b";
        else
            return "n";
    }
}
void ConsoleUI::printMoveError(MoveResult moveResult)
{
    if (moveResult.error == MoveError::InvalidPieceMovement)
        cout << "This piece cannot move to that square, try again." << endl;
    else if (moveResult.error == MoveError::CannotCaptureKing)
        cout << "You cannot capture the king." << endl;
    else if (moveResult.error == MoveError::KingWouldBeInCheck)
        cout << "Your king would be in check, try a different move." << endl;
}

bool ConsoleUI::didPlayerTypeExit(string input)
{
    return input == "exit" || input == "Exit" || input == "EXIT" ||
           input == "q" || input == "Q";
}
bool ConsoleUI::didPlayerTypeHelp(string input)
{
    return input == "help" || input == "Help" || input == "HELP" ||
           input == "h" || input == "H";
}