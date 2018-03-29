#include "Viewer.h"
#include <QMessageBox>
#include <QTextStream>


Viewer::Viewer(QWidget * par) : QGraphicsView(par)
{	
	in_out = new In_Out(this);

	INFINITE = -1;
	//Создание сцены
	scene = new QGraphicsScene(QRectF(0,0,767,535));
	setScene(scene);

	//Задание полей
	signalEmiter = new SignalEmiter;
	topA = NULL;
	topB = NULL;
	nextDemo = NULL;
	previousDemo = NULL;
	exitDemo = NULL;
	progressDemoLabel = NULL;
	bestWayValueDemoLabel = NULL;

	//Таймер обновления
	timer = new QTimer;
	timer->setInterval(10);
	connect(timer,SIGNAL(timeout()), this, SLOT (_Updater()));
	timer->start(10);

	//Сигнал-слот
	connect (signalEmiter, SIGNAL (TopIsSelected(GraphTop*)), this, SLOT(EditTop(GraphTop*)));
	connect (signalEmiter, SIGNAL (EdgeIsSelected(GraphEdge*)), this, SLOT (EditEdge(GraphEdge*)));
}

Viewer::~Viewer(void)
{

}

//СОБЫТИЕ НАЖАТИЯ МЫШИ
void Viewer::mousePressEvent(QMouseEvent *e)
{
	QGraphicsItem *item; //указатель на выделенный элемент
	GraphTop *_top; //указатель на вершину при добавлении ребра
	int color = 0; //Стандартный цвет элемента
	int form = 0; //Стандартная форма элемента
	QString nameTop; //Имя вершины
	QString weightEdge; //Вес ребра
	QList<GraphEdge*>::iterator iEdge; //Итератор ребра

	//Действие в зависимости от режима
	switch (currentMode)
	{
	case ADD_TOP:
		//Предупреждение о превышении количества вершин
		if (tops.count() == 50)
		{
			QMessageBox::warning(0,"Warning!","Maximum count of tops is 50");
			return;
		}
		if (scene->itemAt(e->pos()) != NULL && tops.contains((GraphTop*)scene->itemAt(e->pos())))	
			return;

		//Добавление вершины
		form = comboFormTop->currentIndex();
		nameTop = lineNameTop->text();
		currentTop = new GraphTop(form, buttonColor->color(), nameTop, signalEmiter);
		currentTop->setPos(e->pos());
		currentTop->setId(tops.size());
		currentTop->setFlags(QGraphicsItem::ItemIsSelectable);
		scene->addItem(currentTop);
		scene->update();
		tops << currentTop;
		break;
	case ADD_EDGE:	
		//Проверка нажатия на 2 вершины или 2 раза на одну
		_top = (GraphTop*) scene->itemAt(e->pos());
		if(_top == NULL || !tops.contains(_top)) 
			return;
		if (topA == NULL)
			topA = _top;
		else
			topB = _top;

		//Добавление ребра
		if (topA != NULL && topB != NULL)
		{
			//проверка на наличие подобной вершины
			iEdge = edges.begin();
			while (iEdge != edges.end())
			{
				//Предупреждение о существовании ребра между вершинами
				if (((*iEdge)->topA == topA && (*iEdge)->topB == topB) || ((*iEdge)->topA == topB && (*iEdge)->topB == topA))
				{
					QMessageBox::warning(0,"Warning!","Edge already created!");
					topA = NULL;
					topB = NULL;
					return;
				}
				iEdge++;
			}
			AppendEdge(topA,topB);
			topA = NULL;
			topB = NULL;
		}			
		break;
	case DEL_ELEMENT:
		//Проверка попадания на элемент
		item = scene->itemAt(e->pos());
		if (!item)	
			return;

		//Удаление
		if (tops.contains((GraphTop*) item))
			delete_top((GraphTop*)item,tops,edges);
		else 
		{
			iEdge = edges.begin();
			while (iEdge != edges.end() && !(*iEdge)->IsClickedAreaEdge(e->pos()))
			{
				iEdge++;
			}
			if (iEdge != edges.end())	
				delete_edge(*iEdge,edges);	
		}
		scene->update();
		break;
	case EDIT_GRAPH:
		//Выделение элементов при нажатии
		item = scene->itemAt(e->pos());
		if (tops.contains((GraphTop*) item))	
			item->setSelected(true);
		else if (edges.contains((GraphEdge*) item))
		{
			iEdge = edges.begin();
			while(iEdge != edges.end())
			{
				if ((*iEdge)->IsClickedAreaEdge(e->pos()))
				{
					(*iEdge)->setFlags(QGraphicsItem::ItemIsSelectable);
					(*iEdge)->setSelected(true);
					return;
				}
				iEdge++;
			}
		}
		break;
	default:	break;
	}
	QGraphicsView::mousePressEvent(e);
}

//ЭКСПОРТ СЦЕНЫ В КАРТИНКУ
void Viewer::ExpportToImage(QString &filename)
{
	QImage image(scene->width(),scene->height(),QImage::Format_ARGB32_Premultiplied);
	image.fill("#FFFFFF"); //Цвет фона
	QPainter _painter(&image); //Картинка
	scene->render(&_painter);
	image.save(filename);
}

//ОЧИСТКА СЦЕНЫ
void Viewer::ClearScene()
{
	currentTop = NULL;
	currentEdge = NULL;
	tops.clear();
	edges.clear();
	scene->clear();
	scene->update();
}

//отвязывает и удаляет вершину
void delete_top(GraphTop *_top, QList<GraphTop*> &tops, QList<GraphEdge*> &edges)
{
	if (!_top)	
		return;

	if (_top->edges.count() == 1 && _top->edges[0]->topA == _top->edges[0]->topB)
	{
		delete	_top->edges[0];
		delete _top;
	}

	//отвязываем вершину
	QList<GraphEdge*>::iterator iter = _top->edges.begin();
	while (iter != _top->edges.end())
	{
		delete_edge(*iter,edges);
		iter = _top->edges.begin();
	}

	//удалить вершину из списка
	QList<GraphTop*>::iterator i = tops.begin();
	while (*i != _top && i != tops.end())
	{
		i++;
	}
	if (i != tops.end())	
		tops.erase(i);

	delete _top;
}

//отвязывает и удаляет ребро
void delete_edge(GraphEdge * _edge, QList<GraphEdge*> &edges)
{
	if (!_edge)	
		return;

	//отвязать ребро
	GraphTop* A = _edge->topA;
	GraphTop* B = _edge->topB;

	//отвязать ребро от первой вершины
	QList<GraphEdge*>::iterator iter = A->edges.begin();
	while (iter != A->edges.end() && *iter != _edge)
	{
		iter++;
	}
	if (iter != A->edges.end()) 
		A->edges.erase(iter);

	//отвязать ребро от второй вершины
	iter = B->edges.begin();
	while (iter != B->edges.end() && *iter != _edge)
	{
		iter++;
	}
	if (iter != B->edges.end()) 
		B->edges.erase(iter);

	QList<GraphEdge*>::iterator i = edges.begin();
	while (*i != _edge && i != edges.end())
	{
		i++;
	}

	if (i != edges.end())	
		edges.erase(i);

	delete _edge;
}

void Viewer::_Updater()
{
	if (currentMode == DEMO_MODE && scene->selectedItems().size() == tops.size())
	{
		threads.clear();
		nextDemo->setEnabled(true);
		previousDemo->setEnabled(true);
	}
	QList<QGraphicsItem*> items;
	scene->update();
	if (currentMode == EDIT_GRAPH)
	{
		items = scene->selectedItems();
		if (items.count() != 1)
		{
			topParameters->setVisible(false);
			edgeParameters->setVisible(false);
		}
	}
	
	for (int i = 0; i < tops.size(); i++)
	{
		tops[i]->setId(i);
		if (tops[i]->pos().x() < 20)
		{
			tops[i]->setPos(QPointF(20, tops[i]->pos().y()));
		}
		if (tops[i]->pos().y() < 20) tops[i]->setPos(QPointF(tops[i]->pos().x(), 20));
		if (tops[i]->pos().x() > 747) tops[i]->setPos(QPointF(747, tops[i]->pos().y()));
		if (tops[i]->pos().y() > 515) tops[i]->setPos(QPointF(tops[i]->pos().x(), 515));		
	}
}


//УСТАНОВКА РЕЖИМА РАБОТЫ
void Viewer::SetMode(unsigned int mode)
{
	if (currentMode == DEMO_MODE && !threads.isEmpty())	return;
	if (currentMode == DEMO_MODE && previousDemo != NULL && nextDemo != NULL && exitDemo != NULL && progressDemoLabel != NULL && bestWayValueDemoLabel != NULL)
	{
		delete previousDemo;
		previousDemo = NULL;
		delete nextDemo;
		nextDemo = NULL;
		delete exitDemo;
		exitDemo = NULL;
		delete progressDemoLabel;
		progressDemoLabel = NULL;
		delete bestWayValueDemoLabel;
		bestWayValueDemoLabel = NULL;

		GraphTop *top;
		foreach(top, tops)
		{
			top->resetShowStepFlag();
		}
		threads.clear();
	}

	int topsCount = tops.count();
	currentMode = mode;
	
	topA = NULL;
	topB = NULL;

	switch (mode)
	{
	case ADD_TOP:
		weightEdge1->setValue(0);
		weightEdge2->setValue(0);
		while (topsCount != 0)
		{
			tops[topsCount-1]->setFlags(QGraphicsItem::ItemIsSelectable);
			topsCount--;
		}
		deleteAutoEdge->setVisible(false);
		break;

	case ADD_EDGE:
		while (topsCount != 0)
		{
			tops[topsCount-1]->setFlags(QGraphicsItem::ItemIsSelectable);
			topsCount--;
		}
		revers->setVisible(false);
		break;

	case DEL_ELEMENT:
		while (topsCount != 0)
		{
			tops[topsCount-1]->setFlags(QGraphicsItem::ItemIsSelectable);
			topsCount--;
		}
		break;

	case EDIT_GRAPH:
		while (topsCount != 0)
		{
			tops[topsCount-1]->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
			topsCount--;
		}
		break;
	case DEMO_MODE:

		previousDemo = new QPushButton("< Назад", this);
		previousDemo->setGeometry(5, 5, 150, 30);
		previousDemo->show();

		nextDemo = new QPushButton("Далее >",this);
		nextDemo->setGeometry(160, 5, 150, 30);
		nextDemo->show();

		exitDemo = new QPushButton("Отмена", this);
		exitDemo->setGeometry(315, 5, 150, 30);
		exitDemo->show();

		progressDemoLabel = new QLabel("Прогресс", this);
		progressDemoLabel->setGeometry(470, 5, 140, 30);
		progressDemoLabel->show();

		bestWayValueDemoLabel = new QLabel("Путь",this);
		bestWayValueDemoLabel->setGeometry(615, 5, 160, 30);
		bestWayValueDemoLabel->show();

		connect(previousDemo, SIGNAL(clicked()), this, SLOT(PreviousDemo()));
		connect(nextDemo, SIGNAL(clicked()), this, SLOT(NextDemo()));
		connect(exitDemo, SIGNAL(clicked()), this, SLOT(ExitDemo()));

		scene->addWidget(previousDemo);
		scene->addWidget(nextDemo);
		scene->addWidget(exitDemo);

		scene->update();
		break;
	default: break;
	}
}

//РЕЖИМ РЕДАКТИРОВАНИЯ: редактирование вершины
void Viewer::EditTop(GraphTop* _top)
{
	if (currentMode == EDIT_GRAPH && scene->selectedItems().count() == 1)
	{
		topParameters->setVisible(true);
		topParameters->setEnabled(true);
		edgeParameters->setVisible(false);
		currentTop = _top;
		comboFormTop->setCurrentIndex(_top->formTop);
		buttonColor->setColor(_top->_color);
		lineNameTop->setText(_top->nameTop);
		deleteAutoEdge->setVisible(currentTop->autoEdge != NULL);
	}
}

//РЕЖИМ РЕДАКТИРОВАНИЯ: редактирование ребра
void Viewer::EditEdge(GraphEdge* _edge)
{
	if (currentMode == EDIT_GRAPH && scene->selectedItems().count() == 1)
	{
		edgeParameters->setVisible(true);
		edgeParameters->setEnabled(true);
		topParameters->setVisible(false);
		currentEdge = _edge;
		comboFormEdge->setCurrentIndex(_edge->formEdge);
		buttonColor->setColor(_edge->_color);
		weightEdge1->setValue(_edge->weightEdge1);
		weightEdge2->setValue(_edge->weightEdge2);
		revers->setVisible(comboFormEdge->currentIndex() == 1);
	}
}

//========СЛОТЫ ДЛЯ ИЗМЕНЕНИЯ СВОЙСТВ ЭЛЕМЕНОВ==========
void Viewer::ModifyFormTop(int formTop)
{
	if (currentMode == EDIT_GRAPH)
	{
		if ((formTop == 1 || formTop == 2 || formTop == 3) && currentTop->formTop == 0)	
			currentTop->setPos(QPoint(currentTop->pos().toPoint().x()-25,currentTop->pos().toPoint().y()-25));
		if ((currentTop->formTop == 1 || currentTop->formTop == 2 || currentTop->formTop ==3) && formTop == 0)		
			currentTop->setPos(QPoint(currentTop->pos().toPoint().x()+25,currentTop->pos().toPoint().y()+25));
		currentTop->formTop = formTop;
		currentTop->setSelected(false);
		currentTop->setSelected(true);
	}
}

void Viewer::ModifyColor(QColor color)
{
	QList<QGraphicsItem*> items;
	scene->update();
	if (currentMode == EDIT_GRAPH)
	{
		items = scene->selectedItems();
		for(int i = 0; i < items.count(); i++)
		{
			((AbstractGraphItem*)items[i])->_color = color;
			((AbstractGraphItem*)items[i])->setSelected(false);
			((AbstractGraphItem*)items[i])->setSelected(true);
		}
	}
}

void Viewer::ModifyFormEdge(int formEdge)
{
	if (formEdge == 2)	
		weightEdge2->setVisible(true);
	else 
		weightEdge2->setVisible(false);

	if (currentMode == EDIT_GRAPH)
	{
		revers->setVisible(comboFormEdge->currentIndex() == 1);
		if (currentEdge != NULL)currentEdge->formEdge = formEdge;
	}
}

void Viewer::ReversEdge()
{
	currentEdge->reversEdge = !currentEdge->reversEdge;
}

void Viewer::ModifyNameTop(QString nameTop)
{
	if (currentMode == EDIT_GRAPH)
	{
		currentTop->nameTop = nameTop;
	}
}

void Viewer::ModifyWeightEdge1(int weightEdge1)
{
	if (currentMode == EDIT_GRAPH)
	{
		currentEdge->weightEdge1 = weightEdge1;
	}
}

void Viewer::ModifyWeightEdge2(int weightEdge2)
{
	if (currentMode == EDIT_GRAPH)
	{
		currentEdge->weightEdge2 = weightEdge2;
	}
}

void Viewer::DeleteAutoAdge()
{
	QList<GraphEdge*>::iterator iEdge = currentTop->edges.begin(); //Итератор ребра
	while (*iEdge != currentTop->autoEdge && iEdge != currentTop->edges.end())
	{
		iEdge++;
	}
	if (iEdge != currentTop->edges.end())	
		currentTop->edges.erase(iEdge);

	iEdge = edges.begin();
	while (iEdge != edges.end() && *iEdge != currentTop->autoEdge)
	{
		iEdge++;
	}
	if (iEdge != edges.end())	
		edges.erase(iEdge);

	delete currentTop->autoEdge;
	currentTop->autoEdge = NULL;
	deleteAutoEdge->setVisible(false);
}

GraphEdge *Viewer::AppendEdge(GraphTop *topA, GraphTop *topB)
{
	//параметры вершины
	int form = comboFormEdge->currentIndex();
	int weightEdge1 = this->weightEdge1->value();
	int weightEdge2 = this->weightEdge2->value();

	//создать ребро
	currentEdge = new GraphEdge(form,buttonColor->color(),weightEdge1,weightEdge2,signalEmiter);
	currentEdge->start = new QPoint;
	*(currentEdge->start) = topA->pos().toPoint();
	currentEdge->finish = new QPoint;
	*(currentEdge->finish) = topB->pos().toPoint();
	currentEdge->topA = topA;
	currentEdge->topB = topB;
	topA->edges << currentEdge;
	if (currentEdge->topA != currentEdge->topB)	
		topB->edges << currentEdge;

	//Добавить ребро в сцену
	scene->addItem(currentEdge);
	edges << currentEdge;
	scene->update();
	GraphEdge *grEd = currentEdge;
	currentEdge = NULL;
	return grEd;
}

void Viewer::save(QString filename, QString viewType)
{
	//Открыть/создать файл
	QFile file(filename);
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&file);

	if (viewType == "Матрица смежности графа")
	{
		in_out->outAdjacencyMatrixToFile(out);
	}
	else if (viewType == "Матрица весов графа")
	{
		in_out->outWeightMatrixToFile(out);
	}
	else if (viewType == "Структура смежности графа")
	{
		in_out->outAdjacencyStructToFile(out);
	}

	//Закрываем файл
	file.close();
}

void Viewer::load(QString filename, QString viewType)
{
	QFile file(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream in(&file);
	tops.clear();
	edges.clear();

	if (viewType == "Матрица смежности графа")
	{
		in_out->getAdjacencyMatrixFromFile(in);
	}
	else if (viewType == "Матрица весов графа")
	{
		in_out->getWeightMatrixFromFile(in);
	}
	else if (viewType == "Структура смежности графа")
	{
		in_out->getAdjacencyStructFromFile(in);
	}
	
	//Рисуем граф
	drawGraph();

	//Закрываем файл
	file.close();
}

void Viewer::drawGraph()
{
	int max = -1;
	//Рисуем вершины
	for (int i = 0; i < tops.count(); i++)
	{
		scene->addItem(tops[i]);
	}
	//Рисуем ребра
	for (int i=0; i < edges.count(); i++)
	{
		scene->addItem(edges[i]);
	}
	scene->update();
}

int Viewer::getMode()
{
	return currentMode;
}

void Viewer::commiy()
{
	QList<QGraphicsItem*> selectedItems = scene->selectedItems();

	if (selectedItems.size() != 1)
	{
		QMessageBox::warning(this, "Повторите попытку", "Для решения задачи коммивояжёра необходимо выделить одну вершину, убедитесь, что выделена только одна вершина (отправная точка) и повторите попытку");
		return;
	}
	
	if (strcmp(typeid(*(selectedItems[0])).name(), "class GraphTop") != 0)
	{
		QMessageBox::warning(this, "Повторите попытку", "Для решения задачи коммивояжёра необходимо выделить одну вершину, снимите выделение с ребра, выберете отпраную точку и повторите попытку");
		return;
	}

	if (tops.size() < 2)
	{
		QMessageBox::warning(this, "Повторите попытку", "Граф состоит из одной вершины. Решение задачи коммивояжёра не имеет смысла. Задайте граф, как минимум, с двумя вершинами и повторите попытку.");
		return;
	}

	threads.end();

	std::vector<GraphTop *> v;
	for (int i = 0; i < tops.size(); i++)
	{
		if (tops[i] != selectedItems[0])	v.push_back(tops[i]); //если не начальная вершина - положить её индекс в контейнер.
	}
	
	try
	{
		PermutationsGenerator<GraphTop*> *pg = new PermutationsGenerator<GraphTop*>(v, true);
		pg->generate();
		std::vector<std::vector <GraphTop*> > result = pg->getResult();

		for (int i = 0; i < result.size(); i++)
		{
			result[i].insert(result[i].begin(),(GraphTop*)selectedItems[0]);
		}

		unsigned long min = INFINITE;

		bestWays.clear();

		for (int i = 0; i < result.size(); i++)
		{
			unsigned long summ = 0;
			unsigned long weight = 0;

			for (int j = 0; j < result[i].size() && summ != INFINITE; j++)
			{
				if (j != result[i].size()-1)	weight = in_out->getEdgeWeight(result[i][j], result[i][j+1]);
				else weight = in_out->getEdgeWeight(result[i][j], result[i][0]);

				if (weight != INFINITE)	summ += weight;
				else summ = INFINITE;
			}
			if (min >= summ && summ != INFINITE)
			{
				if (min > summ)
				{
					min = summ;
					bestWays.clear(); //найдено рекордно минимальное значение, предыдущие минимумы не имеют смысла!
				}
				bestWays.push_back(result[i]);
			}
			bestWaysValue.fill(min, bestWays.size());
		}
	}
	catch (std::string error)
	{
		QMessageBox::warning(this, "Ошибка", "Maximum size is 10\nДля решения задачи коммивояжера полным перебором необходимо сгенерировать перестановки для n-1 вершин графа, максимых размер множества, поддерживаемый библиотекой CombLib равен 10");
		return;
	}

	SetMode(DEMO_MODE);

	currentBestWay = 0;
	NextDemo();
}

void Viewer::PreviousDemo()
{
	threads.clear();

	GraphTop *top;
	foreach(top, tops)
	{
		top->resetShowStepFlag();
	}

	//организовать прогонку всех кротчайших путей
	unsigned int wait = 0;

	if (currentBestWay == 1) return;

	currentBestWay--;

	nextDemo->setEnabled(false);
	previousDemo->setEnabled(false);

	for (int j = 0; j < bestWays[currentBestWay-1].size(); j++)
	{
		wait += 1000;
		ThreadDemo *thread = new ThreadDemo(this, bestWays[currentBestWay-1][j], wait, j+1);
		if (j == bestWays[currentBestWay-1].size()-1) thread->isLast = true;
		thread->start();
		threads << thread;
	}

	QString progressText = "<font size=\"5\" color = \"#FF0000\"><b>Путь ";
	progressText += QString::number(currentBestWay);
	progressText += " из ";
	progressText += QString::number(bestWays.size());
	progressText += "</b></font>";
	progressDemoLabel->setText(progressText);

	QString bestWayValueText = "<font size=\"5\" color = \"#FF0000\"><b>Длина пути: ";
	bestWayValueText += QString::number(bestWaysValue[currentBestWay-1]);
	bestWayValueText += "</b></font>";
	bestWayValueDemoLabel->setText(bestWayValueText);
}

void Viewer::NextDemo()
{
	if (bestWays.size() == 0)
	{
		QMessageBox::warning(this, "Путей не найдено", "Гамильтоновых путей не найдено, задача коммивояжера решений не имеет, проверьте правильность постороения графа");
		ExitDemo();
		return;
	}

	threads.clear();
	GraphTop *top;
	foreach(top, tops)
	{
		top->resetShowStepFlag();
	}
	//организовать прогонку всех кротчайших путей
	unsigned int wait = 0;

	if (currentBestWay == bestWays.size()) return;

	currentBestWay++;

	nextDemo->setEnabled(false);
	previousDemo->setEnabled(false);

	for (int j = 0; j < bestWays[currentBestWay-1].size(); j++)
	{	
		wait += 1000;
		ThreadDemo *thread = new ThreadDemo(this, bestWays[currentBestWay-1][j], wait, j+1);
		if (j == bestWays[currentBestWay-1].size()-1) thread->isLast = true;
		connect(thread, SIGNAL(demoEnd(QThread *)), this, SLOT (deleteThread(QThread *)));
		thread->start();
		threads << thread;
	}

	QString progressText = "<font size=\"5\" color = \"#FF0000\"><b>Путь ";
	progressText += QString::number(currentBestWay);
	progressText += " из ";
	progressText += QString::number(bestWays.size());
	progressText += "</b></font>";
	progressDemoLabel->setText(progressText);

	QString bestWayValueText = "<font size=\"5\" color = \"#FF0000\"><b>Длина пути: ";
	bestWayValueText += QString::number(bestWaysValue[currentBestWay-1]);
	bestWayValueText += "</b></font>";
	bestWayValueDemoLabel->setText(bestWayValueText);
}

void Viewer::ExitDemo()
{
	if (threads.isEmpty()) SetMode(EDIT_GRAPH);
}

void Viewer::deleteThread(QThread *thread)
{
	/*ThreadDemo *conteinerThread;
	QList<ThreadDemo *>::iterator iter = threads.begin();
	while (iter != threads.end())
	{
		if (*iter == thread)
		{
			threads.erase(iter);
			break;
		}
		iter++;
	}

	delete thread;
	thread = NULL;

	if (threads.isEmpty())
	{
		nextDemo->setEnabled(true);
		previousDemo->setEnabled(true);
	}*/
	threads.clear();
	nextDemo->setEnabled(true);
	previousDemo->setEnabled(true);
}