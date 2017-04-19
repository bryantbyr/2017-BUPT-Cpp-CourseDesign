/*
1.每一关的难度要有所增加，体现为如下三个条件中的一个或者多个:单词难度可以递增或者持平（即长度加长或不变）/进行轮数增多（即单词数目如：前三关仅仅通过一个单词就过关，后续需要通过两个，三个甚至更多才过关）/单词显示时间缩短（随着关卡的增加显示时间越来越短）
2.闯关者每闯过一关，增加一定经验值。经验值会根据闯过的该关卡的关卡号、该关的闯关耗费时间共同决定。当经验值累计到一定程度闯关者等级增加。闯关失败需要重新闯该关
3.游戏自带词库，而且已经注册的出题者可以为系统出题，即增加词库的新词，已经存在的单词不能再次添加（词库中的单词构成一个单词池，但建议根据单词的长度来组织存储，每次出题时，系统从该单词池中按照关卡难度随机的选择相应长度的单词）。每成功出题一次，更新该出题者的出题数目。出题者等级根据出题人成功出题数目来升级。
****必须在题目一的基础上进行修改。
****请根据要求设计每一关的出题方式，注意随着关卡数增加，题目难度增加。请合理处理出题人新添加新词的使用方式，并且新加词组不会影响游戏难度。
*/
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <windows.h>
using namespace std;

/**
 * Game.h
 */
class Game {
private:
    int level;
    static vector<vector<string>> wordList;//被添加的单词列表
public:
    Game(int l = 0) {level = l;}
    ~Game() {}
    static vector<vector<string>>& getWordList() { return wordList; }
    int Display();
    static void InitFromFile();
};
/**
 * Game.cpp
 */
vector<vector<string>> Game::wordList = {};
int Game::Display()
{
    cout << "This the " << level + 1 << "th level : "
         << "There are " << wordList.size() << " levels totally" << endl;
    if ((unsigned int)level < wordList.size() && wordList[level].size() > 0) {
        srand(time(NULL));
        int pos = rand() % wordList[level].size();
        cout << wordList[level][pos];
        Sleep(2000);
        cout << "\r                   \r";
        time_t timeBegin, timeEnd;
        timeBegin = time(NULL);
        string answer;
        cin >> answer;
        if (wordList[level][pos] == answer) {
            timeEnd = time(NULL);
            cout << "Your answer is right. "
                 << "Time used: "
                 << timeEnd - timeBegin << endl;
            level++;
            int point;
            if(level > 7&&(timeEnd - timeBegin)<10)
                point = 3;
            else if(level > 4&&(timeEnd - timeBegin)<4)
                point = 2;
            else
                point = 1;
            return point;
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
    ifstream infile("wordList.txt");
    if (!infile) {
        cout << "File wordList.txt open failed!" << endl;
        abort();
    }
    unsigned int i = 0;
    while (!infile.eof())
    {
        infile >> w;
        for (; i < wordList.size(); i++)
            if (w.size() == wordList[i][0].size())
                break;
        if (i < wordList.size())
            wordList[i].push_back(w);
        else {
            vector <string> add;
            add.push_back(w);
            wordList.push_back(add);
        }
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
    void quitLogin() {isLogin = false;}
    void addLevel() {playerLevel+=experience/5;}
    void addExperience(int exp) {experience+=exp;}
    void addGainedGrades() {gainedGrades++;}
    void SearchPlayer(string& name);
    void SearchTestBuilder(string& name);
    void RankPlayersByGrades();
    void RankPlayersByExperience();
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
};
/**
 * Player.cpp
 */
bool Player::isLogin = 0;
vector<Player> Player::playerList = {};
void Player::Register()
{
    ofstream outfile("playerList.txt", ios::out | ios::app);
    if (!outfile) {
        cout << "File playerList.txt open failed!" << endl;
        abort();
    }
    outfile << endl << this->getName() << "      " << this->getPassword();
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
        cout << i << "th   " << v[i].getName()
             << " with experience is: " << v[i].getExperience() << endl;
}
void Player::InitFromFile()
{
    string n, s;
    ifstream infile("playerList.txt");
    if (!infile) {
        cout << "File playerList.txt open failed!" << endl;
        abort();
    }
    while (!infile.eof())
    {
        infile >> n;
        infile >> s;
        Player player(n, s);
        playerList.push_back(player);
    }
    infile.close();
}
/**
 * TestBuilder.cpp
 */
vector<TestBuilder> TestBuilder::testBuilderList = {};
bool TestBuilder::isLogin = 0;
void TestBuilder::Register()
{
    ofstream outfile("testerList.txt", ios::out | ios::app);
    outfile << endl << this->getName() << "      " << this->getPassword();
    if (!outfile) {
        cout << "File testerList.txt open failed!" << endl;
        abort();
    }
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
    unsigned int i = 0;
    for (; i < Game::getWordList().size(); i++)
        if (s.size() < Game::getWordList()[i][0].size())
            break;
    if (Game::getWordList()[i - 1][0].size() == s.size()) {
        for (unsigned int m = 0; m < Game::getWordList()[i - 1].size(); m++) {
            if (Game::getWordList()[i - 1][m] == s) {
                cout << "The word is already exited! Add word failed!" << endl;
                return;
            }
        }
        Game::getWordList()[i - 1].push_back(s);
        string n;
        vector<string> s1;
        vector<string> s2;
        ifstream infile("wordList.txt");
        if (!infile) {
            cout << "File wordList.txt open failed!" << endl;
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
        ofstream outfile("wordList.txt");
        if (!outfile) {
            cout << "File wordList.txt open failed!" << endl;
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
        ifstream infile("wordList.txt");
        if (!infile) {
            cout << "File wordList.txt open failed!" << endl;
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
        ofstream outfile("wordList.txt");
        if (!outfile) {
            cout << "File wordList.txt open failed!" << endl;
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
        ofstream outfile("wordList.txt", ios::out | ios::app);
        if (!outfile) {
            cout << "File wordList.txt open failed!" << endl;
            abort();
        }
        outfile << endl << s;
        outfile.close();
    }
}
void TestBuilder::InitFromFile()
{
    string n, s;
    ifstream infile("testerList.txt");
    if (!infile) {
        cout << "File testerList.txt open failed!" << endl;
        abort();
    }
    while (!infile.eof())
    {
        infile >> n;
        infile >> s;
        TestBuilder tester(n, s);
        testBuilderList.push_back(tester);
    }
    infile.close();
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
            cout << "Please set your username:" << endl;
            string name;
            cin >> name;
            cout << "Please set your account password:" << endl;
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
                    case 1:

                        break;
                    case 2:{
                        int b=game.Display();
                        if (b>0) {
                            tempPlayer.addGainedGrades();
                            tempPlayer.addLevel();
                            tempPlayer.addExperience(b);
                        }
                    }

                        break;
                    case 3:{
                        int b=game.Display();
                        if (b>0) {
                            tempPlayer.addGainedGrades();
                            tempPlayer.addLevel();
                            tempPlayer.addExperience(b);
                        }
                    }

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
                             << "              b experience    \n";
                        char rankChoice;
                        cin >> rankChoice;
                        if (rankChoice == 'a')
                            tempPlayer.RankPlayersByGrades();
                        else
                            tempPlayer.RankPlayersByExperience();
                    }
                    break;
                    }
                }
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
