#ifndef THREADDEMO_H
#define THREADDEMO_H

#include <QThread>
#include "GraphTop.h"
#include <QMutex>
#include <QWaitCondition>

class ThreadDemo : public QThread
{
	Q_OBJECT

public:
	ThreadDemo(QObject *parent, GraphTop *top, int wait, int stepNumber);
	~ThreadDemo();

	bool isLast;
	//QList<ThreadDemo*> *threads;

private:
	virtual void run();

	GraphTop *_top;
	int _wait;
	int _stepNumber;

signals:
	void demoEnd(QThread *);
};

#endif // THREADDEMO_H
