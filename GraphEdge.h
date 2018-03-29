#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>
#include <QFocusEvent>
#include "GraphTop.h"
#include "math.h"

//����� ����� �����
class GraphEdge : public AbstractGraphItem
{
public:
	//�����������
	GraphEdge(int form, QColor color, int weight1, int weight2, SignalEmiter* emiter);
	//����������
	~GraphEdge();
	//������� ������� �����
	QRectF boundingRect() const;
	//����� ��������� �����
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	//���������� ������� ��������� ����� � ������� ����� �����
	bool IsClickedAreaEdge(QPoint clickPoint);

	//��������� ������ �������� ��������� ����� ���������
	SignalEmiter* signalEmiter;
	//���� ��������������� �����
	bool reversEdge;
	//����� �����
	int formEdge;
	//��� �����
	int weightEdge1;
	int weightEdge2;
	//����� �����
	int lengthEdge;
	//��������� �� ��������� � �������� ����� �����
	QPoint* start, *finish;
	//�������, ������� ��������� �����
	GraphTop* topA, *topB;

private:
	//�������� ������� �����
	QPoint k1;
	QPoint k2;
};

#endif // GRAPHEDGE_H