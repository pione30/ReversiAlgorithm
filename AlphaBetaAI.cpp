#include "AI.h"
#define FOR(i, a, b) for(int (i) = (a); (i) <= (b); ++(i))
#define rep(i, n) FOR(i, 0, n - 1)
#define rep1(i, n) FOR(i, 1, n)
#define rrep(i, n) for(int (i) = (n) - 1; (i) >= 0; --(i))
#define all(a) (a).begin(),(a).end()

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
  AlphaBetaAI();
  void move(Board& board) override;

private:
  int evaluate(const Board& board);
  void sort(Board& board, vP&, int limit);
  int alphabeta(Board& board, int limit, int alpha, int beta);
};


void AlphaBetaAI::move(Board& board){
  vP movables = board.getMovablePos();

  if(movables.empty()){
    board.pass();
    return;
  }

  if(movables.size() == 1){
    // 打てる箇所が一箇所だけなら探索は行わず、即座に打って返る
    board.move(movables[0]);
    return;
  }
 
  // 事前に手を良さそうな順にソート
  sort(board, movables, presearch_depth);

  int limit;
  if(MAX_TURNS - board.getTurns() <= wld_depth){
    limit = numeric_limits<int>::max();
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
  if(board.isGameOver() || limit == 0) return evaluate(board);

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

