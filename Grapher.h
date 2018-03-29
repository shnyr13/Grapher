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

//Класс главного окна приложения
class Grapher : public QMainWindow
{
	Q_OBJECT

public:
	//Конструктор
	Grapher(QWidget *parent = 0, Qt::WFlags flags = 0);
	//Деструктор
	~Grapher();

private:
	//Объект класса виджетов
	Ui::GrapherClass ui;
	//Валидатор имени вершины
	ValidatorOfName val;
	//имя файла
	QString fileName;
	QString viewName;
	//строка состояния
	QStatusBar *status;
	HelpWindow *helpWindow;

	//Задание иконок кнопок
	void buttonIcon();
	//Перенапраление виджетов
	void redirectButtom();

	virtual void closeEvent(QCloseEvent *event);

private slots: 
	//Выход из программы
	void exit();
	//добавить вершину
	void APPEND_TOP();
	//добавить ребро
	void APPEND_EDGE();
	//удалить элемент
	void DELETE_ELEMENT();
	//освободить указатель
	void RESET_POINTER();
	//задать цвет кнопки
	void SET_COLOR();
	//открыть файл с графом
	void OPEN();
	//сохранить граф в текущий файл
	void SAVE();
	//сохранить граф в заданный файл
	void SAVE_AS();
	//сохраняет изображение в картику
	void ExportTopImage();
	//очистить всё поле
	void CLEAR();

	void HELP_WINDOW_CLOSE();

	//справка
	void HELP();
	//о программе
	void ABOUT_PROGRAM();
};

#endif // GRAPHER_H
