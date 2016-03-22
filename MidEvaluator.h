#pragma once
#include "Evaluator.h"

class MidEvaluator : public Evaluator {
public:
  MidEvaluator();
  int evaluate(const Board& board) override;

private:
  struct EdgeParam{
    EdgeParam& operator+= (const EdgeParam& src){
      stable += src.stable;
      wing += src.wing;
      mountain += src.mountain;
      Cmove += src.Cmove;

      return *this;
    }

    short stable;   // 確定石の個数
    short wing;     // ウイングの個数
    short mountain; // 山の個数
    short Cmove;    // 危険なC打ちの個数
  };

  struct CornerParam{
    short corner; // 隅にある石の数
    short Xmove;  // 危険なXうちの個数
  };

  // 重みを規定する構造体
  struct Weight{
    int mobility_w;
    int liberty_w;
    int stable_w;
    int wing_w;
    int Xmove_w;
    int Cmove_w;
  } EvalWeight;

  using EdgeStat = ColorStorage<EdgeParam>;
  using CornerStat = ColorStorage<CornerParam>;

  static constexpr unsigned TABLE_SIZE = 6561; // 3^8
  static EdgeStat EdgeTable[TABLE_SIZE];
  static bool TableInit;

  void generateEdge(vC& edge, unsigned index);
  EdgeParam evalEdge(vC& edge, Color color) const;
  CornerStat evalCorner(const Board&) const;

  static constexpr array<unsigned, 8> idx3digit = 
    { 2187, 729, 243, 81, 27, 9, 3, 1 };
  inline unsigned idxTop(const Board&) const;
  inline unsigned idxBottom(const Board&) const;
  inline unsigned idxRight(const Board&) const;
  inline unsigned idxLeft(const Board&) const;
  unsigned idxLine(const vC& l);

  ColorStorage<unsigned> countLiberty(const Board& board) const;

};
