//ThreadPool.h
#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
	ThreadPool(size_t threadCcount);
	~ThreadPool();

	void enqueue(std::function<void()> task);
	
private:
	void workerThread();

	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;
	std::mutex mtx;
	std::condition_variable cv;
	std::atomic<bool> stop;
};

#endif //_THREADPOOL_H