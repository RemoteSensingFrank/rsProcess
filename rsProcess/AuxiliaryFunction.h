#pragma once
#include"Global.h"

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

//дENVIͷ�ļ���ʽ
/*
	const char* pathHeader:ͷ�ļ����·��
	ENVIHeader mImgHeader :ͷ�ļ���Ϣ
*/
void WriteENVIHeader(const char* pathHeader, ENVIHeader mImgHeader);

//��ȡ���ݾ�ֵ�ͱ�׼��
/*
	unsigned char *pBuffer������
	int nSamples, int nLines, int nBand���������кͲ�����
	float &fAverage, float &fDeviate�����ݾ�ֵ�ͱ�׼��
*/
void GetAveDev(unsigned char *pBuffer, int nSamples, int nLines, int nBand, float &fAverage, float &fDeviate);
void GetAveDev(unsigned short *pBuffer, int nSamples, int nLines, int nBand, float &fAverage, float &fDeviate);
void GetAveDev(float *pBuffer, int nSamples, int nLines, int nBand, float &fAverage, float &fDeviate);

//���ݹ�һ��
void NormalizeData(float* pBuffer,int length,float* pNormalBuffer);
void NormalizeData(double* pBuffer, int length, double* pNormalBuffer);
//��ȡ���ϵ��
float GetCoefficient(float* data1, float* data2, int num);
float GetSSD(float* data1, float* data2, int num);
//�����ݽ��в���
/*
	�ڼ���У����У��ǰ���ݺ�У�������ݲ�������ȫ��Ӧ
	ͨ�����������õ�û������λ������ֵ
*/
//��float���͵����ݺ�unsigned short���͵����ݽ��д���
void GetImgSample(unsigned char *pImgBuffer, DPOINT &minPt, DPOINT &maxPt, THREEDPOINT *pGoundPt, float fGSDX, float fGSDY, int nSamples, int nLines, int nReSamples, int nReLines, unsigned char *pRegBuffer);
void GetImgSample(unsigned short *pImgBuffer, DPOINT &minPt, DPOINT &maxPt, THREEDPOINT *pGoundPt, float fGSDX, float fGSDY, int nSamples, int nLines, int nReSamples, int nReLines, unsigned short *pRegBuffer);
void GetImgSample(float *pImgBuffer, DPOINT &minPt, DPOINT &maxPt, THREEDPOINT *pGoundPt, float fGSDX, float fGSDY, int nSamples, int nLines, int nReSamples, int nReLines, float *pRegBuffer);

void GetImgSample(float *pImgBuffer, DPOINT *pPositions, int nImgWidth, int nLines, int nReImgWidth, int nReLines, float *pRegBuffer);//pPositionsΪУ��ǰӰ��Ϊֹ��ӦУ����Ӱ���Ӱ��λ��
void GetImgSample(unsigned char *pImgBuffer, DPOINT *pPositions, int nImgWidth, int nLines, int nReImgWidth, int nReLines, unsigned char *pRegBuffer);//pPositionsΪУ��ǰӰ��Ϊֹ��ӦУ����Ӱ���Ӱ��λ��


//��ȡ����Ӱ��ƥ��ֱ��ͼ
//��img2Ϊ��׼
void GetImgHistroMatch(double* img1, double *img2, int xsize1, int ysize1, int xsize2, int ysize2, int minPix, int maxPix, int* histroMap);
void GetImgHistroMatch(unsigned char* img1, unsigned char*img2, int xsize1, int ysize1, int xsize2, int ysize2, int minPix, int maxPix, int* histroMap);

//��ȡ����֮��ľ���
double GetDisofPoints(THREEDPOINT pnt1, THREEDPOINT pnt2);
double GetDisofPoints(CPOINT pnt1, CPOINT pnt2);

//��ȡӰ���б�
void GetImageList(const char* pathList, vector<string> &pszImage);

//�������������������ƽ��ֵ
double MaxAvg(double data1, double data2, double data3);
//����������С��������ƽ��ֵ
double MinAvg(double data1, double data2, double data3);

