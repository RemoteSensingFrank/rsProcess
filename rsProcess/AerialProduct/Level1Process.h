#pragma once

#include"..\gdal\include\gdal_priv.h"
#include"..\Global.h"

#pragma comment(lib,"gdal_i.lib")

#include<vector>
using namespace std;

//1�����ݲ�Ʒ����
//1�����ݲ�Ʒ�Ļ�������Ϊ����У������Ҫ������Է���У�������Է���У�������ݱ궨
class Level1Process
{
public:
	//���ݾ��Է��䶨��ϵ�����о��Է��䶨��
	virtual long Level1Proc_RadiationAbsolute(const char* pathImg, const char* pathImgRad, const char* pathAbsRegFile) = 0;
	//��ȡ���Է��䶨��ϵ���ļ�
	//��һ������Ϊ���Է���У��ϵ���ļ����ڶ�������Ϊ����ϵ��������������Ϊ����ϵ�������ĸ�����Ϊ��ѡ���������Բ���Ĭ��Ϊ��
	virtual long Level1Proc_AbsoluteParameters(const char* pathAbsRegFile, float* paramA, float* paramB, float* paramAux = NULL)=0;

	//��Է���У��
	virtual long Level1Proc_RadiationRelative(const char* pathImg, const char* pathImgRad, const char* pathRelRegFile) = 0;
	//��ȡ��Է���У������
	////��һ������Ϊ��ԶԷ���У��ϵ���ļ����ڶ�������Ϊ����ϵ��������������Ϊ����ϵ�������ĸ�����Ϊ��ѡ���������Բ���Ĭ��Ϊ��
	virtual long LevelProc_RelativeParameters(const char* pathRelRegFile, float* paramA, float* paramB, float* paramAux = NULL) = 0;

	//����У����ͨ��ģ��̫���������ֵ���Ա�Ӱ�������ֵ�õ�������
	virtual long Level1Proc_AtmosphereCorrect(const char* pathImgRad, const char* pathRef, const char* pathAtmosphere) = 0;

};


//ȫ�׶�1�����ݲ�Ʒ����
class QPDLevel1Process : public Level1Process
{
public:
	//===============================================================================================================================================
	//����У��
	//���Է���У��
	long Level1Proc_RadiationAbsolute(const char* pathImg, const char* pathImgRad, const char* pathAbsRegFile);
	//��ȡ���Է���У��ϵ��
	long Level1Proc_AbsoluteParameters(const char* pathAbsRegFile, float* paramA, float* paramB, float* paramAux = NULL);

	//��Է���У��
	long Level1Proc_RadiationRelative(const char* pathImg, const char* pathImgRad, const char* pathRelRegFile);
	//��ȡ��Է���У��ϵ��
	long LevelProc_RelativeParameters(const char* pathRelRegFile, float* paramA, float* paramB, float* paramAux = NULL);

	//����QPD���ݲ��÷ֶ�����У�� ���һ��ϵ��Ϊ�ֶ���
	long LevelProc_GetParameterInfo(const char* pathImgRad, int& nSamples, int &nBands, int &nLevels);

	//���صĴ���У������������û�в�����ֻ�ÿ�����
	long Level1Proc_AtmosphereCorrect(const char* pathImgRad, const char* pathRef, const char* pathAtmosphere) { return 0; }
	//=================================================================================================================================================
	//������QPD���ݴ������еĺ���
	//�����ӳ����ӳ�ƴ��
	/*****************************************************************************
	* @brief : ����Ӱ���ӳ�ƴ��
	* @author : W.W.Frank
	* @date : 2015/11/12 23:34
	* @version : version 1.0
	* @inparam : const char* pathview1,const char* pathview2,const char* pathview3,������ƴ���ӳ�Ӱ��·��
	float over_left_center_x,float over_right_center_x,�����ӳ���x�ص� �����ӳ���x�ص�
	float err_left_center_y,float err_right_center_y,	�����ӳ�y��λ�������ӳ�y��λ���������ӳ�Ϊ�����������ӳ�Ϊ��
	const char* pathJointView							������Ӱ��
	* @outparam : ƴ�Ӻ�����
			*˼������������Ԫ���ȵ���������Ϊ�����Ĵӳ����Ӱ��Ƕ���û�а취���ģ�����һ��Ҫ�������а취�ģ�
			�������ƻ�Ӱ����׵������ԣ����ȶ�Ӱ����Ҫ��ľ��Ƚ��������������������ƾ����Ԫ�ľ��ȣ�
			Ȼ���Ӱ����н������������Ļ�������Ӧ�ÿ��Դﵽ����Ԫ�ľ��ȣ����ǲ��������ǲ�����ģ���Ӱ
			�����������������Ȼ�󽵲����Ĳ����Ʊػ��ƻ�Ӱ����������������Ƽ���ô�������ֻ�����ξ���Ӧ����ô������
	*****************************************************************************/
	long Level1Proc_ViewJointThree(const char* pathview1, const char* pathview2, const char* pathview3,
		float over_left_center_x, float over_right_center_x, float err_left_center_y, float err_right_center_y, const char* pathJointView);
	//�����ӳ���ƴ��(�����ӳ���ƴ�ӱ������ӳ���ƴ��Ҫ�򵥵Ķ�)
	long Level1Proc_ViewJointTwo(const char* pathview1, const char* pathview2, float over_x, float err_y, const char* pathJointView);

private:
	//�ӳ�ƴ�ӵ��ڲ��ӿ�==================================================================================================================================
	//�ӳ�ƴ��ɫ������
	//�����ӳ���ɫ���������м��Ӱ����
	long Level1Proc_ColorMatch(unsigned short* imgBuffer1, unsigned short* imgBuffer2, unsigned short* imgBuffer3, int xsize1, int ysize1, int xsize2, int ysize2, int xsize3, int ysize3,
		int over_left_center_x, int over_right_center_x, int err_left_center_y, int err_right_center_y);
	//�����ӳ������ص�������ɫ
	long Level1Proc_ColorMatch(unsigned short* imgBuffer1, unsigned short* imgBuffer2, int xsize1, int ysize1, int xsize2, int ysize2, int over_x, int err_y);

	/*
	���ܣ���ȡ�ӳ�ƴ�Ӻ�Ӱ���С
	������int xsizeLeft,int ysizeLeft,			���ӳ�Ӱ���С
	*/
	long Level1Proc_ViewJointRange(int xsize1, int ysize1, int xsize2, int ysize2, int xsize3, int ysize3, float over_left_centerx,
		float err_left_centery, float over_right_centerx, float err_right_centery, int &xmosaic, int &ymosaic);
	long Level1Proc_ViewJointRange(int xsize1, int ysize1, int xsize2, int ysize2, float over_x, float err_y, int &xmosaic, int &ymosaic);
	/*
	���ܣ���ȡ�ӳ�ƴ�Ӻ�Ӱ��ƴ�ӱ�λ��
	������int xsizeLeft,int ysizeLeft,			���ӳ�Ӱ���С
	*/
	long Level1Proc_ViewJointEdge(int xsize1, int ysize1, int xsize2, int ysize2, int xsize3, int ysize3, float over_left_centerx, float err_left_centery,
		float over_right_centerx, float err_right_centery, vector<CPOINT>& edge_position);
	long Level1Proc_ViewJointEdge(int xsize1, int ysize1, int xsize2, int ysize2, float over_x, float err_y, vector<CPOINT>& edge_position);

	/*
	���ܣ���Ӱ��ƴ�ӱ߽����𻯴���
	������float* imagedata,						�ӳ�ƴ�Ӻ�Ӱ������
	int xsize,int ysize,					�ӳ�ƴ�Ӻ�Ӱ��ߴ�
	int* xmosaicedge,int *ymosaicedge,int edgePixels	ƴ�ӱ�λ�ú�ƴ�ӱ���Ԫ��Ŀ
	*/
	long Level1Proc_ViewJointFeather(unsigned short* imagedata, int xsize, int ysize, vector<CPOINT>& edge_position);


	/*
	���ܣ��ӳ�ƴ�ӣ���ƴ�Ӻ�Ӱ����������
	������float* imgViewData,								�ӳ����ݼ�
	int xsize,int ysize,								�ӳ�Ӱ���С
	float over_left_center_x,float over_right_center_x,����Ӱ������Ӱ��x�ص�
	float err_left_center_y,float err_right_center_y,	 ����Ӱ������Ӱ��y��λ
	GDALDatasetH m_datasetLeft,GDALDatasetH m_datasetCenter,GDALDatasetH m_datasetRight,���У���Ӱ�����ݼ�
	float* datamosaic,GDALDatasetH m_datasetOut,int xmosaic,int ymosaic	������ݣ�������ݼ���x��С��y��С
	*/
	long Level1Proc_ViewJointFillData(unsigned short* imgViewData, int xsize1, int ysize1, int xsize2, int ysize2, int xsize3, int ysize3, float over_left_center_x, float over_right_center_x, float err_left_center_y, float err_right_center_y,
		GDALDatasetH m_datasetLeft, GDALDatasetH m_datasetCenter, GDALDatasetH m_datasetRight, unsigned short* datamosaic, FILE* fDst, int xmosaic, int ymosaic, vector<CPOINT>& edge_position);
	long Level1Proc_ViewJointFillData(unsigned short* imgViewData, int xsize1, int ysize1, int xsize2, int ysize2, float over_x, float err_y, GDALDatasetH m_datasetLeft, GDALDatasetH m_datasetRight,
		unsigned short* datamosaic, FILE* fDst, int xmosaic, int ymosaic, vector<CPOINT>& edge_position);
	/*
	���ܣ���������Ӱ��Χ��ÿ��Ӱ��������������������
	������float* imgViewData		ĳ���ӳ�����
	int xsize,int ysize		�ӳ����ݴ�С
	int stposx,int stposy		�ӳ�������ʼλ��
	float* jointData			�ӳ�ƴ�Ӻ�����
	*/
	long Level1Proc_ViewJointFillData(unsigned short* imgViewData, int xsize, int ysize, float stposx, float stposy, unsigned short* jointData, int xmosaic, int ymosaic);
};


//�Ⱥ������ݵķ���У��
//�Ⱥ������ݷ���У����Ҫ�ص�Ϊ����һ������У��ģ��
class THRLevel1Process : public Level1Process
{
public:
	//����У��û�в�������֪����ôУ�����Ȱѽӿ�������
	//���Է���У��
	long Level1Proc_RadiationAbsolute(const char* pathImg, const char* pathImgRad, const char* pathAbsRegFile) { return 0; }
	//��ȡ���Է���У��ϵ��
	long Level1Proc_AbsoluteParameters(const char* pathAbsRegFile, float* paramA, float* paramB, float* paramAux = NULL) { return 0; }

	//��Է���У��
	long Level1Proc_RadiationRelative(const char* pathImg, const char* pathImgRad, const char* pathRelRegFile) { return 0; }
	//��ȡ��Է���У��ϵ��
	long LevelProc_RelativeParameters(const char* pathRelRegFile, float* paramA, float* paramB, float* paramAux = NULL) { return 0; }

	//����QPD���ݲ��÷ֶ�����У�� ���һ��ϵ��Ϊ�ֶ���
	long LevelProc_GetParameterInfo(const char* pathImgRad, int& nSamples, int &nBands, int &nLevels) { return 0; }

	long Level1Proc_AtmosphereCorrect(const char* pathImgRad, const char* pathRef, const char* pathAtmosphere) { return 0; }
	//===============================================================================================================================
	/*
	���ܣ����Ⱥ���߹������ݽ��й��׶���
			������1.const char* hyperModtran		��modtranģ��ģ��ķ������ļ�·��
			2.const char* bandInfo,			��������Ϣ �����Ĳ����Ͱ벨��
			3.const char* fileImg,			��Ӱ��·��
			4.const char* plank_line_coeff,	�����ʿ����Ի�ϵ��
			5.float bin,						��ģ����׷ֱ���
			6.float begWaveLength,			�����ײ�����ʼλ��
			7.float endWaveLength				�����ײ�����ֹ��λ��
	*/
	long Level1Proc_SpectarlCalibTIR(const char* hyperModtran, const char* bandInfo, const char* fileImg, const char* plank_line_coeff, float bin, float begWaveLength, float endWaveLength, const char* pathShift);

	/*
	���ܣ����Ⱥ���߹������ݽ��й��׶���
			������1.const char* hyperModtran		��modtranģ��ģ��ķ������ļ�·��
			2.const char* bandInfo,			��������Ϣ �����Ĳ����Ͱ벨��
			3.const char* fileImg,			��Ӱ��·��
			4.const char* plank_line_coeff,	�����ʿ����Ի�ϵ��
			5.float bin,						��ģ����׷ֱ���
			6.float begWaveLength,			�����ײ�����ʼλ��
			7.float endWaveLength				�����ײ�����ֹ��λ��
			8.int sampleNum					:�������
	*/
	long Level1Proc_SpectarlCalibTIRSample(const char* hyperModtran, const char* bandInfo, const char* fileImg,
		const char* plank_line_coeff, float bin, float begWaveLength,
		float endWaveLength, int sampleNum, const char* pathShift);

private:
	/*
		���ܣ�ͨ��MODTRANģ������.tp5��ʽ�ļ���MODTRAN5ģ�͵�����Ϊ�������
		������1.const char* tap_name   MODTRAN��������ļ���
		2.const char* modtranDir MODTRAN��ִ�г���·��
		3.char* SURREF1������ģ�� "BRDF" "LAMBER" >=0��Ϊ�� <0
		4.float V11 ��ʼ��Ƶ�ʣ�����
		5.float V12 ���գ�Ƶ�ʣ�����
	*/
	long Level1Proc_MODTRAN_Batch(const char* tap_name, const char* modtranDir, char* SURREF1, float V11, float V21);
	/*
		���ܣ����й������ݵ�ģ��
		������1.float *hyperRadiance ������Ӱ��ķ�����
		2.float* hyperWaveLength ������Ӱ��Ĳ���
		3.float *center_wave_Length ʵ�ʲ������ݵ����Ĳ���
		4.int hyperNum		  ������Ӱ��Ĳ�����
		5.float *band_FWHM	  ÿ�����ε�FWHM
		6.int bandNum			  ʵ�����ݵĲ�����
		7.float shift_lamda,float delta_shift_lamda �������Ĳ�����ƫ�ƺ�FWHM��ƫ��
		8.float* simuluate_radiance �õ���ģ����׷���������
	*/
	long Level1Proc_Simulate(float *hyperRadiance, float* hyperWaveLength, float *center_wave_Length, int hyperNum,
		float *band_FWHM, int bandNum, float shift_lamda, float delta_shift_lamda, float* simuluate_radiance);
	/*
		���ܣ�NODD�任��Normalized optical depth derivative��
		������1.float* radiance ����������
		2.int index_min   �������չ���ʼλ��
		3.int index_max	�������չ���ֹλ��
		4.float* radiance_NODD �任��Ĺ�һ��������
	*/
	long Level1Proc_NODD(float* radiance, int index_min, int index_max, float* radiance_NODD);

	/*
		���ܣ�������״��ۺ���
		������1.float* simulateRadiance ģ�������
		2.float* realRadiance	   ��ʵ������
		3.int index_min		   �������չ���ʼλ��
		4.int index_max		   �������չ���ֹλ��
		�������
	*/
	float Level1Proc_MeritFunc(float* simulateRadiance, float* realRadiance, int index_min, int index_max);

	/*
		���ܣ�ʹ�ñ������㷨���е������,��ʼֵΪ��0��0������������Ϊ��0��1����1��0�������ۺ���ΪLevel1Proc_MeritFunc��
		������1.float* optParam	���Ž�
		2.float* hyperSpectral �����׷���������
		3.float *hyperWavelength �����ײ��β���
		4.int hyperNum	�����ײ�����
		5.float* bandCenterLength ʵ���Ҷ������Ĳ���
		6.float* bandFWHM ʵ���Ҷ���İ벨��
		7.float* realSpectral ʵ�ʹ�������
		8.int realNum	ʵ�ʹ��ײ�����
		9.int index_min �������ղ�����ʼ����
		10.int index_max �������ղ�����󲨳�
	*/
	long Level1Proc_Powell(float* optParam, float* hyperSpectral, float *hyperWavelength, int hyperNum,
		float* bandCenterLength, float* bandFWHM, float* realSpectral, int realNum,
		int index_min, int index_max);

};

