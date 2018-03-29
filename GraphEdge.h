#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>
#include <QFocusEvent>
#include "GraphTop.h"
#include "math.h"

//Класс ребра графа
class GraphEdge : public AbstractGraphItem
{
public:
	//Конструктор
	GraphEdge(int form, QColor color, int weight1, int weight2, SignalEmiter* emiter);
	//Деструктор
	~GraphEdge();
	//Задание области ребра
	QRectF boundingRect() const;
	//Метод рисования ребра
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	//возвращает признак попадания точки в область клика ребра
	bool IsClickedAreaEdge(QPoint clickPoint);

	//Экземпляр класса сигналов изменения формы элементов
	SignalEmiter* signalEmiter;
	//Флаг перенаправления ребра
	bool reversEdge;
	//форма ребра
	int formEdge;
	//вес ребра
	int weightEdge1;
	int weightEdge2;
	//длина ребра
	int lengthEdge;
	//Указатели на начальную и конечную точку ребра
	QPoint* start, *finish;
	//Вершины, которые соединяет ребро
	GraphTop* topA, *topB;

private:
	//Реальные границы ребра
	QPoint k1;
	QPoint k2;
};

#endif // GRAPHEDGE_H