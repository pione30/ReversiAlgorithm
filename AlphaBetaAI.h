#pragma once
#include "AI.h"
#include "MidEvaluator.h"
#include "PerfectEvaluator.h"
#include "WLDEvaluator.h"
#include "BookManager.h"

struct Move : public Point {
  int eval;
  Move() : eval(0) {}
  Move(int _x, int _y, int _eval) : Point(_x, _y), eval(_eval) {}
};

struct MoveGreater{
  bool operator() (const Move& lhs, const Move& rhs){
    return (lhs.eval > rhs.eval);
  }
};


class AlphaBetaAI : public AI {
public:
  void move(Board& board) override;

private:
  unique_ptr<Evaluator> Eval;
  // int evaluate(const Board& board);
  void sort(Board& board, vP&, int limit);
  int alphabeta(Board& board, int limit, int alpha, int beta);
};
