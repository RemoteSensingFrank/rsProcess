#pragma once
#include<string>
#include<vector>
using namespace std;

//Ŀǰ���д�����1����ȷ���������0����������������ٶԴ��������е���

//��־���·��
//�����Ķ���
#define _log_path_ "c:\\process_log\\rsProcess.log"
#define PI 3.14159265358
#define MAX_NUM  99999999
#define MIN_NUM -99999999
#define WGS84LRadius 6378137
#define WGS84Eccentricity 0.0066943850
#define EQUAL_LIMIT 0.000001

//����ͷ��Ϣ
typedef struct stDataInfo
{
	unsigned int	nSensorType;
	unsigned int	nSensorOrder;
	unsigned int	nWidths;
	unsigned int	nSamples;
	unsigned int	nLines;
	unsigned int	nBands;
	unsigned int	nYear;
	unsigned int	nMonth;
	unsigned int	nDay;
	unsigned int	nSecond;
	unsigned int	nMode;
	__int64 nHeadOffset;
	__int64 nEofOffset;
}DINFO;

//ENVIͷ�ļ���ʽ
struct MapInfo {
	string projection;	//ͶӰ
	string directions;  //����
	double adfGeotransform[6];//������Ϣ
	int    zone;
};
typedef struct stImgHeader {
	//Ӱ�������Ϣ
	int imgWidth;
	int imgHeight;
	int imgBands;

	bool byteorder;	//�ֽ�˳�� һ����0 ����ĳЩ����ϵͳ�ֽڿ�����С����
	int  datatype;

	bool    bimgGeoProjection;	//�Ƿ����ͶӰ��Ϣ
	MapInfo mapInfo;			//ͶӰ�ֶ�
	string coordianteSys;	//�����ֶ�

	string interleave;		//Ӱ���Ų���ʽ

	//Ĭ����ʾ����
	bool bimgDefaultBands;
	vector<int> imgDefaultBands;

	//������
	bool bimgBandNames;
	vector<string> imgBandNnames;

	//������Ϣ
	bool bimgWaveLength;
	vector<float> imgWaveLength;

	//�벨����Ϣ
	bool bimgFWHM;
	vector<float> imgFWHM;
}ENVIHeader;

//SBET�ļ��ṹ
typedef struct stSbetElement
{
	double dGpsTime;
	double dLatitude;
	double dLongitude;
	double dHeight;
	double dVx;
	double dVy;
	double dVz;
	double dRoll;
	double dPitch;
	double dHeading;
	double dWander;
	double dAx;
	double dAy;
	double dAz;
	double dArx;
	double dAry;
	double dArz;
}SBETELEMENT;

//�����ݽṹ�Ķ���
//��ά�����ά�����ݽṹ
struct CPOINT
{
	int x;
	int y;
};
struct DPOINT
{
	double dX;
	double dY;
};
struct THREEDPOINT
{
	double dX;
	double dY;
	double dZ;
};


