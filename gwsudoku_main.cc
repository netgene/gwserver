#include "sudokuservermulti.h"

#include <muduo/base/Atomic.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpServer.h>

#include <boost/bind.hpp>

#include <utility>

#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

/*
int main(int argc, char** argv)
{
  LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
  EventLoop loop;
  InetAddress listenAddr(9981);
  gwsudoku::SudokuServer server(&loop, listenAddr);

  server.start();

  loop.loop();
}
*/

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
  int numThreads = 0;
  if (argc > 1)
  {
    numThreads = atoi(argv[1]);
  }
  EventLoop loop;
  InetAddress listenAddr(9981);
  gwsudoku::SudokuServer server(&loop, listenAddr, numThreads);

  server.start();

  loop.loop();
}
