#pragma once
#include"..\Global.h"
#include"..\GeoPOSProcess.h"
//�������ݲ�Ʒ��������,����Ϊ����ࣻ
//��������������������ݲ�Ʒ��������
class Level2Process
{
public:
	//===============================================================================================================================================================
	/*
	���ܣ�����IGM��EOԪ�ض�Ӱ�����У����ȡ��DEM������Ӱ���У��
	������1.char *pFile У��ǰӰ��
			2.char *pCFile У����Ӱ��
			3.char *pEOFile ��POS����õ���EOԪ��
			4.float fGSDX, float fGSDY ����ֱ���
			5.double fElevation ����ƽ���߳�
			6. int nEOOffset EO��ƫ����
			7.float fFov, float fIFov ���ӳ���˲ʱ�ӳ�
			8.float fFocalLen ����������
			9.bool bIGM �Ƿ�����IGM����
			10.char *pIGMFile IGM�ļ�·��
			11.bool bInverse=false �Ƿ�ת
	*/
	virtual long Level2Proc_Product2A(const char *pFile, const char *pCFile, const char *pEOFile, float fGSDX, float fGSDY, double fElevation, int nEOOffset,
		float fFov, float fIFov, float fFocalLen, bool bIGM, const char *pIGMFile,bool bInverse=false)=0;
	//��2A�����ݲ�Ʒ�����������2B�����ݲ�Ʒ����������Ҫ���ӵĽӿ�ΪDEM����·����������·��ΪNULL���������ͬ���������DEM������DEM���н���
	virtual long Level2Proc_Product2B(const char *pFile, const char *pCFile, const char *pEOFile, float fGSDX, float fGSDY, double fElevation, int nEOOffset,
		float fFov, float fIFov, float fFocalLen, const char* pDEMFile, bool bIGM, const char *pIGMFile, bool bInverse = false)=0;
	//================================================================================================================================================================
	/*
	���ܣ���������Ӱ�������Ԫ���м�Ӱ���壩�����д�����꣬��������õ��Ĵ�����걣�浽��ά��������
	����Ӱ���е������С�㣬��������ϵ�д������ת��ΪUTM���꣬��Ϊ������Ҫ����UTM����
	������nLines		Ӱ��߶�
	nImgWidth		Ӱ�������Ԫ���м�Ӱ���壩
	nInterval		Ӱ��EO�ļ�����Ӱ���ж�Ӧ��ϵ
	pGoundPt		��ά������
	pEOElement	EO�ļ���
	fFov			���ӳ��Ƕ�
	fFocalLen		����
	minPnt��		Ӱ������С��������
	maxPnt��		Ӱ��������������
	*/
	//����EOԪ�ؼ���IGM�ļ�
	virtual long Level2Proc_CalculateIGMFile(int nLines, int nImgWidth, int nInterval, THREEDPOINT *pGoundPt, EO *pEOElement, float fFov, float fFocalLen, DPOINT &minPt, DPOINT &maxPt, double dB, double dL, bool bInverse);

	//����EOԪ�غ�DEMԪ�ؼ���IGM�ļ�
	virtual long Level2Proc_CalculateIGMFileDEM(int nLines, int nImgWidth, int nInterval, THREEDPOINT *pGoundPt, EO *pEOElement, float fFov, float fFocalLen, DPOINT &minPt, DPOINT &maxPt, double dB, double dL, const char* pathDEM, bool bInverse);
	/*
	const char* pIGMFile:	�ļ����·��
	THREEDPOINT *pGoundPnt: IGM�ļ�
	int nImgWidth, int nHeight IGM�ļ���Ⱥ͸߶�
	IGM�ļ���������ֱ���ö������ļ�д��ȥ�˶���ʱ��Ҳֱ�Ӷ��ͺ���
	*/
	virtual long Level2Proc_SaveIGMFile(const char* pIGMFile, THREEDPOINT *pGoundPnt, int nImgWidth, int nImgHeight);

	//��ȡIGM�ļ�
	virtual long Level2Proc_ReadIGMFile(const char* pIGMFile, THREEDPOINT *pGoundPnt, int &nImgWidth, int &nImgHeight);

	//��ȡDEM�ļ�
	/*
	������const char *pDEMFile DEM�ļ�·��
	int &nSamples, int &nLines DEM����������
	DPOINT rangePnt[]      DEM����Χ
	double dGeoTransform[] DEM����任ϵ��
	int &nZone,int projType	ͶӰ����ͶӰ���� 1ΪWGS84��γ�� 2ΪUTMͶӰ��0Ϊ����ͶӰ
	double *pDEMPt DEM����
	*/
	virtual long Level2Proc_ReadDEMFile(const char *pDEMFile, int &nSamples, int &nLines, DPOINT rangePnt[], double dGeoTransform[], int &nZone, int &projType, double *&pDEMPt);
};

//ȫ�׶����ݴ��������࣬����A����Ʒ������B����Ʒ��������
class QPDLevel2Process : public Level2Process
{
public:
	long Level2Proc_Product2A(const char *pFile, const char *pCFile, const char *pEOFile, float fGSDX, float fGSDY, double fElevation, int nEOOffset,
		float fFov, float fIFov, float fFocalLen, bool bIGM, const char *pIGMFile, bool bInverse = false);


	//��2A�����ݲ�Ʒ�����������2B�����ݲ�Ʒ����������Ҫ���ӵĽӿ�ΪDEM����·����������·��ΪNULL���������ͬ���������DEM������DEM���н���
	long Level2Proc_Product2B(const char *pFile, const char *pCFile, const char *pEOFile, float fGSDX, float fGSDY, double fElevation, int nEOOffset,
		float fFov, float fIFov, float fFocalLen, const char* pDEMFile, bool bIGM, const char *pIGMFile, bool bInverse = false);

};