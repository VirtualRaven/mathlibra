#pragma once
#include <thread>
#include <queue>
#include <vector>
#include <condition_variable>
#include <mutex>
#include "exception"
typedef const unsigned short int _g_size;

template<typename T>
class parallel_tree
{
	

	bool _exit;
	bool _has_work;
	 short _workersN;
	std::queue<T*> tasks;
	std::vector<std::thread> _workers;
	void notify();
public:
	std::condition_variable _condition;
	std::mutex _queue_mutex;
	bool hasWork()
	{
		return _has_work;
	}
	void add_task(T* task)
	{
		_condition.notify_one();
	}
	parallel_tree():
		_workersN(std::thread::hardware_concurrency()),
		_exit(false)
	{
		for (short i = 0; i < this->_workersN; i++)
		{
			_workers.push_back(std::thread(&parallel_tree_worker<T>, this));
			_workers.back().detach();
		}

	}
	~parallel_tree()
	{
		_exit = true;
		_has_work = true;
		_condition.notify_all();

	}
	bool shouldExit()
	{
		return _exit;
	}

};

template<typename T>
void  parallel_tree_worker(parallel_tree<T>* ptr)
{
	while (!ptr->shouldExit())
	{

	
	
	try
	{	
		{
			std::unique_lock<std::mutex> lock(ptr->_queue_mutex);
			ptr->_condition.wait(lock, [&]{return ptr->hasWork(); });
				std::cout << "Getting work...\n";
			
		}
		
	}
	catch (exception& e)
	{
		std::cout << "Exception: " << e.what() << " \nDescription: " << e.desc() << "\n";
	}
	catch (...)
	{

	}
	
	}
	std::cout << "Exeting thread...\n";
	
	return;
	
};




 


