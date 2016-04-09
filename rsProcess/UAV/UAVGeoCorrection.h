#pragma once

#include"..\GeoPOSProcess.h"
#include"..\AerialProduct\Level2Process.h"

//���˻�Ӱ��POS���ݴ���
class UAVPOSProcess : public GeoPOSProcess
{
public:
	//1.��ȡӰ������
	long GeoPOSProc_ReadPartPOS(const char *pPOSFile, long nLines, double &dB, double &dL, double &dH, int nbeginLine);

	//2.���˻�Ӱ��һ�㲻��Ҫ����SBET�ļ������˻�POS���ݿ���ֱ�ӻ�ȡ
	long GeoPOSProc_ExtractSBET(const char* pathSBET, const char* pathEvent, const char* pathPOS, float fOffsetGPS) { return 0; }

	//3.�ɵ���POS���ݽ���EOԪ�أ����˻������ǰ��ýǺͰ���ʸ��
	long GeoPOSProc_ExtractEO(POS m_perpos, EO &m_pereo);
};

//UAV�ļ���У������ɨʽ��Ӱ�񼸺�У���бȽϴ�Ĳ���
//UAV��һ��Ӱ��ֻ��1��POS��Ϣ
//1.��У��������Ӧ�ý����������Ϊ��֪��
//2.��POS���ݽ����EOԪ��
//3.ͨ��EOԪ�ؽ���У��
class UAVGeoCorrection : public Level2Process
{
public:
	//1.��Ӱ�񼯽���У��
	long UAVGeoCorrection_GeoCorrect(const char* pathSrcDir, const char* pathDstDir, const char* pathPOS, const char* pathDEM,
																			int IMGbegline, int POSbegline, int lines,double fLen , double fGSD, double AvgHeight);

	//2.�Ե���Ӱ�����У��
	long UAVGeoCorrection_GeoCorrect(const char* pathSrc, const char* pathDst, EO  m_preeo, double dL, double dB, double fLen, double fGSD, double AvgHeight, char* pathDEM = NULL);

	//3.����ÿ����ĵ�������
	long UAVGeoCorrection_GeoPntsProb(double dB, double dL, double dH, EO pEO, double fLen, int width, int height, THREEDPOINT *pGoundPnt, DPOINT* pMapImgPnt, int pntNum);

	//4.��ȷ����ÿ���������Ȼ�����У��
	long UAVGeoCorrection_GeoPntsAccu(double dB, double dL, double dH, EO pEO, double fLen, int width, int height, THREEDPOINT *pGoundPnt,const char* pathDEM);
};