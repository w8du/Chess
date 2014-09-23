#ifndef __AI_H__
#define __AI_H__
#include <vector>

struct Move;
class Posn;
class Piece;
class ChessBoard;
/*
Move bestMove(bool, const ChessBoard&);

int assess(bool, const ChessBoard&);
*/
//return all legal moves of a piece
std::vector<Move> legalMove(ChessBoard&, Piece*);
//return the total value of a player's survived pieces
int gain(ChessBoard&, bool);
//return the number of pieces that is threatened by player's opponent
int threats(ChessBoard&, bool);
#endif 
