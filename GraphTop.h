#ifndef GRAPHTOP_H
#define GRAPHTOP_H

#include <AbstractGraphItem.h>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include "SignalEmiter.h"

#define	ADD_TOP 0
#define ADD_EDGE 1
#define DEL_ELEMENT 2
#define EDIT_GRAPH 3
#define SHORT_WAY 4

class GraphEdge;
class Viewer;

class GraphTop : public AbstractGraphItem, QObject
{
public:
	GraphTop(int form, QColor color, QString name, SignalEmiter* emiter);
	~GraphTop();

	SignalEmiter* signalEmiter;

	//Петля
	GraphEdge* autoEdge;

	//форма вершины
	int formTop;

	//указатель на центр окружности
	QPoint center;

	//имя вершины
	QString nameTop;

	QRectF boundingRect() const;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

	//отвязать вершину (удалить все рёбра, идущие от неё)
	void disconnectTop();

	//контейнер с указтелями на дуги, идущие от вершины
	QList<GraphEdge*> edges;

	void showStep(int stepNumber);
	void resetShowStepFlag();

	void setRandomPos();

	void setId(int ID);

protected:
     void mousePressEvent(QGraphicsSceneMouseEvent* e);

	 bool _showStepFlag;

	 short int id;

	 int _stepNumber;
};

#endif // GRAPHTOP_H

