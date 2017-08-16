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
	static vector<TestBuilder> testBuilderList;//registered player list
public:
	TestBuilder(const string& n, const string& p, int pNum = 0, int g = 0) : User(n, p)
	{
		problemsNumbers = pNum;
		testBuilderLevel = g;
	}
	~TestBuilder() {}
	static vector<TestBuilder>& getTestBuilderList() { return testBuilderList; }
	void Register();
	int Login();
	void quitLogin() { isLogin = false; }
	void addProblemsNumbers() { problemsNumbers++; }
	void addTestBuilderLevel() { testBuilderLevel++; }
	void SearchTestBuilder(const string& name);
	void SearchPlayer(const string& name);
	void SearchTesterBylevel(int level);
	int getProNum() { return problemsNumbers; }
	void RankPlayersByProNum();
	int getTesterLevel() { return testBuilderLevel; }
	void RankPlayersByTesterLevel();
	void AddWord();
	static void InitFromFile();
	void updateInfo();
};

