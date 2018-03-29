#include "Grapher.h"
#include <QMessageBox>

Grapher::Grapher(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	helpWindow = NULL;

	//������-���
	status = new QStatusBar(this);

	//������ ���������
	this->setWindowIcon(QIcon("icon.png"));

	//��������������� ���������� ��������
	redirectButtom();

	//������ ������
	buttonIcon();

	//������ ������� ���� �� �����
	ui.weightEdge2->setVisible(false);

	//������-����
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
	//������� ����-����
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
	
	//��������� ����� ������
	RESET_POINTER();

	//���������
	ui.lineNameTop->setValidator(&val);
}

Grapher::~Grapher()
{
	
}

//��������������� ���������� ��������
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

//������� ������ �������
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
		
	//������� ��������� ��� ������
	ui.append_edge->setToolTip("�������� �����");
	ui.append_top->setToolTip("�������� �������");
	ui.delete_element->setToolTip("������� �������");
	ui.edit_mode->setToolTip("������� � ����� ���������");
}

//�������� �������
void Grapher::APPEND_TOP()
{
	//������ ������� ����� ��� ����� ��� ���������� ������
	for (int i = 0; i < ui.paint_field->edges.count(); i++)
	{
		ui.paint_field->edges[i]->setFlags(0);
	}

	//������� ���������� ��������
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

	//������ ���������
	status->showMessage("����� ���������� �������");
	setStatusBar(status);
}

//�������� �����
void Grapher::APPEND_EDGE()
{
	//������ ������� ����� ��� ����� ��� ���������� ������
	for (int i = 0; i < ui.paint_field->edges.count(); i++)
	{
		ui.paint_field->edges[i]->setFlags(0);
	}

	//������� ���������� ��������
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

	//������ ���������
	status->showMessage("����� ���������� ����");
	setStatusBar(status);
}

//������� �������
void Grapher::DELETE_ELEMENT()
{
	//������ ������� ����� ��� ����� ��� ���������� ������
	for (int i = 0; i < ui.paint_field->edges.count(); i++)
	{
		ui.paint_field->edges[i]->setFlags(0);
	}

	//������� ���������� ��������
	ui.paint_field->SetMode(DEL_ELEMENT);
	ui.delete_element->setEnabled(false);
	ui.append_top->setEnabled(true);
	ui.append_edge->setEnabled(true);
	ui.edit_mode->setEnabled(true);
	ui.comboFormEdge->setEnabled(false);
	ui.comboFormTop->setEnabled(false);
	ui.topParameters->setVisible(false);
	ui.edgeParameters->setVisible(false);
	
	//������ ���������
	status->showMessage("����� ��������");
	setStatusBar(status);
}


//���������� ���������
void Grapher::RESET_POINTER()
{
	//������ ������� ����� ��� ����� ��� ���������� ������
	for (int i = 0; i < ui.paint_field->edges.count(); i++)
	{
		ui.paint_field->edges[i]->setFlags(0);
	}

	//������� ���������� ��������
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

	//������ ���������
	status->showMessage("����� ���������");
	setStatusBar(status);
}

void Grapher::SET_COLOR()
{
	QColor color = QColorDialog::getColor(ui.set_color->color(), this, "����� �����");
	if (color.isValid())
	{
		ui.set_color->setColor(color);
	}
}

//������� ���� � ������
void Grapher::OPEN()
{
	//�������
	QString Filename = QFileDialog::getOpenFileName( this, QString("������� ����"), QString());
	if (Filename.isEmpty())	
		return;

	//����� ���� �������������
	QStringList items;
	items << "������� ��������� �����" << "������� ����� �����" << "��������� ��������� �����";
	bool pressedOK;
	QString type = QInputDialog::getItem(this, "����� ���� �������������", "��� �������������:", items, 0, false, &pressedOK);
	if (!pressedOK)	return;

	CLEAR();

	ui.paint_field->load(Filename, type);
	fileName = Filename;

	//����� ���������
	RESET_POINTER();
}

//��������� ���� � ������� ����
void Grapher::SAVE()
{
	//������� ���� �������?
	if(fileName != "")
		ui.paint_field->save(fileName, viewName);
	else
	{
		SAVE_AS();
	}
}

//��������� ���� � �������� ����
void Grapher::SAVE_AS()
{
	//����� ���� �������������
	QStringList items;
	items << "������� ��������� �����" << "������� ����� �����" << "��������� ��������� �����";
	bool pressedOK;
	QString type = QInputDialog::getItem(this, "����� ���� �������������", "��� �������������:", items, 0, false, &pressedOK);
	if (!pressedOK)	return;
	
	QString Filename = QFileDialog::getSaveFileName( this, QString("��������� ���"), QString());	
	
	if (Filename.isEmpty())	return;

	ui.paint_field->save(Filename, type);
	fileName = Filename;
}

//������� � ��������
void Grapher::ExportTopImage()
{
	QString filename = QFileDialog::getSaveFileName( this, QString("������� � PNG"), QString(), QString("������ (*.jpg)"));
	if (filename.isEmpty())
		return;
	ui.paint_field->ExpportToImage(filename);
}
//��������� �� ����
void Grapher::CLEAR()
{
	//�������� �����
	ui.paint_field->ClearScene();
	//����� ���������
	RESET_POINTER();
}

void Grapher::exit()
{
	//��������� ����� �������
	//SAVE();
	//����� �� ���������
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
	QMessageBox::about(this, "� ���������", "��������� ����������� � ������� ���������� Qt 4.8.2\n�����������:\n������� ������ ����-466, ������ �.�.\n���������, �������, 2015");
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