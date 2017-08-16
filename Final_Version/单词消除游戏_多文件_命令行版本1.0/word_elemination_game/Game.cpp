#include "Game.h"

vector<string> Game::wordList = {};
bool Game::Display()
{
	cout << "This the " << level + 1 << "th level : "
		<< "There are " << wordList.size() << " levels totally" << endl;
	if ((unsigned)level < wordList.size()) {
		cout << wordList[level];
		Sleep(2000);
		cout << "\r                   \r";
		string answer;
		cin >> answer;
		if (wordList[level] == answer) {
			cout << "Your answer is right." << endl;
			level++;
			return true;
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
	while (!infile.eof())
	{
		infile >> w;
		wordList.push_back(w);
	}
	infile.close();
}
