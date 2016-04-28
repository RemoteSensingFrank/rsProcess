#pragma once
#include"..\OPENCVTools.h"
#include<string>
using namespace std;

//���˻�Ӱ�����ݵĿ���ƴ�Ӵ���
struct adfAffineTrans
{
	double m_affineTransParameters[6];
};

class UAVMosaicFast
{
private:
	//����Ӱ���ȡ����任ϵ�� ������У���������һ��Ӱ��Ϊ��׼��
	long UAVMosaicFast_AffineTrans(vector<string> pszImages);

	//��ȡӰ��ƴ�ӵķ�Χ
	long UAVMosaicFast_GetMosaicRange(vector<string> pszImages,int& mosaic_width,int &mosaic_height);

	//Ӱ��ɫ��������һ��Ӱ��Ϊ��Ҫ������Ӱ�񣬵ڶ���Ӱ��Ϊ��׼Ӱ��
	long UAVMosaicFast_HistroMatch(unsigned char* imgBuffer1, unsigned char* imgBuffer2, int xsize1, int ysize1, int xsize2, int ysize2 );

	//ͨ������任������������������
	long UAVMosaicFast_AffineTrans(adfAffineTrans& affineTransParam, unsigned char* imgBuffer, int xsize, int ysize, unsigned char* imgMosaic,int mosaicx,int mosaicy);

public:
	//Ӱ����Ƕ
	long UAVMosaicFast_Mosaic(vector<string> pszImages,const char* pathDst);

private:
	vector<adfAffineTrans> m_affineTransParameters;
};