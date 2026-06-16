#include <iostream>
#include <string>

using namespace std;

class Board
{
private:
    int moveNumber = 1;
    int whoseTurn = 0; // 0 -> white (Uppercase); 1 -> black (Lowercase)
    bool isGameFinished = false;

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
    void startGame()
    {
        while (isGameFinished == false)
        {
            cout << endl << "Move number: " << moveNumber << endl;
            printBoard();

            if (isCheckMate() == true)
            {
                printWinner();
                isGameFinished = true;
            }
            else if (isStaleMate() == true)
            {
                cout << "Stalemate. The game is a draw." << endl;
                isGameFinished = true;
            }
            else
            {
                if (isSquareAttacked(findKingPosition()) == true)
                    cout << getCurrentPlayerName() << " is in check." << endl;

                makeTurn();
            }
        }
    };
    void makeTurn()
    {
        bool wasMoveDone = false;

        if (getCurrentPlayer() == 1)
        {
            while (!wasMoveDone && isGameFinished == false)
            {
                wasMoveDone = blackMoveChecklist();
            }

            if (isGameFinished == false)
            {
                clearLastPawnPushTableWhite();
                changeTurn();
                moveNumber++;
            }
        }
        else
        {
            while (!wasMoveDone && isGameFinished == false)
            {
                wasMoveDone = whiteMoveChecklist();
            }

            if (isGameFinished == false)
            {
                clearLastPawnPushTableBlack();
                changeTurn();
            }
        }
    };

    void printBoard()
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
                cout << "| " << board[row][col] << " ";
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

    int getCurrentPlayer()
    {
        return whoseTurn;
    }
    string getCurrentPlayerName()
    {
        if (getCurrentPlayer() == 0)
            return "WHITE";
        else
            return "BLACK";
    }
    void setCurrentPlayer(int a)
    {
        whoseTurn = a;
    }
    void changeTurn()
    {
        if (getCurrentPlayer() == 1)
            setCurrentPlayer(0);
        else
            setCurrentPlayer(1);
    }
    bool whiteMoveChecklist()
    {
        string fromSquare;
        string toSquare;

        cout << "WHITE: Enter the starting square. Type exit to quit." << endl;
        cin >> fromSquare;

        if (didPlayerTypeExit(fromSquare) == true)
        {
            isGameFinished = true;
            return true;
        }

        while (isValidSquare(fromSquare) == false || isOccupiedByOwnPiece(fromSquare) == false)
        {
            if (isValidSquare(fromSquare) == false)
                cout << "Invalid square, please try again." << endl;
            else
                cout << "Selected square does not contain your piece, please try again." << endl;

            cin >> fromSquare;

            if (didPlayerTypeExit(fromSquare) == true)
            {
                isGameFinished = true;
                return true;
            }
        }

        cout << "WHITE: Enter the target square." << endl;
        cin >> toSquare;

        if (didPlayerTypeExit(toSquare) == true)
        {
            isGameFinished = true;
            return true;
        }

        while (isValidSquare(toSquare) == false || isOccupiedByOwnPiece(toSquare) == true)
        {
            cout << "Invalid target square, please try again." << endl;
            cin >> toSquare;

            if (didPlayerTypeExit(toSquare) == true)
            {
                isGameFinished = true;
                return true;
            }
        }

        return tryMovePiece(fromSquare, toSquare);
    }
    bool blackMoveChecklist()
    {
        string fromSquare;
        string toSquare;

        cout << "BLACK: Enter the starting square. Type exit to quit." << endl;
        cin >> fromSquare;

        if (didPlayerTypeExit(fromSquare) == true)
        {
            isGameFinished = true;
            return true;
        }

        while (isValidSquare(fromSquare) == false || isOccupiedByOwnPiece(fromSquare) == false)
        {
            if (isValidSquare(fromSquare) == false)
                cout << "Invalid square, please try again." << endl;
            else
                cout << "Selected square does not contain your piece, please try again." << endl;

            cin >> fromSquare;

            if (didPlayerTypeExit(fromSquare) == true)
            {
                isGameFinished = true;
                return true;
            }
        }

        cout << "BLACK: Enter the target square." << endl;
        cin >> toSquare;

        if (didPlayerTypeExit(toSquare) == true)
        {
            isGameFinished = true;
            return true;
        }

        while (isValidSquare(toSquare) == false || isOccupiedByOwnPiece(toSquare) == true)
        {
            cout << "Invalid target square, please try again." << endl;
            cin >> toSquare;

            if (didPlayerTypeExit(toSquare) == true)
            {
                isGameFinished = true;
                return true;
            }
        }

        return tryMovePiece(fromSquare, toSquare);
    }

    bool didPlayerTypeExit(string check)
    {
        if (check == "exit" || check == "Exit" || check == "EXIT" || check == "q" || check == "Q")
            return true;
        else
            return false;
    }
    bool isValidSquare(string check)
    {
        if (check.length() != 2)
            return false;
        else if (check[1] < '1' || check[1] > '8')
            return false;
        else if ((check[0] < 'a' || check[0] > 'h') && (check[0] < 'A' || check[0] > 'H'))
            return false;
        return true;
    }
    bool isWhitePiece(string piece)
    {
        if (piece == "B" || piece == "K" || piece == "N" || piece == "P" || piece == "Q" || piece == "R")
            return true;
        else
            return false;
    }
    bool isBlackPiece(string piece)
    {
        if (piece == "b" || piece == "k" || piece == "n" || piece == "p" || piece == "q" || piece == "r")
            return true;
        else
            return false;
    }
    bool isCurrentPlayersPiece(string piece)
    {
        if (getCurrentPlayer() == 0 && isWhitePiece(piece) == true)
            return true;
        else if (getCurrentPlayer() == 1 && isBlackPiece(piece) == true)
            return true;
        else
            return false;
    }
    bool isOCCupiedByEnemyPiece(string check)
    {
        string piece = getPieceFromSquare(check);

        if (getCurrentPlayer() == 0)
        {
            if (isBlackPiece(piece) == true)
                return true;
        }
        else if (getCurrentPlayer() == 1)
        {
            if (isWhitePiece(piece) == true)
                return true;
        }

        return false;
    }
    bool isOccupiedByOwnPiece(string check)
    {
        string piece = getPieceFromSquare(check);

        if (isCurrentPlayersPiece(piece) == true)
            return true;
        else
            return false;
    }
    bool isNotOccupied(string check)
    {
        string piece = getPieceFromSquare(check);

        if (piece == ".")
            return true;
        return false;
    }

    int convertColumnToNumber(string check)
    {
        int converter = -1;

        for (int i = 0; i < 8; i++) // converts check[0] from a letter to a number
        {
            if (check[0] == ('a' + i) || check[0] == ('A' + i))
            {
                converter = i;
            };
        };

        return converter;
    }
    int convertRowtoNumber(string check)
    {
        return 8 - (check[1] - '0');
    }
    string convertPositionToSquare(int row, int column)
    {
        char file = 'a' + column;
        char rank = '8' - row;

        string square = "";
        square += file;
        square += rank;

        return square;
    }
    void clearLastPawnPushTableWhite()
    {
        for (int row = 0; row < 8; row++)
        {
            for (int column = 0; column < 8; column++)
            {
                whiteWasLastMovementPawnPush[row][column] = 0;
            }
        }
    }
    void clearLastPawnPushTableBlack()
    {
        for (int row = 0; row < 8; row++)
        {
            for (int column = 0; column < 8; column++)
            {
                blackWasLastMovementPawnPush[row][column] = 0;
            }
        }
    }

    string getPiece(int row, int column)
    {
        if (row >= 0 && row < 8 && column >= 0 && column < 8)
            return board[row][column];
        else
            return "";
    }
    void setPiece(int row, int column, string piece)
    {
        if (row >= 0 && row < 8 && column >= 0 && column < 8)
            board[row][column] = piece;
    }
    string getPieceFromSquare(string square)
    {
        int row = convertRowtoNumber(square);
        int column = convertColumnToNumber(square);

        return getPiece(row, column);
    }
    void setPieceOnSquare(string square, string piece)
    {
        int row = convertRowtoNumber(square);
        int column = convertColumnToNumber(square);

        setPiece(row, column, piece);
    }

    bool tryMovePiece(string fromSquare, string toSquare, bool shouldMove = true, bool shouldShowMessages = true)
    {
        int fromColumn = convertColumnToNumber(fromSquare);
        int fromRow = convertRowtoNumber(fromSquare);

        int toColumn = convertColumnToNumber(toSquare);
        int toRow = convertRowtoNumber(toSquare);

        string movingPiece = getPieceFromSquare(fromSquare);
        bool isMoveValid = false;

        if ((movingPiece == "P" || movingPiece == "p") && isPawnMovementValid(fromSquare, toSquare) == true)
            isMoveValid = true;
        else if ((movingPiece == "R" || movingPiece == "r") && isRookMovementValid(fromSquare, toSquare) == true)
            isMoveValid = true;
        else if ((movingPiece == "B" || movingPiece == "b") && isBishopMovementValid(fromSquare, toSquare) == true)
            isMoveValid = true;
        else if ((movingPiece == "N" || movingPiece == "n") && isKnightMovementValid(fromSquare, toSquare) == true)
            isMoveValid = true;
        else if ((movingPiece == "Q" || movingPiece == "q") && isQueenMovementValid(fromSquare, toSquare) == true)
            isMoveValid = true;
        else if ((movingPiece == "K" || movingPiece == "k") && isKingMovementValid(fromSquare, toSquare) == true)
            isMoveValid = true;

        if (isMoveValid == false)
        {
            if (shouldShowMessages == true)
                cout << "This piece cannot move to that square, try again." << endl;
            return false;
        }

        string capturedPiece = getPieceFromSquare(toSquare);

        if (capturedPiece == "K" || capturedPiece == "k")
        {
            if (shouldShowMessages == true)
                cout << "You cannot capture the king." << endl;
            return false;
        }

        bool isEnPassant = false;
        string enPassantCapturedPiece = ".";
        int enPassantCapturedRow = -1;
        int enPassantCapturedColumn = -1;

        bool isCastling = false;
        string castlingRookPiece = ".";
        int castlingRookFromColumn = -1;
        int castlingRookToColumn = -1;

        if ((movingPiece == "P" || movingPiece == "p") && fromColumn != toColumn && capturedPiece == ".")
        {
            isEnPassant = true;
            enPassantCapturedRow = fromRow;
            enPassantCapturedColumn = toColumn;
            enPassantCapturedPiece = getPiece(enPassantCapturedRow, enPassantCapturedColumn);

            setPiece(enPassantCapturedRow, enPassantCapturedColumn, ".");
        }

        if ((movingPiece == "K" || movingPiece == "k") && (toColumn == fromColumn + 2 || toColumn == fromColumn - 2))
        {
            isCastling = true;

            if (toColumn == 6)
            {
                castlingRookFromColumn = 7;
                castlingRookToColumn = 5;
            }
            else
            {
                castlingRookFromColumn = 0;
                castlingRookToColumn = 3;
            }

            castlingRookPiece = getPiece(fromRow, castlingRookFromColumn);
            setPiece(fromRow, castlingRookFromColumn, ".");
            setPiece(fromRow, castlingRookToColumn, castlingRookPiece);
        }

        setPieceOnSquare(fromSquare, ".");
        setPieceOnSquare(toSquare, movingPiece);

        if (isSquareAttacked(findKingPosition()) == true)
        {
            setPieceOnSquare(fromSquare, movingPiece);
            setPieceOnSquare(toSquare, capturedPiece);

            if (isEnPassant == true)
                setPiece(enPassantCapturedRow, enPassantCapturedColumn, enPassantCapturedPiece);

            if (isCastling == true)
            {
                setPiece(fromRow, castlingRookFromColumn, castlingRookPiece);
                setPiece(fromRow, castlingRookToColumn, ".");
            }

            if (shouldShowMessages == true)
                cout << "Your king would be in check, try a different move." << endl;
            return false;
        }

        if (shouldMove == false)
        {
            setPieceOnSquare(fromSquare, movingPiece);
            setPieceOnSquare(toSquare, capturedPiece);

            if (isEnPassant == true)
                setPiece(enPassantCapturedRow, enPassantCapturedColumn, enPassantCapturedPiece);

            if (isCastling == true)
            {
                setPiece(fromRow, castlingRookFromColumn, castlingRookPiece);
                setPiece(fromRow, castlingRookToColumn, ".");
            }

            return true;
        }

        if (movingPiece == "P" && fromRow == 6 && toRow == 4)
            whiteWasLastMovementPawnPush[toRow][toColumn] = 1;

        if (movingPiece == "p" && fromRow == 1 && toRow == 3)
            blackWasLastMovementPawnPush[toRow][toColumn] = 1;

        if (movingPiece == "R" || movingPiece == "r")
            haveRooksMovedDuringTheGame[fromRow][fromColumn] = 1;

        if ((capturedPiece == "R" || capturedPiece == "r") && (toRow == 0 || toRow == 7) && (toColumn == 0 || toColumn == 7))
            haveRooksMovedDuringTheGame[toRow][toColumn] = 1;

        if (movingPiece == "K" || movingPiece == "k")
            haveKingsMovedDuringTheGame[getCurrentPlayer()] = 1;

        if ((movingPiece == "P" && toRow == 0) || (movingPiece == "p" && toRow == 7))
            promotePawn(toSquare);

        return true;
    }
    bool isPawnMovementValid(string fromSquare, string toSquare)
    {
        int fromColumn = convertColumnToNumber(fromSquare);
        int fromRow = convertRowtoNumber(fromSquare);

        int toColumn = convertColumnToNumber(toSquare);
        int toRow = convertRowtoNumber(toSquare);

        if (whoseTurn == 0) // white pawn
        {
            if (toRow == fromRow - 1 && fromColumn == toColumn && getPieceFromSquare(toSquare) == ".") // one-square move
                return true;

            else if (fromRow == 6 && toRow == 4 && fromColumn == toColumn && getPieceFromSquare(toSquare) == "." && getPiece(fromRow - 1, fromColumn) == ".") // pawn push
                return true;

            else if (toRow == fromRow - 1 && (toColumn == fromColumn - 1 || toColumn == fromColumn + 1) && isOCCupiedByEnemyPiece(toSquare))
                return true;

            else if (fromRow == 3 && toRow == 2 && (toColumn == fromColumn + 1 || toColumn == fromColumn - 1) && blackWasLastMovementPawnPush[fromRow][toColumn] == 1) // en passant
            {
                return true;
            }
            else
                return false;
        }

        else // black pawn
        {
            if (toRow == fromRow + 1 && fromColumn == toColumn && getPieceFromSquare(toSquare) == ".") // one-square move
                return true;

            else if (fromRow == 1 && toRow == 3 && fromColumn == toColumn && getPieceFromSquare(toSquare) == "." && getPiece(fromRow + 1, fromColumn) == ".") // pawn push
                return true;

            else if (toRow == fromRow + 1 && (toColumn == fromColumn - 1 || toColumn == fromColumn + 1) && isOCCupiedByEnemyPiece(toSquare))
                return true;

            else if (fromRow == 5 && toRow == 6 && (toColumn == fromColumn + 1 || toColumn == fromColumn - 1) && whiteWasLastMovementPawnPush[fromRow][toColumn] == 1) // en passant
            {
                return true;
            }

            else
                return false;
        }
    }
    bool isRookMovementValid(string fromSquare, string toSquare)
    {
        int fromColumn = convertColumnToNumber(fromSquare);
        int fromRow = convertRowtoNumber(fromSquare);

        int toColumn = convertColumnToNumber(toSquare);
        int toRow = convertRowtoNumber(toSquare);

        bool isWayClear = true;

        if (fromColumn == toColumn)
        {
            if (toRow > fromRow)
            {
                for (int row = fromRow + 1; row < toRow; row++)
                {
                    if (getPiece(row, fromColumn) != ".")
                        isWayClear = false;
                }
            }
            else
            {
                for (int row = fromRow - 1; row > toRow; row--)
                {
                    if (getPiece(row, fromColumn) != ".")
                        isWayClear = false;
                }
            }
        }

        if (fromRow == toRow)
        {
            if (toColumn > fromColumn)
            {
                for (int column = fromColumn + 1; column < toColumn; column++)
                {
                    if (getPiece(fromRow, column) != ".")
                        isWayClear = false;
                }
            }
            else
            {
                for (int column = fromColumn - 1; column > toColumn; column--)
                {
                    if (getPiece(fromRow, column) != ".")
                        isWayClear = false;
                }
            }
        }

        if (fromColumn != toColumn && fromRow != toRow)
            return false;
        else if (isOccupiedByOwnPiece(toSquare))
            return false;
        else if (isWayClear == false)
            return false;
        else if (fromSquare == toSquare)
            return false;
        else
            return true;
    }
    bool isBishopMovementValid(string fromSquare, string toSquare)
    {
        int fromColumn = convertColumnToNumber(fromSquare);
        int fromRow = convertRowtoNumber(fromSquare);

        int toColumn = convertColumnToNumber(toSquare);
        int toRow = convertRowtoNumber(toSquare);

        int rowDiff = toRow - fromRow;
        int columnDiff = toColumn - fromColumn;

        if (rowDiff < 0)
            rowDiff = -rowDiff;
        if (columnDiff < 0)
            columnDiff = -columnDiff;

        if (rowDiff != columnDiff)
            return false;

        if (isOccupiedByOwnPiece(toSquare))
            return false;

        if (fromSquare == toSquare)
            return false;

        int rowStep;
        int columnStep;

        if (toRow > fromRow)
            rowStep = 1;
        else
            rowStep = -1;

        if (toColumn > fromColumn)
            columnStep = 1;
        else
            columnStep = -1;

        int row = fromRow + rowStep;
        int column = fromColumn + columnStep;

        while (row != toRow && column != toColumn)
        {
            if (getPiece(row, column) != ".")
                return false;

            row = row + rowStep;
            column = column + columnStep;
        }

        return true;
    }
    bool isKnightMovementValid(string fromSquare, string toSquare)
    {
        int fromColumn = convertColumnToNumber(fromSquare);
        int fromRow = convertRowtoNumber(fromSquare);

        int toColumn = convertColumnToNumber(toSquare);
        int toRow = convertRowtoNumber(toSquare);

        int rowDiff = toRow - fromRow;
        int columnDiff = toColumn - fromColumn;

        if (rowDiff < 0)
            rowDiff = -rowDiff;
        if (columnDiff < 0)
            columnDiff = -columnDiff;

        bool isKnightMove = (rowDiff == 2 && columnDiff == 1) || (rowDiff == 1 && columnDiff == 2);

        if (isKnightMove && isOccupiedByOwnPiece(toSquare) == false)
            return true;
        else
            return false;
    }
    bool isQueenMovementValid(string fromSquare, string toSquare)
    {
        if (isBishopMovementValid(fromSquare, toSquare) || isRookMovementValid(fromSquare, toSquare))
            return true;
        else
            return false;
    }
    bool isKingMovementValid(string fromSquare, string toSquare)
    {
        int fromColumn = convertColumnToNumber(fromSquare);
        int fromRow = convertRowtoNumber(fromSquare);

        int toColumn = convertColumnToNumber(toSquare);
        int toRow = convertRowtoNumber(toSquare);

        int rowDiff = toRow - fromRow;
        int columnDiff = toColumn - fromColumn;

        if (rowDiff < 0)
            rowDiff = -rowDiff;
        if (columnDiff < 0)
            columnDiff = -columnDiff;

        if (rowDiff == 0 && columnDiff == 2 && isCastlingMoveValid(fromSquare, toSquare) == true)
            return true;

        if ((rowDiff < 2 && columnDiff < 2) && isOccupiedByOwnPiece(toSquare) == false && fromSquare != toSquare)
            return true;

        return false;
    }
    bool isCastlingMoveValid(string fromSquare, string toSquare)
    {
        int fromColumn = convertColumnToNumber(fromSquare);
        int fromRow = convertRowtoNumber(fromSquare);

        int toColumn = convertColumnToNumber(toSquare);
        int toRow = convertRowtoNumber(toSquare);

        string king = getPieceFromSquare(fromSquare);

        if (fromRow != toRow)
            return false;

        if (fromColumn != 4)
            return false;

        if (getCurrentPlayer() == 0 && (fromSquare != "e1" && fromSquare != "E1"))
            return false;

        if (getCurrentPlayer() == 1 && (fromSquare != "e8" && fromSquare != "E8"))
            return false;

        if (haveKingsMovedDuringTheGame[getCurrentPlayer()] == 1)
            return false;

        if (getPieceFromSquare(toSquare) != ".")
            return false;

        if (isSquareAttacked(fromSquare) == true)
            return false;

        if (toColumn == 6) // short castling
        {
            if (getCurrentPlayer() == 0 && king != "K")
                return false;
            if (getCurrentPlayer() == 1 && king != "k")
                return false;

            if (getCurrentPlayer() == 0 && getPiece(fromRow, 7) != "R")
                return false;
            if (getCurrentPlayer() == 1 && getPiece(fromRow, 7) != "r")
                return false;

            if (haveRooksMovedDuringTheGame[fromRow][7] == 1)
                return false;

            if (getPiece(fromRow, 5) != "." || getPiece(fromRow, 6) != ".")
                return false;

            if (isSquareAttacked(convertPositionToSquare(fromRow, 5)) == true)
                return false;
            if (isSquareAttacked(convertPositionToSquare(fromRow, 6)) == true)
                return false;

            return true;
        }
        else if (toColumn == 2) // long castling
        {
            if (getCurrentPlayer() == 0 && king != "K")
                return false;
            if (getCurrentPlayer() == 1 && king != "k")
                return false;

            if (getCurrentPlayer() == 0 && getPiece(fromRow, 0) != "R")
                return false;
            if (getCurrentPlayer() == 1 && getPiece(fromRow, 0) != "r")
                return false;

            if (haveRooksMovedDuringTheGame[fromRow][0] == 1)
                return false;

            if (getPiece(fromRow, 1) != "." || getPiece(fromRow, 2) != "." || getPiece(fromRow, 3) != ".")
                return false;

            if (isSquareAttacked(convertPositionToSquare(fromRow, 3)) == true)
                return false;
            if (isSquareAttacked(convertPositionToSquare(fromRow, 2)) == true)
                return false;

            return true;
        }

        return false;
    }

    void promotePawn(string square)
    {
        string piece;
        string promotedPiece = "Q";

        if (getCurrentPlayer() == 0)
        {
            cout << "White pawn promotion. Choose Q, R, B or N." << endl;
            cin >> piece;

            while (piece != "Q" && piece != "R" && piece != "B" && piece != "N" && piece != "q" && piece != "r" && piece != "b" && piece != "n")
            {
                cout << "Invalid promotion piece. Choose Q, R, B or N." << endl;
                cin >> piece;
            }

            if (piece == "q" || piece == "Q")
                promotedPiece = "Q";
            else if (piece == "r" || piece == "R")
                promotedPiece = "R";
            else if (piece == "b" || piece == "B")
                promotedPiece = "B";
            else if (piece == "n" || piece == "N")
                promotedPiece = "N";
        }
        else
        {
            cout << "Black pawn promotion. Choose q, r, b or n." << endl;
            cin >> piece;

            while (piece != "Q" && piece != "R" && piece != "B" && piece != "N" && piece != "q" && piece != "r" && piece != "b" && piece != "n")
            {
                cout << "Invalid promotion piece. Choose q, r, b or n." << endl;
                cin >> piece;
            }

            if (piece == "q" || piece == "Q")
                promotedPiece = "q";
            else if (piece == "r" || piece == "R")
                promotedPiece = "r";
            else if (piece == "b" || piece == "B")
                promotedPiece = "b";
            else if (piece == "n" || piece == "N")
                promotedPiece = "n";
        }

        setPieceOnSquare(square, promotedPiece);
    }

    string findKingPosition()
    {
        for (int row = 0; row < 8; row++)
        {
            for (int column = 0; column < 8; column++)
            {
                if ((whoseTurn == 0 && board[row][column] == "K") || (whoseTurn == 1 && board[row][column] == "k"))
                {
                    return convertPositionToSquare(row, column);
                }
            }
        }
        return "";
    }
    bool isSquareAttackedByPawn(string squareToCheck)
    {
        int squareColumn = convertColumnToNumber(squareToCheck);
        int squareRow = convertRowtoNumber(squareToCheck);

        if (whoseTurn == 0)
        {
            if (getPiece(squareRow - 1, squareColumn + 1) == "p" || getPiece(squareRow - 1, squareColumn - 1) == "p")
                return true;
            else
                return false;
        }
        else
        {
            if (getPiece(squareRow + 1, squareColumn + 1) == "P" || getPiece(squareRow + 1, squareColumn - 1) == "P")
                return true;
            else
                return false;
        }
    }
    bool isSquareAttackedByRook(string squareToCheck)
    {
        int squareColumn = convertColumnToNumber(squareToCheck);
        int squareRow = convertRowtoNumber(squareToCheck);

        string attackingRook;

        if (whoseTurn == 0)
            attackingRook = "r";
        else
            attackingRook = "R";

        int rookDirections[4][2] =
            {
                {1, 0},
                {-1, 0},
                {0, 1},
                {0, -1}};

        for (int i = 0; i < 4; i++)
        {
            int checkedRow = squareRow + rookDirections[i][0];
            int checkedColumn = squareColumn + rookDirections[i][1];

            while (checkedRow >= 0 && checkedRow < 8 && checkedColumn >= 0 && checkedColumn < 8)
            {
                string piece = getPiece(checkedRow, checkedColumn);

                if (piece == attackingRook)
                    return true;

                if (piece != ".")
                    break;

                checkedRow = checkedRow + rookDirections[i][0];
                checkedColumn = checkedColumn + rookDirections[i][1];
            }
        }

        return false;
    }
    bool isSquareAttackedByBishop(string squareToCheck)
    {
        int squareColumn = convertColumnToNumber(squareToCheck);
        int squareRow = convertRowtoNumber(squareToCheck);

        string attackingBishop;

        if (whoseTurn == 0)
            attackingBishop = "b";
        else
            attackingBishop = "B";

        int bishopDirections[4][2] =
            {
                {1, 1},
                {1, -1},
                {-1, 1},
                {-1, -1}};

        for (int i = 0; i < 4; i++)
        {
            int checkedRow = squareRow + bishopDirections[i][0];
            int checkedColumn = squareColumn + bishopDirections[i][1];

            while (checkedRow >= 0 && checkedRow < 8 && checkedColumn >= 0 && checkedColumn < 8)
            {
                string piece = getPiece(checkedRow, checkedColumn);

                if (piece == attackingBishop)
                    return true;

                if (piece != ".")
                    break;

                checkedRow = checkedRow + bishopDirections[i][0];
                checkedColumn = checkedColumn + bishopDirections[i][1];
            }
        }

        return false;
    }
    bool isSquareAttackedByKnight(string squareToCheck)
    {
        int squareColumn = convertColumnToNumber(squareToCheck);
        int squareRow = convertRowtoNumber(squareToCheck);

        string attackingKnight;

        if (whoseTurn == 0)
            attackingKnight = "n";
        else
            attackingKnight = "N";

        int knightMoves[8][2] =
            {
                {2, 1},
                {2, -1},
                {-2, 1},
                {-2, -1},
                {1, 2},
                {1, -2},
                {-1, 2},
                {-1, -2}};

        for (int i = 0; i < 8; i++)
        {
            int checkedRow = squareRow + knightMoves[i][0];
            int checkedColumn = squareColumn + knightMoves[i][1];

            if (getPiece(checkedRow, checkedColumn) == attackingKnight)
                return true;
        }

        return false;
    }
    bool isSquareAttackedByQueen(string squareToCheck)
    {
        int squareColumn = convertColumnToNumber(squareToCheck);
        int squareRow = convertRowtoNumber(squareToCheck);

        string attackingQueen;

        if (whoseTurn == 0)
            attackingQueen = "q";
        else
            attackingQueen = "Q";

        int bishopDirections[4][2] =
            {
                {1, 1},
                {1, -1},
                {-1, 1},
                {-1, -1}};

        int rookDirections[4][2] =
            {
                {1, 0},
                {-1, 0},
                {0, 1},
                {0, -1}};

        for (int i = 0; i < 4; i++)
        {
            int checkedRow = squareRow + bishopDirections[i][0];
            int checkedColumn = squareColumn + bishopDirections[i][1];

            while (checkedRow >= 0 && checkedRow < 8 && checkedColumn >= 0 && checkedColumn < 8)
            {
                string piece = getPiece(checkedRow, checkedColumn);

                if (piece == attackingQueen)
                    return true;

                if (piece != ".")
                    break;

                checkedRow = checkedRow + bishopDirections[i][0];
                checkedColumn = checkedColumn + bishopDirections[i][1];
            }
        }

        for (int i = 0; i < 4; i++)
        {
            int checkedRow = squareRow + rookDirections[i][0];
            int checkedColumn = squareColumn + rookDirections[i][1];

            while (checkedRow >= 0 && checkedRow < 8 && checkedColumn >= 0 && checkedColumn < 8)
            {
                string piece = getPiece(checkedRow, checkedColumn);

                if (piece == attackingQueen)
                    return true;

                if (piece != ".")
                    break;

                checkedRow = checkedRow + rookDirections[i][0];
                checkedColumn = checkedColumn + rookDirections[i][1];
            }
        }

        return false;
    }
    bool isSquareAttackedByKing(string squareToCheck)
    {
        int squareColumn = convertColumnToNumber(squareToCheck);
        int squareRow = convertRowtoNumber(squareToCheck);

        string attackingKing;

        if (whoseTurn == 0)
            attackingKing = "k";
        else
            attackingKing = "K";

        for (int row = squareRow - 1; row <= squareRow + 1; row++)
        {
            for (int column = squareColumn - 1; column <= squareColumn + 1; column++)
            {
                if (row == squareRow && column == squareColumn)
                    continue;

                if (getPiece(row, column) == attackingKing)
                    return true;
            }
        }

        return false;
    }
    bool isSquareAttacked(string squareToCheck)
    {
        string kingPosition = squareToCheck;

        if (kingPosition == "")
            return false;

        if (isSquareAttackedByPawn(kingPosition) == true)
            return true;
        else if (isSquareAttackedByRook(kingPosition) == true)
            return true;
        else if (isSquareAttackedByBishop(kingPosition) == true)
            return true;
        else if (isSquareAttackedByKnight(kingPosition) == true)
            return true;
        else if (isSquareAttackedByQueen(kingPosition) == true)
            return true;
        else if (isSquareAttackedByKing(kingPosition) == true)
            return true;
        else
            return false;

        return false;
    }

    bool playerHasAnyLegalMove()
    {
        for (int row = 0; row < 8; row++)
        {
            for (int column = 0; column < 8; column++)
            {
                string piece = getPiece(row, column);

                if (isCurrentPlayersPiece(piece) == true)
                {
                    string fromSquare = convertPositionToSquare(row, column);

                    for (int targetRow = 0; targetRow < 8; targetRow++)
                    {
                        for (int targetColumn = 0; targetColumn < 8; targetColumn++)
                        {
                            string toSquare = convertPositionToSquare(targetRow, targetColumn);

                            if (tryMovePiece(fromSquare, toSquare, false, false) == true)
                                return true;
                        }
                    }
                }
            }
        }

        return false;
    }
    bool isCheckMate()
    {
        if (isSquareAttacked(findKingPosition()) == true && playerHasAnyLegalMove() == false)
            return true;
        else
            return false;
    }
    bool isStaleMate()
    {
        if (isSquareAttacked(findKingPosition()) == false && playerHasAnyLegalMove() == false)
            return true;
        else
            return false;
    }
    void printWinner()
    {
        if (getCurrentPlayer() == 0)
            cout << "Checkmate. BLACK wins." << endl;
        else
            cout << "Checkmate. WHITE wins." << endl;
    }
};

int main()
{
    Board chessBoard;
    chessBoard.startGame();

    return 0;
}
