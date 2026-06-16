# C++ Console Chess Game

A standalone console-based chess game written in C++. The project allows two players to play chess directly in the terminal using a text-based 8x8 board.

Each turn, the current player enters a starting square and a target square using standard chess coordinates, such as `e2` and `e4`. The program validates the input, checks whether the selected piece belongs to the current player, verifies whether the move follows the rules of chess, and prevents moves that would leave the player's own king in check.

After a valid move, the board is updated and the turn changes to the other player. The game supports the main rules required to complete a chess game, including check, checkmate, stalemate, castling, en passant, and pawn promotion.

## Running the project

The game is a standalone C++ console application with no external dependencies. It can be compiled using any C++17-compatible compiler.
