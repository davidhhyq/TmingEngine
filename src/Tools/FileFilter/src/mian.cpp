#include <iostream>
#include <vector>
#include <map>
#include <string>
#include<sstream>
#include <io.h>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>

using namespace std;
std::map<string, string> commands;
std::vector<string> filters;
std::vector<std::string> files;

int writeFile(string file)
{
	ofstream examplefile(file);
	if (examplefile.is_open())
	{
		examplefile << "This is a line.\n";
		examplefile << "This is another line.\n";
		examplefile.close();
	}
	return 0;
}

int readFile(string file)
{
	char buffer[256];
	ifstream examplefile(file);
	if (!examplefile.is_open())
	{
		cout << "Error opening file"; exit(1);
	}
	while (!examplefile.eof())
	{
		examplefile.getline(buffer, 100);
		cout << buffer << endl;
	}
	return 0;
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

void listFiles(const char * dir)
{
	char dirNew[200];
	strcpy(dirNew, dir);
	strcat(dirNew, "\\*.*");    // ��Ŀ¼�������"\\*.*"���е�һ������

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1)        // ����Ƿ�ɹ�
		return;

	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			//cout << dir << "\\" << findData.name << "\t<dir>\n";

			// ��Ŀ¼�������"\\"����������Ŀ¼��������һ������
			strcpy(dirNew, dir);
			strcat(dirNew, "\\");
			strcat(dirNew, findData.name);

			listFiles(dirNew);
		}
		else
		{
			string s = string(dir) + "\\" + string(findData.name);
			auto ext = split(findData.name, '.');
			//cout <<"."<< *(ext.end()-1) << endl;

			vector<string>::iterator it;
			string value = "." + *(ext.end() - 1);
			it = find(filters.begin(), filters.end(), value);
			if (it != filters.end())
			{
				files.push_back(s);
			}
		}
	} while (_findnext(handle, &findData) == 0);

	_findclose(handle);    // �ر��������
}

int main(int argc, char const *argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			commands.insert(pair<string, string>(argv[i], argv[i + 1]));
		}
	}
	filters = split(commands["-f"], ' ');

	listFiles(commands["-p"].c_str());

	vector<string>::iterator iter;
	for (iter = files.begin(); iter != files.end(); iter++)
	{
		cout << *iter << endl;
		readFile(*iter);
	}
	return 0;
}