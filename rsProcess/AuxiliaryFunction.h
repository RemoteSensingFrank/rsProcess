#pragma once
#include"Global.h"

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