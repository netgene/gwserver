#include "sudoku.h"
#include "db.h"
#include "json/json.h"

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
#include <dlfcn.h>

using namespace muduo;
using namespace muduo::net;

namespace gwsudoku {

class SudokuServer
{
 public:
  Db db;

 public:
  SudokuServer(EventLoop* loop, const InetAddress& listenAddr, int numThreads)
    : server_(loop, listenAddr, "SudokuServer"),
      numThreads_(numThreads),
      startTime_(Timestamp::now())
  {
    server_.setConnectionCallback(
        boost::bind(&SudokuServer::onConnection, this, _1));
    server_.setMessageCallback(
        boost::bind(&SudokuServer::onMessage, this, _1, _2, _3));
    server_.setThreadNum(numThreads);
  }

  void start()
  {
    LOG_INFO << "starting " << numThreads_ << " threads.";
    server_.start();

    db.dbQuery();
  }

 private:
  void onConnection(const TcpConnectionPtr& conn)
  {
    LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
        << conn->localAddress().toIpPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp)
  {
    LOG_DEBUG << conn->name();
    size_t len = buf->readableBytes();
    while (len >= kCells + 2)
    {
      const char* crlf = buf->findCRLF();
      if (crlf)
      {
        string request(buf->peek(), crlf);
        buf->retrieveUntil(crlf + 2);
        len = buf->readableBytes();
        if (!processRequest(conn, request))
        {
          conn->send("Bad Request!\r\n");
          conn->shutdown();
          break;
        }
      }
      else if (len > 100) // id + ":" + kCells + "\r\n"
      {
        conn->send("Id too long!\r\n");
        conn->shutdown();
        break;
      }
      else
      {
        break;
      }
    }
  }

  bool processRequest(const TcpConnectionPtr& conn, const string& request)
  {
    string id;
    string puzzle;
    bool goodRequest = true;

    string::const_iterator colon = find(request.begin(), request.end(), ':');
    if (colon != request.end())
    {
      id.assign(request.begin(), colon);
      puzzle.assign(colon+1, request.end());
    }
    else
    {
      puzzle = request;
    }

    if (puzzle.size() == implicit_cast<size_t>(kCells))
    {
      LOG_DEBUG << conn->name();
      string result = solveSudoku(puzzle);
      Json::Value jsonResult;
      jsonResult["sudoku"] = result.c_str();
      std::string newresult = jsonResult.toStyledString();
      if (id.empty())
      {
        conn->send(newresult.c_str());
      }
      else
      {
        //conn->send(id+":"+newresult.c_str()+"\r\n");
      }
    }
    else
    {
      goodRequest = false;
    }
    return goodRequest;
  }

  TcpServer server_;
  int numThreads_;
  Timestamp startTime_;
};

} // end gwsudoku
