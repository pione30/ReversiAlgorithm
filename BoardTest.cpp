#include "Board.h"
#define FOR(i, a, b) for(int (i) = (a); (i) <= (b); ++(i))
#define rep(i, n) FOR(i, 0, n - 1)
#define rep1(i, n) FOR(i, 1, n)
#define rrep(i, n) for(int (i) = (n) - 1; (i) >= 0; --(i))
#define all(a) (a).begin(),(a).end()

class ConsoleBoard : public Board {
public:
  void print(){
    cout << "  |a |b |c |d |e |f |g |h |  " << endl;
    cout << "-----------------------------" << endl;
    rep1(y, 8){
      cout << " " << y << "|";
      rep1(x, 8){
        switch (getColor(Point(x, y))) {
          case Color::BLACK:
              cout << "● ";
              break;
          case Color::WHITE:
              cout << "○ ";
              break;
          default:
              cout << "  ";
              break;
        }
        cout << "|";
      }
      cout << y << endl;
      cout << "-----------------------------" << endl;
    }
    cout << "  |a |b |c |d |e |f |g |h |  " << endl << endl;
  }
};

int main(){
  ConsoleBoard board;

  while(true){
    board.print();
    cout << "● 黒石" << board.countDisc(Color::BLACK) << " ";
    cout << "○ 白石" << board.countDisc(Color::WHITE) << " ";
    cout << "空マス" << board.countDisc(Color::EMPTY) << endl << endl;
    
    if(board.isGameOver()){
      cout << "----------------ゲーム終了---------------" << endl;

      if(board.countDisc(Color::BLACK) > board.countDisc(Color::WHITE)){
        cout << "● 黒の勝ちです！" << endl;
      }
      else if(board.countDisc(Color::BLACK) < board.countDisc(Color::WHITE)){
        cout << "○ 白の勝ちです！" << endl;
      }
      else cout << "引き分けです！" << endl;
      
      return 0;
    }

    cout << (board.getCurrentColor() == Color::BLACK ? "● 黒の番です。" : "○ 白の番です。") << endl;
    cout << "手を入力して下さい: ";

    string in;
    cin >> in;

    if(in == "p"){
      // パス
      if(!board.pass()) cerr << "パスできません！" << endl;
      continue;
    }
    if(in == "u"){
      // undo
      board.undo();
      continue;
    }
    
    Point p;
    try{
      p = Point(in);
    }
    catch(invalid_argument e){
      cerr << "リバーシ形式の手を入力して下さい！" << endl;
      continue;
    }

    if(board.move(p) == false){
      cerr << "そこには置けません！" << endl;
    }
  }

  return 0;
}
