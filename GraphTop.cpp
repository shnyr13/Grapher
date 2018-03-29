#include "GraphTop.h"
#include <GraphEdge.h>
#include <QMessageBox>
#include <Viewer.h>

GraphTop::GraphTop(int form, QColor color, QString name, SignalEmiter *emiter)
{
	signalEmiter = emiter;
	nameTop = name;
	formTop = form;
	_color = color;
	//graph = view;
	autoEdge = NULL;
	setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(0.5);
	_showStepFlag = false;
}

GraphTop::~GraphTop()
{

}

QRectF GraphTop::boundingRect() const
{
	if (formTop == 0)
		return QRect(-25,-25, 50, 50);
	if (formTop == 1)
		return QRect(0,0,50,50);
	if (formTop == 2)
		return QRect(0,0,60,50);
	if (formTop == 3)
		return QRect(0,0,50,40);
}


void GraphTop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (_showStepFlag)
	{
		painter->setBrush(QBrush(Qt::red));
		painter->setPen(QPen(Qt::red));
		setSelected(true);
	}

	setToolTip(nameTop);
	painter->setRenderHint(QPainter::Antialiasing, true);
	QBrush brush;
	QPen pen;
			
	
	brush = QBrush(_color);
	pen = QPen(_color);

	if (isSelected())
	{
		brush.setStyle(Qt::BrushStyle::DiagCrossPattern);
		signalEmiter->EmitSignalSelectedTop(this);
	}

	painter->setPen(pen);
	painter->setBrush(brush);

	if (formTop == 0)
	{
		painter->setBrush(QBrush(Qt::white));
		painter->drawEllipse(-25,-25,50,50);
		painter->setBrush(brush);
		painter->drawEllipse(-25,-25,50,50);
	}
	else if (formTop == 1)
	{
		painter->setBrush(QBrush(Qt::white));
		painter->drawRect(0,0,50,50);
		painter->setBrush(brush);
		painter->drawRect(0,0,50,50);
	}
	else if (formTop == 2)
	{
		QPolygon polygon;
		polygon << QPoint(30,0) << QPoint(60,25) << QPoint (30,50)  << QPoint(0,25);
		painter->setBrush(QBrush(Qt::white));
		painter->drawPolygon(polygon);
		painter->setBrush(brush);
		painter->drawPolygon(polygon);
	}
	else if (formTop == 3)
	{
		QPolygon polygon;
		polygon << QPoint(25,0) << QPoint(0,40) << QPoint (50,40);
		painter->setBrush(QBrush(Qt::white));
		painter->drawPolygon(polygon);
		painter->setBrush(brush);
		painter->drawPolygon(polygon);
	}
	if (!isSelected())	
		painter->setPen(QPen(Qt::white));
	else	
	{
		if (_color != QColor("#000000"))	
			painter->setPen(QPen(Qt::black,2));
		else 
			painter->setPen(QPen(Qt::red,2));
	}
	if (_showStepFlag)
	{
		if (formTop == 0)
		{
			painter->drawText(QPoint(-25,-18), QString::number(_stepNumber));
		}
	}
	if (formTop == 0)
	{
		painter->drawText(QPoint(-10,0), QString::number(id));
	}
	else 
	{
		painter->drawText(QPoint(15,25), QString::number(id));
	}
}

void GraphTop::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void GraphTop::showStep(int stepNumber)
{
	_stepNumber = stepNumber;
	_showStepFlag = true;
	update();
}

void GraphTop::resetShowStepFlag()
{
	_showStepFlag = false;
	setSelected(false);
	update();
}

void GraphTop::setRandomPos()
{
	int x = rand() % 75;
	x *= 10;
	int y = rand() % 50;
	y *= 10;
	setPos(QPointF(x,y));
}

void GraphTop::setId(int ID)
{
	id = ID;
}