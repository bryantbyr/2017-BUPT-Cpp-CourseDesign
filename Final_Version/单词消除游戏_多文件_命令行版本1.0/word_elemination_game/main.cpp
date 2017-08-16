#include "common.h"
#include "Game.h"
#include "User.h"
#include "Player.h"
#include "TestBuilder.h"

int main()
{
	int choice = 1;
	Game::InitFromFile();
	Player::InitFromFile();
	TestBuilder::InitFromFile();
	while (choice != 0) {
		cout << "\n\n\n" << "                    **********************************          " << endl;
		cout << "                       Welcome to word elemination system ! " << endl;
		cout << "\n                                 1  user-register(Note:TesterBuilder's name starts as't_')" << endl;//-�û�ע��
		cout << "\n                                 2  player-login" << endl;//-��ҵ�½
		cout << "\n                                 3  tester-login" << endl;//-�����ߵ�½
		cout << "\n                                 0  quit" << endl;//-�˳�ϵͳ
		cout << "              ****************************************************         " << endl;
		do {
			cin >> choice;
			if (choice < 0 || choice > 8)
				cout << "Input is wrong��please input again!" << endl;
			else
				break;
		} while (1);
		switch (choice) {
		case 1: {
			cout << "Please set your username:";
			string name;
			cin >> name;
			cout << "Please set your account password:";
			string password;
			cin >> password;
			if (name[0] == 't') {
				TestBuilder tester(name, password);
				tester.Register();
			}
			else {
				Player player(name, password);
				player.Register();
			}
		}
				break;
				/**
				* player operation page
				*/
		case 2: {
			cout << "Please input username:";
			string name;
			cin >> name;
			cout << "Please input password:";
			string password;
			cin >> password;
			Player player(name, password);
			int pos = player.Login();
			if (pos != -2) {
				while (pos == -1) {
					cout << "Username or password is wrong,please input them again:" << endl;
					cout << "Please input username:";
					string name;
					cin >> name;
					cout << "Please input password:";
					string password;
					cin >> password;
					Player player(name, password);
					pos = player.Login();
				}
				Player& tempPlayer = (Player&)Player::getList()[pos];
				int playerChoice = 1;
				Game game;
				while (playerChoice != 0) {
					cout << "\n\n\n\n" << "      *************************" << endl;
					cout << "                Welcome to playing page ! " << endl;
					cout << "\n                 1  select the difficulty of the game" << endl;//-��ѡ���Ѷ���Ϸ�Ѷ�
					cout << "\n                 2  start the game" << endl;// -��ʼ��Ϸ
					cout << "\n                 3  next level" << endl;// -��һ��
					cout << "\n                 4  search user " << endl;//-��ѯ
					cout << "\n                 5  rank playerList" << endl;// -���а�
					cout << "\n                 0  quit playing page" << endl;// -�˳���Ϸ����
					cout << "              ************************" << endl;
					do {
						cin >> playerChoice;
						if (playerChoice < 0 || playerChoice > 5)
							cout << "Input is wrong��please input again!" << endl;
						else
							break;
					} while (1);
					switch (playerChoice) {
					case 1: {
						int l = 0;
						cout << "Please choose the difficulty (0-10):" << endl;
						cin >> l;
						game.setLevel(l);
					}
							break;
					case 2:
						if (game.Display()) {
							tempPlayer.addGainedGrades();
							tempPlayer.addLevel();
						}
						tempPlayer.addExperience();

						break;
					case 3:
						if (game.Display()) {
							tempPlayer.addGainedGrades();
							tempPlayer.addLevel();
						}
						tempPlayer.addExperience();

						break;
					case 4: {
						cout << "Please choose the way to search user:" << endl;
						cout << "              a username  \n              b userLevel    \n" << endl;
						char rankChoice;
						cin >> rankChoice;
						if (rankChoice == 'a') {
							cout << "Please input the username:" << endl;
							string name;
							cin >> name;
							tempPlayer.SearchPlayer(name);
							tempPlayer.SearchTestBuilder(name);
						}
						else {
							cout << "Please input the Player's level:" << endl;
							int level;
							cin >> level;
							tempPlayer.SearchPlayerBylevel(level);
						}
					}
							break;
					case 5: {
						cout << "      Please choose the characters to rank the Player list:\n"
							<< "              a gainedGrades  \n"
							<< "              b experience    \n"
							<< "              c best level    \n";
						char rankChoice;
						cin >> rankChoice;
						if (rankChoice == 'a')
							tempPlayer.RankPlayersByGrades();
						else if (rankChoice == 'b')
							tempPlayer.RankPlayersByExperience();
						else
							tempPlayer.RankPlayersByBestLevel();
					}
							break;
					}
				}
				tempPlayer.updateInfo();
				tempPlayer.quitLogin();
			}
			else
				cout << "There is others online. Please wait for a while.";
		}
				break;
				/**
				* tester operation page
				*/
		case 3: {
			cout << "Please input username:";
			string name;
			cin >> name;
			cout << "Please input password:";
			string password;
			cin >> password;
			TestBuilder tester(name, password);
			int pos = tester.Login();
			if (pos != -2) {
				while (pos == -1) {
					cout << "Username or password is wrong,please input them again:" << endl;
					cout << "Please input username again:";
					string name;
					cin >> name;
					cout << "Please input password again:";
					string password;
					cin >> password;
					TestBuilder tester(name, password);
					pos = tester.Login();
				}
				TestBuilder& tempTester = (TestBuilder&)TestBuilder::getTestBuilderList()[pos];
				tempTester.addTestBuilderLevel();
				int testerChoice = 1;
				while (testerChoice != 0) {
					cout << "\n\n\n\n" << "      *************************" << endl;
					cout << "                Welcome to management page ! " << endl;
					cout << "\n                 1  build new tester case" << endl;//-����
					cout << "\n                 2  search user" << endl;// -��ѯ
					cout << "\n                 3  rank testBuilderList " << endl;//-���а�
					cout << "\n                 0  quit management page " << endl;//-�˳��������
					cout << "              ************************" << endl;
					do {
						cin >> testerChoice;
						if (testerChoice < 0 || testerChoice > 3)
							cout << "Input is wrong��please input again!" << endl;
						else
							break;
					} while (1);
					switch (testerChoice) {
					case 1:
						tempTester.AddWord();
						tempTester.addProblemsNumbers();
						break;
					case 2: {
						cout << "Please choose the way to search user:" << endl;
						cout << "              a username  \n              b userLevel    \n" << endl;
						char rankChoice;
						cin >> rankChoice;
						if (rankChoice == 'a') {
							cout << "Please input the username:" << endl;
							string name;
							cin >> name;
							tempTester.SearchPlayer(name);
							tempTester.SearchTestBuilder(name);
						}
						else {
							cout << "Please input the tester's level:" << endl;
							int level;
							cin >> level;
							tempTester.SearchTesterBylevel(level);
						}
					}

							break;
					case 3: {
						cout << "      Please choose the characters to rank the Tester list:\n"
							<< "              a problemsNumbers  \n"
							<< "              b testBuilderLevel    \n";
						char rankChoice;
						cin >> rankChoice;
						if (rankChoice == 'a')
							tempTester.RankPlayersByProNum();
						else
							tempTester.RankPlayersByTesterLevel();
					}
							break;
					}
				}
				tempTester.updateInfo();
				tempTester.quitLogin();
			}
			else
				cout << "There is others online. Please wait for a while.";
		}
				break;
		}
	}
	return 0;
}