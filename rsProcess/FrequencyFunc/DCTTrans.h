#pragma once

//����DCT�任Ч�ʼ��ͣ�����ܹ�ͨ�����α任���٣���

//1άDCT�任
void DCT1D(float* dataIn, int length, float* DCTData);
void IDCT1D(float* DCTData, int length, float* IDCTData);

//2άDCT�任
void DCT2D(float* dataIn, int xsize,int ysize, float* DCTData);
void IDCT2D(float* DCTData, int xsize, int ysize, float* IDCTData);

//���ٱ任
void FDCT2D(float* dataIn, int xsize, int ysize, float* DCTData);
void IFDCT2D(float* DCTData, int xsize, int ysize, float* IDCTData);

//3άDCT�任 
void DCT3D(float* dataIn, int xsize, int ysize, int bands, float* DCTData);
void IDCT3D(float* DCTData, int xsize, int ysize, int bands, float* IDCTData);

//ͼ��DCT�任
void DCT2D(const char* pathImgIn, const char* pathOut, int bandIdx);
void IDCT2D(const char* pathImgIn, const char* pathOut, int bandIdx);
void DCT3D(const char* pathImgIn, const char* pathOut);
void IDCT3D(const char* pathImgIn, const char* pathOut);

//ͼ���˲�
void DCTFliter2D(const char* pathImgIn, const char* pathOut, int bandIdx, float thresthod);
void DCTFliter3D(const char* pathImgIn, const char* pathOut, float thresthod);