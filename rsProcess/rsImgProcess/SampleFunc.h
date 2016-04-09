#pragma once
/*
	Ӱ���ز����������������Զ�Ӱ�����resize����
	Ӱ���ز�����������ڲ�ֵ��˫���Բ�ֵ��˫���ξ����ֵ
	���ö��̴߳���������ߴ���Ч�ʣ��ڴ���������߳��л��Ƚϰ���ʱ
	����СӰ��ʱ���ʺ�ʹ�ö��̴߳����ڴ���ϴ�Ӱ�������ʹ�ö��߳�
	������ߴ���Ч��
*/
#ifndef _IMAGE_SAMPLE_
#define _IMAGE_SAMPLE_


#include "..\gdal\include\ogrsf_frmts.h"
#include "..\gdal\include\gdal_priv.h"
#include "..\ThreadPool.h"

#pragma comment(lib,"gdal_i.lib")

// ȫ�֣��߳���
static CRITICAL_SECTION cs_read_sample;	//����
static CRITICAL_SECTION cs_write_sample;//д��

//���߳̽ṹ��
struct  SAMPLE_STUECT
{
	GDALDatasetH m_dataset_In;
	GDALDatasetH m_dataset_Out;
	int bandIndex;
};


/************************************************************************/
/*								�̺߳���    			                */
/************************************************************************/

//���߳�����
//��ÿ�����ζ������̳߳ؽ��д��� ���߳� ��������
DWORD  WINAPI NearestThreadFunc(LPVOID vsampleSt);
DWORD  WINAPI BilinerThreadFunc(LPVOID vsampleSt);
DWORD  WINAPI CubicThreadFunc(LPVOID vsampleSt);

/************************************************************************/
/*								��������    			                */
/************************************************************************/
class  CSampleFunc
{
public:
	CSampleFunc(void);
	~CSampleFunc(void);

	//���߳��ز���
	/*
		������char* pathIn		������Ӱ��·��
			  int xsampleszie	��������Ӱ���Сx
			  int ysamplesize	��������Ӱ���Сy
			  char* pathOut		��������Ӱ��·��

	*/
	long nearest_sample_thread(char* pathIn,int xsampleszie,int ysamplesize,char* pathOut);
	long biliner_sample_thread(char* pathIn,int xsampleszie,int ysamplesize,char* pathOut);
	long cubic_sample_thread(char* pathIn,int xsampleszie,int ysamplesize,char* pathOut);

	//ֱ�Ӹ���Ӱ��·�������ز��� ���̲߳���
	/*
		������char* pathIn		������Ӱ��·��
			  int xsampleszie	��������Ӱ���Сx
			  int ysamplesize	��������Ӱ���Сy
			  char* pathOut		��������Ӱ��·��

	*/
	long nearest_sample(char* pathIn,int xsampleszie,int ysamplesize,char* pathOut);
	long biliner_sample(char* pathIn,int xsampleszie,int ysamplesize,char* pathOut);
	long cubic_sample(char* pathIn,int xsampleszie,int ysamplesize,char* pathOut);
	long gauss_down_sample(char* pathIn,int xsampleszie,int ysamplesize,char* pathOut);
	
	//��Ӱ�����ݽ����ز���
	/*
		������int in_xsize			����Ӱ��x�Ĵ�С
			  int in_ysize			���Ӱ��y�Ĵ�С
			  float* imgBufferIn	����Ӱ�����ݵ�����
			  int sample_xsize		������Ӱ��Ĵ�Сx
			  int sample_ysize		������Ӱ��Ĵ�Сy
			  float* imgBufferOut	������Ӱ���������
	*/
	long gauss_down_sample(int in_xsize,int in_ysize,float* imgBufferIn,int sample_xsize,int sample_ysize,float* imgBufferOut);
	long nearest_sample(int in_xsize,int in_ysize,float* imgBufferIn,int out_xsize,int out_ysize,float* imgBufferOut);
	long biliner_sample(int in_xsize,int in_ysize,float* imgBufferIn,int out_xsize,int out_ysize,float* imgBufferOut);
	long cubic_sample(int in_xsize,int in_ysize,float* imgBufferIn,int out_xsize,int out_ysize,float* imgBufferOut);


	//�ز������� ���ݶ�Ӧ��Ķ�Ӧ��������ز���
	/*
		������float* imgBuffer		������Ӱ��·��
			  int xsize				��x��������Ӱ���С
			  int ysize				��y��������Ӱ���С
			  float* xpos			����Ӧ�������λ��x
			  float* ypos			����Ӧ�������λ��y
			  int out_xsize			�����Ӱ��x��С
			  int out_ysize			�����Ӱ��y��С
			  float* imgBufferOut	�����Ӱ������
	*/
	long nearest_sample(float* imgBuffer,int xsize,int ysize,float* xpos,float* ypos,int out_xsize,int out_ysize,float* imgBufferOut);
	long biliner_sample(float* imgBuffer,int xsize,int ysize,float* xpos,float* ypos,int out_xsize,int out_ysize,float* imgBufferOut);
	long cubic_sample(float* imgBuffer,int xsize,int ysize,float* xpos,float* ypos,int out_xsize,int out_ysize,float* imgBufferOut);

	//�ز���������ԭ�Ӳ���
	/*
		������float *imgBuffer	������Ӱ������
			  int xsize			������Ӱ��x�Ĵ�С
			  int ysize			������Ӱ��y�Ĵ�С
			  float xpos		������������λ��x
			  float ypos		������������λ��y
	*/
	float _stdcall nearest_sample(float *imgBuffer,int xsize,int ysize,float xpos,float ypos);
	float _stdcall biliner_sample(float *imgBuffer,int xsize,int ysize,float xpos,float ypos);
	float _stdcall cubic_sample(float *imgBuffer,int xsize,int ysize,float xpos,float ypos);

private:

	//��ϲ�������
	float __fastcall s_function(float x);

	CThreadPool m_threadpool;
};

#endif

//  [2/6/2015 wuwei just you]
//TODO��������ά��ɢ��Ĳ�ֵ�㷨