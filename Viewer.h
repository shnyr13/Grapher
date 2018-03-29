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

//Класс сцены
class Viewer : public QGraphicsView
{
	Q_OBJECT
		friend class In_Out;
public:

	Viewer(QWidget * parent);
	~Viewer(void);

	In_Out *in_out;

	//Таймер обновления сцены
	QTimer *timer;
	//Экземпляр касса сигналов изменения элементов
	SignalEmiter *signalEmiter;
	//Сцена
	QGraphicsScene *scene;
	//Имя файла
	QString filename;
	//Указатель кнопки выбора цвета
	PushButtonColor* buttonColor;
	//Указатель виджета перенаправления ребра
	QPushButton* revers;
	//Указатель кнопки удаления петли
	QPushButton* deleteAutoEdge;
	//Указатель виджета изменения цвета ребра
	QComboBox *comboColorEdge;
	//Указатель виджета изменения формы вершины
	QComboBox *comboFormTop;
	//Указатель виджета изменения формы ребра
	QComboBox *comboFormEdge;
	//Указатель виджета изменения имени вершины
	QLineEdit *lineNameTop;
	//Указатели виджетов изменения веса ребра
	QSpinBox *weightEdge1;
	QSpinBox *weightEdge2;
	//Указатель виджета редактора вершины
	QGroupBox *topParameters;
	//Указатель виджета редактора ребра
	QGroupBox *edgeParameters;
	//Контейнер вершин
	QList<GraphTop*> tops;
	//Контейнер ребер
	QList<GraphEdge*> edges;

	//Событие клика по сцене
	virtual void mousePressEvent(QMouseEvent *e);
	
	//сохранить в файл
	void save(QString filename, QString viewType);
	//загрузить из файла
	void load(QString filename, QString viewType);
	
	//Очищение сцены
	void ClearScene();
	//Экспорт в картинку
	void ExpportToImage(QString &filename);
	//Задать текущий режим
	void SetMode(unsigned int mode);
	//Получить текущий режим
	int getMode();

 private:

	 unsigned long INFINITE;

	//Вершины для добавления ребер
	GraphTop *topA;
	GraphTop *topB;

	//Текущий режим
	unsigned int currentMode;
	//Текущая вершины
	GraphTop* currentTop;
	//Текущее ребро
	GraphEdge* currentEdge;

	//кнопки переключения решений и отмены для режима демонстрации решений
	QPushButton *nextDemo, *previousDemo, *exitDemo;
	QLabel *progressDemoLabel;
	QLabel *bestWayValueDemoLabel;
	unsigned int currentBestWay;
	std::vector<std::vector<GraphTop*>> bestWays;
	QVector<unsigned long> bestWaysValue;

	QList<ThreadDemo*> threads;

	//Добавление ребра
	GraphEdge *AppendEdge(GraphTop *topA, GraphTop *topB);
	
	//Рисование графа
	void drawGraph();

private slots:
	//Обновление
	void _Updater();
	//Редактирование вершины
	void EditTop(GraphTop* _top);
	//Редактирование ребра
	void EditEdge(GraphEdge* _edge);

	//Упраление демонстарацией решения
	void PreviousDemo();
	void NextDemo();
	void ExitDemo();

	

public slots:
	void deleteThread(QThread *thread);
	//Модификация формы вершины
	void ModifyFormTop(int formTop);
	//Модификация цвета вершины
	void ModifyColor(QColor color);
	//Модификация формы ребра
	void ModifyFormEdge(int formEdge);
	//Модификация имени вершины
	void ModifyNameTop(QString nameTop);
	//Модификация веса ребра
	void ModifyWeightEdge1(int weightEdge1);
	void ModifyWeightEdge2(int weightEdge2);
	//Перенаправление ребра
	void ReversEdge();
	//Удаление петли
	void DeleteAutoAdge();

	//Задача коммивояжёра
	void commiy();
};
//отвязывает и удаляет вершину
void delete_top(GraphTop *_top, QList<GraphTop*> &tops, QList<GraphEdge*> &edges);
//отвязывает и удаляет ребро
void delete_edge(GraphEdge * _edge, QList<GraphEdge*> &edges);