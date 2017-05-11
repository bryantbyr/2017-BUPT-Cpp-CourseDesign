// 开发环境: Visual Studio 2013
// 程序说明: WordElemination Game服务器端/基于TCP,socket多线程通信
// 2017/5
//----------------------------------------------------------
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <time.h>
#include "Socket.h"
#include "stdafx.h"
using namespace std;
/**
 * [sendToClient description]
 * @param sockClient [description]
 * @param s          [description]
 */
void sendToClient(SOCKET sockClient, const char s[MaxSize])
{
    char sendBuf[MaxSize];
    strcpy(sendBuf, s);
    int byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
    if (byte <= 0)
        cout << "send error\n";
}

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
public:
    static vector<vector<string>> wordList;//被添加的单词列表
    int level;
    Game(int l = 0) {level = l;}
    ~Game() {}
    static vector<vector<string>>& getWordList() { return wordList; }
    int Display(SOCKET sockClient);
    //int Battle(SOCKET sockClientA, SOCKET sockClientB);
    static void InitFromFile(SOCKET sockClient);
};
/**
 * Game.cpp
 */
vector<vector<string>> Game::wordList = {};
int Game::Display(SOCKET sockClient)
{
    char sendBuf[MaxSize];
    char revBuf[MaxSize];
    // strcpy(sendBuf, "The level is: ");
    // byte=send(sockClient,sendBuf,strlen(sendBuf),0);
    // char l[MaxSize];
    // itoa(level+1,l,10);
    // //  << "There are " << wordList.size() << " levels totally" << endl;
    // byte=send(sockClient,l,strlen(l),0);
    if ((unsigned int)level < wordList.size() && wordList[level].size() > 0) {
        srand(time(NULL));
        int pos = rand() % wordList[level].size();
        char w[MaxSize];
        strcpy(w, (wordList[level][pos]).c_str());
        send(sockClient, w, strlen(w), 0);
        Sleep(100);
        strcpy(sendBuf, "\r 				 \r");
        send(sockClient, sendBuf, strlen(sendBuf), 0);
        time_t timeBegin, timeEnd;
        timeBegin = time(NULL);
        string answer;
        int n = recv(sockClient, revBuf, MaxSize, 0);
        revBuf[n] = '\0';
        if (n < 0)
            cout << "Rec Error" << endl;
        answer = revBuf;
        if (wordList[level][pos] == answer) {
            timeEnd = time(NULL);
            // cout << timeEnd - timeBegin << endl;
            strcpy(sendBuf, "Your answer is right.   \n");
            send(sockClient, sendBuf, strlen(sendBuf), 0);
            level++;
            int point;
            if (level > 7 && (timeEnd - timeBegin) < 10)
                point = 3;
            else if (level > 4 && (timeEnd - timeBegin) < 4)
                point = 2;
            else
                point = 1;
            return point;
        }
        else {
            strcpy(sendBuf, "Your answer is wrong.\n");
            send(sockClient, sendBuf, strlen(sendBuf), 0);
            return false;
        }
    }
    else {
        sendToClient(sockClient, "Congratulations! You have reached the last level.\n");
        return false;
    }
}

void Game::InitFromFile(SOCKET sockClient)
{
    string w;
    ifstream infile("wordList.csv");
    if (!infile) {
        sendToClient(sockClient, "File wordList.csv open failed!\n");
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
 * Battle.h
 */
class Battle: public Game {
private:
public:
    string word;
    time_t t1;
    time_t t2;
    SOCKET sockClientA;
    SOCKET sockClientB;
    Battle() {t1 = 100000; t2 = 100000;}
    ~Battle() {}
    void givePro();
    int onBattle(SOCKET sockClient);
};
void Battle::givePro()
{
    if ((unsigned int)level < wordList.size() && wordList[level].size() > 0) {
        srand(time(NULL));
        int pos = rand() % wordList[level].size();
        word = wordList[level][pos];
        sendToClient(sockClientA, (wordList[level][pos]).c_str());
        sendToClient(sockClientB, (wordList[level][pos]).c_str());
        Sleep(1000);
        sendToClient(sockClientA, "\r                  \r");
        sendToClient(sockClientB, "\r                  \r");
    }
    else {
        sendToClient(sockClientA, "Congratulations! You have reached the last level.\n");
        sendToClient(sockClientB, "Congratulations! You have reached the last level.\n");
    }
    level++;
}
int Battle::onBattle(SOCKET sockClient)
{
    char revBuf[MaxSize];
    time_t timeBegin, timeEnd;
    timeBegin = time(NULL);
    string answer;
    int n = recv(sockClient, revBuf, MaxSize, 0);
    revBuf[n] = '\0';
    if (n < 0)
        cout << "Rec Error" << endl;
    answer = revBuf;

    if (word == answer) {
        timeEnd = time(NULL);
        if (sockClientA == sockClient)
            t1 = timeEnd - timeBegin ;
        else
            t2 = timeEnd - timeBegin ;

        if (t1 > t2) {
            sendToClient(sockClientA, "You lose the battle!   \n");
            sendToClient(sockClientB, "You win the battle!   \n");
            return 2;
        }
        else if (t1 < t2) {
            sendToClient(sockClientA, "You win the battle!   \n");
            sendToClient(sockClientB, "You lose the battle!   \n");
            return 1;
        }
        else {
            sendToClient(sockClientA, "It is a draw!   \n");
            sendToClient(sockClientB, "It is a draw!   \n");
            return 0;
        }
    }
    else {
        sendToClient(sockClient, "Your answer is wrong.\n");
    }

}

Battle battle;

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
    virtual void Register(SOCKET sockClient) {}
    virtual int Login(SOCKET sockClient) {return false;}
};
/**
 * Player.h
 */
class Player: public User {
private:
    int playerLevel;
    int experience;
    int gainedGrades;
    bool isLogin;
    static vector<Player> playerList;//registered player list
    SOCKET relatedSocket;
public:
    Player(const string& n, const string& p, int e = 0, int l = 0, int g = 0, int is = false, bool isA = false): User(n, p)
    {
        playerLevel = l;
        experience = e;
        gainedGrades = g;
        isLogin = is;
        isAccept = isA;
    }
    ~Player() {}
    static vector<Player>& getList() {return playerList;}
    static void InitFromFile(SOCKET sockClient);
    void updateInfo(SOCKET sockClient);
    void Register(SOCKET sockClient);
    int Login(SOCKET sockClient);
    int getGrades() {return gainedGrades;}
    int getExperience() {return experience;}
    SOCKET& getRelatedSocket() {return relatedSocket;}
    void quitLogin() {isLogin = false;}
    void addLevel() {playerLevel += experience / 5;}
    void addExperience(int exp) {experience += exp;}
    void addGainedGrades() {gainedGrades++;}
    void addGainedPoints(int a) {gainedGrades += a;}
    void SearchPlayer(SOCKET sockClient, string& name);
    void SearchTestBuilder(SOCKET sockClient, string& name);
    void RankPlayersByGrades(SOCKET sockClient);
    void RankPlayersByExperience(SOCKET sockClient);
    vector<Player> getOnlineList();

    bool isAccept;
};
/**
 * TestBuilder.h
 */
class TestBuilder: public User {
private:
    int problemsNumbers;
    int testBuilderLevel;
    static bool isLogin;
    static vector <TestBuilder> testBuilderList;//registered player list
public:
    TestBuilder(const string& n, const string& p, int pNum = 0, int g = 0): User(n, p)
    {
        problemsNumbers = pNum;
        testBuilderLevel = g;
    }
    ~TestBuilder() {}
    static vector<TestBuilder>& getTestBuilderList() {return testBuilderList;}
    static void InitFromFile(SOCKET sockClient);
    void updateInfo(SOCKET sockClient);
    void Register(SOCKET sockClient);
    int Login(SOCKET sockClient);
    void quitLogin() {isLogin = false;}
    void addProblemsNumbers() {problemsNumbers++;}
    void addTestBuilderLevel() {testBuilderLevel++;}
    void SearchTestBuilder(SOCKET sockClient, const string& name);
    void SearchPlayer(SOCKET sockClient, const string& name);
    int getProNum() {return problemsNumbers;}
    void RankPlayersByProNum(SOCKET sockClient);
    int getTesterLevel() {return testBuilderLevel;}
    void RankPlayersByTesterLevel(SOCKET sockClient);
    void AddWord(SOCKET sockClient);
};
/**
 * Player.cpp
 */
vector<Player> Player::playerList = {};
void Player::Register(SOCKET sockClient)
{
    char sendBuf[MaxSize];
    ofstream outfile("playerList.csv", ios::out | ios::app);
    if (!outfile) {
        strcpy(sendBuf, "File playerList.csv open failed!\n");
        send(sockClient, sendBuf, strlen(sendBuf), 0);
        abort();
    }
    outfile << this->getName() << "," << this->getPassword() << "," << this->playerLevel << "," << this->experience << "," << this->gainedGrades << endl;
    outfile.close();
    playerList.push_back(*this);
    strcpy(sendBuf, "Player successfully registers!\n");
    send(sockClient, sendBuf, strlen(sendBuf), 0);
}
int Player::Login(SOCKET sockClient)
{
    char sendBuf[MaxSize];
    for (int i = 0; i < (int)playerList.size(); i++)
        if (playerList[i].getName() == this->getName() && playerList[i].getPassword() == this->getPassword()) {
            strcpy(sendBuf, "Player successfully logins!\n");
            Sleep(10);
            send(sockClient, sendBuf, strlen(sendBuf), 0);
            playerList[i].isLogin = true;
            playerList[i].relatedSocket = sockClient;
            return i;
        }
    strcpy(sendBuf, "Player-login failed!\n");
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
            itoa(i + 1, l, 10);
            sendToClient(sockClient, l);
            Sleep(10);
            sendToClient(sockClient, "th player\n");
            return;
        }
    sendToClient(sockClient, "It is not exited in the playerList.\n");
}
void Player::SearchTestBuilder(SOCKET sockClient, string& name)
{
    vector<TestBuilder> v = TestBuilder::getTestBuilderList();
    for (unsigned int i = 0; i < v.size(); i++)
        if (v[i].getName() == name) {
            sendToClient(sockClient, "It is the ");
            Sleep(10);
            char l[MaxSize];
            itoa(i + 1, l, 10);
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
        itoa(i + 1, l, 10);
        sendToClient(sockClient, l);
        Sleep(10);
        sendToClient(sockClient, "th   ");
        Sleep(10);
        sendToClient(sockClient, v[i].getName().c_str());
        Sleep(10);
        sendToClient(sockClient, " with gainedGrades is: ");
        Sleep(10);
        itoa(v[i].getGrades(), l, 10);
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
        itoa(i + 1, l, 10);
        sendToClient(sockClient, l);
        Sleep(10);
        sendToClient(sockClient, "th   ");
        Sleep(10);
        sendToClient(sockClient, v[i].getName().c_str());
        Sleep(10);
        sendToClient(sockClient, " with experience is: ");
        Sleep(10);
        itoa(v[i].getGrades(), l, 10);
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
    //int loc = -getList().size();
    while (!outfile.eof())
    {
        getline(outfile, line);
        v = split(line, ",");
        if (strcmp(this->getName().c_str(), v[0].c_str()) == 0) {
            outfile.seekp(cur_tmp);
            outfile.seekp(0, ios::cur );
            outfile << this->getName() << "," << this->getPassword() << "," << this->playerLevel << "," << this->experience << "," << this->gainedGrades;
            break;
        }
        //loc++;
        cur_tmp = outfile.tellg();
    }
    outfile.close();
}
vector<Player> Player::getOnlineList()
{
    vector<Player> onlintList;
    for (int i = 0; i < playerList.size(); i++) {
        if (playerList[i].isLogin)
            onlintList.push_back(playerList[i]);
    }
    return onlintList;
}
/**
 * TestBuilder.cpp
 */
vector<TestBuilder> TestBuilder::testBuilderList = {};
bool TestBuilder::isLogin = 0;
void TestBuilder::Register(SOCKET sockClient)
{
    char sendBuf[MaxSize];
    ofstream outfile("testerList.csv", ios::out | ios::app);
    outfile << this->getName() << "," << this->getPassword() << "," << this->problemsNumbers << "," << this->testBuilderLevel << endl;
    if (!outfile) {
        strcpy(sendBuf, "File playerList.csv open failed!");
        send(sockClient, sendBuf, strlen(sendBuf), 0);
        abort();
    }
    outfile.close();
    testBuilderList.push_back(*this);
    strcpy(sendBuf, "Tester successfully registers! \n");
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
    strcpy(sendBuf, "Tester-login failed!\n");
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
            itoa(i + 1, l, 10);
            sendToClient(sockClient, l);
            Sleep(10);
            sendToClient(sockClient, "th tester\n");
            return;
        }
    cout << "It is not exited in the testBuilderList.\n" << endl;
}
void TestBuilder::SearchPlayer(SOCKET sockClient, const string& name)
{
    vector<Player> v = Player::getList();
    for (unsigned int i = 0; i < v.size(); i++)
        if (v[i].getName() == name) {
            sendToClient(sockClient, "It is the ");
            Sleep(10);
            char l[MaxSize];
            itoa(i + 1, l, 10);
            sendToClient(sockClient, l);
            Sleep(10);
            sendToClient(sockClient, "th player\n");
            return;
        }
    cout << "It is not exited in the playerList." << endl;
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
        itoa(i + 1, l, 10);
        sendToClient(sockClient, l);
        Sleep(10);
        sendToClient(sockClient, "th   ");
        Sleep(10);
        sendToClient(sockClient, v[i].getName().c_str());
        Sleep(10);
        sendToClient(sockClient, " with problemsNumbers is: ");
        Sleep(10);
        itoa(v[i].getProNum(), l, 10);
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
        itoa(i + 1, l, 10);
        sendToClient(sockClient, l);
        Sleep(10);
        sendToClient(sockClient, "th   ");
        Sleep(10);
        sendToClient(sockClient, v[i].getName().c_str());
        Sleep(10);
        sendToClient(sockClient, " with testBuilderLevel is: ");
        itoa(v[i].getTesterLevel(), l, 10);
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


//HANDLE hMutex;
void SendAndRec(SOCKET sockClient)
{
    int choice = 1;

    char sendBuf[MaxSize];
    char revBuf[MaxSize];
    int byte = 0;
    while (1)
    {
        //WaitForSingleObject(hMutex, INFINITE);
        while (choice != 0) {
            strcpy(sendBuf, "             Welcome to word elemination system !\n                        1  register\n                        2  plogin\n                        3  tlogin\n                        0  quit\n");
            byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
            //for (auto& c : sendBuf) { c = (char)NULL; }
            if (byte <= 0)
                cout << "Send error" << endl;
            do {
                int n = recv(sockClient, revBuf, MaxSize, 0);
                if (n < 0)
                    cout << "Recv error" << endl;
                choice = revBuf[0] - '0';
                if (choice < 0 || choice > 3) {
                    strcpy(sendBuf, "Input is wrong, please input again!\n");
                    byte = send(sockClient, sendBuf, strlen(sendBuf), 0); //服务器向客户端发送数据
                }
                else
                    break;
            } while (1);
            switch (choice) {
            case 1: {
                strcpy(sendBuf, "Please set your username: ");
                byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
                string name;
                int n = recv(sockClient, revBuf, MaxSize, 0);
                revBuf[n] = '\0';
                while (n < 0)
                    cout << "Rec Error" << endl;
                name = revBuf;

                strcpy(sendBuf, "Please set your account password: ");
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
                strcpy(sendBuf, "Please input username:");
                byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
                int n = recv(sockClient, revBuf, MaxSize, 0);
                revBuf[n] = '\0';
                while (n < 0)
                    cout << "Rec Error" << endl;
                name = revBuf;

                string password;
                strcpy(sendBuf, "Please input password:");
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
                        strcpy(sendBuf, "Username or password is wrong,please input them again:\n Please input username:");
                        byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
                        int n = recv(sockClient, revBuf, MaxSize, 0);
                        revBuf[n] = '\0';
                        if (n < 0)
                            cout << "Rec Error" << endl;
                        name = revBuf;

                        string password;
                        strcpy(sendBuf, "Please input password:");
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
                                strcpy(sendBuf, "Input is wrong, please input again!");
                                byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
                            }
                            else
                                break;
                        } while (1);
                        switch (playerChoice) {
                        case 1:

                            break;
                        case 2: {
                            int b = game.Display(sockClient);
                            if (b > 0) {
                                tempPlayer.addGainedGrades();
                                tempPlayer.addLevel();
                                tempPlayer.addExperience(b);
                            }
                        }
                        break;
                        case 3: {
                            int b = game.Display(sockClient);
                            if (b > 0) {
                                tempPlayer.addGainedGrades();
                                tempPlayer.addLevel();
                                tempPlayer.addExperience(b);
                            }
                        }
                        break;
                        case 4: {
                            sendToClient(sockClient, "Please choose the way to search user:\n");
                            string name;
                            int n = recv(sockClient, revBuf, MaxSize, 0);
                            revBuf[n] = '\0';
                            if (n < 0)
                                cout << "Rec Error" << endl;
                            name = revBuf;
                            tempPlayer.SearchPlayer(sockClient, name);
                            tempPlayer.SearchTestBuilder(sockClient, name);
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
                        case 6: {
                            vector<Player> onList;
                            onList = tempPlayer.getOnlineList();
                            sendToClient(sockClient, "The onLine player list is:\n");
                            Sleep(10);
                            for (int i = 0; i < onList.size(); i++) {
                                sendToClient(sockClient, onList[i].getName().c_str());
                                Sleep(10);
                                sendToClient(sockClient, "\n");
                                Sleep(10);
                            }
                            sendToClient(sockClient, "If you want to battle with someone online,please input its username:\n");
                            int n = recv(sockClient, revBuf, MaxSize, 0);
                            revBuf[n] = '\0';
                            if (n < 0)
                                cout << "Rec Error" << endl;
                            string username = revBuf;
                            int i = 0;
                            for (; i < onList.size(); i++) {
                                if (username == onList[i].getName())
                                    break;
                            }
                            battle.sockClientA = sockClient;
                            battle.sockClientB = onList[i].getRelatedSocket();
                            sendToClient(onList[i].getRelatedSocket(), tempPlayer.getName().c_str());
                            Sleep(10);
                            sendToClient(onList[i].getRelatedSocket(), " want to battle with you,do you agree('y'or'n'):\n");
                            // n = recv(sockClient, revBuf, MaxSize, 0);
                            // revBuf[n] = '\0';
                            // if (n < 0)
                            //     cout << "Rec Error" << endl;
                            Sleep(5000);
                            for (int k = 0; k < Player::getList().size(); k++) {
                                if (Player::getList()[k].getName() == onList[i].getName())
                                    onList[i].isAccept = Player::getList()[k].isAccept;
                            }
                            if (onList[i].isAccept) {
                                onList[i].isAccept = false;
                                sendToClient(onList[i].getRelatedSocket(), "The battle will start after 3 seconds!\n");
                                sendToClient(sockClient, "The oponent has agreed,the battle will start after 3 seconds!\n");
                                Sleep(3000);
                                battle.givePro();
                                int a = battle.onBattle(sockClient);
                                if (a == 1)
                                    tempPlayer.addGainedPoints(10);
                                else if (a == 2)
                                    tempPlayer.addGainedPoints(-1);
                                else
                                    tempPlayer.addGainedPoints(0);

                                //getchar();
                            }
                            else {
                                sendToClient(sockClient, "The oponent refused your request!\n");
                            }
                        }
                        break;
                        case 7: {
                            tempPlayer.isAccept = true;
                            int a = battle.onBattle(sockClient);
                            if (a == 2)
                                tempPlayer.addGainedPoints(10);
                            else if (a == 1)
                                tempPlayer.addGainedPoints(-1);
                            else
                                tempPlayer.addGainedPoints(0);
                            //getchar();
                        }
                        break;
                        }
                    }
                    //battle.~Battle();
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
                        case 1:
                            tempTester.AddWord(sockClient);
                            tempTester.addProblemsNumbers();
                            break;
                        case 2: {
                            sendToClient(sockClient, "Please input the name to search user:");
                            string name;
                            int n = recv(sockClient, revBuf, MaxSize, 0);
                            revBuf[n] = '\0';
                            if (n < 0)
                                cout << "Rec Error" << endl;
                            name = revBuf;
                            tempTester.SearchPlayer(sockClient, name);
                            tempTester.SearchTestBuilder(sockClient, name);
                        }

                        break;
                        case 3: {
                            sendToClient(sockClient, "\n      Please choose the characters to rank the Tester list:\n");
                            sendToClient(sockClient, "\n             a problemsNumbers  \n              b testBuilderLevel    \n");
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

        Sleep(200);
    }

    getchar();
    return 0;
}
