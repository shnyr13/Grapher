#pragma once


#include <string>
#include <vector>
#include <fstream>

template <typename T>
class CombinatoricsOutput
{
public:
	CombinatoricsOutput()
	{}
	~CombinatoricsOutput()
	{}

	static void toFile(std::string filename, std::vector< std::vector<T> > &data)
	{
		/*ofstream out;
		out.open(filename);

		for (int i = 0; i < data.size(); i++)
		{
			if (data[i].size() == 0)	out << "EMPTY";
			for (int j = 0; j < data[i].size(); j++)
			{
				out << data[i][j];
				if (j != data[i].size()-1)	out << " ";
			}
			if (i != data.size()-1)	out << "\n";
		}
		out.close();*/
	}
};

