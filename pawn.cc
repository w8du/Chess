#include "pawn.h"
#include "posn.h"
#include "move.h"
#include <cstdlib>
#include <vector>
#include "chessboard.h"
using namespace std;

Pawn::Pawn(ChessBoard* b, char c, bool player) : Piece(b, c, player), prom(0) {}

Pawn::~Pawn() {
 delete prom;
}

int Pawn::val() {
 return prom ? prom->val() : 1;
}

char Pawn::getName() {
 return prom ? prom->getName() : name;
}

void Pawn::promote(Piece* p) {
 prom = p;
 owner ? p->update(pos, covers, threats) : p->update(pos, covers, threats);
} 
 
int Pawn::canReach(const Posn posn) {
 if (prom) return prom->canReach(posn);
 int o = board->isOccupied(posn, owner);
 if (owner) {
  if (posn.col == pos.col) {
   if (posn.row == pos.row - 2) {
    if (!moved) {
        if (board->isOccupied(Posn(pos.row-1,posn.col), owner) == 0) {
            return o ? 0 : 1;
        } else {
            return 0;
        }
    }
    else return 0;
   }
   else if (posn.row == pos.row - 1) 
    return o ? 0 : (posn.row == 0 && prom == 0 ? 3 : 1);
  }
  if (abs(posn.col - pos.col) == 1) {
   if (posn.row == pos.row - 1) {
    return o ? (posn.row == 0 && prom == 0 ? 3 : 1) : isenPassant(posn);
   }
   else return 0;
  }
 }
 else {
  if (posn.col == pos.col) {
   if (posn.row == pos.row + 2) {
    if (board->isOccupied(Posn(pos.row+1, posn.col), owner)) return 0;
    if (!moved) return o ? 0 : (posn.row == 7 && prom == 0 ? 3 : 1);
    else return 0;
   }
   else if (posn.row == pos.row + 1) 
    return o ? 0 : (posn.row == 7 && prom == 0 ? 3 : 1);
  }
  if (abs(posn.col - pos.col) == 1) {
   if (posn.row == pos.row + 1) {
    return o ? (posn.row == 7 ? 3 : 1) : isenPassant(posn);
   }
   else return 0;
  }
 }
 return 0;
}

int Pawn::isenPassant(const Posn posn) {
 if (prom) return 0;
 Posn p (pos.row, posn.col);
 if (board->getRecord()->size() == 0) return 0;
 Move m = (board->getRecord())->back();
 Posn o (owner ? 1 : 6, posn.col);
 if (m.orig == o && m.dest == p && m.name == (owner ? 'p' : 'P')) {
  return 4;
 }
 else return 0;
}

void Pawn::unpromote() {
 delete prom;
 prom = 0;
}

void Pawn::update(const Posn p, vector<Piece*> white, vector<Piece*> black) {
 if (!(pos == Posn(-1, -1)) && !(pos == p)) moved = true;
 else {
 vector<Move>* record = board->getRecord();
 bool res = false;
 if (record->size()) {
  for (vector<Move>::reverse_iterator i = record->rbegin(); i != record->rend(); i++) {
   if (i->mover == this) {
    res = true;
    break;                                                                         }
  }
 }
 moved = res; 
 }
 pos = p;
 threats = owner ? black : white;
 covers = owner ? white : black;
 Threatened = threats.empty() ? false : true;
 Covered = covers.empty() ? false : true;
}
