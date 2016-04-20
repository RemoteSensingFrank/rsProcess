#pragma once
#include"..\OPENCVTools.h"
#include"UAVGeoCorrection.h"
#include<vector>

using namespace std;
using namespace cv;

/*�������ϵ�ṹ�� 4���ǵ�*/
struct CornerCoordi4
{
	double xPixel[4];			//�����������x
	double yPixel[4];			//�����������y

	double xPic[4];				//�����Ƭ����x
	double yPic[4];				//�����Ƭ����y
};
/*�������ϵ�ṹ�� 8���ǵ�*/
struct CornerCoordi8
{
	double xPixel[8];			//�����������x
	double yPixel[8];			//�����������y

	double xPic[8];				//�����Ƭ����x
	double yPic[8];				//�����Ƭ����y
};


/*��������һ�������Ĺ��߳���������Ӱ������OPENCV�Ĺ��߱Ͼ�����SFM�ģ�������Ӱ������̫����*/
class UAVPhotogrammetryTools
{
public:
	//1.�ռ�ǰ������ �����ⷽλԪ�غ�ƥ������ռ����ά����
	long UAVPhotogrammetryTools_UAVFesction(vector<Point2f> pnt1,EO imgEO1, vector<Point2f> pnt2, EO imgEO2,vector<Point3d> &points);

	//2.�ռ�󷽽��ᣬ���ݿ��Ƶ��Ӱ������Ӱ���ⷽλԪ��
	long UAVPhotogrammetryTools_UAVResction(vector<Point2f> pnt1, vector<Point3d> pnt2,EO &eoElement);

	//3.�ռ���Զ��򣬸���ƥ�����пռ���Զ���
	long UAVPhotogrammetryTools_ROrientation(vector<Point2f> pnt1, vector<Point2f> pnt2, REO &reoRElementL, REO &reoRElementR);

	//4.���Զ���
	long UAVPhotogrammetryTools_AOrientation(vector<Point2f> pntModel1, vector<Point2f> pntModel2, REO& REOl, REO& REOr,vector<Point3d> pntGeo, EO &eoAElement,double &lamda);

	//6.��������ڲ���
	long UAVPhotogrammetryTools_SetParam(double len,double px0,double py0,double tk[3],double tp[3],double alpha,double belta);

	//7.�������굽Ӱ�������ת��ϵ��
	long UAVPhotogrammetryTools_PixelToImgTrans(CornerCoordi4 m_coordiTrans4);		//�ĸ����Ŀ������ϵ��ת��
	long UAVPhotogrammetryTools_PixelToImgTrans(CornerCoordi8 m_coordiTrans8);		//�˸����Ŀ������ϵ��ת��

	//8.������������У��
	long UAVPhotogrammetryTools_DistortionCorrection(vector<Point2f> &pntCamera);

	//TODO:5.������ƽ��
	long UAVPhotogrammetryTools_BundlerAdj();

	//8.�����Ժ�������Ϊ��Ԫ���������ȡ
	friend void PhotogrammetryToolsTest();

private:
	//1.����ڶ���
	long UAVPhotogrammetryTools_IOrientation(vector<Point2f> &pointImg);

	//��Ӱ��������Ҫ��궨�������������ڲ�Ӧ�ø���
	double m_PixelToImgTrans[6];				//���������굽Ӱ�������ת��
	double m_fLen;								//������� mΪ��λ
	double m_px, m_py;							//�������Ƭ�Ķ�Ӧλ��
	double m_k[3], m_p[3], m_alpha, m_belta;	//������������
	bool  isInternal;
};

void PhotogrammetryToolsTest();