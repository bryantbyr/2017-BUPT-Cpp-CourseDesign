#pragma once
#include "User.h"
#include "common.h"

class TestBuilder :
	public User
{
private:
	int problemsNumbers;
	int testBuilderLevel;
	static bool isLogin;
	static vector <TestBuilder> testBuilderList;//registered player list
public:
	TestBuilder(const string& n, const string& p, int pNum = 0, int g = 0) : User(n, p)
	{
		problemsNumbers = pNum;
		testBuilderLevel = g;
	}
	~TestBuilder() {}
	static vector<TestBuilder>& getTestBuilderList() { return testBuilderList; }
	static void InitFromFile(SOCKET sockClient);
	void updateInfo(SOCKET sockClient);
	void Register(SOCKET sockClient);
	int Login(SOCKET sockClient);
	void quitLogin() { isLogin = false; }
	void addProblemsNumbers() { problemsNumbers++; }
	void addTestBuilderLevel() { testBuilderLevel += problemsNumbers / 5; }
	int getTestBuilderLevel() { return testBuilderLevel; }
	void SearchTestBuilder(SOCKET sockClient, const string& name);
	void SearchPlayer(SOCKET sockClient, const string& name);
	void SearchTestBuilderBylevel(SOCKET sockClient, int level);
	int getProNum() { return problemsNumbers; }
	void RankPlayersByProNum(SOCKET sockClient);
	int getTesterLevel() { return testBuilderLevel; }
	void RankPlayersByTesterLevel(SOCKET sockClient);
	void AddWord(SOCKET sockClient);
};

