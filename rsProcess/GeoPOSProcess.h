#pragma once
#include"Global.h"
#include"AuxiliaryFunction.h"

#include<vector>
using namespace std;

//POS��EO���ݴ����ļ����ļ��а���POS��EO���ݵĴ���

//EO��POS���ݽṹ�Ķ���
typedef struct stExteriorOrientation
{
	//������Ԫ��
	double m_dX;
	double m_dY;
	double m_dZ;

	//������Ԫ��
	double m_phia;
	double m_omega;
	double m_kappa;

	//��ת����
	double m_dRMatrix[9];
}EO;
typedef struct stRelativeOrientation
{
	//������Ԫ��
	double m_Bx;
	double m_By;
	double m_Bz;
	//������Ԫ��
	double m_phia;
	double m_omega;
	double m_kappa;

	//��ת����
	double m_dRMatrix[9];
}REO;
typedef struct stPositOrientationSys
{
	//��γ�Ⱥ͸߶�
	double m_latitude;
	double m_longitude;
	double m_height;

	//�����������ƫ��
	double m_roll;
	double m_pitch;
	double m_yaw;
}POS;

class GeoPOSProcess
{
	//�������������
public:
	GeoPOSProcess() {
		m_geo_POS = NULL;
		m_geo_EO = NULL;
	}
	~GeoPOSProcess() {
		if (m_geo_EO != NULL)
			delete[]m_geo_EO;
		if (m_geo_POS != NULL)
			delete[]m_geo_POS;
	}

	//���麯����������ʱ���������
	//��ȡPOS���ݣ����麯��������POS���ݴ�����������ش˺���
	/*
		const char *pPOSFile:POS����·��
		long nLines			:POS��������
		double &dB, double &dL, double &dH ���ľ�γ�Ⱥ͸߶�
		int nbeginLine		:POS����ͷ����
	*/
	virtual long GeoPOSProc_ReadPartPOS(const char *pPOSFile, long nLines, double &dB, double &dL, double &dH, int nbeginLine) = 0;
	
	//��SBET�ļ��н����POS����
	virtual long GeoPOSProc_ExtractSBET(const char* pathSBET, const char* pathEvent, const char* pathPOS, float fOffsetGPS)=0;

	//��POS���ݽ���EOԪ��
	virtual long GeoPOSProc_ExtractEO(const char *pPOSFile, int nLines, const  char *pEOFile, THREEDPOINT THETA, float *fSetupAngle, int nbeginLine);

	/*
		���ܣ���ȡ����õ���EO����
		������1.const char* pathEO EOԪ��·��
		2.double &dB ����ƽ��γ��
		3.double &dL ����ƽ������
		4.double &dH ����ƽ���߶�
	*/
	virtual long GeoPOSProc_ReadEOFile(const char* pathEO, double &dB, double &dL, double &dH);

//private:
	//����POS���ݼ��㺽����������
	//EMMatrix Ϊ��������ϵ����ͼ����ϵ��ת��
	long GeoPOSProc_EOQuadrant(int nLines, double EMMatrix[], THREEDPOINT &XYZPoint);
	//���ݵ���POS���㺽����������
	long GeoPOSProc_EOQuadrant(POS curPOS, double EMMatrix[], THREEDPOINT &XYZPoint);

	//����POS��ȡEOԪ�غ���ת����������ļ�����������
	long GeoPOSProc_EOMatrixTurn(int nCurLine, THREEDPOINT &XYZPoint, int nQuadNum, double EMMatrix[], THREEDPOINT ANGLETHETA, THREEDPOINT POSISTTHETA, FILE *fEO);
	//���ݵ���POS���ݻ�ȡEOԪ��
	long GeoPOSProc_EOMatrixTurn(POS pdfPOS, THREEDPOINT &XYZPoint, THREEDPOINT THETA, int nQuadNum, double EMMatrix[], EO &pdfEO);

public:
	//POS���ݺ�EO����
	POS* m_geo_POS;
	EO*  m_geo_EO;

	//�����򺽴����ľ�γ�Ⱥͷ��е�ƽ���߶�
	double m_geo_dB;
	double m_geo_dL;
	double m_geo_dH;
	int    m_nPOSLines;	//POS��������
};

//ȫ�׶����ݴ���
class QPDGeoPOSProcess : public GeoPOSProcess
{
public:
	//��SBET�ļ��н����POS����
	long GeoPOSProc_ExtractSBET(const char* pathSBET, const char* pathEvent, const char* pathPOS, float fOffsetGPS);

	//��ȡPOS����
	long GeoPOSProc_ReadPartPOS(const char *pPOSFile, long nLines, double &dB, double &dL, double &dH, int nbeginLine);

	//��ȡEOԪ�ص�����
	long GeoPOSProc_GetEOLines(const char *pEoFile, int &nEOLines);
};