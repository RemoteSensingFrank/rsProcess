#pragma once

#ifndef _SPFLITER_
#define _SPFLITER_

//GDAL��װ
#include <vector>
#include <algorithm>

#include"..\Global.h"
#include "..\gdal/include/gdal_priv.h"
#include "..\ThreadPool.h"
#include "ImageBlock.h"

using namespace std;
#pragma comment(lib,"gdal_i.lib")

//#define PI 3.1415926534
//Ӱ������˲�
class  spFliter
{
public:

	//���캯��������������ʼ��
	spFliter(){
		m_is_core=false;
		m_fliter_core=NULL;
	}
   ~spFliter(){
		if (m_is_core&&m_fliter_core!=NULL)
		{
			for (int i=0;i<m_core_ysize;i++)
			{
				delete[]m_fliter_core[i];
			}
			delete[]m_fliter_core;
		}
	}

	//�����˲���
	void spFliterCore(float** core,int xsize,int ysize);
	void spFliterCore(float*  core,int xsize,int ysize);

	//�����˲�����
	//GDAL��װ
	void spitalFliter(char* pathSrc,char* pathDst,int xpos,int ypos,int xoffset,int yoffset);
	void spitalFliter(char* pathSrc,char* pathDst);

	//������˹���ӽ����˲�
	void LaplaceFliter(char* pathSrc,char* pathDst,int xpos,int ypos,int xoffset,int yoffset);
	void LaplaceFliter(char* pathSrc,char* pathDst);

	//sobel���ӽ����˲�
	void SobelFliter(char* pathSrc,char* pathDst,int xpos,int ypos,int xoffset,int yoffset);
	void SobelFliter(char* pathSrc,char* pathDst);

	//��ֵ�˲�
	void MeanFliter(char* pathSrc,char* pathDst,int xpos,int ypos,int xoffset,int yoffset);
	void MeanFliter(char* pathSrc,char* pathDst);

	//0ֵ�˲�
	void ZeroFliter(char* pathSrc,char* pathDst,int xpos,int ypos,int xoffset,int yoffset);
	void ZeroFliter(char* pathSrc,char* pathDst);


	//��ֵ�˲�
	void MedFliter(float*imgBufferIn,int width,int heigh,int xpos,int ypos,
							int xoffset,int yoffset,int xfsize,int yfsize,float* imgBufferOut);
	void MedFliter(char* pathSrc,char* pathDst,int xpos,int ypos,int xoffset,int yoffset);
	void MedFliter(char* pathSrc,char* pathDst);


	//�˲�������
	void spFliterOperation(float*imgBufferIn,int width,int heigh,int xpos,int ypos,int xoffset,int yoffset,float* imgBufferOut);
private:

	//�˲�������
	float __fastcall spFliterOperationCore(float*imgBufferIn,int xpos,int ypos);

	//��Ա����
	bool m_is_core;				//�˲����Ƿ����
	int m_core_xsize;			//�˲���x�ߴ��С
	int m_core_ysize;			//�˲���y�ߴ��С
	int m_width;				//Ӱ���width
	int m_heigh;				//Ӱ���heigh
	float **m_fliter_core;		//Ӱ���˲���
};


//���ø�˹�˲��˺���
long  SetGaussCore(float **kernel,float sigma, int xsize=3,int ysize=3);
long  SetGaussCore(float *kernel, float sigma, int xsize=3,int ysize=3);


//Ӱ����и�˹�˲�
long  GaussFliter(float *srcImg,float *dstImg,int xImg,int yImg,float ** kernel,int xkernel=3,int ykernel=3);

//GDAL��װӰ���˹�˲�
long  GaussFliter(char* pathSrc,char* pathDst,float** kernel, int xkernel=3, int ykernel=3);


//���и�˹���
long  GaussDifference(float* srcImg1,float* srcImg2,float* dstImg,int xImg,int yImg);

//GDAL��װ�ĸ�˹���
long  GaussDifference(char* pathsrc1,char* pathsrc2,char* pathdst);


//  [2/6/2015 wuwei just you]
//TODO��Ӱ��ֿ�����˲����˲���Ӱ��ֿ�д���ļ���
//		���Ӷ��̴߳����������Ч��
//Ӱ��ֿ��˲�
long  ImageSpaitalBlockFliter(char* pathsrc,char* pathdst,float* fliterCore,int xcoresize,int ycoresize);

//���߳�Ӱ��ֿ��˲�
long  ThreadImageSpaitalFliter(char* pathsrc,char* pathdst,float* fliterCore,int xcoresize,int ycoresize);


//  [4/11/2015 wuwei just you]
//Ӱ����̬ѧ�˲�����̬ѧ�˲������Ͼ������ֵ����Сֵ�˲�
//���ú��ʵ��˲����ں��˲������ĵ�λ��
class  MorphologyFliter
{
	//���캯��������������ʼ��
	MorphologyFliter(){
		m_is_core=false;
		m_fliter_core=NULL;
	}
	~MorphologyFliter(){
		if (m_is_core&&m_fliter_core!=NULL)
		{
			for (int i=0;i<m_core_ysize;i++)
			{
				delete[]m_fliter_core[i];
			}
			delete[]m_fliter_core;
		}
	}

	//������̬ѧ�˲���
	void MorphologyCore(float** core,int xsize,int ysize,int centerx,int centery);
	void MorphologyCore(float*  core,int xsize,int ysize,int centerx,int centery);

	//��̬ѧ��ʴ
	void MorphologyCorrode(float* dataIn,int sizex,int sizey,int bands,float* dataOut);

	//��̬ѧ����
	void MorphologyDilation(float* dataIn,int sizex,int sizey,int bands,float* dataOut);

	//GDAL��װ����̬ѧ��ʴ������
	//��̬ѧ��ʴ
	void MorphologyCorrode(char* pathIn,char* pathOut);

	//��̬ѧ����
	void MorphologyDilation(char* pathIn,char* pathOut);

private:
	//��Ա����
	bool m_is_core;				//�˲����Ƿ����
	int m_core_xsize;			//�˲���x�ߴ��С
	int m_core_ysize;			//�˲���y�ߴ��С
	int m_width;				//Ӱ���width
	int m_heigh;				//Ӱ���heigh
	int m_centerx;				//�˲�������xλ��
	int m_centery;				//�˲�������yλ��
	float **m_fliter_core;		//Ӱ���˲���
};

#endif
//  [4/9/2014 wuwei just you]
//	version 1.0Ӱ��Ŀռ����˲��͸�˹�˲�



