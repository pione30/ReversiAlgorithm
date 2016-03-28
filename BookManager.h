#pragma once
#include "Board.h"

class BookManager{
public:
  BookManager();
  vP find(const Board& board) const;

private:
  struct Node{
    Node() : child(nullptr), sibling(nullptr){}

    shared_ptr<Node> child;
    shared_ptr<Node> sibling;
    Point point;
  };

  shared_ptr<Node> Root; // 定石木の根

  void add(const vP& book);
  Point getNextMove(const shared_ptr<Node>& node) const;
};
