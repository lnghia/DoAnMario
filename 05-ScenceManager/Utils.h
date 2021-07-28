#pragma once

#include <Windows.h>

#include <signal.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

#include "GameObject.h"


using namespace std;

void DebugOut(wchar_t* fmt, ...);

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);

vector<int> stringToVectorOfInt(string& str, string delimeters = "\t");
vector<char> StringToVectorOfChar(string& str, string delimeters = " ");

bool cmp(LPGAMEOBJECT a, LPGAMEOBJECT b);

bool doOverlap(const float& l1, const float& t1, const float& r1, const float& b1,
	const float& l2, const float& t2, const float& r2, const float& b2);