#include <Windows.h>

#include "Utils.h"

void DebugOut(wchar_t* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	wchar_t dbg_out[4096];
	vswprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugString(dbg_out);
}

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

/*
char * string to wchar_t* string.
*/
wstring ToWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring wstr(wcstring);

	// delete wcstring   // << can I ? 
	return wstr;
}

/*
	Convert char* string to wchar_t* string.
*/
LPCWSTR ToLPCWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring* w = new wstring(wcstring);

	// delete wcstring   // << can I ? 
	return w->c_str();
}

vector<int> stringToVectorOfInt(string& str, string delimeters)
{
	vector<int> ans;

	vector<string> tokens = split(str, delimeters);

	for (auto& token : tokens) {
		ans.push_back(stoi(token));
	}

	return ans;
}

vector<char> StringToVectorOfChar(string& str, string delimeters)
{
	vector<char> ans;

	vector<string> tokens = split(str, delimeters);

	for (auto& token : tokens) {
		ans.push_back(token[0]);
	}

	return ans;
}

bool cmp(LPGAMEOBJECT a, LPGAMEOBJECT b)
{
	return a->GetRenderPriority() < b->GetRenderPriority();
}

bool doOverlap(const float& l1, const float& t1, const float& r1, const float& b1, const float& l2, const float& t2, const float& r2, const float& b2)
{
	// To check if either rectangle is actually a line
	// For example :  l1 ={-1,0}  r1={1,1}  l2={0,-1}
	// r2={0,1}

	if (l1 == r1 || t1 == b2 || l2 == r2
		|| t2 == b2) {
		// the line cannot have positive overlap
		return false;
	}

	// If one rectangle is on left side of other
	if (l1 >= r2 || l2 >= r1)
		return false;

	// If one rectangle is above other
	if (t1 <= b2 || t2 <= b1)
		return false;

	return true;
}
