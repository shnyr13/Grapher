#ifndef PUSHBUTTONCOLOR_H
#define PUSHBUTTONCOLOR_H

#include <QPushButton>

class PushButtonColor : public QPushButton
{
	Q_OBJECT

public:
	PushButtonColor(QWidget *parent);
	~PushButtonColor();
	void setColor(QColor color);
	QColor color();

private:
	QColor _color;

signals: void colorChanged(QColor color);
	
};

#endif // PUSHBUTTONCOLOR_H
