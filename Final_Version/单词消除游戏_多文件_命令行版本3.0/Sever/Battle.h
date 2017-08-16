#pragma once
#include "Game.h"
#include "common.h"
//#include "Socket.h"

class Battle :
	public Game
{
private:
public:
	string word;//题目
	time_t t1;//客户端A挑战用时
	time_t t2;//客户端B挑战用时
	time_t timeBegin;//挑战开始时间
	SOCKET sockClientA;//A挑战者
	SOCKET sockClientB;//B挑战者
	Battle() { t1 = 1000000; t2 = 1000000; }
	~Battle() {}
	void resetTime() { t1 = 1000000; t2 = 1000000; }//重置时间参数
	void givePro();//出题函数
	int onBattle(SOCKET sockClient);//客户端答题
};

