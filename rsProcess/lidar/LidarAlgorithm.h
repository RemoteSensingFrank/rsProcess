#pragma once
#include"LidarDefs.h"
#include"..\Global.h"


//�о��㷨����������̵ģ��ƺ���̫�÷�װ������˵��װ����ûʲô����

//����Lidar���ݴ�����������������һ�����⣬�ƺ�Ӧ�ø��ݲ��ֵ���д�����Ҳ���Ǵ�˵�е�LODģ��
long LidarGetFrustumPlanes(float** g_frustumPlanes);

//�жϵ��Ƿ��ڽ�����
bool LidarPointInFrustum(float** g_frustumPlanes,THREEDPOINT pnt);

//ͨ���ӵ�λ�ú��ӵ㷽���ȡ�۲����
long LidarGetRange(LASSet m_lasDataset, vector<int> &m_rectIdx);

//�����ӵ�ͷ��򴴽�������
long LidarTinCreate(LASSet &m_lasDataset,THREEDPOINT centerView, vector<int> &m_rectIdx);