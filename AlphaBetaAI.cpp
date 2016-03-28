#include "AlphaBetaAI.h"

void AlphaBetaAI::move(Board& board){
  BookManager book;
  vP movables = book.find(board);

  if(movables.empty()){
    board.pass();
    return;
  }

  if(movables.size() == 1){
    // 打てる箇所が一箇所だけなら探索は行わず、即座に打って返る
    board.move(movables[0]);
    return;
  }

  Eval = make_shared<MidEvaluator>();
  // 事前に手を良さそうな順にソート
  sort(board, movables, presearch_depth);

  int limit;
  if(MAX_TURNS - board.getTurns() <= wld_depth){
    limit = numeric_limits<int>::max();

    Eval.reset();
    if(MAX_TURNS - board.getTurns() <= perfect_depth){
      Eval = make_shared<PerfectEvaluator>();
    }
    else{
      Eval = make_shared<WLDEvaluator>();
    }
  }
  else{
    limit = normal_depth;
  }

  int eval;
  int alpha = numeric_limits<int>::min();
  int beta  = numeric_limits<int>::max();
  Point p;
  for(const auto& mb : movables){
    board.move(mb);
    eval = -alphabeta(board, limit - 1, -beta, -alpha);
    board.undo();

    if(eval > alpha){
      alpha = eval;
      p = mb;
    }
  }

  board.move(p);
}

void AlphaBetaAI::sort(Board& board, vP& movables, int limit){
  vector<Move> moves;

  for(const auto& mb : movables){
    board.move(mb);
    int eval = -alphabeta(board, limit - 1,
      -numeric_limits<int>::max(), -numeric_limits<int>::min());
    board.undo();

    moves.emplace_back(mb.x, mb.y, eval);
  }

  // 評価値の大きい順にソート
  stable_sort(all(moves), MoveGreater());

  // 結果の書き戻し
  movables.clear();
  for(const auto& m : moves){
    movables.emplace_back(m.x, m.y);
  }
}

int AlphaBetaAI::alphabeta(Board& board, int limit, int alpha, int beta){
  // 深さ制限に達したら評価値を返す
  if(board.isGameOver() || limit == 0) return Eval->evaluate(board);

  int score;
  const vP movables = board.getMovablePos();

  if(movables.empty()){
    board.pass();
    score = -alphabeta(board, limit, -beta, -alpha);
    board.undo();
    return score;
  }

  for(const auto& mb : movables){
    board.move(mb);
    score = -alphabeta(board, limit - 1, -beta, -alpha);
    board.undo();

    if(score >= beta) return score;

    alpha = max(alpha, score);
  }

  return alpha;
}

