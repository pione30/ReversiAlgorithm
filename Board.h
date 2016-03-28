#pragma once
#include "Reversi.h"

class Board{
public:
  Board();

  void init();                           // ボードをゲーム開始直後の状態にする
  bool move(const Point& point);         // pointで指定された位置に石を打つ
  bool pass();                           // パスする
  bool undo();                           // 1つ前の手の状態に戻す
  bool isGameOver() const;               // ゲームが終了しているかどうかを判定する

  unsigned countDisc(Color color) const; // 指定された石の個数を返す
  vP getHistory() const;                 // これまでに打たれてきた手を並べたvectorを返す
  Color getColor(const Point& p) const;  // 指定した座標の石の色を調べる
  const vP& getMovablePos() const;       // 石を打てる座標が並んだvectorを返す
  vD getUpdate() const;                  // 直前の手で返された石が並んだvectorを返す
  Color getCurrentColor() const;         // 現在の手番の色を返す
  unsigned getTurns() const;             // 現在の手数を返す
  int getLiberty(const Point& p) const;  // 指定した座標の開放度を得る

private:
  enum Direction : unsigned {
    NONE        =      0,
    UPPER       = 1 << 0,
    UPPER_LEFT  = 1 << 1,
    LEFT        = 1 << 2,
    LOWER_LEFT  = 1 << 3,
    LOWER       = 1 << 4,
    LOWER_RIGHT = 1 << 5,
    RIGHT       = 1 << 6,
    UPPER_RIGHT = 1 << 7
  };

  constexpr static array<int, 8> dx = {0, -1, -1, -1, 0, 1, 1, 1};
  constexpr static array<int, 8> dy = {-1, -1, 0, 1, 1, 1, 0, -1};

  unsigned Turns;
  Color CurrentColor;
  ColorStorage<unsigned> Discs;
  vvC RawBoard;

  // 各手番において変更点を格納するvD updateを構築する。
  // updateの先頭には新しく石を置いた座標を格納し、それ以降には裏返した石の座標を格納していく。
  // このupdateを手番順にさらに格納するvvD をUpdateLogとする。
  vvD UpdateLog;

  vP MovablePos[MAX_TURNS + 1];
  vvvu MovableDir;

  vvi Liberty;

  // pointで指定された位置に石を打ち、挟み込めるすべての石を裏返す。
  // 「打った石」と「裏返した石」をUpdateLogに挿入する。
  // Libertyを更新する。
  void flipDiscs(const Point& point);

  // diskで指定された座標にdisc.colorの色の石を打てるかどうか、また
  // どの方向に石を裏返せるかを判定する。
  // 石を裏返せる方向にフラグが立った整数値が返る。
  unsigned checkMobility(const Disc& disc) const; 

  // MovablePos[Turns]とMovableDir[Turns]を再計算する。
  void initMovable();

};
