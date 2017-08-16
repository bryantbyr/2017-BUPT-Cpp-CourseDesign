#pragma once
#include "common.h"

class User {
private:
	string name;
	string password;
public:
	User(const string& n, const string& p) : name(n), password(p) {}
	~User() {}
	string getName() { return name; }
	string getPassword() { return password; }
	virtual void Register(SOCKET sockClient) {}
	virtual int Login(SOCKET sockClient) { return false; }
};
