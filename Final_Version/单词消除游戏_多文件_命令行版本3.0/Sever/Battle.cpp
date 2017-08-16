#include "Battle.h"


void Battle::givePro()
{
	if ((unsigned int)level < wordList.size() && wordList[level].size() > 0) {
		srand((unsigned int)time(NULL));
		int pos = rand() % wordList[level].size();
		word = wordList[level][pos];
		sendToClient(sockClientA, (wordList[level][pos]).c_str());
		sendToClient(sockClientB, (wordList[level][pos]).c_str());
		Sleep(1000);
		sendToClient(sockClientA, "\r                  \r");
		sendToClient(sockClientB, "\r                  \r");
	}
	else {
		sendToClient(sockClientA, "Congratulations! You have reached the last level.\n");
		sendToClient(sockClientB, "Congratulations! You have reached the last level.\n");
	}
	level++;
}
int Battle::onBattle(SOCKET sockClient)
{
	char revBuf[MaxSize];
	string answer;
	int n = recv(sockClient, revBuf, MaxSize, 0);
	revBuf[n] = '\0';
	if (n < 0)
		cout << "Rec Error" << endl;
	answer = revBuf;

	if (word == answer) {
		time_t timeEnd;
		timeEnd = time(NULL);
		if (sockClientA == sockClient)
			t1 = timeEnd - timeBegin;
		else
			t2 = timeEnd - timeBegin;

		if (t1 > t2) {
			sendToClient(sockClientA, "You lose the battle!   \n");
			sendToClient(sockClientB, "You win the battle!   \n");
			return 2;
		}
		else if (t1 < t2) {
			sendToClient(sockClientA, "You win the battle!   \n");
			sendToClient(sockClientB, "You lose the battle!   \n");
			return 1;
		}
		else {
			sendToClient(sockClientA, "It is a draw!   \n");
			sendToClient(sockClientB, "It is a draw!   \n");
			return 0;
		}
	}
	else{
		sendToClient(sockClient, "Your answer is wrong.\n");
		if (sockClientA == sockClient)
			return 2;
		else
			return 1;
	}
}
