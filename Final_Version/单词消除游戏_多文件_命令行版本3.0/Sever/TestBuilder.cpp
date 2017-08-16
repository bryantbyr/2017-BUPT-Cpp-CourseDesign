#include "TestBuilder.h"
#include "Player.h"
#include "Game.h"
#include "split.h"

vector<TestBuilder> TestBuilder::testBuilderList = {};
bool TestBuilder::isLogin = 0;
void TestBuilder::Register(SOCKET sockClient)
{
	char sendBuf[MaxSize];
	
	for (int i = 0; i < (int)testBuilderList.size(); i++)
		if(testBuilderList[i].getName()==this->getName()){
			strcpy_s(sendBuf, "The Tester-Name has been registered. Register failed!\n");
			send(sockClient, sendBuf, strlen(sendBuf), 0);
			return;
		}
	
	ofstream outfile("testerList.csv", ios::out | ios::app);
	outfile << this->getName() << "," << this->getPassword() << "," << this->problemsNumbers << "," << this->testBuilderLevel << "     " << endl;
	if (!outfile) {
		strcpy_s(sendBuf, "File playerList.csv open failed!");
		send(sockClient, sendBuf, strlen(sendBuf), 0);
		abort();
	}
	outfile.close();
	testBuilderList.push_back(*this);
	strcpy_s(sendBuf, "Tester successfully registers! \n");
	send(sockClient, sendBuf, strlen(sendBuf), 0);
}
int TestBuilder::Login(SOCKET sockClient)
{
	char sendBuf[MaxSize];
	for (int i = 0; i < (int)testBuilderList.size(); i++)
		if (testBuilderList[i].getName() == this->getName() && testBuilderList[i].getPassword() == this->getPassword()) {
			sendToClient(sockClient, "Tester successfully logins!\n");
			Sleep(100);
			isLogin = true;
			return i;
		}
	strcpy_s(sendBuf, "Tester-login failed!\n");
	Sleep(10);
	send(sockClient, sendBuf, strlen(sendBuf), 0);
	return -1;
}
void TestBuilder::SearchTestBuilder(SOCKET sockClient, const string& name)
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
void TestBuilder::SearchPlayer(SOCKET sockClient, const string& name)
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
void TestBuilder::SearchTestBuilderBylevel(SOCKET sockClient, int level)
{
	vector<TestBuilder> v = TestBuilder::getTestBuilderList();
	for (unsigned int i = 0; i < v.size(); i++)
		if (v[i].getTestBuilderLevel() >= level) {
			sendToClient(sockClient, "The ");
			Sleep(10);
			char l[MaxSize];
			_itoa_s(i + 1, l, 10);
			sendToClient(sockClient, l);
			Sleep(10);
			sendToClient(sockClient, "th tester: ");
			Sleep(10);
			sendToClient(sockClient, v[i].getName().c_str());
			Sleep(10);
			sendToClient(sockClient, "\n");
		}
}

bool sortByProNum(TestBuilder a, TestBuilder b)
{
	return a.getProNum() > b.getProNum();
}
void TestBuilder::RankPlayersByProNum(SOCKET sockClient)
{
	vector<TestBuilder> v = TestBuilder::getTestBuilderList();
	sort(v.begin(), v.end(), sortByProNum);
	for (unsigned int i = 0; i < v.size(); i++) {
		char l[MaxSize];
		_itoa_s(i + 1, l, 10);
		sendToClient(sockClient, l);
		Sleep(10);
		sendToClient(sockClient, "th   ");
		Sleep(10);
		sendToClient(sockClient, v[i].getName().c_str());
		Sleep(10);
		sendToClient(sockClient, " with problemsNumbers is: ");
		Sleep(10);
		_itoa_s(v[i].getProNum(), l, 10);
		sendToClient(sockClient, l);
		Sleep(10);
		sendToClient(sockClient, "\n");
	}
}
bool sortByLevel(TestBuilder a, TestBuilder b)
{
	return a.getTesterLevel() > b.getTesterLevel();
}
void TestBuilder::RankPlayersByTesterLevel(SOCKET sockClient)
{
	vector<TestBuilder> v = TestBuilder::getTestBuilderList();
	sort(v.begin(), v.end(), sortByLevel);
	for (unsigned int i = 0; i < v.size(); i++) {
		char l[MaxSize];
		_itoa_s(i + 1, l, 10);
		sendToClient(sockClient, l);
		Sleep(10);
		sendToClient(sockClient, "th   ");
		Sleep(10);
		sendToClient(sockClient, v[i].getName().c_str());
		Sleep(10);
		sendToClient(sockClient, " with testBuilderLevel is: ");
		_itoa_s(v[i].getTesterLevel(), l, 10);
		sendToClient(sockClient, l);
		Sleep(10);
		sendToClient(sockClient, "\n");
	}
}
void TestBuilder::AddWord(SOCKET sockClient)
{
	string s;
	char revBuf[MaxSize];
	sendToClient(sockClient, "Please input newly added word:\n");
	int n = recv(sockClient, revBuf, MaxSize, 0);
	if (n < 0)
		cout << "error" << endl;
	revBuf[n] = '\0';
	s = revBuf;
	unsigned int i = 0;
	for (; i < Game::getWordList().size(); i++)
		if (s.size() < Game::getWordList()[i][0].size())
			break;
	if (Game::getWordList()[i - 1][0].size() == s.size()) {
		for (unsigned int m = 0; m < Game::getWordList()[i - 1].size(); m++) {
			if (Game::getWordList()[i - 1][m] == s) {
				sendToClient(sockClient, "The word is already exited! Add word failed!\n");
				return;
			}
		}
		Game::getWordList()[i - 1].push_back(s);
		string n;
		vector<string> s1;
		vector<string> s2;
		ifstream infile("wordList.csv");
		if (!infile) {
			sendToClient(sockClient, "File wordList.csv open failed!\n");
			abort();
		}
		while (!infile.eof())
		{
			infile >> n;
			if (s.size() < n.size())
				s2.push_back(n);
			else
				s1.push_back(n);
		}
		infile.close();
		ofstream outfile("wordList.csv");
		if (!outfile) {
			sendToClient(sockClient, "File wordList.csv open failed!\n");
			abort();
		}
		for (unsigned int i = 0; i < s1.size(); i++)
			outfile << s1[i] << endl;
		outfile << s;
		if (s2.size() > 0)
			outfile << endl;
		unsigned int k = 0;
		for (; k < s2.size() - 1; k++)
			outfile << s2[k] << endl;
		outfile << s2[k];
		infile.close();
	}
	else if (i < Game::getWordList().size()) {
		vector <string> add;
		add.push_back(s);
		Game::getWordList().insert(Game::getWordList().begin() + i, add);
		string n;
		vector<string> s1;
		vector<string> s2;
		ifstream infile("wordList.csv");
		if (!infile) {
			sendToClient(sockClient, "File wordList.csv open failed!\n");
			abort();
		}
		while (!infile.eof())
		{
			infile >> n;
			if (s.size() < n.size())
				s2.push_back(n);
			else
				s1.push_back(n);
		}
		infile.close();
		ofstream outfile("wordList.csv");
		if (!outfile) {
			sendToClient(sockClient, "File wordList.csv open failed!\n");
			abort();
		}
		for (unsigned int i = 0; i < s1.size(); i++)
			outfile << s1[i] << endl;
		outfile << s;
		if (s2.size() > 0)
			outfile << endl;
		unsigned int k = 0;
		for (; k < s2.size() - 1; k++)
			outfile << s2[k] << endl;
		outfile << s2[k];
		infile.close();
	}
	else {
		vector <string> add;
		add.push_back(s);
		Game::getWordList().push_back(add);
		ofstream outfile("wordList.csv", ios::out | ios::app);
		if (!outfile) {
			sendToClient(sockClient, "File wordList.csv open failed!\n");
			abort();
		}
		outfile << endl << s;
		outfile.close();
	}
}
void TestBuilder::InitFromFile(SOCKET sockClient)
{
	string line;
	vector<string> v;
	ifstream infile("testerList.csv");
	if (!infile) {
		sendToClient(sockClient, "File testerList.csv open failed!\n");
		abort();
	}
	getline(infile, line);
	while (!infile.eof())
	{
		getline(infile, line);
		if (line == "")
			break;
		v = split(line, ",");
		TestBuilder tester(v[0], v[1], atoi(v[2].c_str()), atoi(v[3].c_str()));
		testBuilderList.push_back(tester);
	}
	infile.close();
}
void TestBuilder::updateInfo(SOCKET sockClient)
{
	fstream outfile("testerList.csv", ios::out | ios::in);
	if (!outfile) {
		sendToClient(sockClient, "File testerList.csv open failed!\n");
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
			//outfile.seekp(loc, ios::cur );
			outfile << this->getName() << "," << this->getPassword() << "," << this->problemsNumbers << "," << this->testBuilderLevel;
			break;
		}
		cur_tmp = outfile.tellg();
	}
	outfile.close();
}
