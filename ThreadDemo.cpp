#include "ThreadDemo.h"

ThreadDemo::ThreadDemo(QObject *parent, GraphTop *top, int wait, int stepNumber)
	: QThread(parent)
{
	_top = top;
	_wait = wait;
	_stepNumber = stepNumber;
	isLast = false;
}

ThreadDemo::~ThreadDemo()
{

}

void ThreadDemo::run()
{
	QWaitCondition sleep;
	QMutex mutex;
	sleep.wait(&mutex, _wait);

	_top->showStep(_stepNumber);

	if (isLast)
	{
		emit demoEnd(this);
	}
}
