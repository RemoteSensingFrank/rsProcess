#pragma once
#include"LidarDefs.h"
#include"..\Global.h"

//�����������ֲ�
long LidarTinCreateLocal(LASSet &m_lasDataset);

//����ȫ��������
GDALTriangulation* LidarTinCreateGlobal(LASSet &m_lasDataset);

//������������ֲ�

