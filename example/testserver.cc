#include "mymuduo/TcpServer.h"
#include "mymuduo/Logger.h"
#include <string>
#include <functional>

class EchoServer
{
public:
    EchoServer(EventLoop *loop,
            const InetAddress &listenAddr,
            const std::string &nameArg) : server(loop,listenAddr,nameArg),loop_(loop)
    {
        server.setConnectionCallback(std::bind(&EchoServer::OnConnection,this,std::placeholders::_1));
        server.setMessageCallback(std::bind(&EchoServer::OnMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        //设置合适的loop线程数量
        server.setThreadNum(3);

    };

    void start()
    {
        server.start();
    }

    

private:
    void OnConnection(const TcpConnectionPtr& conn) 
    {
        if(conn->connected())
        {
            LOG_INFO("Connection up : %s",conn->peerAddress().toIpPort().c_str());
        }
        else
        {
            LOG_INFO("Connection down : %s",conn->peerAddress().toIpPort().c_str());
        }
    }
    void OnMessage(const TcpConnectionPtr& conn,Buffer* buf,Timestamp time)
    {
        std::string msg = buf->retrieveAllAsString();
        conn->send(msg);
        conn->shutdown();
    }

    EventLoop* loop_;
    TcpServer server;

};

int main()
{
    InetAddress addr(8000);
    EventLoop loop;
    EchoServer echoserver(&loop,addr,"echoserver");
    echoserver.start();
    loop.loop();
    return 0;
}
