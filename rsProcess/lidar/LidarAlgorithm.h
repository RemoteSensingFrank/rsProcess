#pragma once
#include"LidarDefs.h"
#include"..\Global.h"

//�о��㷨����������̵ģ��ƺ���̫�÷�װ������˵��װ����ûʲô����

//����Lidar���ݴ�����������������һ�����⣬�ƺ�Ӧ�ø��ݲ��ֵ���д�����Ҳ���Ǵ�˵�е�LODģ��
//���������ڻ�û���LODģ����ôʵ�֣����Ƿֿ鴴���ƺ��ǿ��е�

//ͨ���ӵ�λ�ú��ӵ㷽���ȡ�۲����
long LidarGetRange(LASSet &m_lasDataset, THREEDPOINT viewPnt, THREEDPOINT viewDrection, Rect &viewRect);

//�����ӵ�ͷ��򴴽�������
long LidarTinCreate(LASSet &m_lasDataset, THREEDPOINT viewPnt, THREEDPOINT viewDrection);