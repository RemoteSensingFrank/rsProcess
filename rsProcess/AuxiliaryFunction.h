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

//�����ݽ��в���
/*
	�ڼ���У����У��ǰ���ݺ�У�������ݲ�������ȫ��Ӧ
	ͨ�����������õ�û������λ������ֵ
*/
//��float���͵����ݺ�unsigned short���͵����ݽ��д���
void GetImgSample(unsigned short *pImgBuffer, DPOINT &minPt, DPOINT &maxPt, THREEDPOINT *pGoundPt, float fGSDX, float fGSDY, int nSamples, int nLines, int nReSamples, int nReLines, unsigned short *pRegBuffer);
void GetImgSample(float *pImgBuffer, DPOINT &minPt, DPOINT &maxPt, THREEDPOINT *pGoundPt, float fGSDX, float fGSDY, int nSamples, int nLines, int nReSamples, int nReLines, float *pRegBuffer);