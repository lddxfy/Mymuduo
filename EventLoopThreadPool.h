#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "noncopyable.h"
#include "EventLoopThread.h"
class EventLoop;

class EventLoopThreadPool
{
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;
    EventLoopThreadPool(EventLoop *baseLoop,const std::string &nameArg);

    void setThreadNum(int numThreads){  numThreads_ = numThreads; };
    void start(const ThreadInitCallback &cb = ThreadInitCallback());
    // 如果工作在多线程中，baseLoop_默认以轮询的方式分配channel给subloop
    EventLoop* getNextLoop();
    std::vector<EventLoop*> getAllLoops();

    bool started() const {  return startd_; }
    const std::string name() const {return name_;}


private:
    EventLoop *baseLoop_;
    std::string name_;
    bool startd_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop *> loops_;
};