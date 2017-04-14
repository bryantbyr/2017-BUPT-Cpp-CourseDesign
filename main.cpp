/*
单词消除游戏总体介绍：
    单词消除游戏由两类参与者组成，闯关者（即游戏玩家），出题者（为游戏增加游戏中
使用单词）。游戏规则为，游戏每一轮，程序会根据该关卡难度，显示一个单词，一定时间
后单词消失。闯关者需要在相应地方输入刚刚显示并消失的单词，如果闯关者输入正确（即
闯关者输入的单词与刚刚显示的单词完全一致，包含大小写）则为通过。一关可以由一轮或
者多轮组成。
题目一
    闯关者属性要求必须含有：闯关者姓名、已闯关关卡数、闯关者经验值、闯关者等级，
若有需要可以自行添加其他属性。出题者属性要求必须含有：出题者姓名、出题者出题数
目，等级，若有需要可以自行添加其余属性。具体功能包括：

    *实现闯关者，出题者本地的注册，登录。
  *程序支持多人注册，同一时间只有一人可以登录。
  *任何角色均可查询所有闯关者，出题者，按照属性查找相应闯关者，出题者。
  *可以根据闯关者闯过关卡数，经验，等级等对闯关者排名，根据出题者出题数目，
     等级对出题者排名。

    要求：
  **采用面向对象的方式，使用类设计。
  **在设计类时请注意类的继承关系，关注闯关者，出题者的关联（闯关者与出题者有共
      同的基类）。
  **使用文件或者数据库作为存储对象，自行设计设计文件格式，或者数据库表结构。
  **在题目的要求下自行扩展功能，功能扩展适当者酌情加分。

 */

#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;

class Game {
private:
    int level;
    static vector<string> wordList;//被添加的单词列表
public:
    Game(int l = 0) {level = l;}
    ~Game(){}
    static vector<string> getWordList()
    {
        return wordList;
    }
    void Display();
};
vector<string> Game::wordList = {};

void Game::Display()
{
    cout << wordList[level];
    Sleep(2000);
    cout << "                   \r";
    string answer;
    cin >> answer;
    if (wordList[level] == answer) {
        cout << "答案正确,闯关成功" << endl;
        level++;
    }
}

class TestBuilder;
class Player;
class User {
private:
    string name;
    string password;
public:
    User(const string& n, const string& p): name(n), password(p) {}
    ~User(){}
    string getName(){return name;}
    string getPassword(){return password;}
    virtual void Register(){}
    virtual void Login(){}
};

class Player: public User {
private:
    int playerLevel;
    int experience;
    int grades;
    bool isLogin;
    static vector<Player> playerList;//已注册的玩家列表
public:
    Player(const string& n, const string& p, int e = 0, int l = 0, int g = 0, bool isL = 0): User(n, p)
    {
        playerLevel = l;
        experience = e;
        grades = g;
        isLogin = isL;
    }
    ~Player(){}
    static vector<Player> getList()
    {
    	return playerList;
	}
    void Register(string n,string p);
    void Login(string n,string p);
    void SearchPlayer(string name);
    void RankPlayersByGrades();
    void RankPlayersByExperience();
};
vector<Player> Player::playerList={};
void Player::Register(string& n,string& p)
{
    Player player(n,p);
    playerList.push_back(player);
}
void Player::Login(string& n,string& p)
{
    for(int i=0;i<(int)playerList.size();i++){
        if(playerList[i].getName()==n&&playerList[i].getPassword()==p)
            cout<<"玩家登录成功"<<endl;
    }
}
void SearchPlayer(const string& name)
{

}
void Player::RankPlayersByGrades()
{

}
void Player::RankPlayersByExperience()
{

}

class TestBuilder: public User {
private:
    int problemsNumbers;
    int grades;
    bool isLogin;
    static vector<TestBuilder> testBuilderList;//已注册的玩家列表
public:
    TestBuilder(const string& n, const string& p, int pNum = 0, int g = 0, bool isL = 0): User(n, p)
    {
        problemsNumbers = pNum;
        grades = g;
        isLogin = isL;
    }
    ~TestBuilder(){}
    void Register(string n,string p);
    void Login(string n,string p);
    void SearchTestBuilder();
    void RankPlayersByGrades();
    void RankPlayersByExperience();
    void AddWord();
};
vector<TestBuilder> TestBuilder::testBuilderList={};
void TestBuilder::Register(string n,string p)
{
    TestBuilder t(n,p);
    testBuilderList.push_back(t);
}
void TestBuilder::Login(string& n,string& p)
{
    for(int i=0;i<(int)testBuilderList.size();i++){
        if(testBuilderList[i].getName()==n&&testBuilderList[i].getPassword()==p)
            cout<<"管理者登录成功"<<endl;
    }
}
void TestBuilder::SearchTestBuilder()
{

}
void TestBuilder::RankPlayersByExperience()
{

}
void TestBuilder::RankPlayersByGrades()
{

}
void TestBuilder::AddWord()
{
    string s;
    Game::getWordList().push_back(s);
}


int main()
{
    int choice = 1;
    while (choice != 0) {
        cout << "\n\n\n\n" << "              *************************" << endl;
        cout <<  "                欢 迎 进入 单 词 消 除 游 戏 系 统 ! " << endl;
        cout <<  "\n                                 1  用户注册" << endl;
        cout <<  "\n                                 2  玩家登陆" << endl;
        cout <<  "\n                                 3  出题者登陆" << endl;
        cout <<  "\n                                 4  开始游戏" << endl;
        cout <<  "\n                                 5  出题" << endl;
        cout <<  "\n                                 6  继续游戏" << endl;
        cout <<  "\n                                 7  查询" << endl;
        cout <<  "\n                                 8  排行榜" << endl;
        cout <<  "\n                                 0  退出游戏" << endl;
        cout <<  "              **************************************" << endl;
        do {
            cin >> choice;
            if (choice < 0 || choice > 8)
                cout << "输入错误，请重新输入!" << endl;
            else
                break;
        } while (1);
        switch (choice) {
        case 0:

            break;
        case 1:

            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        case 5:

            break;
        case 6:

            break;
        case 7:

            break;
        case 8:

            break;
        }

    }
    return 0;
}
