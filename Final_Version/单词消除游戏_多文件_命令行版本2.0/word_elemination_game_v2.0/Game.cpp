#include "Game.h"

vector<vector<string>> Game::wordList = {};
int Game::Display()
{
	cout << "This the " << level + 1 << "th level : "
		<< "There are " << wordList.size() << " levels totally" << endl;
	if ((unsigned int)level < wordList.size() && wordList[level].size() > 0) {
		srand((unsigned int)time(NULL));
		int pos = rand() % wordList[level].size();
		cout << wordList[level][pos];
		Sleep(2000);
		cout << "\r                   \r";
		time_t timeBegin;
		timeBegin = time(NULL);
		string answer;
		cin >> answer;
		if (wordList[level][pos] == answer) {
			time_t timeEnd;
			timeEnd = time(NULL);
			cout << "Your answer is right. "
				<< "Time used: "
				<< timeEnd - timeBegin << endl;
			level++;
			int point;
			if (level > 7 && (timeEnd - timeBegin) < 10)
				point = 3;
			else if (level > 4 && (timeEnd - timeBegin) < 4)
				point = 2;
			else
				point = 1;
			return point;
		}
		else {
			cout << "Your answer is wrong." << endl;
			return false;
		}
	}
	else {
		cout << "Congratulations! You have reached the last level." << endl;
		return false;
	}
}
void Game::InitFromFile()
{
	string w;
	ifstream infile("wordList.csv");
	if (!infile) {
		cout << "File wordList.csv open failed!" << endl;
		abort();
	}
	unsigned int i = 0;
	while (!infile.eof())
	{
		infile >> w;
		for (; i < wordList.size(); i++)
			if (w.size() == wordList[i][0].size())
				break;
		if (i < wordList.size())
			wordList[i].push_back(w);
		else {
			vector <string> add;
			add.push_back(w);
			wordList.push_back(add);
		}
	}
	infile.close();
}
