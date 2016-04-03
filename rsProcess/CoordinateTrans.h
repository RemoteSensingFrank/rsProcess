#pragma once
/*  Created on : 2016 - 04 - 01
* Author : wuwei
*		description��Ӱ�����ݲ�Ʒ��������ת����
*/
#include "stdafx.h"
#include "CoordinateTrans.h"
#include "Global.h"
#include "matrixOperation.h"
#include "tsmUTM.h"
//#include"GeoPOSProcess.h"
#include <iostream>
#include<fstream>

using namespace std;
#include "gdal/include/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

/*
����ת����ṹ���壺
���ܣ�  1.�������굽��γ��֮���ת����WGS84����
		2.����EO����ÿ��Ӱ����ڳ�ͼ����ϵ�µ�����
		3.�ɳ�ͼ����ϵ�µ��������UTM����ϵ�µ�����
		4.����ԭʼӰ����ÿ����Ԫλ�ö�Ӧ��У��Ӱ���ϵ�λ��
		5.��֪ת��ǰÿ������λ�ö�Ӧת�������ص�λ�� ����ת����Ӧ����ת����ÿ�����ص�ֵ
*/
//WGS84 ��������ϵ�;�γ�ȵ�ת��
class CoordinateTrans
{
public:

	long BLHToXYZ(double dB, double dL, double dH, THREEDPOINT &XYZPnt);

	long XYZToBLHS(double dB, double dL, double dH, THREEDPOINT *pGoundPnt, int PxelNum);

	long XYZToBLH(THREEDPOINT XYZPnt, double &dB, double &dL, double &dH);

	//virtual long CoordinateImgToGeoMap(int nImgHeight, int nImgWidth, THREEDPOINT *pGoundPt, EO *pdEOData, float fFov, float fFocalLen);
	//virtual long CoordinateGeoMapToUTM(double dB, double dL, double dH, THREEDPOINT *pGoundPnt, int nImgHeight, int nImgWidth, DPOINT &minPnt, DPOINT &maxPnt);
	//virtual long CoordinateSrcToCorrectMap(THREEDPOINT *pGoundPnts, DPOINT* pfMapPositions, float fGSD, DPOINT minPnt, DPOINT maxPnt, int nImgHeight, int nImgWidth);
};
