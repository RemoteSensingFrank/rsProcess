#pragma once
#include"lidar\LidarDefs.h"
//����������Ⱦ������
long LidarTriangleRenderLocal(LASSet* lasDataset);

long LidarTriangleRenderGlobal(LASSet* lasDataset, GDALTriangulation*  m_triangle);