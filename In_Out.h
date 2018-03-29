#pragma once
#include "Viewer.h"
#include <QTextStream>
#include <QVector>

class In_Out
{
public:
	In_Out(Viewer *viewer);
	~In_Out(void);

	QVector<QVector<bool>> getAdjacencyMatrix();
	QVector<QVector<bool>> getAdjacencyMatrixFromFile(QTextStream &in);
	bool outAdjacencyMatrixToFile(QTextStream &out);

	QVector<QVector<unsigned long>> getWeightMatrix();
	QVector<QVector<unsigned int>> getWeightMatrixFromFile(QTextStream &in);
	bool outWeightMatrixToFile(QTextStream &out);

	QVector<QVector<int>> getAdjacencyStruct();
	QVector<QVector<int>> getAdjacencyStructFromFile(QTextStream &in);
	bool outAdjacencyStructToFile(QTextStream &out);

	unsigned long getEdgeWeight(GraphTop *first, GraphTop *second);

private:

	unsigned long INFINITE;

	Viewer *_viewer;
};
