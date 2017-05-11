#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <windows.h>
using namespace std;

vector<string> split(const  string& s, const string& delim)
{
    vector<std::string> elems;
    size_t pos = 0;
    size_t len = s.length();
    size_t delim_len = delim.length();
    if (delim_len == 0) return elems;
    while (pos < len)
    {
        int find_pos = s.find(delim, pos);
        if (find_pos < 0)
        {
            elems.push_back(s.substr(pos, len - pos));
            break;
        }
        elems.push_back(s.substr(pos, find_pos - pos));
        pos = find_pos + delim_len;
    }
    return elems;
}

/**
 * Game.h
 */
class Game {
private:
    int level;
    static vector<string> wordList;//被添加的单词列表
public:
    Game(int l = 0) {level = l;}
    ~Game() {}
    bool Display();
    void setLevel(int a) {level = a;}
    static vector<string>& getWordList() { return wordList; }
    static void InitFromFile();
};
/**
 * Game.cpp
 */
vector<string> Game::wordList = {};
bool Game::Display()
{
    cout << "This the " << level + 1 << "th level : "
         << "There are " << wordList.size() << " levels totally" << endl;
    if ((unsigned)level < wordList.size()) {
        cout << wordList[level];
        Sleep(2000);
        cout << "\r                   \r";
        string answer;
        cin >> answer;
        if (wordList[level] == answer) {
            cout << "Your answer is right." << endl;
            level++;
            return true;
        }
        else {
            cout << "Your answer is wrong." << endl;
            return false;
        }
    }
    else {
        cout << "Congratulations! You have reached the last level." << endl;
        return false;
    }
}
void Game::InitFromFile()
{
    string w;
    ifstream infile("wordList.csv");
    if (!infile) {
        cout << "File wordList.csv open failed!" << endl;
        abort();
    }
    while (!infile.eof())
    {
        infile >> w;
        wordList.push_back(w);
    }
    infile.close();
}
/**
 * User.h
 */
class User {
private:
    string name;
    string password;
public:
    User(const string& n, const string& p): name(n), password(p) {}
    ~User() {}
    string getName() {return name;}
    string getPassword() {return password;}
    virtual void Register() {}
    virtual int Login() {return false;}
};
/**
 * Player.h
 */
class Player: public User {
private:
    int playerLevel;
    int experience;
    int gainedGrades;
    static bool isLogin;
    static vector<Player> playerList;//registered player list
public:
    Player(const string& n, const string& p, int e = 0, int l = 0, int g = 0): User(n, p)
    {
        playerLevel = l;
        experience = e;
        gainedGrades = g;
    }
    ~Player() {}
    static vector<Player>& getList() {return playerList;}
    void Register();
    int Login();
    int getGrades() {return gainedGrades;}
    int getExperience() {return experience;}
    int getPlayerLvel() {return playerLevel;}
    void quitLogin() {isLogin = false;}
    void addLevel() {playerLevel++;}
    void addExperience() {experience++;}
    void addGainedGrades() {gainedGrades++;}
    void SearchPlayer(string& name);
    void SearchTestBuilder(string& name);
    void RankPlayersByGrades();
    void RankPlayersByExperience();
    void RankPlayersByBestLevel();
    void updateInfo();
    static void InitFromFile();
};
/**
 * TestBuilder.h
 */
class TestBuilder: public User {
private:
    int problemsNumbers;
    int testBuilderLevel;
    static bool isLogin;
    static vector<TestBuilder> testBuilderList;//registered player list
public:
    TestBuilder(const string& n, const string& p, int pNum = 0, int g = 0): User(n, p)
    {
        problemsNumbers = pNum;
        testBuilderLevel = g;
    }
    ~TestBuilder() {}
    static vector<TestBuilder>& getTestBuilderList() {return testBuilderList;}
    void Register();
    int Login();
    void quitLogin() {isLogin = false;}
    void addProblemsNumbers() {problemsNumbers++;}
    void addTestBuilderLevel() {testBuilderLevel++;}
    void SearchTestBuilder(const string& name);
    void SearchPlayer(const string& name);
    int getProNum() {return problemsNumbers;}
    void RankPlayersByProNum();
    int getTesterLevel() {return testBuilderLevel;}
    void RankPlayersByTesterLevel();
    void AddWord();
    static void InitFromFile();
    void updateInfo();
};
/**
 * Player.cpp
 */
bool Player::isLogin = 0;
vector<Player> Player::playerList = {};
void Player::Register()
{
    ofstream outfile("playerList.csv", ios::out | ios::app);
    if (!outfile) {
        cout << "File playerList.csv open failed!" << endl;
        abort();
    }
    outfile << this->getName() << "," << this->getPassword() << "," << this->playerLevel << "," << this->experience << "," << this->gainedGrades << endl;
    outfile.close();
    playerList.push_back(*this);
    cout << "Player successfully registers!" << endl;
}
int Player::Login()
{
    if (isLogin)
        return -2;
    for (int i = 0; i < (int)playerList.size(); i++)
        if (playerList[i].getName() == this->getName() && playerList[i].getPassword() == this->getPassword()) {
            cout << "Player successfully logins!" << endl;
            isLogin = true;
            return i;
        }
    cout << "Player-login failed!" << endl;
    return -1;
}
void Player::SearchPlayer(string& name)
{
    vector<Player> v = Player::getList();
    for (unsigned int i = 0; i < v.size(); i++)
        if (v[i].getName() == name) {
            cout << "It is the " << i + 1 << "th player" << endl;
            return;
        }
    cout << "It is not exited in the playerList." << endl;
}
void Player::SearchTestBuilder(string& name)
{
    vector<TestBuilder> v = TestBuilder::getTestBuilderList();
    for (unsigned int i = 0; i < v.size(); i++)
        if (v[i].getName() == name) {
            cout << "It is the " << i + 1 << "th tester" << endl;
            return;
        }
    cout << "It is not exited in the testBuilderList." << endl;
}
bool sortByGrades(Player a, Player b)
{
    return a.getGrades() > b.getGrades();
}
void Player::RankPlayersByGrades()
{
    vector<Player> v = Player::getList();
    sort(v.begin(), v.end(), sortByGrades);
    for (unsigned int i = 0; i < v.size(); i++)
        cout << i + 1 << "th   " << v[i].getName()
             << " with gainedGrades is: " << v[i].getGrades() << endl;
}
bool sortByExperience(Player a, Player b)
{
    return a.getExperience() > b.getExperience();
}
void Player::RankPlayersByExperience()
{
    vector<Player> v = Player::getList();
    sort(v.begin(), v.end(), sortByGrades);
    for (unsigned int i = 0; i < v.size(); i++)
        cout << i + 1 << "th   " << v[i].getName()
             << " with experience is: " << v[i].getExperience() << endl;
}
bool sortByBestLevel(Player a, Player b)
{
    return a.getPlayerLvel() > b.getPlayerLvel();
}
void Player::RankPlayersByBestLevel()
{
    vector<Player> v = Player::getList();
    sort(v.begin(), v.end(), sortByBestLevel);
    for (unsigned int i = 0; i < v.size(); i++)
        cout << i + 1 << "th   " << v[i].getName()
             << " with best level is: " << v[i].getPlayerLvel() << endl;
}
void Player::InitFromFile()
{
    string line;
    vector<string> v;
    ifstream infile("playerList.csv");
    if (!infile) {
        cout << "File playerList.csv open failed!" << endl;
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

void Player::updateInfo()
{
    fstream outfile("playerList.csv", ios::out | ios::in);
    if (!outfile) {
        cout << "File playerList.csv open failed!" << endl;
        abort();
    }
    string line;
    vector<string> v;
    outfile.seekp(0);
    getline(outfile, line);
    streamoff cur_tmp = outfile.tellg();
    int loc = -getList().size();
    while (!outfile.eof())
    {
        getline(outfile, line);
        v = split(line, ",");
        if (strcmp(this->getName().c_str(), v[0].c_str()) == 0) {
            outfile.seekp(cur_tmp);
            outfile.seekp(loc, ios::cur );
            outfile << this->getName() << "," << this->getPassword() << "," << this->playerLevel << "," << this->experience << "," << this->gainedGrades;
            break;
        }
        loc++;
        cur_tmp = outfile.tellg();
    }
    outfile.close();
}
/**
 * TestBuilder.cpp
 */
vector<TestBuilder> TestBuilder::testBuilderList = {};
bool TestBuilder::isLogin = 0;
void TestBuilder::Register()
{
    ofstream outfile("testerList.csv", ios::out | ios::app);
    if (!outfile) {
        cout << "File testerList.csv open failed!" << endl;
        abort();
    }
    outfile << this->getName() << "," << this->getPassword() << "," << this->problemsNumbers << "," << this->testBuilderLevel << endl;
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
    int loc = -getTestBuilderList().size();
    while (!outfile.eof())
    {
        getline(outfile, line);
        v = split(line, ",");
        if (strcmp(this->getName().c_str(), v[0].c_str()) == 0) {
            outfile.seekp(cur_tmp);
            outfile.seekp(loc, ios::cur );
            outfile << this->getName() << "," << this->getPassword() << "," << this->problemsNumbers << "," << this->testBuilderLevel;
            break;
        }
        loc++;
        cur_tmp = outfile.tellg();
    }
    outfile.close();
}

int main()
{

    int choice = 1;
    Game::InitFromFile();
    Player::InitFromFile();
    TestBuilder::InitFromFile();
    while (choice != 0) {
        cout << "\n\n\n\n" << "              *************************" << endl;
        //cout <<  "                欢 迎 进入 单 词 消 除 游 戏 系 统 ! " << endl;
        cout <<  "               Welcome to word elemination system ! " << endl;
        cout <<  "\n                                 1  regi-用户注册" << endl;
        cout <<  "\n                                 2  plogin-玩家登陆" << endl;
        cout <<  "\n                                 3  tlogin-出题者登陆" << endl;
        cout <<  "\n                                 0  quit-退出系统" << endl;
        cout <<  "              **************************************" << endl;
        do {
            cin >> choice;
            if (choice < 0 || choice > 8)
                cout << "Input is wrong，please input again!" << endl;
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
                    //cout <<  "                  欢 迎 进 入 游 戏 界 面 ! " << endl;
                    cout <<  "                Welcome to playing page ! " << endl;
                    cout <<  "\n                 1  select the difficulty of the game-请选择难度游戏难度" << endl;
                    cout <<  "\n                 2  start the game -开始游戏" << endl;
                    cout <<  "\n                 3  next level -下一关" << endl;
                    //cout <<  "\n                 4  currentLevel" << endl;
                    cout <<  "\n                 4  search user -查询" << endl;
                    cout <<  "\n                 5  rank playerList -排行榜" << endl;
                    cout <<  "\n                 0  quit playing page -退出游戏界面" << endl;
                    cout <<  "              ************************" << endl;
                    do {
                        cin >> playerChoice;
                        if (playerChoice < 0 || playerChoice > 5)
                            cout << "Input is wrong，please input again!" << endl;
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
                        string name;
                        cin >> name;
                        tempPlayer.SearchPlayer(name);
                        tempPlayer.SearchTestBuilder(name);
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
                    //cout <<  "                  欢 迎 进 入 管 理 界 面 ! " << endl;
                    cout <<  "                Welcome to management page ! " << endl;
                    cout <<  "\n                 1  build new tester case-出题" << endl;
                    cout <<  "\n                 2  search user -查询" << endl;
                    cout <<  "\n                 3  rank testBuilderList -排行榜" << endl;
                    cout <<  "\n                 0  quit management page -退出管理界面" << endl;
                    cout <<  "              ************************" << endl;
                    do {
                        cin >> testerChoice;
                        if (testerChoice < 0 || testerChoice > 3)
                            cout << "Input is wrong，please input again!" << endl;
                        else
                            break;
                    } while (1);
                    switch (testerChoice) {
                    case 1:
                        tempTester.AddWord();
                        tempTester.addProblemsNumbers();
                        break;
                    case 2: {
                        cout << "Please input the name to search user:" << endl;
                        string name;
                        cin >> name;
                        tempTester.SearchPlayer(name);
                        tempTester.SearchTestBuilder(name);
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
