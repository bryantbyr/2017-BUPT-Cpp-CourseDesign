#include "common.h"

class Game {
private:
	int level;
	static vector<string> wordList;//����ӵĵ����б�
public:
	Game(int l = 0) { level = l; }
	~Game() {}
	bool Display();
	void setLevel(int a) { level = a; }
	static vector<string>& getWordList() { return wordList; }
	static void InitFromFile();
};

