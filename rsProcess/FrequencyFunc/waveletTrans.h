#pragma once
#ifndef _WaveLets_
#define _WaveLets_

//С���任GDAL��װ
#include "..\gdal/include/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

//////////////////////////////////////////////////////////////////////////
//С���任��Ӱ���˲�
//��ͨ�˲�
void  HPFliter(char* pathSrc,char* pathDst,int band);
void  HPFliter(char* pathSrc,char* pathDst);

//��ͨ�˲�
void  LPFliter(char* pathSrc,char* pathDst,int band);
void  LPFliter(char* pathSrc,char* pathDst);

//////////////////////////////////////////////////////////////////////////

//ͼ��һ��С���任
void  WaveletTransOnce(char* pathSrc,char* pathDst1,char* pathDst2,int band);
void  WaveletTransOnce(char* pathSrc,char* pathDst1,char* pathDst2);	//���в���

//ͼ�����С���任
void  WaveletTransTwice(char* pathSrc,char* pathDst1,char* pathDst2,int band);
void  WaveletTransTwice(char* pathSrc,char* pathDst1,char* pathDst2);

//ͼ������С���任
void  WaveletTransTri(char* pathSrc,char* pathDst1,char* pathDst2,int band);
void  WaveletTransTri(char* pathSrc,char* pathDst1,char* pathDst2);

//ͼ��С���任����任
void  WaveletInvTrans(char* pathDst,char* pathSrc1,char* pathSrc2,int layer,int band);
void  WaveletInvTrans(char* pathDst,char* pathSrc1,char* pathSrc2,int layer);

//////////////////////////////////////////////////////////////////////////
//ͼ��С���任����任
void  DWTi_Once(float ** spData, float ** spTransData0, float** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
void  DWTi_Once(float*   spData, float*   spTransData0, float*  spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);


//����С���任
void  DWT_TriLayers(float ** spOriginData, float ** spTransData0, float ** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
void  DWT_TriLayers(float *  spOriginData, float *  spTransData0, float *  spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);


//����С���任
void  DWT_TwoLayers(float ** spOriginData, float ** spTransData0, float ** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
void  DWT_TwoLayers(float *  spOriginData, float *  spTransData0, float *  spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);


//һ��С���任
void  DWT_Once(float ** spOriginData, float ** spTransData0, float ** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
void  DWT_Once(float *  spOriginData, float *  spTransData0, float *  spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);


//��ֱ�����С���任
void  Vert_Transform(float ** spOriginData, float ** spTransData1, int nHeight_H, int nWidth, float fRadius);
void  Vert_Transform(float *  spOriginData, float *  spTransData1, int nHeight_H, int nWidth, float fRadius);


//ˮƽ�����С���任
void  Hori_Transform(float ** spOriginData, float ** spTransData0, int nHeight, int nWidth_H, float fRadius);
void  Hori_Transform(float *  spOriginData, float *  spTransData0, int nHeight, int nWidth_H, float fRadius);

#endif

//  [3/25/2014 wuwei just you]
