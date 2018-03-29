#pragma once
#include <QGraphicsView>
#include <QMouseEvent>
#include <QComboBox>
#include <QRadioButton>
#include "GraphTop.h"
#include "GraphEdge.h"
#include <QTimer>
#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>

#include <PushButtonColor.h>

#define	ADD_TOP 0
#define ADD_EDGE 1
#define DEL_ELEMENT 2
#define EDIT_GRAPH 3
#define DEMO_MODE 4

#include <QMultiMap>

#include "ThreadDemo.h"
#include "In_Out.h"

#include "IntegerPermutationsGenerator.h"

using namespace std;

//����� �����
class Viewer : public QGraphicsView
{
	Q_OBJECT
		friend class In_Out;
public:

	Viewer(QWidget * parent);
	~Viewer(void);

	In_Out *in_out;

	//������ ���������� �����
	QTimer *timer;
	//��������� ����� �������� ��������� ���������
	SignalEmiter *signalEmiter;
	//�����
	QGraphicsScene *scene;
	//��� �����
	QString filename;
	//��������� ������ ������ �����
	PushButtonColor* buttonColor;
	//��������� ������� ��������������� �����
	QPushButton* revers;
	//��������� ������ �������� �����
	QPushButton* deleteAutoEdge;
	//��������� ������� ��������� ����� �����
	QComboBox *comboColorEdge;
	//��������� ������� ��������� ����� �������
	QComboBox *comboFormTop;
	//��������� ������� ��������� ����� �����
	QComboBox *comboFormEdge;
	//��������� ������� ��������� ����� �������
	QLineEdit *lineNameTop;
	//��������� �������� ��������� ���� �����
	QSpinBox *weightEdge1;
	QSpinBox *weightEdge2;
	//��������� ������� ��������� �������
	QGroupBox *topParameters;
	//��������� ������� ��������� �����
	QGroupBox *edgeParameters;
	//��������� ������
	QList<GraphTop*> tops;
	//��������� �����
	QList<GraphEdge*> edges;

	//������� ����� �� �����
	virtual void mousePressEvent(QMouseEvent *e);
	
	//��������� � ����
	void save(QString filename, QString viewType);
	//��������� �� �����
	void load(QString filename, QString viewType);
	
	//�������� �����
	void ClearScene();
	//������� � ��������
	void ExpportToImage(QString &filename);
	//������ ������� �����
	void SetMode(unsigned int mode);
	//�������� ������� �����
	int getMode();

 private:

	 unsigned long INFINITE;

	//������� ��� ���������� �����
	GraphTop *topA;
	GraphTop *topB;

	//������� �����
	unsigned int currentMode;
	//������� �������
	GraphTop* currentTop;
	//������� �����
	GraphEdge* currentEdge;

	//������ ������������ ������� � ������ ��� ������ ������������ �������
	QPushButton *nextDemo, *previousDemo, *exitDemo;
	QLabel *progressDemoLabel;
	QLabel *bestWayValueDemoLabel;
	unsigned int currentBestWay;
	std::vector<std::vector<GraphTop*>> bestWays;
	QVector<unsigned long> bestWaysValue;

	QList<ThreadDemo*> threads;

	//���������� �����
	GraphEdge *AppendEdge(GraphTop *topA, GraphTop *topB);
	
	//��������� �����
	void drawGraph();

private slots:
	//����������
	void _Updater();
	//�������������� �������
	void EditTop(GraphTop* _top);
	//�������������� �����
	void EditEdge(GraphEdge* _edge);

	//��������� �������������� �������
	void PreviousDemo();
	void NextDemo();
	void ExitDemo();

	

public slots:
	void deleteThread(QThread *thread);
	//����������� ����� �������
	void ModifyFormTop(int formTop);
	//����������� ����� �������
	void ModifyColor(QColor color);
	//����������� ����� �����
	void ModifyFormEdge(int formEdge);
	//����������� ����� �������
	void ModifyNameTop(QString nameTop);
	//����������� ���� �����
	void ModifyWeightEdge1(int weightEdge1);
	void ModifyWeightEdge2(int weightEdge2);
	//��������������� �����
	void ReversEdge();
	//�������� �����
	void DeleteAutoAdge();

	//������ �����������
	void commiy();
};
//���������� � ������� �������
void delete_top(GraphTop *_top, QList<GraphTop*> &tops, QList<GraphEdge*> &edges);
//���������� � ������� �����
void delete_edge(GraphEdge * _edge, QList<GraphEdge*> &edges);