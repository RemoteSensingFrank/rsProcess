#pragma once
#include"..\gdal\include\gdal_priv.h"
#include"..\Global.h"
#include"../FrequencyFunc/DCTTrans.h"
#include<ctime>
#include<vector>
#include<algorithm>
#include<numeric>
using namespace std;
#pragma comment(lib,"gdal_i.lib")

//�߹���ͼ���޸�����
class HyperRepairFunc
{
public:
	//Ӱ���޸���������ͨ��������޸���Ӱ��maskӰ��ʹ��޸�����ʼ����ֹ���εõ��޸����
	void RepairFunc_SpectralRun(const char* pathDeImg, const char* pathMaskImg, const char* pathReDst, int st, int end);

	//Ӱ���޸���������ͨ��������޸���Ӱ��maskӰ��ʹ��޸�����ʼ����ֹ���εõ��޸����
	void RepairFunc_SpatialRun(const char* pathDeImg, const char* pathMaskImg, const char* pathReDst, int st, int end);

	//Ӱ���޸������������ݿռ�ƥ��λ�ý��й����޸�
	void RepairFunc_SpatialSpectralRun(const char* pathDeImg,const char* pathMaskImg,const char* pathReDst,int st,int end);

	//Ӱ���޸������������б߽����ȵ�ͼ���޸�
	void RepairFunc_EdgePriorityRun(const char* pathDeImg, const char* pathMaskImg, const char* pathReDst, int st, int end);


	//ģ���˻�Ӱ��
	void RepairFunc_SimuliDegradImg(const char* pathImg, const char* pathDeImg, int st, int end, CPOINT lup, CPOINT rbottom);
	
	//�����������
	void RepairFunc_GenerateTexture(const char* pathImg, int rangesize, const char* pathOut);

	//����Ӱ�񲨶�ȱʧ���޸�
	void RepairFunc_RepairPartialSpectral(int* dataImg, int* dataMask,int xsize,int ysize,int bands);

	//ȫ��Ӱ�񲨶�ȱʧ���޸�
	void RepairFunc_RepairTotal(int* dataImg, int* dataMask, int xsize, int ysize, int bands);

	//��ȡ�����Ԫ
	void RepairFunc_TextureElement(const char* pathEle, int rangesize, int num, const char* pathDist);

	//ͼ��ֽ�ֽ�Ϊ��Ƶ�͵�Ƶ�ɷ�
	void RepairFunc_ImgDecompose(const char* pathImg,const char* pathHyper,const char* pathLow,float threshold);

	//ͨ��ͼ��任ͼ����Ϣ��������
	void RepairFunc_PCATrans(const char* pathImg, const char* pathPCA, const char* pathParam);

	//��PCA�任��ͼ�����ͼ����ǿʹ��ǿ������Χͼ��һ��
	void RepairFunc_PCAIntensity(const char* pathImgPCA,const char* InitialMask,const char* pathPCAIntensity);

private:
	////////////////////////////////����ƥ��Ĺ����޸�����/////////////////////////////////////////////
	//����Ӱ���޸�����ʼ����ֹ����
	void RepairFunc_SetLossBands(int st, int end);

	//ȱʧ���ݺ��������ݽ���ƥ�䲢��Ȩ��
	//(����Ҫ�������в��ε�ȥ�������Ա��뽫�������ݶ��룬�ڴ�Ҫ��Ƚϴ��ڴ�ӳ�䣿)
	void RepairFunc_MatchLoss(int* dataImg, int* dataMask,int posx,int posy,int xsize, int ysize, int bands, vector<int>&matchPos, vector<float> &matchWeight);

	//Ȼ������޸�
	void RepairFunc_RepairSpectral(int* dataImg, int posx, int posy, int xsize, int ysize,vector<int>matchPos, vector<float> matchWeight);

	/////////////////////////////////�ռ�ƥ��������޸�����//////////////////////////////////////////////
	//��ͼ�����ҵ�����Ҫ�޸���λ��
	void RepairFunc_FirstPosition(float* imgData, float* maskRepair, int xsize, int ysize, CPOINT &firstPos);

	//�޸�Ӱ���ϵ�ĳһ������,ͬʱ������Ĥ����
	void RepairFunc_ExemplarPos(float* imgData, float* maskInitial, float* maskRepair, int xsize, int ysize, CPOINT firstPos);

	//���ǹ����ڽ��Ե�����ƥ���޸�
	void RepairFunc_ExemplarPosSpectral(float* imgData,float*imgDataPrv1,float* imgDataPrv2, float* maskInitial, float* maskRepair, int xsize, int ysize, CPOINT firstPos);

	//��ȡ��һ���οռ�ƥ�����ƥ���
	void RepairFunc_GetBestMacthPos(float* imgData, float* maskInitial, float* maskRepair, int xsize, int ysize, CPOINT firstPos, CPOINT &bestPos);

	//���Hellinger�����ȡ���ƥ���
	void RepairFunc_GetBestMacthPosWithHellinger(float* imgData, float* maskInitial, float* maskRepair, int xsize, int ysize, CPOINT firstPos, CPOINT &bestPos);

	//����Ƿ���ɳ����޸�
	bool RepairFunc_ExemplarCheckTerminal(float* maskRepair, int xsize, int ysize);

	//���������޸����
	void RepairFunc_IterativeRefine(float* imgData, float* maskInitial, int xsize, int ysize);

	//���ȱ߽���޸�����
	//���Ȼ�ȡӰ��߽� 
	void RepairFunc_GetRepairEdge(float* maskRepair,int xsize,int ysize, vector<CPOINT> &edge_list);

	//Ȼ���޸�Ӱ��߽�
	void RepairFunc_RepairEdge(float* imgData,float* maskInitial, float* maskRepair, vector<CPOINT> &edge_list,int xsize, int ysize);

	/////////////////////////////////ͼ��ֽ��Ӱ���޸�����///////////////////////////////////////////////




private:
	int m_HyperRepairFunc_st;
	int m_HyperRepairFunc_end;
};

//�߹����޸����Ժ���
void HyperRepairFuncTest();