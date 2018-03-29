#include "GraphEdge.h"
#include <QMessageBox>

GraphEdge::GraphEdge(int form, QColor color, int weight1,int weight2, SignalEmiter * emiter)
{
	reversEdge = false;
	setBoundingRegionGranularity(1.0);
	weightEdge1 = weight1;
	weightEdge2 = weight2;
	signalEmiter = emiter;
	formEdge = form;
	_color = color;
	start = NULL;
	finish = NULL;

	lengthEdge = 0;
}

GraphEdge::~GraphEdge()
{

}

QRectF GraphEdge::boundingRect() const
{
	if(start != NULL && finish != NULL)
	{
		return QRectF(*start, *finish);
	}
	return QRectF(0,0,0,0);
}

void GraphEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(topA != NULL && topB != NULL)
	{		
		painter->setRenderHint(QPainter::Antialiasing,true);

		QPoint p1 = topA->scenePos().toPoint();
		QPoint p2 = topB->scenePos().toPoint();

		if (topA->formTop > 0)	
		{
			p1.setX(p1.x()+25);
			p1.setY(p1.y()+25);	
		}

		if (topB->formTop > 0)
		{
			p2.setX(p2.x()+25);
			p2.setY(p2.y()+25);
		}
		*(this->start) = p1;
		*(this->finish) = p2;

		if (p1 == p2)
		{
			_color = topA->_color;
			topA->autoEdge = this;
		}

		QPen pen;
		QBrush brush;

		if (!isSelected())
			setFlags(0);
		
		pen = QPen(_color,2);
		brush = QBrush(_color);

		if (isSelected())
		{
			pen.setStyle(Qt::PenStyle::DotLine);
			signalEmiter->EmitSignalSelectedEdge(this);
		}

		painter->setPen(pen);

		if (p1 != p2)
		{
			QLine line(p1,p2);
			painter->drawLine(line);	

			//расчЄт длины отрезка и угла отклонени€
			const double pi = 3.141592654;
			int width = abs(p1.x() - p2.x());
			int height = abs(p1.y() - p2.y());
			float tg_alpha = 0.0;
			if (width != 0)
			{
				tg_alpha = (float)height/(float)width;
			}
			float alpha = atan(tg_alpha);

			float lengthF = sqrt(pow((float)height,2) + pow((float)width,2));
			int length = (int)lengthF;
			lengthEdge = length;

			const double Pi = 3.141592654;

			int w = (length-25)*width/length;
			int h = (length-25)*height/length;

			//точки дл€ стрелки
			QPoint A1;
			QPoint A2;

			QPoint B1;
			QPoint B2;

			QPoint C1;
			QPoint C2;

			QPoint T0;

			int _l = 25;	//длина стрелки
			int _w = 5;		//половина ширины стрелки
			int _lx = 0;	//проекци€ длины стрелки на ось ’
			int _ly = 0;	//проекци€ длины стрелки на ось Y
			int _wx = 0;	//проекци€ половины ширины стрелки на ось ’
			int _wy = 0;	//проекци€ половины ширины стрелки на ось Y

			_lx = (float)_l*cos(alpha);
			_ly = (float)_l*sin(alpha);

			_wx = (float)_w*sin(alpha);
			_wy = (float)_w*cos(alpha);

			if (p1.x() < p2.x() && p1.y() < p2.y())
			{
				k2 = QPoint(p1.x()+w,p1.y()+h);
				k1 = QPoint(p2.x()-w,p2.y()-h);

				A2 = QPoint(k2.x()-_lx,k2.y()-_ly);
				A1 = QPoint(k1.x()+_lx,k1.y()+_ly);

				B2 = QPoint(A2.x()-_wx, A2.y()+_wy);
				B1 = QPoint(A1.x()-_wx, A1.y()+_wy);

				C2 = QPoint(A2.x()+_wx, A2.y()-_wy);
				C1 = QPoint(A1.x()+_wx, A1.y()-_wy);

				T0 = QPoint(p1.x()+w/2,p1.y()+h/2);
				T0 = QPoint(T0.x()+10, T0.y()-2);
				painter->drawText(T0,QString::number(weightEdge1));
				if (formEdge == 2)
				{
					T0 = QPoint(T0.x()-20, T0.y()+20);
					painter->drawText(T0,QString::number(weightEdge2));
				}
			}
			else if (p1.x() > p2.x() && p1.y() > p2.y())
			{
				k2 = QPoint(p1.x()-w,p1.y()-h);
				k1 = QPoint(p2.x()+w,p2.y()+h);

				A2 = QPoint(k2.x()+_lx,k2.y()+_ly);
				A1 = QPoint(k1.x()-_lx,k1.y()-_ly);

				B2 = QPoint(A2.x()-_wx, A2.y()+_wy);
				B1 = QPoint(A1.x()-_wx, A1.y()+_wy);

				C2 = QPoint(A2.x()+_wx, A2.y()-_wy);
				C1 = QPoint(A1.x()+_wx, A1.y()-_wy);

				if (formEdge == 2)
				{
					T0 = QPoint(p2.x()+w/2,p2.y()+h/2);
					T0 = QPoint(T0.x()+10, T0.y()-2);
					painter->drawText(T0,QString::number(weightEdge2));
					T0 = QPoint(T0.x()-20, T0.y()+20);
					painter->drawText(T0,QString::number(weightEdge1));
				}
				else 
				{
					T0 = QPoint(p2.x()+w/2,p2.y()+h/2);
					T0 = QPoint(T0.x()+10, T0.y()-2);
					painter->drawText(T0,QString::number(weightEdge1));
				}
			}
			else if (p1.x() > p2.x() && p1.y() < p2.y())
			{
				k2 = QPoint(p1.x()-w,p1.y()+h);
				k1 = QPoint(p2.x()+w,p2.y()-h);

				A2 = QPoint(k2.x()+_lx,k2.y()-_ly);
				A1 = QPoint(k1.x()-_lx,k1.y()+_ly);

				B2 = QPoint(A2.x()-_wx, A2.y()-_wy);
				B1 = QPoint(A1.x()-_wx, A1.y()-_wy);

				C2 = QPoint(A2.x()+_wx, A2.y()+_wy);
				C1 = QPoint(A1.x()+_wx, A1.y()+_wy);

				if (formEdge == 2)
				{
					T0 = QPoint(p2.x()+w/2,p2.y()-h/2);
					T0 = QPoint(T0.x()-10, T0.y()-2);
					painter->drawText(T0,QString::number(weightEdge2));
					T0 = QPoint(T0.x()+20, T0.y()+20);
					painter->drawText(T0,QString::number(weightEdge1));
				}
				else 
				{
					T0 = QPoint(p2.x()+w/2,p2.y()-h/2);
					T0 = QPoint(T0.x()-10, T0.y()-2);
					painter->drawText(T0,QString::number(weightEdge1));
				}
			}
			else if (p1.x() < p2.x() && p1.y() > p2.y())
			{
				k2 = QPoint(p1.x()+w,p1.y()-h);
				k1 = QPoint(p2.x()-w,p2.y()+h);

				A2 = QPoint(k2.x()-_lx,k2.y()+_ly);
				A1 = QPoint(k1.x()+_lx,k1.y()-_ly);

				B2 = QPoint(A2.x()-_wx, A2.y()-_wy);
				B1 = QPoint(A1.x()-_wx, A1.y()-_wy);

				C2 = QPoint(A2.x()+_wx, A2.y()+_wy);
				C1 = QPoint(A1.x()+_wx, A1.y()+_wy);

				T0 = QPoint(p1.x()+w/2,p1.y()-h/2);
				T0 = QPoint(T0.x()-10, T0.y()-2);
				painter->drawText(T0,QString::number(weightEdge1));
				if (formEdge == 2)
				{
					T0 = QPoint(T0.x()+20, T0.y()+20);
					painter->drawText(T0,QString::number(weightEdge2));
				}
			}
			else if (p1.x() == p2.x())
			{
				if (p1.y() < p2.y())
				{
					k1 = QPoint(p1.x(), p1.y()+25);
					k2 = QPoint(p2.x(), p2.y()-25);
					A1 = QPoint(k1.x(), k1.y()+_l);
					A2 = QPoint(k2.x(), k2.y()-_l);
				}
				if (p1.y() > p2.y())	
				{
					k1 = QPoint(p1.x(), p1.y()-25);
					k2 = QPoint(p2.x(), p2.y()+25);
					A1 = QPoint(k1.x(), k1.y()-_l);
					A2 = QPoint(k2.x(), k2.y()+_l);
				}
				B1 = QPoint(A1.x()-_w, A1.y());
				C1 = QPoint(A1.x()+_w, A1.y());

				B2 = QPoint(A2.x()-_w, A2.y());
				C2 = QPoint(A2.x()+_w, A2.y());		
			}
			else if (p1.y() == p2.y())
			{
				if (p1.x() < p2.x())
				{
					k1 = QPoint(p1.x()+25, p1.y());
					k2 = QPoint(p2.x()-25, p2.y());
					A1 = QPoint(k1.x()+_l, k1.y());
					A2 = QPoint(k2.x()-_l, k2.y());
				}
				if (p1.x() > p2.x())	
				{
					k1 = QPoint(p1.x()-25, p1.y());
					k2 = QPoint(p2.x()+25, p2.y());
					A1 = QPoint(k1.x()-_l, k1.y());
					A2 = QPoint(k2.x()+_l, k2.y());
				}
				B1 = QPoint(A1.x(), A1.y()-_w);
				C1 = QPoint(A1.x(), A1.y()+_w);

				B2 = QPoint(A2.x(), A2.y()-_w);
				C2 = QPoint(A2.x(), A2.y()+_w);		
			
			}
			painter->setBrush(brush);

			if (!reversEdge)
			{
				if (formEdge == 1 || formEdge == 2)		
					painter->drawPolygon(QPolygon() << k2 << B2 << C2);
				if (formEdge == 2)			
					painter->drawPolygon(QPolygon() << k1 << B1 << C1);
			}
			else 
			{
				if (formEdge == 1 || formEdge == 2)	
					painter->drawPolygon(QPolygon() << k1 << B1 << C1);		
				if (formEdge == 2)		
					painter->drawPolygon(QPolygon() << k2 << B2 << C2);	
			}
		}
		else
		{
			painter->drawEllipse(p1.x(),p1.y(),70,50);
			QPoint k = QPoint (p1.x()+25,p2.y());
			QPoint A = QPoint (k.x()+25,k.y()+5);
			QPoint B = QPoint (k.x()+25,k.y()-5);
			painter->setBrush(brush);
			painter->drawPolygon(QPolygon() << k << A << B);
		}
	}	
}

//возвращает признак попадани€ точки в область клика ребра
bool GraphEdge::IsClickedAreaEdge(QPoint clickPoint)
{
	QLine kLine = QLine(k1,k2);
	QRect rect = QRect(k1,k2);

	if (topA == topB)
	{
		if (topA->formTop == 0)	rect = QRect(pos().toPoint().x(),pos().toPoint().y(),50,70);
			return rect.contains(clickPoint);
	}

	QLine line1 = QLine(k1,clickPoint);
	QLine line2 = QLine(k2,clickPoint);

	QPainter painter;
	painter.setPen(QPen(Qt::red,2));

	painter.drawPoint(clickPoint);

	float kLineLength = sqrt((float)(kLine.dx()*kLine.dx() + kLine.dy()*kLine.dy()));
	float line1Length = sqrt((float) (line1.dx()*line1.dx() + line1.dy()*line1.dy()));
	float line2Length = sqrt((float) (line2.dx()*line2.dx() + line2.dy()*line2.dy()));

	bool f = abs(line1Length + line2Length - kLineLength) < 2.0;

	if (!f)	
	{
		setSelected(false);
		setFlags(0);
	}

	return f;
}