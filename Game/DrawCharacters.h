#pragma once
#pragma once
#include<string>
using namespace std;

class DrawCharacters
{
public:
	void DrawNumber(int max, float x, float y, int s);
	void DrawHP(float x, float y, int type, int s);
	int Convert(char c);
};