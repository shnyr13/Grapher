#ifndef ABSTRACTGRAPHITEM_H
#define ABSTRACTGRAPHITEM_H

#include <QGraphicsItem>

class AbstractGraphItem : public QGraphicsItem
{
public:
	AbstractGraphItem();
	~AbstractGraphItem();

	virtual QRectF boundingRect() const = 0;

	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) = 0;
	
	QColor _color;
private:
	
};

#endif // ABSTRACTGRAPHITEM_H
