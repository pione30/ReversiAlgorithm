# Makefile

PROGRAM = a.out
SRCS = \
			Play.cpp \
			AlphaBetaAI.cpp \
			Board.cpp \
			BookManager.cpp \
			CoordinatesTransformer.cpp \
			MidEvaluator.cpp

# 定義済みマクロの再定義
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

# オブジェクトファイルは.cppを.oに置換したもの
OBJS = $(SRCS:.cpp=.o)

# 依存関係ファイル
DPNS = $(SRCS:.cpp=.d)

# プライマリターゲット
$(PROGRAM): $(OBJS)
	$(CXX) -o $@ $^

# パターンルール
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# .dファイルを読み込む
-include $(DPNS)

# .cppファイルを解析して、.cppが依存しているヘッダファイルを.dファイルに書き出す
%.d: %.cpp
	$(CXX) -M $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

# make clean
clean:
	$(RM) $(PROGRAM) *.o *.d
