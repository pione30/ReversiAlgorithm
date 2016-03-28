#include "Board.h"
#include "AI.h"
#include "AlphaBetaAI.h"

class Player{
public:
  virtual void onTurn(Board& board) = 0;
};

class UndoException{};
class ExitException{};
class GameOverException{};

class HumanPlayer : public Player {
public:
  void onTurn(Board& board) override {
    if(board.getMovablePos().empty()){
      // パス
      cout << "あなたはパスです。" << endl;
      board.pass();
      return;
    }

    while(true){
      cout << "手を\"f5\"のように入力、もしくは(U:取消/X:終了)を入力してください:" << endl;
      string in;
      cin >> in;
      if(in == "U" || in == "u") throw UndoException();
      if(in == "X" || in == "x") throw ExitException();

      Point p;
      try{ p = Point(in); }
      catch(invalid_argument& e){
        cout << "正しい形式で入力して下さい！" << endl;
        continue;
      }
      
      if(!board.move(p)){
        cout << "そこには置けません！" << endl;
        continue;
      }

      if(board.isGameOver()) throw GameOverException();

      break;
    }
  }
};

class AIPlayer : public Player {
  unique_ptr<AI> Ai;

public:
  AIPlayer() : Ai(nullptr){
    Ai.reset(new AlphaBetaAI());
  }

  void onTurn(Board& board) override {
    cout << "コンピュータが思考中..." << endl;
    Ai->move(board);
    cout << "完了" << endl;
    if(board.isGameOver()) throw GameOverException();
  }
};

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

    cout << "● 黒石" << countDisc(Color::BLACK) << " ";
    cout << "○ 白石" << countDisc(Color::WHITE) << " ";
    cout << "空マス" << countDisc(Color::EMPTY) << endl << endl;
  }
};


int main(int argc, char* argv[]){
  unique_ptr<Player> player[2];
  int current_player = 0;
  ConsoleBoard board;
  bool reverse = false;

  if(argc > 1){
    // コマンドラインプション-rが与えられるとコンピュータ先手にする
    if(strcmp(argv[1], "-r") == 0) reverse = true;
  }

  if(!reverse){
    player[0].reset(new HumanPlayer());
    player[1].reset(new AIPlayer());
  }
  else {
    player[1].reset(new HumanPlayer());
    player[0].reset(new AIPlayer());
  }

  while(true){
    board.print();

    cout << (board.getCurrentColor() == Color::BLACK ? "● 黒の番です。" : "○ 白の番です。") << endl;

    try{
      player[current_player]->onTurn(board);
    }
    catch(UndoException& e){
      do{
        board.undo(); board.undo();
      } while(board.getMovablePos().empty());
      continue;
    }
    catch(ExitException& e){
      return 0;
    }
    catch(GameOverException& e){
      board.print();
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
  
    // プレイヤーの交代
    current_player = ++current_player % 2;
  }
}
