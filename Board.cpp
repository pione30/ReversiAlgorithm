#include "Board.h"
#define FOR(i, a, b) for(int (i) = (a); (i) <= (b); ++(i))
#define rep(i, n) FOR(i, 0, n - 1)
#define rep1(i, n) FOR(i, 1, n)
#define rrep(i, n) for(int (i) = (n) - 1; (i) >= 0; --(i))
#define all(a) (a).begin(),(a).end()


Board::Board(){ init(); }

void Board::init(){
  RawBoard.resize(BOARD_SIZE + 2, vC(BOARD_SIZE + 2));
  MovableDir.resize(MAX_TURNS + 1, vvu(BOARD_SIZE + 2, vu(BOARD_SIZE + 2)));
  Liberty.resize(BOARD_SIZE + 2, vi(BOARD_SIZE + 2));

  rep1(x, BOARD_SIZE) rep1(y, BOARD_SIZE){
    RawBoard[x][y] = EMPTY;
  }

  rep(y, BOARD_SIZE + 2){
    RawBoard[0][y] = WALL;
    RawBoard[BOARD_SIZE + 1][y] = WALL;
  }
  rep(x, BOARD_SIZE + 2){
    RawBoard[x][0] = WALL;
    RawBoard[x][BOARD_SIZE + 1] = WALL;
  }

  // 初期配置
  constexpr int MID = BOARD_SIZE / 2;
  RawBoard[MID    ][MID    ] = WHITE;
  RawBoard[MID + 1][MID + 1] = WHITE;
  RawBoard[MID    ][MID + 1] = BLACK;
  RawBoard[MID + 1][MID    ] = BLACK;

  Discs[BLACK] = 2;
  Discs[WHITE] = 2;
  Discs[EMPTY] = BOARD_SIZE * BOARD_SIZE - 4;

  Turns = 0;
  CurrentColor = BLACK;

  UpdateLog.clear();

  initMovable();
}

bool Board::move(const Point& point){
  if(point.x < 1 || BOARD_SIZE < point.x) return false;
  if(point.y < 1 || BOARD_SIZE < point.y) return false;
  if(MovableDir[Turns][point.x][point.y] == Direction::NONE) return false;

  flipDiscs(point);

  Turns++;
  CurrentColor = (Color) -CurrentColor;
  initMovable();

  return true;
}

bool Board::pass(){
  if(MovablePos[Turns].size() != 0) return false;
  if(isGameOver()) return false;

  CurrentColor = (Color) -CurrentColor;
  // 空のupdateを挿入しておく
  UpdateLog.emplace_back(vD());

  // パスで手数は増えないが、色が反転したので
  // MovableDirとMovablePosを調べ直す
  initMovable();

  return true;
}

bool Board::undo(){
  if(Turns == 0) return false;

  CurrentColor = (Color) -CurrentColor;

  const vD& update = UpdateLog.back();
  // 前回がパスかどうかで場合分け
  if(update.empty()){
    MovablePos[Turns].clear();
    rep1(x, BOARD_SIZE){
      rep1(y, BOARD_SIZE){
        MovableDir[Turns][x][y] = Direction::NONE;
      }
    }
  }
  else{
    Turns--;
    // 石を元に戻す
    int S = update.size();
    rep(i, S){
      RawBoard[update[i].x][update[i].y] = i == 0 ? EMPTY : (Color) -CurrentColor;
    }

    // 開放度を元に戻す
    rep(k, 8) Liberty[update[0].x + dx[k]][update[0].y + dy[k]]++;

    // 石数の更新
    unsigned discdiff = S;
    Discs[CurrentColor] -= discdiff;
    Discs[(Color) -CurrentColor] += discdiff - 1;
    Discs[Color::EMPTY]--;
  }

  UpdateLog.pop_back();
  return true;
}

bool Board::isGameOver() const {
  if(Turns == MAX_TURNS) return true;
  if(MovablePos[Turns].size() != 0) return false;

  // 現在の手番と逆の色が打てるかどうか調べる
  Disc disc;
  disc.color = (Color) -CurrentColor;
  rep1(x, BOARD_SIZE){
    disc.x = x;
    rep1(y, BOARD_SIZE){
      disc.y = y;
      if(checkMobility(disc) != Direction::NONE) return false;
    }
  }
  return true;
}

unsigned Board::countDisc(Color color) const {
  return Discs[color]; 
}

vP Board::getHistory() const {
  vP history;
  for(const auto& update : UpdateLog){
    if(!update.empty()) history.emplace_back(update[0]);
  }
  return history;
}

Color Board::getColor(const Point& p) const {
  return RawBoard[p.x][p.y];
}

const vP& Board::getMovablePos() const {
  return MovablePos[Turns];
}

vD Board::getUpdate() const {
  if(UpdateLog.empty()) return vD();
  else return UpdateLog.back();
}

Color Board::getCurrentColor() const { return CurrentColor; }

unsigned Board::getTurns() const { return Turns; }

int Board::getLiberty(const Point& p) const { return Liberty[p.x][p.y]; }

void Board::flipDiscs(const Point& point){
  // 行った操作を表す石
  Disc operation(point.x, point.y, CurrentColor);

  // 打った石の周囲8マスの開放度を1ずつ減らす
  rep(k, 8) Liberty[point.x + dx[k]][point.y + dy[k]]--;

  unsigned dir = MovableDir[Turns][point.x][point.y];
  vD update;

  RawBoard[point.x][point.y] = CurrentColor;
  update.emplace_back(operation);

  rep(k, 8){
    if(dir & (1 << k)){
      int x = point.x;
      int y = point.y;
      while(RawBoard[x + dx[k]][y + dy[k]] != CurrentColor){
        x += dx[k];
        y += dy[k];
        RawBoard[x][y] = CurrentColor;
        operation.x = x;
        operation.y = y;
        update.emplace_back(operation);
      }
    }
  }
  
  // 石の数を更新
  int discdiff = update.size();
  Discs[CurrentColor] += discdiff;
  Discs[(Color) -CurrentColor] -= discdiff - 1;
  Discs[Color::EMPTY]--;
  
  UpdateLog.emplace_back(update);
}

unsigned Board::checkMobility(const Disc& disc) const {
  if(RawBoard[disc.x][disc.y] != Color::EMPTY){
    return Direction::NONE;
  }

  unsigned dir = Direction::NONE;
  
  rep(k, 8){
    if(RawBoard[disc.x + dx[k]][disc.y + dy[k]] == -disc.color){
      int x = disc.x + 2 * dx[k];
      int y = disc.y + 2 * dy[k];
      while(RawBoard[x][y] == -disc.color){
        x += dx[k];
        y += dy[k];
      }
      if(RawBoard[x][y] == disc.color) dir |= (1 << k);
    }
  }
  return dir;
}

void Board::initMovable(){
  Disc disc(0, 0, CurrentColor);

  MovablePos[Turns].clear();

  rep1(x, BOARD_SIZE){
    disc.x = x;
    rep1(y, BOARD_SIZE){
      disc.y = y;
      unsigned dir = checkMobility(disc);
      if(dir != Direction::NONE){
        MovablePos[Turns].emplace_back(disc);
      }
      MovableDir[Turns][x][y] = dir;
    }
  }
}
