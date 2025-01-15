//ThreadPool.cpp
#include "ThreadPool.h"
#include <afx.h>

ThreadPool::ThreadPool(size_t threadCount)
	:stop(false)
{
	for (size_t i = 0; i < threadCount; i++)
		workers.emplace_back([this] {workerThread(); });
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		stop = true;
	}
	cv.notify_all();
	for (std::thread& thread : workers)
	{
		if (thread.joinable())
			thread.join();
	}
}

void ThreadPool::enqueue(std::function<void()> task)
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		tasks.push(task);
	}
	cv.notify_one();
}

void ThreadPool::workerThread()
{
	while (true)
	{
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock, [this] { return stop || !tasks.empty(); });
			if (stop && tasks.empty()) return;
			
			if(!tasks.empty())
			{
				task = std::move(tasks.front());
				tasks.pop();
			}
		}
		if(task)
			task();
	}
}
