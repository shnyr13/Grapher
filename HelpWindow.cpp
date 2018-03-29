#include "HelpWindow.h"

HelpWindow::HelpWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	show();
	setWindowIcon(QIcon("icon.png"));
	ui.webView->load(QUrl("help.html"));
}

HelpWindow::~HelpWindow()
{

}

void HelpWindow::closeEvent(QCloseEvent *event)
{
	emit HelpWindowClose();
	event->accept();
}
