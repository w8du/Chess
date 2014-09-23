#include "chessboard.h"
#include "posn.h"
#include "move.h"
#include "cell.h"
#include "piece.h"
#include "king.h"
#include "queen.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "pawn.h"
#include "textdisplay.h"
#include "graphdisplay.h"
#include "rook.h"
#include "king.h"
#include "computer.h"
#include "ai.h"

using namespace std;

Piece* newPiece(ChessBoard* const b, const char c, bool player) {
 if (c == 'p' || c == 'P') return new Pawn(b, c, player);
 if (c == 'k' || c == 'K') return new King(b, c, player);
 if (c == 'q' || c == 'Q') return new Queen(b, c, player);
 if (c == 'b' || c == 'B') return new Bishop(b, c, player);
 if (c == 'n' || c == 'N') return new Knight(b, c, player);
 else return new Rook(b, c, player);
}

ChessBoard::ChessBoard(): tp(new TextDisplay), gp(new GraphDisplay), record(new vector<Move>), turn(0), blackmove(false) {
  for (int n = 0; n < 8; n++) {
   for (int m = 0; m < 8; m++) {
    board[n][m] = new Cell(Posn(n, m), gp);
   }
  }
  white[0] = newPiece(this, 'K', true);
  white[1] = newPiece(this, 'Q', true);
  white[6] = newPiece(this, 'B', true);
  white[7] = newPiece(this, 'B', true);
  white[4] = newPiece(this, 'N', true);
  white[5] = newPiece(this, 'N', true);
  white[2] = newPiece(this, 'R', true);
  white[3] = newPiece(this, 'R', true);
  black[0] = newPiece(this, 'k', false);
  black[1] = newPiece(this, 'q', false);
  black[6] = newPiece(this, 'b', false);
  black[7] = newPiece(this, 'b', false);
  black[4] = newPiece(this, 'n', false);
  black[5] = newPiece(this, 'n', false);
  black[2] = newPiece(this, 'r', false);
  black[3] = newPiece(this, 'r', false);
  for (int n = 8; n < 16; n++) {
   white[n] = newPiece(this, 'P', true);
   black[n] = newPiece(this, 'p', false);
  }
}

ChessBoard::~ChessBoard() {
    for (int i = 0; i < 8; ++i) {
     for (int j = 0; j < 8; j++)
        delete board[i][j];
    }
    for (int n = 0; n < 16; n++) {
     delete white[n];
     delete black[n];
    }
    delete tp;
    delete gp;
    delete record;
}

vector<Move>* ChessBoard::getRecord () {
    return record;
}

void ChessBoard::game() {
 for (int n = 0; n < 8; n++) {
  board[1][n]->putPiece(black[8 + n]);
  board[6][n]->putPiece(white[8 + n]);
 }
 board[0][4]->putPiece(black[0]);
 board[0][3]->putPiece(black[1]);
 board[0][2]->putPiece(black[6]);
 board[0][5]->putPiece(black[7]);
 board[0][1]->putPiece(black[4]);
 board[0][6]->putPiece(black[5]);
 board[0][0]->putPiece(black[2]);
 board[0][7]->putPiece(black[3]);
 board[7][4]->putPiece(white[0]);
 board[7][3]->putPiece(white[1]);
 board[7][2]->putPiece(white[6]);
 board[7][5]->putPiece(white[7]);
 board[7][1]->putPiece(white[4]);
 board[7][6]->putPiece(white[5]);
 board[7][0]->putPiece(white[2]);
 board[7][7]->putPiece(white[3]);
 tp->init();
 cout << *tp;
}

void ChessBoard::setup() {
 string comm;
 do {
  cout << "Enter command." << endl;
  cin >> comm;
  if (comm == "+") {
   char c;
   string pos;
   cin >> c >> pos;
   Posn p = convertCoords(pos);
   if (c > 'a') {
    for (int n = 0; n < 16; n++) {
     Piece* pc = black[n];
     if (pc->getName() == c && pc->getPosn().row == -1 ) {
         board[p.row][p.col]->putPiece(pc);
     tp->init(c, p);
     break;
     }
    }
   } else {
    for (int n = 0; n < 16; n++) {
     Piece* pc = white[n];
     if (pc->getName() == c && pc->getPosn().row == -1 ) {board[p.row][p.col]->putPiece(pc);
     tp->init(c, p);
     break;
     }
    }
   }
   cout << *tp;
  }
  if (comm == "-") {
   string pos;
   cin >> pos;
   Posn p = convertCoords(pos);
   board[p.row][p.col]->takeoff();
   tp->init(p);
   cout << *tp;
  }
  if (comm == "=") {
   string colour;
   cin >> colour;
   blackmove = colour == "white" ? false : true;
  }
  if (comm == "done") break;
 } while (true);
 update();
}

//update the status of cells; piece that is occupying the cell will also be updated
void ChessBoard::update() {
 for (int n = 0; n < 8; n++) {
  for (int m = 0; m < 8; m++) {
   Posn p(n, m);
   vector<Piece*> w;
   vector<Piece*> b;
   for (int l = 0; l < 16; l++) {
    Posn p1 = white[l]->getPosn();
    Posn p2 = black[l]->getPosn();
    if (!(p == p1) && (p1.row >= 0) && white[l]->canReach(p)) {
     w.push_back(white[l]);
    }
    if (!(p == p2) && (p2.row >= 0) && black[l]->canReach(p)) {
     b.push_back(black[l]);
    }
   }
   board[n][m]->update(w, b);
  }
 }
}

//display = false when computer is making a virtual move
int ChessBoard::move(const Posn orig, const Posn dest, bool display, bool computer) {
  Cell* co = board[orig.row][orig.col];
  Cell* cd = board[dest.row][dest.col];
  Piece* p = co->getPiece();
  if (!p) return 0;//no piece
  if (p->getOwner() && blackmove) return 0;//occupied by own piece
  if (!(p->getOwner()) && !blackmove) return 0;//same
  int res = p->move(dest);
  if (res == 0) return 0;
  else {
   Move m = {p, cd->takeoff(display), orig, dest, false, false, 0, p->getName()};
   co->takeoff(display);
   cd->putPiece(p, display);
  if (res == 2) {//chessboard puts the rook to the right place when castling
        m.castling = true;
        if (orig.col < dest.col) {
         Piece* rook = board[orig.row][7]->takeoff(display);
         board[orig.row][5]->putPiece(rook, display);
        } else {
         Piece* rook = board[orig.row][0]->takeoff(display);
         board[orig.row][3]->putPiece(rook, display);
        }
    }
   if (res == 3) {
    if (computer) {
     Pawn* pawn = static_cast<Pawn*>(p);
     pawn->promote(newPiece(this, (blackmove ? 'q' : 'Q'), pawn->getOwner()));
     if (display) gp->draw(pawn->getName(), pawn->getPosn());
    } else {
     char c;
     cout << "Promotion: enter the piece you want." << endl;
     cin >> c;
     Pawn* pawn = static_cast<Pawn*>(p);
     pawn->promote(newPiece(this, c, pawn->getOwner()));
     gp->draw(pawn->getName(), pawn->getPosn());
    }
    m.promotion = true;
   }
   if (res == 4) {//chessboard takes off the captured pawn in an en passant capture
    m.enpassant = board[orig.row][dest.col]->takeoff(display);
   }
   blackmove = !blackmove;
   turn += 1;
   record->push_back(m);
   update();
   if (display) {
     tp->notify(m);
     cout << *tp;
   }
   if (check(blackmove)) {
    if (checkmate(blackmove)) {
     return 3;
    }
    else return 2;
   }
   else if (stalemate(blackmove)) return 4;
   bool twokings = true;
   for (int n = 1; n < 16; n++) {//draw if only two kings are on board
    if (white[n]->getPosn().row >= 0 || black[n]->getPosn().row >= 0) {
    twokings = false;
    break;
    }
   }
   if (twokings) return 4;
   else return 1;
  }
  // shouldn't get here...
  // but if so
  return 0;
}

//undo moves according to the record
void ChessBoard::undo(bool display) {
 if (record->empty()) return;
 const Move m = record->back();
 if (display) tp->notify(m, true);
 record->pop_back();
 Cell* co = board[m.orig.row][m.orig.col];
 Cell* cd = board[m.dest.row][m.dest.col];
 cd->putPiece(m.captured, display);
 if (m.promotion) {
  Pawn* pawn = static_cast<Pawn*>(m.mover);
  pawn->unpromote();
 }
 if (m.castling) {
  if (m.orig.col < m.dest.col) {
    Piece* rook = board[m.orig.row][5]->takeoff(display);
    board[m.orig.row][7]->putPiece(rook, display);
  }
  else {
   Piece* rook = board[m.orig.row][3]->takeoff(display);
   board[m.orig.row][0]->putPiece(rook, display);
  }
 }
 if (m.enpassant) {
  board[m.orig.row][m.dest.col]->putPiece(m.enpassant, display);
 }
 co->putPiece(m.mover, display);
 update();
 if (display) cout << *tp;
 blackmove = !blackmove;
 turn -= 1;
}
  
//check whether making the move will get your king exposed to check
bool ChessBoard::isExposed(const Posn orig, const Posn dest, bool player) {
 Cell* co = board[orig.row][orig.col];
 Cell* cd = board[dest.row][dest.col];
 Piece* p1 = co->takeoff(false);
 Piece* p2 = cd->takeoff(false);
 cd->putPiece(p1, false);
 bool res = false;
 if (player) {
  for (int n = 0; n < 16; n++) {
   if ((black[n]->getPosn()).row != -1) {
    if (black[n]->canReach(white[0]->getPosn())) {
     res = true;
     break;
    }
   }
  }
 }
 else {
  for (int n = 0; n < 16; n++) {
   if ((white[n]->getPosn()).row != -1) {
    if (white[n]->canReach(black[0]->getPosn())) {
     res = true;
     break;
    }
   }
  }
 }
 cd->putPiece(p2, false);
 co->putPiece(p1, false);
 return res;
} 

int ChessBoard::isOccupied(const Posn posn, bool player) {
 Piece* p = board[posn.row][posn.col]->getPiece();
 if (!p) return 0;
 else if (p->getOwner() == player) return 1;
 else return 2;
}


bool ChessBoard::isAttacked(const Posn posn, bool opponent) {
    if (opponent) {
        return board[posn.row][posn.col]->getWhiteReach().size();
    } else {
        return board[posn.row][posn.col]->getBlackReach().size();
    }
}

bool ChessBoard::isWhiteMove() {
    return !blackmove;
}

bool ChessBoard::check(bool player) {
 if (!player) {
  return white[0]->getThreats().size();
 } else {
  return black[0]->getThreats().size();
 }
}

bool ChessBoard::checkmate(bool player) {
 bool res = true;
 for (int n = 0; n < 16; n++) {
  Piece* pc = !player ? white[n] : black[n];
  vector<Move> v = legalMove(*this, pc);
  if (!v.empty()) {
   res = false;
   break;
  }
 }
 return res;
}

bool ChessBoard::stalemate(bool player) {
 bool res = true;
 if (!player) {
 for (int n = 0; n < 16; n++) {
  if (res == false) break;
  if (white[n]->getPosn().row >= 0) {
   for (int r = 0; r < 8; r++) {
    if (res == false) break;
    for (int c = 0; c < 8; c++) {
     if (white[n]->move(Posn(r, c))) {
      res = false;
      break;
     }
    }
   }
  }
 }
 } else {
 for (int n = 0; n < 16; n++) {
  if (res == false) break;
  if (black[n]->getPosn().row >= 0) {
   for (int r = 0; r < 8; r++) {
    if (res == false) break;
    for (int c = 0; c < 8; c++) {
     if (black[n]->move(Posn(r, c))) {
      res = false;                                                                    break;
     }
    }
   }
  }
 }
 }
 return res;
}

ostream& operator<< (ostream& out, ChessBoard& b) {
    cout << *(b.tp);
    return out;
}

//pass the pieces to computer-player(needed for AI)
void ChessBoard::giveaway(Computer& com) {
 com.receive(white, black);
}
  
