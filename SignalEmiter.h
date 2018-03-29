#ifndef SIGNALEMITER_H
#define SIGNALEMITER_H

#include <QObject>

class GraphTop;
class GraphEdge;

//����� �������� ��������� ��������
class SignalEmiter : public QObject
{
	Q_OBJECT

public:
	//�����������
	SignalEmiter();
	//����������
	~SignalEmiter();

	//����� ���������� ������� ��������� �������
	void EmitSignalSelectedTop(GraphTop* top);
	//����� ���������� ������� ��������� �����
	void EmitSignalSelectedEdge(GraphEdge* edge);

signals: 
	//������ � �������
	void TopIsSelected(GraphTop* top);
	//������ � �����
	void EdgeIsSelected(GraphEdge* edge);	
};

#endif // SIGNALEMITER_H
