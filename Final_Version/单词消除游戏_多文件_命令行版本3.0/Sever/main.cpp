#include "common.h"
#include "Game.h"
#include "Player.h"
#include "TestBuilder.h"
#include "Battle.h"


Battle battle;

HANDLE hMutex;
void SendAndRec(SOCKET sockClient)
{
	int choice = 1;

	char sendBuf[MaxSize];
	char revBuf[MaxSize];
	int byte = 0;
	while (1)
	{
		WaitForSingleObject(hMutex, INFINITE);
		while (choice != 0) {
			strcpy_s(sendBuf, "             Welcome to word elemination system !\n                        1  register\n                        2  plogin\n                        3  tlogin\n                        0  quit\n");
			byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
			if (byte <= 0)
				cout << "Send error" << endl;
			do {
				int n = recv(sockClient, revBuf, MaxSize, 0);
				if (n < 0)
					cout << "Recv error" << endl;
				choice = revBuf[0] - '0';
				if (choice < 0 || choice > 3) {
					strcpy_s(sendBuf, "Input is wrong, please input again!\n");
					byte = send(sockClient, sendBuf, strlen(sendBuf), 0); //服务器向客户端发送数据
				}
				else
					break;
			} while (1);
			switch (choice) {
			case 1: {
				strcpy_s(sendBuf, "Please set your username: ");
				byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
				string name;
				int n = recv(sockClient, revBuf, MaxSize, 0);
				revBuf[n] = '\0';
				while (n < 0)
					cout << "Rec Error" << endl;
				name = revBuf;

				strcpy_s(sendBuf, "Please set your account password: ");
				byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
				string password;
				n = recv(sockClient, revBuf, MaxSize, 0);
				while (n < 0)
					cout << "Revc error" << endl;
				password = revBuf;
				if (name[0] == 't') {
					TestBuilder tester(name, password);
					tester.Register(sockClient);
				}
				else {
					Player player(name, password);
					player.Register(sockClient);
				}
			}
					break;
					/**
					* player operation page
					*/
			case 2: {
				string name;
				strcpy_s(sendBuf, "Please input username:");
				byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
				int n = recv(sockClient, revBuf, MaxSize, 0);
				revBuf[n] = '\0';
				while (n < 0)
					cout << "Rec Error" << endl;
				name = revBuf;

				string password;
				strcpy_s(sendBuf, "Please input password:");
				byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
				n = recv(sockClient, revBuf, MaxSize, 0);
				revBuf[n] = '\0';
				while (n < 0)
					cout << "Rec Error" << endl;
				password = revBuf;

				Player player(name, password);
				int pos = player.Login(sockClient);
				if (pos != -2) {
					while (pos == -1) {
						string name;
						strcpy_s(sendBuf, "Username or password is wrong,please input them again:\n Please input username:");
						byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
						int n = recv(sockClient, revBuf, MaxSize, 0);
						revBuf[n] = '\0';
						if (n < 0)
							cout << "Rec Error" << endl;
						name = revBuf;

						string password;
						strcpy_s(sendBuf, "Please input password:");
						byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
						n = recv(sockClient, revBuf, MaxSize, 0);
						revBuf[n] = '\0';
						if (n < 0)
							cout << "Rec Error" << endl;
						password = revBuf;

						Player player(name, password);
						pos = player.Login(sockClient);
					}
					Player& tempPlayer = (Player&)Player::getList()[pos];
					int playerChoice = 1;
					Game game;
					while (playerChoice != 0) {
						sendToClient(sockClient, "\n              ******  Welcome  to  playing  page  ****** ");
						Sleep(10);
						sendToClient(sockClient, "\n                  1  select the difficulty of the game");
						Sleep(10);
						sendToClient(sockClient, "\n                  2  start the game");
						Sleep(10);
						sendToClient(sockClient, "\n                  3  next level ");
						Sleep(10);
						sendToClient(sockClient, "\n                  4  search user ");
						Sleep(10);
						sendToClient(sockClient, "\n                  5  rank playerList ");
						Sleep(10);
						sendToClient(sockClient, "\n                  6  find all the players online ");
						Sleep(10);
						sendToClient(sockClient, "\n                  7  accept battle ");
						Sleep(10);
						sendToClient(sockClient, "\n                  8  refuse battle ");
						Sleep(10);
						sendToClient(sockClient, "\n                  0  quit playing  ");
						Sleep(10);
						sendToClient(sockClient, "\n              ******************************************\n");
						do {
							int n = recv(sockClient, revBuf, MaxSize, 0);
							if (n < 0)
								cout << "Recv error" << endl;
							playerChoice = revBuf[0] - '0';

							if (playerChoice < 0 || playerChoice > 8) {
								strcpy_s(sendBuf, "Input is wrong, please input again!");
								byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
							}
							else
								break;
						} while (1);
						switch (playerChoice) {
						case 1: {
							int l = 0;
							sendToClient(sockClient, "Please choose the difficulty (0-10):");
							int n = recv(sockClient, revBuf, MaxSize, 0);
							if (n < 0)
								cout << "Recv error" << endl;
							l = revBuf[0] - '0';
							game.setLevel(l);
						}
								break;
						case 2: {
							int b = game.Display(sockClient);
							if (b > 0) {
								tempPlayer.setGainedGrades(game.getLevel());
								tempPlayer.addExperience(b);
								tempPlayer.addLevel();
							}
						}
								break;
						case 3: {
							int b = game.Display(sockClient);
							if (b > 0) {
								tempPlayer.setGainedGrades(game.getLevel());
								tempPlayer.addExperience(b);
								tempPlayer.addLevel();
							}
						}
								break;
								/**
								* 查询用户
								*/
						case 4: {
							sendToClient(sockClient, "Please choose the way to search user:\n");
							Sleep(10);
							sendToClient(sockClient, "              a username  \n              b userLevel    \n");
							char rankChoice;
							int n = recv(sockClient, revBuf, MaxSize, 0);
							if (n < 0)
								cout << "error" << endl;
							rankChoice = revBuf[0];
							if (rankChoice == 'a') {
								sendToClient(sockClient, "Please input the username:");
								string name;
								n = recv(sockClient, revBuf, MaxSize, 0);
								if (n < 0)
									cout << "Rec Error" << endl;
								revBuf[n] = '\0';
								name = revBuf;
								tempPlayer.SearchPlayer(sockClient, name);
								tempPlayer.SearchTestBuilder(sockClient, name);
							}
							else {
								sendToClient(sockClient, "Please input the Player's level:");
								int level;
								n = recv(sockClient, revBuf, MaxSize, 0);
								if (n < 0)
									cout << "Rec Error" << endl;
								revBuf[n] = '\0';
								level = atoi(revBuf);
								tempPlayer.SearchPlayerBylevel(sockClient, level);
							}

						}
								break;
						case 5: {
							sendToClient(sockClient, "\n      Please choose the characters to rank the Player list:\n");
							sendToClient(sockClient, "\n             a gainedGrades  \n              b experience    \n");
							char rankChoice;
							int n = recv(sockClient, revBuf, MaxSize, 0);
							if (n < 0)
								cout << "error" << endl;
							rankChoice = revBuf[0];
							if (rankChoice == 'a')
								tempPlayer.RankPlayersByGrades(sockClient);
							else
								tempPlayer.RankPlayersByExperience(sockClient);
						}
								break;
								/**
								* 查询当前时间所有在线用户并选择是否进入对战界面
								*/
						case 6: {
							vector<Player> onList;
							onList = tempPlayer.getOnlineList();
							sendToClient(sockClient, "The onLine player list is:\n");
							Sleep(10);
							for (unsigned int i = 0; i < onList.size(); i++) {
								sendToClient(sockClient, onList[i].getName().c_str());
								Sleep(10);
								sendToClient(sockClient, "\n");
								Sleep(10);
							}
							sendToClient(sockClient, "If you want to battle with someone online,please input 'y';If don't,input 'n':\n");
							n = recv(sockClient, revBuf, MaxSize, 0);
							revBuf[n] = '\0';
							if (n < 0)
								cout << "Rec Error" << endl;
							if (revBuf[0] == 'y') {
								sendToClient(sockClient, "Please input its username:\n");
								int n = recv(sockClient, revBuf, MaxSize, 0);
								revBuf[n] = '\0';
								if (n < 0)
									cout << "Rec Error" << endl;
								string username = revBuf;
								unsigned int i = 0;
								for (; i < onList.size(); i++) {
									if (username == onList[i].getName())
										break;
								}
								battle.sockClientA = sockClient;
								battle.sockClientB = onList[i].getRelatedSocket();
								sendToClient(onList[i].getRelatedSocket(), tempPlayer.getName().c_str());
								Sleep(10);
								sendToClient(onList[i].getRelatedSocket(), " want to battle with you,do you agree(Make a reply in 8 seconds,'7'or'8'):\n");
								Sleep(8000);
								unsigned int k = 0;
								for (; k < Player::getList().size(); k++) {
									if (Player::getList()[k].getName() == onList[i].getName()) {
										onList[i].isAccept = Player::getList()[k].isAccept;
										break;
									}
								}
								if (onList[i].isAccept) {
									Player& oponent = (Player&)Player::getList()[k];
									oponent.isAccept = false;
									battle.resetTime();
									sendToClient(onList[i].getRelatedSocket(), "The battle will start after 3 seconds!\n");
									sendToClient(sockClient, "The oponent has agreed,the battle will start after 3 seconds!\n");
									Sleep(3000);
									battle.givePro();
									battle.timeBegin = time(NULL);
									int a = battle.onBattle(sockClient);
									if (a == 1)
										tempPlayer.addExperience(10);
									else if (a == 2)
										tempPlayer.addExperience(-2);
									else
										tempPlayer.addExperience(0);
									tempPlayer.addLevel();
								}
								else {
									sendToClient(sockClient, "The oponent refused your request!\n");
								}
							}
						}
								break;
								/**
								* 被挑战者应战界面
								*/
						case 7: {
							tempPlayer.isAccept = true;
							Sleep(3000);
							int a = battle.onBattle(sockClient);
							if (a == 2)
								tempPlayer.addExperience(10);
							else if (a == 1)
								tempPlayer.addExperience(-1);
							else
								tempPlayer.addExperience(0);
							tempPlayer.addLevel();
						}
								break;
						}
					}
					tempPlayer.updateInfo(sockClient);
					tempPlayer.quitLogin();
				}
				else
					sendToClient(sockClient, "There is others online. Please wait for a while.");
			}
					break;
					/**
					* tester operation page
					*/
			case 3: {
				sendToClient(sockClient, "Please input username:");
				string name;
				int n = recv(sockClient, revBuf, MaxSize, 0);
				revBuf[n] = '\0';
				if (n < 0)
					cout << "Rec Error" << endl;
				name = revBuf;
				string password;
				sendToClient(sockClient, "Please input password:");
				n = recv(sockClient, revBuf, MaxSize, 0);
				revBuf[n] = '\0';
				if (n < 0)
					cout << "Rec Error" << endl;
				password = revBuf;
				TestBuilder tester(name, password);
				int pos = tester.Login(sockClient);
				if (pos != -2) {
					while (pos == -1) {
						sendToClient(sockClient, "Username or password is wrong,please input them again:\n");
						sendToClient(sockClient, "Please input username again:");
						string name;
						int n = recv(sockClient, revBuf, MaxSize, 0);
						revBuf[n] = '\0';
						if (n < 0)
							cout << "Rec Error" << endl;
						name = revBuf;
						sendToClient(sockClient, "Please input password again:");
						string password;
						n = recv(sockClient, revBuf, MaxSize, 0);
						revBuf[n] = '\0';
						if (n < 0)
							cout << "Rec Error" << endl;
						password = revBuf;
						TestBuilder tester(name, password);
						pos = tester.Login(sockClient);
					}
					TestBuilder& tempTester = (TestBuilder&)TestBuilder::getTestBuilderList()[pos];
					tempTester.addTestBuilderLevel();
					int testerChoice = 1;
					while (testerChoice != 0) {
						sendToClient(sockClient, "\n\n            ********************************");
						sendToClient(sockClient, "\n                Welcome to management page ! ");
						Sleep(10);
						sendToClient(sockClient, "\n                 1  build new tester case");
						Sleep(10);
						sendToClient(sockClient, "\n                 2  search user");
						Sleep(10);
						sendToClient(sockClient, "\n                 3  rank testBuilderList ");
						Sleep(10);
						sendToClient(sockClient, "\n                 0  quit management page ");
						sendToClient(sockClient, "\n             ********************************\n");
						do {
							int n = recv(sockClient, revBuf, MaxSize, 0);
							if (n < 0)
								cout << "Revc error" << endl;
							testerChoice = revBuf[0] - '0';
							if (testerChoice < 0 || testerChoice > 3)
								sendToClient(sockClient, "Input is wrong，please input again!");
							else
								break;
						} while (1);
						switch (testerChoice) {
							/**
							* 新增单词
							*/
						case 1:
							tempTester.AddWord(sockClient);
							tempTester.addProblemsNumbers();
							break;
							/**
							* 查询
							*/
						case 2: {
							sendToClient(sockClient, "Please choose the way to search user:\n");
							Sleep(10);
							sendToClient(sockClient, "              a username  \n              b userLevel    \n");
							char rankChoice;
							int n = recv(sockClient, revBuf, MaxSize, 0);
							if (n < 0)
								cout << "error" << endl;
							rankChoice = revBuf[0];
							if (rankChoice == 'a') {
								sendToClient(sockClient, "Please input the username:");
								string name;
								n = recv(sockClient, revBuf, MaxSize, 0);
								if (n < 0)
									cout << "Rec Error" << endl;
								revBuf[n] = '\0';
								name = revBuf;
								tempTester.SearchPlayer(sockClient, name);
								tempTester.SearchTestBuilder(sockClient, name);
							}
							else {
								sendToClient(sockClient, "Please input the tester's level:");
								int level;
								n = recv(sockClient, revBuf, MaxSize, 0);
								if (n < 0)
									cout << "Rec Error" << endl;
								revBuf[n] = '\0';
								level = atoi(revBuf);
								tempTester.SearchTestBuilderBylevel(sockClient, level);
							}
						}
								break;
								/**
								* 排行榜
								*/
						case 3: {
							sendToClient(sockClient, "\n      Please choose the characters to rank the Tester list:\n");
							sendToClient(sockClient, "             a problemsNumbers  \n             b testBuilderLevel    \n");
							char rankChoice;
							int n = recv(sockClient, revBuf, MaxSize, 0);
							revBuf[n] = '\0';
							if (n < 0)
								cout << "Rec Error" << endl;
							rankChoice = revBuf[0];
							if (rankChoice == 'a')
								tempTester.RankPlayersByProNum(sockClient);
							else
								tempTester.RankPlayersByTesterLevel(sockClient);
						}
								break;
						}
					}
					tempTester.updateInfo(sockClient);
					tempTester.quitLogin();
				}
				else
					sendToClient(sockClient, "There is others online. Please wait for a while.");
			}
					break;
			}
		}
		if (choice == 0) {
			sendToClient(sockClient, "Goodbye, welcome to come again!\n");
			getchar();
		}
	}
	closesocket(sockClient);
}

int main()
{
	SOCKADDR_IN addrServer;
	int sockServer;

	if (SOCKET_ERROR == SocketInit())
		return -1;

	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);    //htol将主机字节序long型转换为网络字节序
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(6666);                      //htos用来将端口转换成字符，1024以上的数字即可

	sockServer = socket(AF_INET, SOCK_STREAM, 0);           //面向连接的可靠性服务SOCK_STRAM
	bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR)); //将socket绑定到相应地址和端口上
	listen(sockServer, 5);                                  //等待队列中的最大长度为5

	printf("Welcome,the Host %s is running!Now Wating for someone comes in!\n", inet_ntoa(addrServer.sin_addr));

	int len = sizeof(SOCKADDR);

	SOCKADDR_IN addrClient;

	int key = 0;

	while (1)
	{
		SOCKET sockClient = accept(sockServer, (SOCKADDR*)&addrClient, &len); //阻塞调用进程直至新的连接出现

		if (sockClient == INVALID_SOCKET)
		{
			printf("Accept Failed!\n");
			continue; //继续监听
		}

		cout << "New Client Joins In.\n";
		if (key == 0) {
			Game::InitFromFile(sockClient);
			Player::InitFromFile(sockClient);
			TestBuilder::InitFromFile(sockClient);
			key = 1;
		}

		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendAndRec, (LPVOID)sockClient, 0, 0);

		if (hThread != NULL)
			CloseHandle(hThread);
		Sleep(100);

	}

	getchar();
	return 0;
}
