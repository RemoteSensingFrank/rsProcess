#pragma once
#include<vector>
using namespace std;
/*
	����Ԥ�������
*/
class PreProcess 
{
	virtual long PreProc_LeakLineCheck(FILE* pathfRaw, vector<short> &nLeakFrameType, vector<int> &nLeakFrameSize, vector<__int64> &nLeakByteCount, int &nLeakFrameCount) = 0;
};