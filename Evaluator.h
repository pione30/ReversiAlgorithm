#pragma once
#include "Board.h"

class Evaluator{
public:
  virtual int evaluate(const Board& board) = 0;
};
