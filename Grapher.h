#ifndef GRAPHER_H
#define GRAPHER_H

#include <QtGui/QMainWindow>
#include "ui_grapher.h"
#include <QPainter>
#include <QPixmap>
#include <QCursor>
#include <QIcon>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMouseEvent>
#include <QList>
#include "GraphTop.h"
#include "GraphEdge.h"
#include "Viewer.h"
#include <ValidatorOfName.h>
#include <QColorDialog>
#include <QInputDialog>
#include <QTextBrowser>

#include "HelpWindow.h"

#define	ADD_TOP 0
#define ADD_EDGE 1
#define DEL_ELEMENT 2
#define EDIT_GRAPH 3
#define DEMO_MODE 4

//����� �������� ���� ����������
class Grapher : public QMainWindow
{
	Q_OBJECT

public:
	//�����������
	Grapher(QWidget *parent = 0, Qt::WFlags flags = 0);
	//����������
	~Grapher();

private:
	//������ ������ ��������
	Ui::GrapherClass ui;
	//��������� ����� �������
	ValidatorOfName val;
	//��� �����
	QString fileName;
	QString viewName;
	//������ ���������
	QStatusBar *status;
	HelpWindow *helpWindow;

	//������� ������ ������
	void buttonIcon();
	//�������������� ��������
	void redirectButtom();

	virtual void closeEvent(QCloseEvent *event);

private slots: 
	//����� �� ���������
	void exit();
	//�������� �������
	void APPEND_TOP();
	//�������� �����
	void APPEND_EDGE();
	//������� �������
	void DELETE_ELEMENT();
	//���������� ���������
	void RESET_POINTER();
	//������ ���� ������
	void SET_COLOR();
	//������� ���� � ������
	void OPEN();
	//��������� ���� � ������� ����
	void SAVE();
	//��������� ���� � �������� ����
	void SAVE_AS();
	//��������� ����������� � �������
	void ExportTopImage();
	//�������� �� ����
	void CLEAR();

	void HELP_WINDOW_CLOSE();

	//�������
	void HELP();
	//� ���������
	void ABOUT_PROGRAM();
};

#endif // GRAPHER_H
