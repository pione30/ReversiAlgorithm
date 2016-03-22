#include "MidEvaluator.h"
#define FOR(i, a, b) for(int (i) = (a); (i) <= (b); ++(i))
#define rep(i, n) FOR(i, 0, n - 1)
#define rep1(i, n) FOR(i, 1, n)
#define rrep(i, n) for(int (i) = (n) - 1; (i) >= 0; --(i))
#define all(a) (a).begin(),(a).end()

bool MidEvaluator::TableInit = false;

MidEvaluator::MidEvaluator(){
  if(!TableInit){
    // 初回起動時にテーブルを生成
    vC line(BOARD_SIZE);
    generateEdge(line, 0);
    TableInit = true;
  }

  // 重み係数の設定(全局面共通)
  EvalWeight.mobility_w = 67;
  EvalWeight.liberty_w  = -13;
  EvalWeight.stable_w   = 101;
  EvalWeight.wing_w     = -308;
  EvalWeight.Xmove_w    = -449;
  EvalWeight.Cmove_w    = -552;
}


int MidEvaluator::evaluate(const Board& board){
  EdgeStat edgestat;
  // 辺の評価
  edgestat  = EdgeTable[idxTop(board)];
  edgestat += EdgeTable[idxBottom(board)];
  edgestat += EdgeTable[idxRight(board)];
  edgestat += EdgeTable[idxLeft(board)];

  // 隅の評価
  CornerStat cornerstat = evalCorner(board);

  // 確定石に関して、隅の石を2回数えてしまっているので補正
  edgestat[Color::BLACK].stable -= cornerstat[Color::BLACK].corner;
  edgestat[Color::WHITE].stable -= cornerstat[Color::WHITE].corner;

  int result = 0;
  // パラメータの線型結合
  result +=
    ( edgestat[Color::BLACK].stable * EvalWeight.stable_w
    + edgestat[Color::BLACK].wing * EvalWeight.wing_w
    + edgestat[Color::BLACK].Cmove * EvalWeight.Cmove_w
    + cornerstat[Color::BLACK].Xmove * EvalWeight.Xmove_w );
    
  result -=
    ( edgestat[Color::WHITE].stable * EvalWeight.stable_w
    + edgestat[Color::WHITE].wing * EvalWeight.wing_w
    + edgestat[Color::WHITE].Cmove * EvalWeight.Cmove_w
    + cornerstat[Color::WHITE].Xmove * EvalWeight.Xmove_w );

  // 開放度・着手可能手数の評価
  if(EvalWeight.liberty_w != 0){
    ColorStorage<unsigned> liberty = countLiberty(board);
    result += liberty[Color::BLACK] * EvalWeight.liberty_w;
    result -= liberty[Color::WHITE] * EvalWeight.liberty_w;
  }

  // 現在の手番の色についてのみ、着手可能手数を数える
  result +=
      board.getCurrentColor()
    * board.getMovablePos().size()
    * EvalWeight.mobility_w;

  return board.getCurrentColor() * result;
}

void MidEvaluator::generateEdge(vC& edge, unsigned index){
  if(index == BOARD_SIZE){
    // このパターンは完成したので、局面のカウント
    EdgeStat stat;
    stat[Color::BLACK] = evalEdge(edge, Color::BLACK);
    stat[Color::WHITE] = evalEdge(edge, Color::WHITE);

    EdgeTable[idxLine(edge)] = stat;
    return;
  }

  // 再帰的にすべてのパターンを網羅
  FOR(c, -1, 1){
    edge[index] = (Color)c;
    generateEdge(edge, index + 1);
  }
}

// 辺のパラメータを数える
MidEvaluator::EdgeParam MidEvaluator::evalEdge(vC& edge, Color color) const {
  EdgeParam edgeparam = { 0, 0, 0, 0 };

  // ウィング等のカウント
  if(edge[0] == Color::EMPTY && edge[7] == Color::EMPTY){
    unsigned x = 2;
    while(x <= 5){
      if(edge[x] != color) break;
      x++;
    }
    if(x == 6){ // 少なくともブロックができている
      if((edge[1] == color && edge[6] == Color::EMPTY) || 
         (edge[1] == Color::EMPTY && edge[6] == color) )
        edgeparam.wing = 1;
      else if(edge[1] == color && edge[6] == color)
        edgeparam.mountain = 1;
    }
  }
  else { // それ以外の場合に、隅に隣接する位置に置いていたら
    if(edge[1] == color) edgeparam.Cmove++;
    if(edge[6] == color) edgeparam.Cmove++;
  }

  // 確定石のカウント
  rep(x, 8){
    if(edge[x] != color) break;
    edgeparam.stable++;
  }
  // 右側からの走査も必要
  if(edgeparam.stable < 8){
    for(unsigned x = 7; x > 0; x--){
      if(edge[x] != color) break;
      edgeparam.stable++;
    }
  }

  return edgeparam;
}

// 隅のパラメータを調べる。この関数は各評価時に使う。
MidEvaluator::CornerStat MidEvaluator::evalCorner(const Board& board) const {
  CornerStat cornerstat;
  cornerstat[Color::BLACK].corner = 0;
  cornerstat[Color::BLACK].Xmove = 0;
  cornerstat[Color::WHITE].corner = 0;
  cornerstat[Color::WHITE].Xmove = 0;
  
  cornerstat[board.getColor(Point(1, 1))].corner++;
  cornerstat[board.getColor(Point(1, 8))].corner++;
  cornerstat[board.getColor(Point(8, 1))].corner++;
  cornerstat[board.getColor(Point(8, 8))].corner++;

  if(board.getColor(Point(1, 1)) == Color::EMPTY)
    cornerstat[board.getColor(Point(2, 2))].Xmove++;
  if(board.getColor(Point(1, 8)) == Color::EMPTY)
    cornerstat[board.getColor(Point(2, 7))].Xmove++;
  if(board.getColor(Point(8, 1)) == Color::EMPTY)
    cornerstat[board.getColor(Point(7, 2))].Xmove++;
  if(board.getColor(Point(8, 8)) == Color::EMPTY)
    cornerstat[board.getColor(Point(7, 7))].Xmove++;

  return cornerstat;
}

ColorStorage<unsigned> MidEvaluator::countLiberty(const Board& board) const {
  ColorStorage<unsigned> liberty;
  liberty[Color::BLACK] = 0;
  liberty[Color::WHITE] = 0;
  liberty[Color::EMPTY] = 0;
  
  rep1(x, BOARD_SIZE){
    rep1(y, BOARD_SIZE){
      Point p(x, y);
      liberty[board.getColor(p)] += board.getLiberty(p);
    }
  }

  return liberty;
}

unsigned MidEvaluator::idxTop(const Board& board) const {
  unsigned index = 0;
  rep(i, 8){
    index += idx3digit[i] * (board.getColor(Point(i + 1, 1)) + 1);
  }
  return index;
}

unsigned MidEvaluator::idxBottom(const Board& board) const {
  unsigned index = 0;
  rep(i, 8){
    index += idx3digit[i] * (board.getColor(Point(i + 1, 8)) + 1);
  }
  return index;
}

unsigned MidEvaluator::idxRight(const Board& board) const {
  unsigned index = 0;
  rep(i, 8){
    index += idx3digit[i] * (board.getColor(Point(8, i + 1)) + 1);
  }
  return index;
}

unsigned MidEvaluator::idxLeft(const Board& board) const {
  unsigned index = 0;
  rep(i, 8){
    index += idx3digit[i] * (board.getColor(Point(1, i + 1)) + 1);
  }
  return index;
}

unsigned MidEvaluator::idxLine(const vC& l){
  unsigned ret = 0;
  rep(i, 8){
    ret += l[i] + 1;
    if(i < 7) ret *= 3;
  }
  return ret;
}
