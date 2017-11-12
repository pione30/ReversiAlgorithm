#include "Evaluator.h"

class PerfectEvaluator : public Evaluator {
public:
  int evaluate(const Board& board) override {
    return board.getCurrentColor() 
      * (board.countDisc(Color::BLACK) - board.countDisc(Color::WHITE));
  }
};
