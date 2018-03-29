#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QWidget>
#include "ui_HelpWindow.h"

#include <QCloseEvent>

class HelpWindow : public QWidget
{
	Q_OBJECT

public:
	HelpWindow(QWidget *parent = 0);
	~HelpWindow();

private:
	Ui::HelpWindow ui;

	virtual void closeEvent(QCloseEvent *event);

signals:
	void HelpWindowClose();
};

#endif // HELPWINDOW_H
