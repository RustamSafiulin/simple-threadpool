// threadpool.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "threadpool.h"
#include <boost/bind.hpp>

void taskFunc1()
{
	std::cout << "It's usual taskFunc1 without args" << std::endl;
}

void taskFunc2(int i)
{
	std::cout << "Test taskFunc2: " << i << " : " << boost::this_thread::get_id() << std::endl;
}

void taskFunc3(int i)
{
	std::cout << "Test taskFunc3: " << i << " : " << boost::this_thread::get_id() << std::endl;
}

class Test {

public:
	void foo(int i) {
		std::cout << "Test::foo: " << i << std::endl;
	}

	void foo1(double i,const std::string &s) {
		std::cout << "Test::foo1: " << i << " " << s << std::endl;
	}
};

int main()
{
	ThreadPool pool;

	//����������� ���������� ����� � ������� �������
	pool.setTaskQueueMaxSize(5);

	/**��� ������� ������� ��� ����������
	   � ������ ����������� ���������� ������
	*/
	fn_type fn = boost::bind(taskFunc1);
	task_ptr task1(new Task(fn,LOW_PRIO));

	/**��� ������� ������� c 1 ����������
	   � ������ ����������� ���������� ������
	*/
	fn = boost::bind(taskFunc2,5);
	task_ptr task2(new Task(fn,LOW_PRIO));

	/**��� ������ ������ � 1 ����������
	   � ������� ����������� ���������� ������
	*/
	Test test;
	fn = boost::bind(&Test::foo,&test,2);
	task_ptr task3(new Task(fn,HIGH_PRIO));

	/**��� ������ ������ � 2 ����������� 
	   � ������� ����������� ���������� ������ (�������� �� ���������)
	*/
	fn = boost::bind(&Test::foo1,&test,1.5,"Hello world!!!");
	task_ptr task4(new Task(fn));

	pool.addTask(task1);
	pool.addTask(task2);
	pool.addTask(task3);
	pool.addTask(task4);

	boost::posix_time::milliseconds ms(1000);
	boost::this_thread::sleep(ms);

	/**�������� forceStop, setWorkThreadCount
	   � �������� ������������ ����� � ������� �������
	*/
	for (size_t i = 0; i < 150; ++i) {
		fn = boost::bind(taskFunc3,i);
		task_ptr task5(new Task(fn));
		pool.addTask(task5);

		if (i == 80) {
			pool.setWorkThreadCount(5);
		}

		if (i == 130) {
			pool.forceStop();
		}
	}

	boost::this_thread::sleep(ms);

	return 0;
}

