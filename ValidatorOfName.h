#pragma once
#include <QValidator>

//����� ���������� ����� �������
class ValidatorOfName :
	public QValidator
{
	Q_OBJECT

public:
	ValidatorOfName(QObject *parent);
	ValidatorOfName(void);
	~ValidatorOfName(void);
	QValidator::State validate(QString & input, int & pos ) const;
};
