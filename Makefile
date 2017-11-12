# Makefile

SRCDIR = ./src
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJDIR = ./obj
TARGETDIR = ./bin
PROGRAM = $(addprefix $(TARGETDIR)/, a.out)

# 定義済みマクロの再定義
CXX = g++
CXXFLAGS = -std=c++14 -O2 -Wall
CPPFLAGS = -I ./include

# オブジェクトファイルは.cppを.oに置換したもの
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))

# 依存関係ファイル
DPNS = $(OBJS:.o=.d)

# プライマリターゲット
$(PROGRAM): $(OBJS)
	@mkdir -p $(TARGETDIR)
	$(CXX) $(CPPFLAGS) -o $@ $^

# パターンルール
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

# .dファイルを読み込む
-include $(DPNS)

# .cppファイルを解析して、.cppが依存しているヘッダファイルを.dファイルに書き出す
$(OBJDIR)/%.d: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CPPFLAGS) -M $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

# make run
.PHONY: run
run:
	$(PROGRAM)

# make clean
.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS) $(DPNS)
