#include "In_Out.h"

In_Out::In_Out(Viewer *viewer)
{
	INFINITE = -1;
	_viewer = viewer;
}

In_Out::~In_Out(void)
{
}
//Ã¿“–»÷¿ —Ã≈∆ÕŒ—“» √–¿‘¿
QVector<QVector<bool>> In_Out::getAdjacencyMatrix()
{
	QVector<QVector<bool>> adjacencyMatrix;

	QVector<bool> A;
	A.fill(false, _viewer->tops.size());
	adjacencyMatrix.fill(A, _viewer->tops.size());

	for (int i = 0; i < adjacencyMatrix.size(); i++)
	{
		for (int j = 0; j < adjacencyMatrix[i].size(); j++)
		{
			if (i != j)
			{
				adjacencyMatrix[i][j] = getEdgeWeight(_viewer->tops[i], _viewer->tops[j]) != INFINITE;
			}
		}
	}

	return adjacencyMatrix;
}

QVector<QVector<bool>> In_Out::getAdjacencyMatrixFromFile(QTextStream &in)
{
	QVector<QVector<bool>> Matrix;

		while (!in.atEnd())
		{
			QVector<bool> A;
			QString line = in.readLine();
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == '1')			A << true;
				else if (line[i] == '0')	A << false;
			}
			Matrix << A;
			GraphTop *grTop = new GraphTop(0, _viewer->buttonColor->color(), "", _viewer->signalEmiter);
			grTop->setRandomPos();
			_viewer->tops.append(grTop);
		}

		for (int i = 0; i < Matrix.size(); i++)
		{
			for (int j = 0; j < Matrix.size(); j++)
			{
				if (Matrix [i][j] && Matrix[j][i])
				{
					_viewer->comboFormEdge->setCurrentIndex(2);
					_viewer->AppendEdge(_viewer->tops[i], _viewer->tops[j]);
				}
				else
				{
					_viewer->comboFormEdge->setCurrentIndex(1);
					if (Matrix [i][j])	_viewer->AppendEdge(_viewer->tops[i], _viewer->tops[j]);
					else if (Matrix [j][i])	_viewer->AppendEdge(_viewer->tops[j], _viewer->tops[i]);
				}
			}
		}
		return Matrix;
}

bool In_Out::outAdjacencyMatrixToFile(QTextStream &out)
{
	QVector<QVector<bool>> adjacencyMatrix = getAdjacencyMatrix();

	for (int i = 0; i < adjacencyMatrix.size(); i++)
	{
		for (int j = 0; j < adjacencyMatrix[i].size(); j++)
		{
			out << adjacencyMatrix[i][j];
			if (j != adjacencyMatrix[i].size()-1)	out << " ";
		}
		if (i != adjacencyMatrix.size()-1)			out << "\n";
	}
	return true;
}

//Ã¿“–»÷¿ ¬≈—Œ¬ √–¿‘¿
QVector<QVector<unsigned long>> In_Out::getWeightMatrix()
{
	//ÒÓÁ‰‡Ú¸ Ë Á‡ÔÓÎÌËÚ¸ Ï‡ÚËˆÛ ÌÛÎˇÏË.
	QVector<QVector<unsigned long>> weightMatrix;
	QVector<unsigned long> A;
	A.fill(0, _viewer->tops.size());
	weightMatrix.fill(A, _viewer->tops.size());

	for (int i = 0; i < weightMatrix.size(); i++)
	{
		for (int j = 0; j < weightMatrix[i].size(); j++)
		{
			if (i != j)
			{
				weightMatrix[i][j] = getEdgeWeight(_viewer->tops[i], _viewer->tops[j]);
			}
		}
	}

	return weightMatrix;
}

QVector<QVector<unsigned int>> In_Out::getWeightMatrixFromFile(QTextStream &in)
{

	unsigned int INF = -1;

	QVector<QVector<unsigned int>> weightMatrix;
		
	while (!in.atEnd())
	{
		QVector<unsigned int> A;

		QString line = in.readLine();
		QStringList strArr = line.split(" ");

		for (int i = 0; i < strArr.size(); i++)
		{
			if (strArr[i] == "INF")	A << INF;
			else A << strArr[i].toInt();
		}
		weightMatrix << A;

		GraphTop *grTop = new GraphTop(0, _viewer->buttonColor->color(), "", _viewer->signalEmiter);
		grTop->setRandomPos();
		_viewer->tops.append(grTop);
	}

	for (int i = 0; i < weightMatrix.size(); i++)
	{
		for (int j = 0; j < weightMatrix.size(); j++)
		{
			if (weightMatrix[i][j] != INF || weightMatrix[j][i] != INF)
			{	
				GraphEdge *graphEdge;
				if (weightMatrix[i][j] != INF && weightMatrix[j][i] == INF)
				{
					graphEdge = _viewer->AppendEdge(_viewer->tops[i], _viewer->tops[j]);
					graphEdge->formEdge = 1;
					graphEdge->weightEdge1 = weightMatrix[i][j];
				}
				else if (weightMatrix[j][i] != INF && weightMatrix[i][j] == INF)
				{
					graphEdge = _viewer->AppendEdge(_viewer->tops[j], _viewer->tops[i]);
					graphEdge->formEdge = 1;
					graphEdge->weightEdge1 = weightMatrix[j][i];
				}
				else if (weightMatrix[i][j] != INF && weightMatrix[j][i] != INF)
				{
					if (weightMatrix[i][j] != weightMatrix[j][i])
					{
						graphEdge = _viewer->AppendEdge(_viewer->tops[i], _viewer->tops[j]);
						graphEdge->formEdge = 2;
						graphEdge->weightEdge1 = weightMatrix[i][j];
						graphEdge->weightEdge2 = weightMatrix[j][i];
					}
					else
					{
						graphEdge = _viewer->AppendEdge(_viewer->tops[i], _viewer->tops[j]);
						graphEdge->formEdge = 0;
						graphEdge->weightEdge1 = weightMatrix[i][j];
					}
				}
			}
		}
	}
	return weightMatrix;
}

bool In_Out::outWeightMatrixToFile(QTextStream &out)
{
	QVector<QVector<unsigned long>> weightMatrix = getWeightMatrix();

	for (int i = 0; i < weightMatrix.size(); i++)
	{
		for (int j = 0; j < weightMatrix[i].size(); j++)
		{
			if (i == j || weightMatrix[i][j] == INFINITE) out << "INF";
			else out << weightMatrix[i][j];
			if (j != weightMatrix[i].size()-1)	out << " ";
		}
		if (i != weightMatrix.size()-1)			out << "\n";
	}
	return true;
}

//—“–” “”–¿ —Ã≈∆ÕŒ—“» √–¿‘¿
QVector<QVector<int>> In_Out::getAdjacencyStruct()
{
	QVector<QVector<int>> structMatrix;

	for (int i = 0; i < _viewer->tops.size(); i++)
	{
		QVector<int> A;
		for (int j = 0; j < _viewer->tops.size(); j++)
		{
			if (i != j)
			{
				if (getEdgeWeight(_viewer->tops[i], _viewer->tops[j]) != INFINITE)
				{
					A << j+1;
				}
			}
		}
		structMatrix << A;
	}


	return structMatrix;
}

QVector<QVector<int>> In_Out::getAdjacencyStructFromFile(QTextStream &in)
{
	QVector<QVector<int>> Matrix;
		
		while (!in.atEnd())
		{
			QVector<int> A;

			QString line = in.readLine();
			QStringList strArr = line.split(" ");

			for (int i = 0; i < strArr.size(); i++)
			{
				A << strArr[i].toInt();
			}
	
			Matrix << A;
			
			GraphTop *grTop = new GraphTop(0, _viewer->buttonColor->color(), "", _viewer->signalEmiter);
			grTop->setRandomPos();
			_viewer->tops.append(grTop);
		}

		for (int i = 0; i < Matrix.size(); i++)
		{
			for (int j = 0; j < Matrix[i].size(); j++)
			{
				if (Matrix[i][j] != 0)
				{
					if (Matrix[j].contains(Matrix[i][j]))		_viewer->comboFormEdge->setCurrentIndex(2);
					else	_viewer->comboFormEdge->setCurrentIndex(1);

					bool find = false;
					for (int i = 0; i < _viewer->edges.size() && !find; i++)
					{	
						//if (edges[i]->topA == tops[i] && edges[i]->topA == tops[Matrix[i][j]-1]) find = true;
					}
					
					if (!find)	_viewer->AppendEdge(_viewer->tops[i], _viewer->tops[Matrix[i][j]-1]);
				}
			}
		}
		return Matrix;
}

bool In_Out::outAdjacencyStructToFile(QTextStream &out)
{
	QVector<QVector<int>> adjacencyStruct = getAdjacencyStruct();

	for (int i = 0; i < adjacencyStruct.size(); i++)
	{
		for (int j = 0; j < adjacencyStruct[i].size(); j++)
		{
			out << adjacencyStruct[i][j];
			if (j != adjacencyStruct[i].size()-1)	out << " ";
		}
		if (i != adjacencyStruct.size()-1)			out << "\n";
	}
	return true;
}


//¬—œŒÃŒ√¿“≈À‹Õ€≈ Ã≈“Œƒ€
unsigned long In_Out::getEdgeWeight(GraphTop *first, GraphTop *second)
{
	for (int i = 0; i < _viewer->edges.size(); i++)
	{
		if (_viewer->edges[i]->topA == first && _viewer->edges[i]->topB == second)
		{
			return _viewer->edges[i]->weightEdge1;
		}
		if (_viewer->edges[i]->formEdge == 0 && _viewer->edges[i]->topA == second && _viewer->edges[i]->topB == first)
		{
			return _viewer->edges[i]->weightEdge1;
		}
		if (_viewer->edges[i]->formEdge == 2 && _viewer->edges[i]->topA == second && _viewer->edges[i]->topB == first)
		{
			return _viewer->edges[i]->weightEdge2;
		}
	}
	return INFINITE;
}