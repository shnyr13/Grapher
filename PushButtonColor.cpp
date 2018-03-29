#include "PushButtonColor.h"

PushButtonColor::PushButtonColor(QWidget *parent)
	: QPushButton(parent)
{
	setGeometry(pos().x(), pos().y(), 64,64);
	setColor(QColor(Qt::black));
}

PushButtonColor::~PushButtonColor()
{

}

void PushButtonColor::setColor(QColor color)
{
	_color = color;
	setIconSize(size());

	QPixmap pixmap(size());
	pixmap.fill(_color);
	setIcon(QIcon(pixmap));

	emit colorChanged(_color);
}

QColor PushButtonColor::color()
{
	return _color;
}
