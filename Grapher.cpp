#include "Grapher.h"
#include <QMessageBox>

Grapher::Grapher(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	helpWindow = NULL;

	//Статус-бар
	status = new QStatusBar(this);

	//Иконка программы
	this->setWindowIcon(QIcon("icon.png"));

	//Перенаправление указателей виджетов
	redirectButtom();

	//Иконки кнопок
	buttonIcon();

	//Виджет второго веса не виден
	ui.weightEdge2->setVisible(false);

	//Сигнал-слот
	connect (ui.append_top, SIGNAL (clicked()), this, SLOT (APPEND_TOP()));
	connect (ui.append_edge, SIGNAL (clicked()), this, SLOT (APPEND_EDGE()));
	connect (ui.delete_element, SIGNAL (clicked()), this, SLOT (DELETE_ELEMENT()));
	connect (ui.edit_mode, SIGNAL (clicked()), this, SLOT (RESET_POINTER()));
	connect (ui.comboFormTop,SIGNAL (currentIndexChanged(int)), ui.paint_field,SLOT(ModifyFormTop(int)));
	connect (ui.set_color,SIGNAL (colorChanged(QColor)), ui.paint_field, SLOT(ModifyColor(QColor)));
	connect (ui.comboFormEdge,SIGNAL (currentIndexChanged(int)), ui.paint_field,SLOT(ModifyFormEdge(int)));
	connect (ui.lineNameTop, SIGNAL (textChanged(QString)), ui.paint_field, SLOT (ModifyNameTop(QString)));
	connect (ui.weightEdge1, SIGNAL (valueChanged(int)), ui.paint_field, SLOT (ModifyWeightEdge1(int)));
	connect (ui.weightEdge2, SIGNAL (valueChanged(int)), ui.paint_field, SLOT (ModifyWeightEdge2(int)));
	connect (ui.revers, SIGNAL (clicked()), ui.paint_field, SLOT (ReversEdge()));
	connect (ui.deleteAutoEdge, SIGNAL (clicked()),ui.paint_field, SLOT (DeleteAutoAdge()));
	connect (ui.set_color, SIGNAL (clicked()), this, SLOT (SET_COLOR()));
	//События меню-бара
	connect (ui.action_del, SIGNAL (triggered(bool)), this, SLOT(DELETE_ELEMENT()));
	connect (ui.action_delall, SIGNAL (triggered(bool)), this, SLOT(CLEAR()));
	connect (ui.action_exit, SIGNAL (triggered(bool)), this, SLOT(exit()));
	
	connect (ui.action_addtop, SIGNAL (triggered(bool)), this, SLOT (APPEND_TOP()));
	connect (ui.action_addedge, SIGNAL (triggered(bool)), this, SLOT (APPEND_EDGE()));
	connect (ui.action_modesee, SIGNAL (triggered(bool)), this, SLOT (RESET_POINTER()));

	connect (ui.action_open, SIGNAL (triggered(bool)), this, SLOT(OPEN()));
	connect (ui.action_JPG, SIGNAL (triggered(bool)), this, SLOT(ExportTopImage()));
	connect (ui.action_savaas, SIGNAL (triggered(bool)), this, SLOT(SAVE_AS()));
	connect (ui.action_save, SIGNAL (triggered(bool)), this, SLOT(SAVE()));


	connect (ui.action_commiy_2, SIGNAL (triggered(bool)), ui.paint_field, SLOT(commiy()));
	connect (ui.action_floyd, SIGNAL (triggered(bool)), ui.paint_field, SLOT(floyd()));

	connect (ui.action_help, SIGNAL (triggered(bool)), this, SLOT (HELP()));
	connect (ui.action_aboutProgram, SIGNAL (triggered(bool)), this, SLOT (ABOUT_PROGRAM()));
	
	//Начальный режим работы
	RESET_POINTER();

	//Валидатор
	ui.lineNameTop->setValidator(&val);
}

Grapher::~Grapher()
{
	
}

//Перенаправление указателей виджетов
void Grapher::redirectButtom()
{
	ui.paint_field->buttonColor = ui.set_color;
	ui.paint_field->revers = ui.revers;
	ui.paint_field->deleteAutoEdge = ui.deleteAutoEdge;
	ui.paint_field->comboFormTop = ui.comboFormTop;
	ui.paint_field->comboFormEdge = ui.comboFormEdge;
	ui.paint_field->lineNameTop = ui.lineNameTop;
	ui.paint_field->topParameters = ui.topParameters;
	ui.paint_field->edgeParameters = ui.edgeParameters;
	ui.paint_field->weightEdge1 = ui.weightEdge1;
	ui.paint_field->weightEdge2 = ui.weightEdge2;
}

//Задание иконок кнопкам
void Grapher::buttonIcon()
{
	QPixmap pixmapNode("circle.png");
	QIcon ButtonNodeIcon(pixmapNode);
	ui.append_top->setIcon(ButtonNodeIcon);
	ui.append_top->setIconSize(pixmapNode.rect().size());
	
	QPixmap pixmapEdge("line-starts-with-arrow.png");
	QIcon ButtonEdge(pixmapEdge);
	ui.append_edge->setIcon(ButtonEdge);
	ui.append_edge->setIconSize(pixmapEdge.rect().size());
	
	QPixmap pixmapPencil("pencil.png");
	QIcon ButtonPencilIcon(pixmapPencil);
	ui.delete_element->setIcon(ButtonPencilIcon);
	ui.delete_element->setIconSize(pixmapPencil.rect().size());
	
	QPixmap pixmapPointer("pointer.png");
	QIcon ButtonPointerIcon(pixmapPointer);
	ui.edit_mode->setIcon(ButtonPointerIcon);
	ui.edit_mode->setIconSize(pixmapPointer.rect().size());
		
	//Задание подсказок для кнопок
	ui.append_edge->setToolTip("Добавить ребро");
	ui.append_top->setToolTip("Добавить вершину");
	ui.delete_element->setToolTip("Удалить элемент");
	ui.edit_mode->setToolTip("Переход в режим просмотра");
}

//добавать вершину
void Grapher::APPEND_TOP()
{
	//Отмена задания флага для ребер при выполнении задачи
	for (int i = 0; i < ui.paint_field->edges.count(); i++)
	{
		ui.paint_field->edges[i]->setFlags(0);
	}

	//Задание активности виджетов
	ui.paint_field->SetMode(ADD_TOP);
	ui.append_top->setEnabled(false);
	ui.append_edge->setEnabled(true);
	ui.delete_element->setEnabled(true);
	ui.edit_mode->setEnabled(true);
	ui.comboFormEdge->setEnabled(false);
	ui.comboFormTop->setEnabled(true);
	ui.edgeParameters->setVisible(false);

	ui.topParameters->setVisible(true);
	ui.topParameters->setEnabled(true);
	ui.comboFormTop->setCurrentIndex(0);

	//строка состояния
	status->showMessage("Режим добавления вершины");
	setStatusBar(status);
}

//добавить ребро
void Grapher::APPEND_EDGE()
{
	//Отмена задания флага для ребер при выполнении задачи
	for (int i = 0; i < ui.paint_field->edges.count(); i++)
	{
		ui.paint_field->edges[i]->setFlags(0);
	}

	//Задание активности виджетов
	ui.paint_field->SetMode(ADD_EDGE);
	ui.append_edge->setEnabled(false);
	ui.append_top->setEnabled(true);
	ui.delete_element->setEnabled(true);
	ui.edit_mode->setEnabled(true);
	ui.comboFormTop->setEnabled(false);
	ui.topParameters->setVisible(false);

	ui.edgeParameters->setVisible(true);
	ui.edgeParameters->setEnabled(true);
	ui.comboFormEdge->setEnabled(true);
	ui.comboFormEdge->setCurrentIndex(0);

	//Строка состояния
	status->showMessage("Режим добавления дуги");
	setStatusBar(status);
}

//удалить элемент
void Grapher::DELETE_ELEMENT()
{
	//Отмена задания флага для ребер при выполнении задачи
	for (int i = 0; i < ui.paint_field->edges.count(); i++)
	{
		ui.paint_field->edges[i]->setFlags(0);
	}

	//Задание активности виджетов
	ui.paint_field->SetMode(DEL_ELEMENT);
	ui.delete_element->setEnabled(false);
	ui.append_top->setEnabled(true);
	ui.append_edge->setEnabled(true);
	ui.edit_mode->setEnabled(true);
	ui.comboFormEdge->setEnabled(false);
	ui.comboFormTop->setEnabled(false);
	ui.topParameters->setVisible(false);
	ui.edgeParameters->setVisible(false);
	
	//Строка состояния
	status->showMessage("Режим удаления");
	setStatusBar(status);
}


//освободить указатель
void Grapher::RESET_POINTER()
{
	//Отмена задания флага для ребер при выполнении задачи
	for (int i = 0; i < ui.paint_field->edges.count(); i++)
	{
		ui.paint_field->edges[i]->setFlags(0);
	}

	//Задание активности виджетов
	ui.paint_field->SetMode(EDIT_GRAPH);
	ui.append_top->setEnabled(true);
	ui.append_edge->setEnabled(true);
	ui.delete_element->setEnabled(true);
	ui.edit_mode->setEnabled(false);
	ui.comboFormEdge->setEnabled(true);
	ui.comboFormTop->setEnabled(true);
	ui.topParameters->setVisible(false);
	ui.topParameters->setEnabled(false);
	ui.edgeParameters->setVisible(false);
	ui.edgeParameters->setEnabled(false);

	//Строка состояния
	status->showMessage("Режим просмотра");
	setStatusBar(status);
}

void Grapher::SET_COLOR()
{
	QColor color = QColorDialog::getColor(ui.set_color->color(), this, "Выбор цвета");
	if (color.isValid())
	{
		ui.set_color->setColor(color);
	}
}

//открыть файл с графом
void Grapher::OPEN()
{
	//Открыть
	QString Filename = QFileDialog::getOpenFileName( this, QString("Открыть файл"), QString());
	if (Filename.isEmpty())	
		return;

	//выбор типа представления
	QStringList items;
	items << "Матрица смежности графа" << "Матрица весов графа" << "Структура смежности графа";
	bool pressedOK;
	QString type = QInputDialog::getItem(this, "Выбор типа представления", "Тип представления:", items, 0, false, &pressedOK);
	if (!pressedOK)	return;

	CLEAR();

	ui.paint_field->load(Filename, type);
	fileName = Filename;

	//Режим просмотра
	RESET_POINTER();
}

//сохранить граф в текущий файл
void Grapher::SAVE()
{
	//Текущий файл имеется?
	if(fileName != "")
		ui.paint_field->save(fileName, viewName);
	else
	{
		SAVE_AS();
	}
}

//сохранить граф в заданный файл
void Grapher::SAVE_AS()
{
	//выбор типа представления
	QStringList items;
	items << "Матрица смежности графа" << "Матрица весов графа" << "Структура смежности графа";
	bool pressedOK;
	QString type = QInputDialog::getItem(this, "Выбор типа прдеставления", "Тип представления:", items, 0, false, &pressedOK);
	if (!pressedOK)	return;
	
	QString Filename = QFileDialog::getSaveFileName( this, QString("Сохранить как"), QString());	
	
	if (Filename.isEmpty())	return;

	ui.paint_field->save(Filename, type);
	fileName = Filename;
}

//Экспорт в картинку
void Grapher::ExportTopImage()
{
	QString filename = QFileDialog::getSaveFileName( this, QString("Экспорт в PNG"), QString(), QString("Снимок (*.jpg)"));
	if (filename.isEmpty())
		return;
	ui.paint_field->ExpportToImage(filename);
}
//очитстить всё поле
void Grapher::CLEAR()
{
	//Очищение сцены
	ui.paint_field->ClearScene();
	//Режим просмотра
	RESET_POINTER();
}

void Grapher::exit()
{
	//Сохранить перед выходом
	//SAVE();
	//Выход из программы
	QApplication::closeAllWindows();
}

void Grapher::HELP()
{
	if (helpWindow == NULL)
	{
		helpWindow = new HelpWindow();

		connect (helpWindow, SIGNAL (HelpWindowClose()), this, SLOT (HELP_WINDOW_CLOSE()));
	}
}

void Grapher::ABOUT_PROGRAM()
{
	QMessageBox::about(this, "О программе", "Программа разработана с помощью библиотеки Qt 4.8.2\nРазработчик:\nстудент группы ПрИн-466, Рожков С.В.\nВолгоград, ВолгГТУ, 2015");
}

void Grapher::HELP_WINDOW_CLOSE()
{
	if (helpWindow != NULL)
	{
		delete helpWindow;
		helpWindow = NULL;
	}
}

void Grapher::closeEvent(QCloseEvent *event)
{
	QApplication::closeAllWindows();
	event->accept();
}