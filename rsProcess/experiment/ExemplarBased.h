#pragma once
#include"..\AuxiliaryFunction.h"
#include"..\Global.h"

class ExemplarBased
{
private:
	//ͨ��Mask���ݸ��±߽�
	void ExemplarBased_UpdateEdge(float* mskData,int xsize,int ysize,vector<CPOINT> &edge);

	//ͨ��Ӱ�����Mask����
	void ExemplarBased_UpdateMask(float* imgData, float *mskData, int xsize, int ysize);

	//ͨ���߽��Ӱ������������޸�λ��
	void ExemplarBased_GetPriority(float* imgData, int xsize, int ysize, vector<CPOINT> edge,CPOINT &pos);

	//���������޸�λ�ý����޸�
	void ExemplarBased_PriorityInpaint(CPOINT pos, int regionSize, float* imgData, float* mskData, int xsize, int ysize);

	//�������ߵĴ�ֱ����
	void ExemplarBased_Np(vector<CPOINT> edge, CPOINT pos, float* imgData,int xisze,int ysize, double* np);

	//������ն��߷���
	void ExemplarBased_Ip(float* imgData, CPOINT pos, int xsize, int ysize, double*ip);

public:
	//Ӱ���޸�
	void ExemplarBased_Inpaint(const char* pathImg,const char* pathMsk,const char* pathDst);
};