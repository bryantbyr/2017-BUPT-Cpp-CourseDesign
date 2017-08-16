#include "common.h"

class Game {
private:
	int level;
	static vector<string> wordList;//被添加的单词列表
public:
	Game(int l = 0) { level = l; }
	~Game() {}
	bool Display();
	void setLevel(int a) { level = a; }
	static vector<string>& getWordList() { return wordList; }
	static void InitFromFile();
};

