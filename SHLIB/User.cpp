#include "User.h"
#include "grammarCheck.h"

User::User()
{
}

User::~User()
{
}

void User::setId(string id)
{
	this->id = id;
}

string User::getId() const
{
	return id;
}

void User::setPassword(string password)
{
	this->password = password;
}

string User::getPassword() const
{
	return password;
}
