#include "Game.h"
#include "common.h"


vector<vector<string>> Game::wordList = {};
int Game::Display(SOCKET sockClient)
{
	char revBuf[MaxSize];
	sendToClient(sockClient, "The level is: ");
	Sleep(10);
	char l[MaxSize];
	_itoa_s(level + 1, l, 10);
	sendToClient(sockClient, l);
	Sleep(10);
	sendToClient(sockClient, "\n");
	Sleep(10);
	if ((unsigned int)level < wordList.size() && wordList[level].size() > 0) {
		srand((unsigned int)time(NULL));
		int pos = rand() % wordList[level].size();
		char w[MaxSize];
		strcpy_s(w, (wordList[level][pos]).c_str());
		send(sockClient, w, strlen(w), 0);
		Sleep(1000);
		char sendBuf[MaxSize];
		strcpy_s(sendBuf, "\r 				 \r");
		send(sockClient, sendBuf, strlen(sendBuf), 0);
		time_t timeBegin;
		timeBegin = time(NULL);
		string answer;
		int n = recv(sockClient, revBuf, MaxSize, 0);
		revBuf[n] = '\0';
		if (n < 0)
			cout << "Rec Error" << endl;
		answer = revBuf;
		if (wordList[level][pos] == answer) {
			time_t timeEnd;
			timeEnd = time(NULL);
			char l[MaxSize];
			_itoa_s((int)(timeEnd - timeBegin), l, 10);
			sendToClient(sockClient, l);
			Sleep(10);
			sendToClient(sockClient, "\n");
			Sleep(10);
			sendToClient(sockClient, "Your answer is right.   \n");
			Sleep(10);
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
			sendToClient(sockClient, "Your answer is wrong.\n");
			Sleep(10);
			return 0;
		}
	}
	else {
		sendToClient(sockClient, "Congratulations! You have reached the last level.\n");
		return 0;
	}
}

void Game::InitFromFile(SOCKET sockClient)
{
	string w;
	ifstream infile("wordList.csv");
	if (!infile) {
		sendToClient(sockClient, "File wordList.csv open failed!\n");
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
