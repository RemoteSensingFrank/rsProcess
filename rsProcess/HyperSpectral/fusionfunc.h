#ifndef _IMAGE_FUSION_
#define _IMAGE_FUSION_
#include"..\Global.h"

//ģ��ʵ������
void simulate_image_data(char* pathEnd,int bands,int endnumbers,int xsize,int ysize,int* oriPos,int* endtype,int oriPosNum,char* pathsimulateImage);
void simulateTest(char* pathEnd,int bands,int endnumbers,char* pathOri,char* pathRatio,char* pathoutS,char* pathRecov);

//��ȡӰ���Ԫ����
float*  get_endmenber_spectralf(char* pathEnd,int bands,int endnumbers);
double* get_endmenber_spectrald(char* pathEnd,int bands,int endnumbers);

//������Ԫ���׵��ļ���
void   export_endmenber_spectral(char* path,double *enddata,int bands,int endnumbers);
void   export_endmenber_spectral(char* path,float *enddata,int bands,int endnumbers);

//�������ķ�Ƚ������
void reduance_total(char* pathRed,char* pathTotal);

//������Ӧ����
//ͨ���߹��׶�Ԫ��ȡ����׶�Ԫ
//�ж�������Ķ���ײ��ε�ÿ�����ι��׷�Χ�������Ĳ���
//��������Ĳ�������ͨ���Ƚϲ�������ĸ߹��ײ��μ��������Ӧ
//����ǹ��׷�Χ��ѡ��Χ�ڵĸ߹��ײ��μ��������Ӧ
/*
	char* pathHy:����ĸ߹��׶�Ԫ
	char* pathM	:����Ķ���׶�Ԫ
	float* hysp int bandsHy:�߹��ײ��ε����Ĳ���&������
	float* msp  int bandm:����ײ��ε����Ĳ����򲨳���Χ&������
	int endnumber:	��Ԫ��Ŀ
	bool range:�ж�������ǹ��׷�Χ�������Ĳ���
*/
void spectral_response_endmenber(char* pathHy,char* pathM,float* hysp,int bandsHy,float* msp,int bandm,int endnumber,bool range);
void spectral_response_image(char* pathHyImage,char* pathMImage,float* hysp,int bandsHy,float* msp,int bandm,int endnumber,bool range);

//�ռ����ɢ���������ø�˹�������н�����
void spatial_spread_function(char* pathImg,char* pathSample);

//������׽Ǻ;���ֵ�в�
void residual_spAngle(char* pathrecovery,char* pathOri,char* pathresidual);


//���ڽ���Ӱ���ں� �����Ƕ�Ԫ��ȡ���� ֱ�ӻ�ȡ�õ��Ķ�Ԫ

//���ݶ�Ԫ���׽�����С����Ӱ���� 
//�������Ⱥ�Ӱ��
void endmember_unmix_LSE(char* pathEnd,char* pathImg,char* pathRed,int endnumbers);

//ͨ����������и�˹����ɢ�����ز���
void endmember_sample(char* pathRed,char* pathSample);

//�������Ĳв�ÿһ�����εĲв���ܲв�
void endmember_residual(char* pathimg,char* pathend,char* pathred,char* pathres,int endnumbers);

//������Ԫ����
void endmember_modify(char* pathred,char* pathimg,char* pathmodify);

//���������Ķ�Ԫ��Ӱ������ؽ�
//��������Ϣ ������Ķ�Ԫ���׺ͻָ�Ӱ��Ĳ�����
void endmember_recovery_image(char* pathred,char* pathmodend,char* pathout,int hyspectral);

//����ָ���Ӱ���ԭʼӰ��֮��Ĳв�͹��׽�
//��һ������Ϊ����Ĺ��׽� �ڶ�������Ϊ�����Ӱ��ֵ�Ĳ�ֵ
void endmember_residual_spAngle(char* pathrecovery,char* pathOri,char* pathresidual);


//ֱ�ӵ������Ӱ���ֵ ���ݳ���ԭ��
//Ч��̫�� û�к��ʵĺ����ֶ� ��̫�ʺ�
void get_image_iterator(char* pathHy,char* pathMs,char* pathout);

//��ȡϡ�����
void sparse_represent_combine(int numsparse,int selsparse,int endnumber,int* selindex);

//ѡȡÿ����������ʵ�ϡ�����
//����Ϊ����Ԫ���� ��Ԫ���� ϡ����� ϡ�������Ŀ ѡȡ��ϡ���ʾ���� ������ ��Ԫ��
//���Ϊ�����Ľ�� �����ɷֵķ��
//float* sparse_represent_fitness(float* endmemberdata,double* pixeldata,int* spareIdx,
//											int numspare,int selsparse,int bands,int endnum);

//ϡ����С�������
void sparse_represent_LES(char* pathEnd,char* pathImg,char* pathRed,int endnumbers,int sparsesel);

//��Ԫ�����ڿռ��Ͻ����ز���
void sparse_represent_sample(char* pathred,char* pathredSmp);

//���ݽ��ķ��������Ԫ����
void sparse_represent_modify(char* pathred,char* pathimg,char* pathmodify);

//ͨ����ȷ���Ӱ��
void sparse_represent_recovery(char* pathred,char* pathEnd,char* pathout,int recbands);

//������׽Ǻ͹��ײв�
void sparse_represent_residual_spAngle(char* pathrecovery,char* pathOri,char* pathresidual);


//����ͶӰ�任��Ӱ���ںϷ���
//����PCA  SVD������Ӱ���ں�
//�Ը߹���Ӱ���ز��� ʹ��ֱ�����߷�Ӱ��һ��
void projection_image_sample(char* pathInHy,char* pathInMs,char* pathSample);

//�Ը߷ֺ͸߹���Ӱ�����ͶӰ�任
void projection_proj_trans(char* pathhyin,char* pathhyout,char* pathmsin,char* pathmsout,char* pathmathy,char* pathmatms);

//ͶӰ���任 
void projection_fusion_trans(char* pathhyin,char* pathmsin,char* pathmathy,char* pathmatms,char* pathout,int numcomponent=1);

//С���任���PCA�ں�
void projection_fusion_wavelet_forward(char* pathHyIn,char* pathMsIn,char* pathHywave1,char* pathHywave2,
											char* pathMswave1,char* pathMswane2,char* pathFusion,int bands=1);

//С���任���PCA�ں�
void projection_fusion_wavelet_inverse(char* pathwavefusion,char* pathHy,char* pathmat,char* pathOut);


//С���任��Ӱ���ں� 
//���׷���
void wavelet_get_spectral_group(float* hywavelen,int hynum,float* mswavelen,int msnum,int *group,bool range);

//�����ز���
void wavelet_image_sample(char* pathInHy,char* pathInMs,char* pathOut);

//Ӱ��С���任
void wavelet_to_image(char* pathHy,char* pathHyW1,char* pathHyW2,char* pathMs,char* pathMsW1,char* pathMsW2);

//С���任��Ӱ���ں�
void wavelet_fusion(char* pathHyW1,char* pathHyW2,char* pathMsW2,char* pathRecov,int* group);

#endif