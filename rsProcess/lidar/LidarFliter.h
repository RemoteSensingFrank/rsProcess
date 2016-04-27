#pragma once
#include"Geometry.h"
#include"LidarDefs.h"
#include"..\Global.h"

//lidar�����˲�����
class LidarFliterBase
{
public:
	//lidar�����˲�����
	long LidarFliterParameter(double fliterWidth, double fliterHeight) {
		m_FliterWidth = fliterWidth;
		m_FliterHeight = fliterHeight;
	}

	//�����˲��������
	virtual long LidarFliterProcess(LASSet &lasDataset)=0;

	//�����˲���Ԫ
	virtual long LidarFliter(LASSet &lasDataset) = 0;

public:
	//�˲����ڿ�Ⱥ��˲����ڸ߶�
	double m_FliterWidth;
	double m_FliterHeight;
};

//���������ֵ�˲�
class LidarMeanFliter : public LidarFliterBase
{
public:
	//�����˲��������
	long LidarFliterProcess(LASSet &lasDataset) { return 0; }

	//�����˲���Ԫ
	long LidarFliter(LASSet &lasDataset) { return 0; }
};

//����ƽ���˲�
class LidarPlaneFliter : public LidarFliterBase
{
public:
	//�����˲��������
	long LidarFliterProcess(LASSet &lasDataset) { return 0; }

	//�����˲���Ԫ
	long LidarFliter(LASSet &lasDataset) { return 0; }
};

//���Ƶ���������˲��˲�������������õ������
class LidarGroundFliter : public LidarFliterBase
{
public:
	//�����˲��������
	long LidarFliterProcess(LASSet &lasDataset) { return 0; }

	//�����˲���Ԫ
	long LidarFliter(LASSet &lasDataset) { return 0; }
};