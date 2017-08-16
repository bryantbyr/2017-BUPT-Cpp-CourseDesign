#include "TestBuilder.h"
#include "Player.h"
#include "Game.h"
#include "split.h"

vector<TestBuilder> TestBuilder::testBuilderList = {};
bool TestBuilder::isLogin = 0;
void TestBuilder::Register()
{
	for (int i = 0; i < (int)testBuilderList.size(); i++)
		if(testBuilderList[i].getName()==this->getName()){
			cout << "The Tester-Name has been registered. Register failed!" << endl;
			return;
		}
	
	ofstream outfile("testerList.csv", ios::out | ios::app);
	if (!outfile) {
		cout << "File testerList.csv open failed!" << endl;
		abort();
	}
	outfile << this->getName() << "," << this->getPassword() << "," << this->problemsNumbers << "," << this->testBuilderLevel << "    " << endl;
	outfile.close();
	testBuilderList.push_back(*this);
	cout << "Tester successfully registers!" << endl;
}
int TestBuilder::Login()
{
	if (isLogin)
		return -2;
	for (int i = 0; i < (int)testBuilderList.size(); i++)
		if (testBuilderList[i].getName() == this->getName() && testBuilderList[i].getPassword() == this->getPassword()) {
			cout << "Tester successfully logins!" << endl;
			isLogin = true;
			return i;
		}
	cout << "Tester-login failed!" << endl;
	return -1;
}
void TestBuilder::SearchTestBuilder(const string& name)
{
	vector<TestBuilder> v = TestBuilder::getTestBuilderList();
	for (unsigned int i = 0; i < v.size(); i++)
		if (v[i].getName() == name) {
			cout << "It is the " << i + 1 << "th tester" << endl;
			return;
		}
	cout << "It is not exited in the testBuilderList." << endl;
}
void TestBuilder::SearchPlayer(const string& name)
{
	vector<Player> v = Player::getList();
	for (unsigned int i = 0; i < v.size(); i++)
		if (v[i].getName() == name) {
			cout << "It is the " << i + 1 << "th player" << endl;
			return;
		}
	cout << "It is not exited in the playerList." << endl;
}
void TestBuilder::SearchTesterBylevel(int level)
{
	vector<TestBuilder> v = TestBuilder::getTestBuilderList();
	for (unsigned int i = 0; i < v.size(); i++)
		if (v[i].getTesterLevel() >= level) {
			cout << "The " << i + 1 << "th tester: " << v[i].getName() << endl;
		}
}
bool sortByProNum(TestBuilder a, TestBuilder b)
{
	return a.getProNum() > b.getProNum();
}
void TestBuilder::RankPlayersByProNum()
{
	vector<TestBuilder> v = TestBuilder::getTestBuilderList();
	sort(v.begin(), v.end(), sortByProNum);
	for (unsigned int i = 0; i < v.size(); i++)
		cout << i + 1 << "th   " << v[i].getName()
		<< " with problemsNumbers is: " << v[i].getProNum() << endl;
}
bool sortByLevel(TestBuilder a, TestBuilder b)
{
	return a.getTesterLevel() > b.getTesterLevel();
}
void TestBuilder::RankPlayersByTesterLevel()
{
	vector<TestBuilder> v = TestBuilder::getTestBuilderList();
	sort(v.begin(), v.end(), sortByLevel);
	for (unsigned int i = 0; i < v.size(); i++)
		cout << i + 1 << "th   " << v[i].getName()
		<< " with testBuilderLevel is: " << v[i].getTesterLevel() << endl;
}
void TestBuilder::AddWord()
{
	string s;
	cout << "Please input newly added word: ";
	cin >> s;
	Game::getWordList().push_back(s);
	ofstream outfile("wordList.csv", ios::out | ios::app);
	if (!outfile) {
		cout << "File wordList.csv open failed!" << endl;
		abort();
	}
	outfile << endl << s;
	outfile.close();
}
void TestBuilder::InitFromFile()
{
	string line;
	vector<string> v;
	ifstream infile("testerList.csv");
	if (!infile) {
		cout << "File testerList.csv open failed!" << endl;
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
void TestBuilder::updateInfo()
{
	fstream outfile("testerList.csv", ios::out | ios::in);
	if (!outfile) {
		cout << "File testerList.csv open failed!" << endl;
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
			outfile << this->getName() << "," << this->getPassword() << "," << this->problemsNumbers << "," << this->testBuilderLevel;
			break;
		}
		cur_tmp = outfile.tellg();
	}
	outfile.close();
}
