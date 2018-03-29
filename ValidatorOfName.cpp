#include "ValidatorOfName.h"

ValidatorOfName::ValidatorOfName(QObject *parent)
		:QValidator(parent)
{
}

ValidatorOfName::ValidatorOfName(void)
{
}

ValidatorOfName::~ValidatorOfName(void)
{
}

QValidator::State ValidatorOfName::validate( QString & input, int & pos ) const
{
	bool result = true;
	for (int i = 0; i < input.length() && result; i++)
	{
		if (!input[i].isLetterOrNumber() && input[i] != ' ')
			result = false;
		if (i != input.length()-1 && input[i] == ' ' && input[i+1] == ' ')
			result = false;
		if (i == 0 && input[i] == ' ')
			result = false;
	}
	//Сам валидатор
	if(input.length() > 20 || !result) //Недопустимое название вершины
	{
		return QValidator::Invalid;
	}
	else if(input.length() <= 20 && result) //Верное название вершины
	{
		return QValidator::Acceptable;
	}
	else //Все остальное возможно исправить
	{
		return QValidator::Intermediate;
	}
}