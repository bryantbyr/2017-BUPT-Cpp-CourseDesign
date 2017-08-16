#pragma once
#include "common.h"

class Game {
private:
	int level;
	static vector<vector<string>> wordList;//µ•¥ ¡–±Ì
public:
	Game(int l = 0) { level = l; }
	~Game() {}
	static vector<vector<string>>& getWordList() { return wordList; }
	static void InitFromFile();
	int Display();
	int getLevel() { return level; }
	void setLevel(int a) { level = a; }
};

