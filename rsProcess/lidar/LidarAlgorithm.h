#pragma once
#include"LidarDefs.h"
#include"..\Global.h"

//�����ӵ�ͷ��򴴽�������
long LidarTinCreateLocal(LASSet &m_lasDataset);

//����ȫ��������
GDALTriangulation* LidarTinCreateGlobal(LASSet &m_lasDataset);