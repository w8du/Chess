#include "player.h"
#include "human.h"
#include "chessboard.h"
#include "posn.h"
#include "move.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

// This class implements the Human Player.
// See human.h for declarations.

using namespace std;

// ctor
Human::Human(ChessBoard* b, bool isWhite) : Player(b, isWhite) {}

// Continues to read in moves until a legal one is reached
// returns 0 if draw, 1 if legal non ending move, 2 if resign, 3 if checkmate
int Human::move() {
    try {
    string s;
    while (getline(cin,s)) {
        int result = readLine(s);
        //if not undo
        if (result != 5) {
            if (result > 0) {
                if (result == 4) {
                    //draw
                    return 0;
                } else {
                    return result;
                }
            }   
        }
    }
    string err = "out of moves";
    throw err;
    } catch (string &s) {
    // throw an exception (which will be caught by its caller in Game) when stdin is exhausted
    throw;
    }
}

// Read in a move and execute it on chessboard
// returns 0 if illegal, 1 if legal non ending move, 2 if resign, 3 if checkmate, 4 if draw, 5 if undo
int Human::readLine(string s) {
    istringstream ss(s);
    string cmd, cmd2, cmd3;
    if (ss >> cmd) {
        if (cmd == "move") {
            if (ss >> cmd2) {
                if (ss >> cmd3) {
                    Posn pos1 = convertCoords(cmd2);
                    Posn pos2 = convertCoords(cmd3);
                    if ((pos1.row != -1) && (pos2.row != -1)) {
                        // other checking is done in ChessBoard (e.g. piece not empty, piece belongs to right guy, etc)
                        int result = board->move(pos1,pos2);
                        if (result == 0) {
                            cout << "Illegal Move" << endl;
                            return 0;
                        } else if (result == 1) {
                            //legal move
                            return 1;
                        } else if (result == 2) {
                            //check (still legal)
                            if (isWhite) {
                                cout << "White checks Black" << endl;
                            } else {
                                cout << "Black checks White" << endl;
                            }
                            return 1;
                        } else if (result == 3) {
                            //checkmate (this player won)
                            return 3;
                        } else if (result == 4) {
                            //draw
                            return 4;
                        } else {
                            return 0; //impossible scenario
                        }
                    }
                }
            }
        } else if (cmd == "resign") {
            //this player lost
            return 2;
        } else if (cmd == "undo") {
           if (board->getRecord()->size() < 2) {
            cout << "Bad input on command" << endl;
            return 0;
           } else {
            board->undo();
            board->undo();
            return 5;
           }
        } else {
            cout << "Bad input on command" << endl;
            return 0;
        }
    } 
    return 0;
}
