#pragma once
#include "User.h"
#include "common.h"

class Player : public User {
private:
	int playerLevel;//等级
	int experience;//经验值
	int gainedGrades;//最好成绩
	static bool isLogin;
	static vector<Player> playerList;//registered player list
public:
	Player(const string& n, const string& p, int e = 0, int l = 0, int g = 0) : User(n, p)
	{
		playerLevel = l;
		experience = e;
		gainedGrades = g;
	}
	~Player() {}
	static vector<Player>& getList() { return playerList; }
	void Register();
	int Login();
	int getGrades() { return gainedGrades; }
	int getExperience() { return experience; }
	int getPlayerLvel() { return playerLevel; }
	void quitLogin() { isLogin = false; }
	void addLevel() { playerLevel += experience / 5; } //经验值每增长5，等级加1
	void addExperience(int exp) { experience += exp; }
	void updateGainedGrades(int l) { gainedGrades = (gainedGrades < l) ? l : gainedGrades; }
	void SearchPlayer(string& name);
	void SearchPlayerBylevel(int level);
	void SearchTestBuilder(string& name);
	void RankPlayersByGrades();
	void RankPlayersByExperience();
	static void InitFromFile();
	void updateInfo();
};
