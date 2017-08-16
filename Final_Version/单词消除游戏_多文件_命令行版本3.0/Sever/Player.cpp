#include "Player.h"
#include "TestBuilder.h"
#include "split.h"


vector<Player> Player::playerList = {};
void Player::Register(SOCKET sockClient)
{
	char sendBuf[MaxSize];
	
	for (int i = 0; i < (int)playerList.size(); i++)
		if(playerList[i].getName()==this->getName()){
			strcpy_s(sendBuf, "The Player-Name has been registered. Register failed!\n");
			send(sockClient, sendBuf, strlen(sendBuf), 0);
			return;
		}
	
	ofstream outfile("playerList.csv", ios::out | ios::app);
	if (!outfile) {
		strcpy_s(sendBuf, "File playerList.csv open failed!\n");
		send(sockClient, sendBuf, strlen(sendBuf), 0);
		abort();
	}
	outfile << this->getName() << "," << this->getPassword() << "," << this->playerLevel << "," << this->experience << "," << this->gainedGrades << "      " << endl;
	outfile.close();
	playerList.push_back(*this);
	strcpy_s(sendBuf, "Player successfully registers!\n");
	send(sockClient, sendBuf, strlen(sendBuf), 0);
}
int Player::Login(SOCKET sockClient)
{
	char sendBuf[MaxSize];
	for (int i = 0; i < (int)playerList.size(); i++)
		if (playerList[i].getName() == this->getName() && playerList[i].getPassword() == this->getPassword()) {
			strcpy_s(sendBuf, "Player successfully logins!\n");
			Sleep(20);
			send(sockClient, sendBuf, strlen(sendBuf), 0);
			playerList[i].isLogin = true;
			playerList[i].relatedSocket = sockClient;
			return i;
		}
	strcpy_s(sendBuf, "Player-login failed!\n");
	Sleep(10);
	send(sockClient, sendBuf, strlen(sendBuf), 0);
	return -1;
}
void Player::SearchPlayer(SOCKET sockClient, string& name)
{
	vector<Player> v = Player::getList();
	for (unsigned int i = 0; i < v.size(); i++)
		if (v[i].getName() == name) {
			sendToClient(sockClient, "It is the ");
			Sleep(10);
			char l[MaxSize];
			_itoa_s(i + 1, l, 10);
			sendToClient(sockClient, l);
			Sleep(10);
			sendToClient(sockClient, "th player\n");
			return;
		}
	sendToClient(sockClient, "It is not exited in the playerList.\n");
}
void Player::SearchPlayerBylevel(SOCKET sockClient, int level)
{
	vector<Player> v = Player::getList();
	for (unsigned int i = 0; i < v.size(); i++)
		if (v[i].getPlayerLevel() >= level) {
			sendToClient(sockClient, "The ");
			Sleep(10);
			char l[MaxSize];
			_itoa_s(i + 1, l, 10);
			sendToClient(sockClient, l);
			Sleep(10);
			sendToClient(sockClient, "th player: ");
			Sleep(10);
			sendToClient(sockClient, v[i].getName().c_str());
			Sleep(10);
			sendToClient(sockClient, "\n");
		}
}
void Player::SearchTestBuilder(SOCKET sockClient, string& name)
{
	vector<TestBuilder> v = TestBuilder::getTestBuilderList();
	for (unsigned int i = 0; i < v.size(); i++)
		if (v[i].getName() == name) {
			sendToClient(sockClient, "It is the ");
			Sleep(10);
			char l[MaxSize];
			_itoa_s(i + 1, l, 10);
			sendToClient(sockClient, l);
			Sleep(10);
			sendToClient(sockClient, "th tester\n");
			return;
		}
	sendToClient(sockClient, "It is not exited in the testBuilderList.\n");
}
bool sortByGrades(Player a, Player b)
{
	return a.getGrades() > b.getGrades();
}
void Player::RankPlayersByGrades(SOCKET sockClient)
{
	vector<Player> v = Player::getList();
	sort(v.begin(), v.end(), sortByGrades);
	for (unsigned int i = 0; i < v.size(); i++) {
		char l[MaxSize];
		_itoa_s(i + 1, l, 10);
		sendToClient(sockClient, l);
		Sleep(10);
		sendToClient(sockClient, "th   ");
		Sleep(10);
		sendToClient(sockClient, v[i].getName().c_str());
		Sleep(10);
		sendToClient(sockClient, " with gainedGrades is: ");
		Sleep(10);
		_itoa_s(v[i].getGrades(), l, 10);
		sendToClient(sockClient, l);
		Sleep(10);
		sendToClient(sockClient, "\n");
	}
}
bool sortByExperience(Player a, Player b)
{
	return a.getExperience() > b.getExperience();
}
void Player::RankPlayersByExperience(SOCKET sockClient)
{
	vector<Player> v = Player::getList();
	sort(v.begin(), v.end(), sortByGrades);
	for (unsigned int i = 0; i < v.size(); i++) {
		char l[MaxSize];
		_itoa_s(i + 1, l, 10);
		sendToClient(sockClient, l);
		Sleep(10);
		sendToClient(sockClient, "th   ");
		Sleep(10);
		sendToClient(sockClient, v[i].getName().c_str());
		Sleep(10);
		sendToClient(sockClient, " with experience is: ");
		Sleep(10);
		_itoa_s(v[i].getGrades(), l, 10);
		sendToClient(sockClient, l);
		Sleep(10);
		sendToClient(sockClient, "\n");
	}
}
void Player::InitFromFile(SOCKET sockClient)
{
	string line;
	vector<string> v;
	ifstream infile("playerList.csv");
	if (!infile) {
		sendToClient(sockClient, "File playerList.csv open failed!\n");
		abort();
	}
	getline(infile, line);
	while (!infile.eof())
	{
		getline(infile, line);
		if (line == "")
			break;
		v = split(line, ",");
		Player player(v[0], v[1], atoi(v[2].c_str()), atoi(v[3].c_str()), atoi(v[4].c_str()));
		playerList.push_back(player);
	}
	infile.close();
}

void Player::updateInfo(SOCKET sockClient)
{
	fstream outfile("playerList.csv", ios::out | ios::in);
	if (!outfile) {
		sendToClient(sockClient, "File playerList.csv open failed!\n");
		abort();
	}
	string line;
	vector<string> v;
	outfile.seekp(0);
	getline(outfile, line);
	streamoff cur_tmp = outfile.tellg();
	while (!outfile.eof())
	{
		getline(outfile, line);
		v = split(line, ",");
		if (strcmp(this->getName().c_str(), v[0].c_str()) == 0) {
			outfile.seekp(cur_tmp);
			//outfile.seekp( 0, ios::cur );
			outfile << this->getName() << "," << this->getPassword() << "," << this->playerLevel << "," << this->experience << "," << this->gainedGrades;
			outfile.seekp(cur_tmp);
			getline(outfile, line);
			break;
		}
		cur_tmp = outfile.tellg();
	}
	outfile.close();
}
vector<Player> Player::getOnlineList()
{
	vector<Player> onlintList;
	for (unsigned int i = 0; i < playerList.size(); i++) {
		if (playerList[i].isLogin)
			onlintList.push_back(playerList[i]);
	}
	return onlintList;
}
