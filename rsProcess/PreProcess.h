#pragma once
#include"Global.h"
#include<vector>
using namespace std;
/*
	����Ԥ�������
	1.����©�м��
	2.����ͷ��Ϣ��ȡ�����������ͷ��ָ�趨��Ӱ�񲨶���������������ʼ��һ������֡��ƫ����
	3.����©�в�ֵ�޸�����
	4.����0�����ݣ�0��������ԭʼ����������0�������Ǿ������ݼ��������޸�������
*/
class PreProcess 
{
public:
	virtual long PreProc_GetHeadInfo(FILE *fRAW, DINFO &mDataHeader, vector<short> &nLeakFrameType, vector<int> &nLeakFrameSize, int &nLeakFrameCount) = 0;

	/*
		nLeakFrameType 0Ϊ��©�У�1Ϊ©����֡��2Ϊ©�У�3Ϊ©����©֡ͷ�ļ�
	*/
	virtual long PreProc_LeakLineCheck(const char *pRAWData, FILE *fRAW, DINFO &mDataHeader, vector<short> &nLeakFrameType, vector<int> &nLeakFrameSize, int &nLeakFrameCount) = 0;

	virtual long PreProc_LeakLineInterpolate(FILE *fRAW, unsigned short *pRepairBuffer, DINFO mDataHeader, vector<short> nLeakFrameType,vector<int> nLeakFrameSize, 
															 int nLoc, __int64 nOffset, unsigned short *pfBuffer, unsigned short *plBuffer, unsigned short *pLeakBuffer)=0;

	/*
		nFixLinesΪһ�ζ�ȡ��֡�������ýϴ�Ķ�ȡ֡������Ч�ʱȽϸ�,����Ҫ�����ڴ������
	*/
	virtual long PreProc_GenerateD0Data(const char *pRAWData, const  char *pData, DINFO mDataHeader, vector<short> nLeakFrameType, vector<int> nLeakFrameSize, int nLeakFrameCount, const int nFixLines)=0;
};

//ȫ�׶�����Ԥ����
class QPDPreProcess: public PreProcess
{
public:
	long PreProc_GetHeadInfo(FILE *fRAW, DINFO &mDataHeader, vector<short> &nLeakFrameType, vector<int> &nLeakFrameSize, int &nLeakFrameCount);

	long PreProc_LeakLineCheck(const char *pRAWData, DINFO &mDataHeader, vector<short> &nLeakFrameType, vector<int> &nLeakFrameSize, int &nLeakFrameCount);

	long PreProc_LeakLineInterpolate(FILE *fRAW, unsigned short *pRepairBuffer, DINFO mDataHeader, vector<short> nLeakFrameType, vector<int> nLeakFrameSize,
											int nLoc, __int64 nOffset, unsigned short *pfBuffer, unsigned short *plBuffer, unsigned short *pLeakBuffer);

	long PreProc_GenerateD0Data(const char *pRAWData, const  char *pData, DINFO mDataHeader, vector<short> nLeakFrameType, vector<int> nLeakFrameSize, int nLeakFrameCount, const int nFixLines);
};