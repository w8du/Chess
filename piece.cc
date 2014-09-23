#include "piece.h"
#include "chessboard.h"
#include "posn.h"
using namespace std;

Piece::Piece(ChessBoard* b, char c, bool o): 
 name(c), owner(o), board(b), pos(Posn(-1, -1)) {
}

char Piece::getName() {
 return name;
}

bool Piece::getOwner() {
 return owner;
}

Posn Piece::getPosn() {
 return pos;
}

vector<Piece*> Piece::getThreats() {
 return threats;
}

bool Piece::isThreatened() {return Threatened;}

bool Piece::isCovered() {return Covered;}

vector<Piece*> Piece::getCovers() {
 return covers;
}

void Piece::update(const Posn p, vector<Piece*> white, vector<Piece*> black) {
 pos = p;
 threats = owner ? black : white;
 covers = owner ? white : black;
 Threatened = threats.empty() ? false : true;
 Covered = covers.empty() ? false : true;
}

int Piece::move(const Posn p) {//a template method
 if (board->isOccupied(p, owner) == 1) {
  return 0;
 }
 else if (board->isExposed(pos, p, owner)) {
  return 0;
 }
 else return canReach(p);
}

Piece::~Piece() {}

bool Piece::isOnBoard() {
	return (pos.row != -1) && (pos.col != -1);
}
