#pragma once
#include "Evaluator.h"

class WLDEvaluator : public Evaluator {
public:
  static constexpr int WIN  =  1;
  static constexpr int DRAW =  0;
  static constexpr int LOSE = -1;
  
  int evaluate(const Board& board) override { 
    int discdiff = board.getCurrentColor() 
      * (board.countDisc(Color::BLACK) - board.countDisc(Color::WHITE));
    
    if(discdiff > 0) return WIN;
    else if(discdiff < 0) return LOSE;
    else return DRAW;
  }
};
