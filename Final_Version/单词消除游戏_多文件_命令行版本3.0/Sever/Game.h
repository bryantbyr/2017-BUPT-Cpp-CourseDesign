#pragma once
#include "common.h"

class Game {
private:
public:
	static vector<vector<string>> wordList;//被添加的单词列表
	int level;
	Game(int l = 0) { level = l; }
	~Game() {}
	static vector<vector<string>>& getWordList() { return wordList; }
	static void InitFromFile(SOCKET sockClient);
	int Display(SOCKET sockClient);
	int getLevel() { return level; }
	void setLevel(int a) { level = a; }
};
