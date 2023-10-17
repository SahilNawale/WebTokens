#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <ctime>
using namespace std;

string getMainHashFromToken(string token)
{
	token.append(".x.x");
	string res;
	int i = 0;
	while (token[i] != '.')
	{
		i++;
	}
	i++;
	while (token[i] != '.')
	{
		res = res + token[i];
		i++;
	}
	return res;
}

int main()
{
    string s = "hello";

    s.append("b.xx");
    cout<<getMainHashFromToken("asda.adawd.awdad.x.x");


    return 0;
}