#pragma once
#include "Board.h"

class AI{
public:
  AI() : presearch_depth(3), normal_depth(5), wld_depth(15), perfect_depth(13) {}

  unsigned presearch_depth; // 事前に手を調べて探索順序を決めるための先読み手数
  unsigned normal_depth;    // 序盤・中盤の探索における先読み手数
  unsigned wld_depth;       // 終盤において、必勝読みを始める残り手数
  unsigned perfect_depth;   // 終盤において、完全読みを始める残り手数

  virtual void move(Board&) = 0;
};
