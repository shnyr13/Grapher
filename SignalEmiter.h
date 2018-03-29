#ifndef SIGNALEMITER_H
#define SIGNALEMITER_H

#include <QObject>

class GraphTop;
class GraphEdge;

//Класс сигналов изменения элемента
class SignalEmiter : public QObject
{
	Q_OBJECT

public:
	//Конструктор
	SignalEmiter();
	//Деструктор
	~SignalEmiter();

	//Метод испускания сигнала изменения вершины
	void EmitSignalSelectedTop(GraphTop* top);
	//Метод испускания сигнала изменения ребра
	void EmitSignalSelectedEdge(GraphEdge* edge);

signals: 
	//Сигнал к вершине
	void TopIsSelected(GraphTop* top);
	//Сигнал к ребру
	void EdgeIsSelected(GraphEdge* edge);	
};

#endif // SIGNALEMITER_H
