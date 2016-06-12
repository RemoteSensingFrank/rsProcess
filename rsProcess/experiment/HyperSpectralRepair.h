#pragma once
#include"..\gdal\include\gdal_priv.h"
#include"..\Global.h"
#include<vector>
using namespace std;
#pragma comment(lib,"gdal_i.lib")

//�߹���ͼ���޸�����
class HyperRepairFunc
{
public:
	//Ӱ���޸���������ͨ��������޸���Ӱ��maskӰ��ʹ��޸�����ʼ����ֹ���εõ��޸����
	void RepairFunc_Run(const char* pathDeImg, const char* pathMaskImg, const char* pathReDst, int st, int end);

	//ģ���˻�Ӱ��
	void RepairFunc_SimuliDegradImg(const char* pathImg, const char* pathDeImg, int st, int end, CPOINT lup, CPOINT rbottom);
	
	//�����������
	void RepairFunc_GenerateTexture(const char* pathImg, int rangesize, const char* pathOut);

	//����Ӱ�񲨶�ȱʧ���޸�
	void RepairFunc_RepairPartial(int* dataImg, int* dataMask,int xsize,int ysize,int bands);

	//ȫ��Ӱ�񲨶�ȱʧ���޸�
	void RepairFunc_RepairTotal(int* dataImg, int* dataMask, int xsize, int ysize, int bands);

	//��ȡ�����Ԫ
	void RepairFunc_TextureElement(const char* pathEle, int rangesize, int num, const char* pathDist);

private:
	//����Ӱ���޸�����ʼ����ֹ����
	void RepairFunc_SetLossBands(int st, int end);

	//ȱʧ���ݺ��������ݽ���ƥ�䲢��Ȩ��
	//(����Ҫ�������в��ε�ȥ�������Ա��뽫�������ݶ��룬�ڴ�Ҫ��Ƚϴ��ڴ�ӳ�䣿)
	void RepairFunc_MatchLoss(int* dataImg, int* dataMask,int posx,int posy,int xsize, int ysize, int bands, vector<int>&matchPos, vector<float> &matchWeight);

	//Ȼ������޸�
	void RepairFunc_Repair(int* dataImg, int posx, int posy, int xsize, int ysize,vector<int>matchPos, vector<float> matchWeight);

private:
	int m_HyperRepairFunc_st;
	int m_HyperRepairFunc_end;
};

//�߹����޸����Ժ���
void HyperRepairFuncTest();