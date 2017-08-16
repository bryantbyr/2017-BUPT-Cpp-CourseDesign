#pragma once
#include "User.h"
#include "common.h"

class Player : public User {
private:
	int playerLevel;
	int experience;
	int gainedGrades;
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
	void addLevel() { playerLevel++; }
	void addExperience() { experience++; }
	void addGainedGrades() { gainedGrades++; }
	void SearchPlayer(string& name);
	void SearchTestBuilder(string& name);
	void SearchPlayerBylevel(int level);
	void RankPlayersByGrades();
	void RankPlayersByExperience();
	void RankPlayersByBestLevel();
	void updateInfo();
	static void InitFromFile();
};
