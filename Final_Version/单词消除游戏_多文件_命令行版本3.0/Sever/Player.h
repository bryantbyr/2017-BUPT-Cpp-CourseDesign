#pragma once
#include "User.h"
#include "common.h"

class Player :
	public User
{
private:
	int playerLevel;//等级
	int experience;//经验值
	int gainedGrades;//最好成绩
	bool isLogin;
	static vector<Player> playerList;//registered player list
	SOCKET relatedSocket;
public:
	Player(const string& n, const string& p, int e = 0, int l = 0, int g = 0, bool is = false, bool isA = false) : User(n, p)
	{
		playerLevel = l;
		experience = e;
		gainedGrades = g;
		isLogin = is;
		isAccept = isA;
	}
	~Player() {}
	static vector<Player>& getList() { return playerList; }
	static void InitFromFile(SOCKET sockClient);
	void updateInfo(SOCKET sockClient);
	void Register(SOCKET sockClient);
	int Login(SOCKET sockClient);
	int getGrades() { return gainedGrades; }
	int getExperience() { return experience; }
	int getPlayerLevel() { return playerLevel; }
	SOCKET& getRelatedSocket() { return relatedSocket; }
	void quitLogin() { isLogin = false; }
	void addLevel() { playerLevel += experience / 5; }
	void addExperience(int exp) { experience += exp; }
	void setGainedGrades(int n) { gainedGrades = (n > gainedGrades) ? n : gainedGrades; }
	void SearchPlayer(SOCKET sockClient, string& name);
	void SearchPlayerBylevel(SOCKET sockClient, int level);
	void SearchTestBuilder(SOCKET sockClient, string& name);
	void RankPlayersByGrades(SOCKET sockClient);
	void RankPlayersByExperience(SOCKET sockClient);
	vector<Player> getOnlineList();

	bool isAccept;
};

