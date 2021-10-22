#pragma once
#include <string>

using namespace std;

class User
{
private:
	string id;
	string password;

public:

	// Constructor
	User();
	//Destructor
	virtual ~User();

	void setId(string id);
	void setPassword(string password);
	string getId() const;
	string getPassword() const;
};
