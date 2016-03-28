#include "BookManager.h"
#include "CoordinatesTransformer.h"

std::random_device rdev;     // 真の乱数生成器
std::mt19937 engine(rdev()); // ランダムなシードを設定する 

// 定石手が記載されたテキストファイル
static const string BOOK_FILE_NAME = "reversi.book";

BookManager::BookManager() : Root(nullptr) {
  Root = make_shared<Node>();
  Root->point = Point("f5");

  ifstream ifs(BOOK_FILE_NAME);
  if(ifs.fail()) return;

  string line;
  while(ifs >> line){
    vP book;
    int L = line.length();
    for(int i = 0; i < L; i += 2){
      Point p;
      try{
        p = Point(line.substr(i));
      }
      catch(invalid_argument& e){ break; }

      book.emplace_back(p);
    }
    
    add(book);
  }
}

vP BookManager::find(const Board& board) const {
  auto node = Root;
  vP history = board.getHistory();
  if(history.empty()) return board.getMovablePos();

  Point first = history[0];
  CoordinatesTransformer transformer(first);

  vP normalized;
  for(auto p : history){
    p = transformer.normalize(p);
    normalized.emplace_back(p);
  }

  // 現在までの棋譜リストと定石の対応を得る
  for(const auto& p : normalized){
    if(p == normalized[0]) continue;

    node = node->child;
    while(node != nullptr){
      if(node->point == p) break;

      node = node->sibling;
    }
    if(node == nullptr){
      // 定石を外れている
      return board.getMovablePos();
    }
  }

  // 履歴と定石の終わりが一致していた場合
  if(node->child == nullptr) return board.getMovablePos();

  Point next_move = getNextMove(node);

  // 座標を元の形に変換する
  next_move = transformer.denormalize(next_move);

  vP v(1, next_move);
  return v;
}


void BookManager::add(const vP& book){
  auto node = Root;
  for(const auto& p : book){
    if(p == book[0]) continue;

    if(node->child == nullptr){
      // 新しい定石手
      node->child = make_shared<Node>();
      node = node->child;
      node->point.x = p.x;
      node->point.y = p.y;
    }
    else {
      // 兄弟ノードの探索に移る
      node = node->child;
      while(true){
        // すでにこの手はデータベース中にあり、その枝を見つけた
        if(node->point == p) break;

        // 定石木の新しい枝
        if(node->sibling == nullptr){
          node->sibling = make_shared<Node>();
          node = node->sibling;
          node->point.x = p.x;
          node->point.y = p.y;
          break;
        }

        node = node->sibling;
      }
    }
  }
}

Point BookManager::getNextMove(const shared_ptr<Node>& node) const {
  vP candidates;

  for(auto p = node->child; p != nullptr; p = p->sibling){
    candidates.emplace_back(p->point);
  }
  
  uniform_int_distribution<int> dist(0, 100);
  int index = dist(engine) % candidates.size();
  return candidates[index];
}
