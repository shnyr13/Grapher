#include "Viewer.h"
#include <QMessageBox>
#include <QTextStream>


Viewer::Viewer(QWidget * par) : QGraphicsView(par)
{	
	in_out = new In_Out(this);

	INFINITE = -1;
	//�������� �����
	scene = new QGraphicsScene(QRectF(0,0,767,535));
	setScene(scene);

	//������� �����
	signalEmiter = new SignalEmiter;
	topA = NULL;
	topB = NULL;
	nextDemo = NULL;
	previousDemo = NULL;
	exitDemo = NULL;
	progressDemoLabel = NULL;
	bestWayValueDemoLabel = NULL;

	//������ ����������
	timer = new QTimer;
	timer->setInterval(10);
	connect(timer,SIGNAL(timeout()), this, SLOT (_Updater()));
	timer->start(10);

	//������-����
	connect (signalEmiter, SIGNAL (TopIsSelected(GraphTop*)), this, SLOT(EditTop(GraphTop*)));
	connect (signalEmiter, SIGNAL (EdgeIsSelected(GraphEdge*)), this, SLOT (EditEdge(GraphEdge*)));
}

Viewer::~Viewer(void)
{

}

//������� ������� ����
void Viewer::mousePressEvent(QMouseEvent *e)
{
	QGraphicsItem *item; //��������� �� ���������� �������
	GraphTop *_top; //��������� �� ������� ��� ���������� �����
	int color = 0; //����������� ���� ��������
	int form = 0; //����������� ����� ��������
	QString nameTop; //��� �������
	QString weightEdge; //��� �����
	QList<GraphEdge*>::iterator iEdge; //�������� �����

	//�������� � ����������� �� ������
	switch (currentMode)
	{
	case ADD_TOP:
		//�������������� � ���������� ���������� ������
		if (tops.count() == 50)
		{
			QMessageBox::warning(0,"Warning!","Maximum count of tops is 50");
			return;
		}
		if (scene->itemAt(e->pos()) != NULL && tops.contains((GraphTop*)scene->itemAt(e->pos())))	
			return;

		//���������� �������
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
		//�������� ������� �� 2 ������� ��� 2 ���� �� ����
		_top = (GraphTop*) scene->itemAt(e->pos());
		if(_top == NULL || !tops.contains(_top)) 
			return;
		if (topA == NULL)
			topA = _top;
		else
			topB = _top;

		//���������� �����
		if (topA != NULL && topB != NULL)
		{
			//�������� �� ������� �������� �������
			iEdge = edges.begin();
			while (iEdge != edges.end())
			{
				//�������������� � ������������� ����� ����� ���������
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
		//�������� ��������� �� �������
		item = scene->itemAt(e->pos());
		if (!item)	
			return;

		//��������
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
		//��������� ��������� ��� �������
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

//������� ����� � ��������
void Viewer::ExpportToImage(QString &filename)
{
	QImage image(scene->width(),scene->height(),QImage::Format_ARGB32_Premultiplied);
	image.fill("#FFFFFF"); //���� ����
	QPainter _painter(&image); //��������
	scene->render(&_painter);
	image.save(filename);
}

//������� �����
void Viewer::ClearScene()
{
	currentTop = NULL;
	currentEdge = NULL;
	tops.clear();
	edges.clear();
	scene->clear();
	scene->update();
}

//���������� � ������� �������
void delete_top(GraphTop *_top, QList<GraphTop*> &tops, QList<GraphEdge*> &edges)
{
	if (!_top)	
		return;

	if (_top->edges.count() == 1 && _top->edges[0]->topA == _top->edges[0]->topB)
	{
		delete	_top->edges[0];
		delete _top;
	}

	//���������� �������
	QList<GraphEdge*>::iterator iter = _top->edges.begin();
	while (iter != _top->edges.end())
	{
		delete_edge(*iter,edges);
		iter = _top->edges.begin();
	}

	//������� ������� �� ������
	QList<GraphTop*>::iterator i = tops.begin();
	while (*i != _top && i != tops.end())
	{
		i++;
	}
	if (i != tops.end())	
		tops.erase(i);

	delete _top;
}

//���������� � ������� �����
void delete_edge(GraphEdge * _edge, QList<GraphEdge*> &edges)
{
	if (!_edge)	
		return;

	//�������� �����
	GraphTop* A = _edge->topA;
	GraphTop* B = _edge->topB;

	//�������� ����� �� ������ �������
	QList<GraphEdge*>::iterator iter = A->edges.begin();
	while (iter != A->edges.end() && *iter != _edge)
	{
		iter++;
	}
	if (iter != A->edges.end()) 
		A->edges.erase(iter);

	//�������� ����� �� ������ �������
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


//��������� ������ ������
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

		previousDemo = new QPushButton("< �����", this);
		previousDemo->setGeometry(5, 5, 150, 30);
		previousDemo->show();

		nextDemo = new QPushButton("����� >",this);
		nextDemo->setGeometry(160, 5, 150, 30);
		nextDemo->show();

		exitDemo = new QPushButton("������", this);
		exitDemo->setGeometry(315, 5, 150, 30);
		exitDemo->show();

		progressDemoLabel = new QLabel("��������", this);
		progressDemoLabel->setGeometry(470, 5, 140, 30);
		progressDemoLabel->show();

		bestWayValueDemoLabel = new QLabel("����",this);
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

//����� ��������������: �������������� �������
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

//����� ��������������: �������������� �����
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

//========����� ��� ��������� ������� ��������==========
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
	QList<GraphEdge*>::iterator iEdge = currentTop->edges.begin(); //�������� �����
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
	//��������� �������
	int form = comboFormEdge->currentIndex();
	int weightEdge1 = this->weightEdge1->value();
	int weightEdge2 = this->weightEdge2->value();

	//������� �����
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

	//�������� ����� � �����
	scene->addItem(currentEdge);
	edges << currentEdge;
	scene->update();
	GraphEdge *grEd = currentEdge;
	currentEdge = NULL;
	return grEd;
}

void Viewer::save(QString filename, QString viewType)
{
	//�������/������� ����
	QFile file(filename);
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&file);

	if (viewType == "������� ��������� �����")
	{
		in_out->outAdjacencyMatrixToFile(out);
	}
	else if (viewType == "������� ����� �����")
	{
		in_out->outWeightMatrixToFile(out);
	}
	else if (viewType == "��������� ��������� �����")
	{
		in_out->outAdjacencyStructToFile(out);
	}

	//��������� ����
	file.close();
}

void Viewer::load(QString filename, QString viewType)
{
	QFile file(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream in(&file);
	tops.clear();
	edges.clear();

	if (viewType == "������� ��������� �����")
	{
		in_out->getAdjacencyMatrixFromFile(in);
	}
	else if (viewType == "������� ����� �����")
	{
		in_out->getWeightMatrixFromFile(in);
	}
	else if (viewType == "��������� ��������� �����")
	{
		in_out->getAdjacencyStructFromFile(in);
	}
	
	//������ ����
	drawGraph();

	//��������� ����
	file.close();
}

void Viewer::drawGraph()
{
	int max = -1;
	//������ �������
	for (int i = 0; i < tops.count(); i++)
	{
		scene->addItem(tops[i]);
	}
	//������ �����
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
		QMessageBox::warning(this, "��������� �������", "��� ������� ������ ����������� ���������� �������� ���� �������, ���������, ��� �������� ������ ���� ������� (��������� �����) � ��������� �������");
		return;
	}
	
	if (strcmp(typeid(*(selectedItems[0])).name(), "class GraphTop") != 0)
	{
		QMessageBox::warning(this, "��������� �������", "��� ������� ������ ����������� ���������� �������� ���� �������, ������� ��������� � �����, �������� �������� ����� � ��������� �������");
		return;
	}

	if (tops.size() < 2)
	{
		QMessageBox::warning(this, "��������� �������", "���� ������� �� ����� �������. ������� ������ ����������� �� ����� ������. ������� ����, ��� �������, � ����� ��������� � ��������� �������.");
		return;
	}

	threads.end();

	std::vector<GraphTop *> v;
	for (int i = 0; i < tops.size(); i++)
	{
		if (tops[i] != selectedItems[0])	v.push_back(tops[i]); //���� �� ��������� ������� - �������� � ������ � ���������.
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
					bestWays.clear(); //������� �������� ����������� ��������, ���������� �������� �� ����� ������!
				}
				bestWays.push_back(result[i]);
			}
			bestWaysValue.fill(min, bestWays.size());
		}
	}
	catch (std::string error)
	{
		QMessageBox::warning(this, "������", "Maximum size is 10\n��� ������� ������ ������������ ������ ��������� ���������� ������������� ������������ ��� n-1 ������ �����, �������� ������ ���������, �������������� ����������� CombLib ����� 10");
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

	//������������ �������� ���� ���������� �����
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

	QString progressText = "<font size=\"5\" color = \"#FF0000\"><b>���� ";
	progressText += QString::number(currentBestWay);
	progressText += " �� ";
	progressText += QString::number(bestWays.size());
	progressText += "</b></font>";
	progressDemoLabel->setText(progressText);

	QString bestWayValueText = "<font size=\"5\" color = \"#FF0000\"><b>����� ����: ";
	bestWayValueText += QString::number(bestWaysValue[currentBestWay-1]);
	bestWayValueText += "</b></font>";
	bestWayValueDemoLabel->setText(bestWayValueText);
}

void Viewer::NextDemo()
{
	if (bestWays.size() == 0)
	{
		QMessageBox::warning(this, "����� �� �������", "������������� ����� �� �������, ������ ������������ ������� �� �����, ��������� ������������ ����������� �����");
		ExitDemo();
		return;
	}

	threads.clear();
	GraphTop *top;
	foreach(top, tops)
	{
		top->resetShowStepFlag();
	}
	//������������ �������� ���� ���������� �����
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

	QString progressText = "<font size=\"5\" color = \"#FF0000\"><b>���� ";
	progressText += QString::number(currentBestWay);
	progressText += " �� ";
	progressText += QString::number(bestWays.size());
	progressText += "</b></font>";
	progressDemoLabel->setText(progressText);

	QString bestWayValueText = "<font size=\"5\" color = \"#FF0000\"><b>����� ����: ";
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