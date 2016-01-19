INCLUDE_MUDUO = /media/sf_share/muduo-master/
INCLUDE_JSONCPP = /media/sf_share/jsoncpp-master/include/
INCLUDE_MYSQL = /media/sf_share/mysql/include/
LIBRARY = ./libs/
SRC = ./

CXXFLAGS = -g -O0 -Wall -Wextra -Werror -std=c++11 \
	   -Wconversion -Wno-unused-parameter \
	   -Wold-style-cast -Woverloaded-virtual \
	   -Wpointer-arith -Wshadow -Wwrite-strings \
	   -march=native -rdynamic \
	   -I$(INCLUDE_MUDUO) \
	   -I$(INCLUDE_JSONCPP) \
	   -I$(INCLUDE_MYSQL)

LDFLAGS = -L$(LIBRARY) -lmuduo_net -lmuduo_base -ljsoncpp -lmysqlcppconn-static -lpthread -lrt -ldl

all: gwsudoku jsoncpptest
clean:
	rm -rf gwsudoku sclient sserver jsoncpptest mysqltest

gwsudoku: $(SRC)/gwsudoku_main.cc $(SRC)/sudoku.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

sclient: $(SRC)/client.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

sserver: $(SRC)/server.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

jsoncpptest: $(SRC)/jsoncpptest.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

mysqltest: $(SRC)/mysqltest.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean