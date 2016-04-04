#ifndef HYPERSPECTURAL_H
#define HYPERSPECTURAL_H
//#include "Eigen/Core"

/********************************************************************************************************/
/*						                        �ļ�����                                                */
/********************************************************************************************************/
//���任��������ļ���
//fileName		:������ļ���
//matrix		:��д���ļ��ľ���
//size			:����Ĵ�С
//����ֵ0		:ִ�гɹ�
//����ֵ1		:����ָ��Ϊ��
//����ֵ2		:���������д���
//����ֵ3		:��������
long  Matrix_Export_File(const char* fileName,double **matrix,int size);


//��ñ任����Ĵ�С
//fileName		:������ļ���
//size			:����Ĵ�С
//����ֵ0		:ִ�гɹ�
//����ֵ1		:����ָ��Ϊ��
//����ֵ2		:���������д���
//����ֵ3		:��������
long  Matrix_Get_Size(const char* fileName,int &size);


//���ļ��е����ݶ���任������
//fileName		:������ļ���
//matrix		:���ļ�����ľ���
//size			:����Ĵ�С
//����ֵ0		:ִ�гɹ�
//����ֵ1		:����ָ��Ϊ��
//����ֵ2		:���������д���
//����ֵ3		:��������
long  Matrix_Import_File(const char* fileName,double **matrix);

//����Ԫ������ļ���
//fileName		:������ļ���
//endmember		��������Ķ�Ԫ����
//bands			��Ӱ�񲨶���
//numbers		����Ԫ��
//����ֵ0		:ִ�гɹ�
//����ֵ1		:����ָ��Ϊ��
//����ֵ2		:���������д���
//����ֵ3		:��������
//  [1/22/2015 wuwei just you]
long  EndMember_Export_File(const char* fileName,double *endmember,int bands,int numbers);

//���ļ��ж�ȡ��Ԫ
long  EndMember_Import_File(const char* fileName,double *endmember,int bands,int numbers);

//��ȡѵ������������
//��ȡenviѵ��������
long  EnviROINumbers(const char* roiFile,int &roiNumbers);

//��ȡÿ��ѵ��������������
long  EnviROIPts(const char* roiFile,int* roiPts,int roiNmbers);

//��ȡ����
//��ȡROI��������
long  EnviROIData(const char* roiFile,int* roiPts,int roiNumbers,int bands,double** roiData);

//////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/
/*						                      Ӱ��ͳ����                                                */
/********************************************************************************************************/
//  [1/22/2015 wuwei just you]
//������������֮��Ĺ��׽�
long  Spectral_Angle_Distance(double* data1,double* data2,int bands,double &angle);
//����Ӱ����ĳһ����Ԫ������֮��Ĺ��׽�
//xsize	,ysize ��Ӱ���С
//xpos  ,ypos  ��Ӱ���ϵ�λ��
//data		   �������������
//bands		   ��������
//angle		   ������Ĺ��׽�
long  Spectral_Angle_Distance(double* imgBuffer,int xsize,int ysize,int xpos,int ypos,double *data,int bands,double& angle);


//�������������Ϊһ������
void  Layerstacking(char *pathIn, int begNum,int endNum,char* pathOut);

//imgBuffer		:������ε�Ӱ�����鰴BSQ�洢
//bandSum		:����Ӱ��Ĳ�����
//bandNo		:����Ӱ��Ĳ������
//xsize			:Ӱ��һ����Ԫ����Ŀ
//ysize			:Ӱ��һ����Ԫ����Ŀ
//entropy		:����ÿ�����ε���Ϣ��
//����ֵ1�������ָ��Ϊ��
//����ֵ2���������������
//����ֵ3����������
long  GetImageEntropy(double *imgBuffer,int bandSum,int* bandNo,int xsize,
														int ysize,double* entropy);

//��ȡӰ�񲿷�����
//imgBuffer		:����Ӱ��
//bands			:�������ݲ�����
//xpos			:��ȡ�������ʼλ��
//ypos			:��ȡ�������ʼλ��
//xsize			:��ȡ����x�Ĵ�С
//ysize			:��ȡ����y�Ĵ�С
double* GetpartImage(double* imgBuffer,int bands,int imgxsize,int imgysize,int xpos,int ypos,int xsize,int ysize);

//����Ӱ���ƽ��ֵ
//imgBuffer		:������ε�Ӱ�����鰴BSQ�洢
//bandSum		:����Ӱ��Ĳ�����
//bandNo		:����Ӱ��Ĳ������
//xsize			:Ӱ��һ����Ԫ����Ŀ
//ysize			:Ӱ��һ����Ԫ����Ŀ
//meanValue		:�����õĲ��εľ�ֵ
//����ֵ1�������ָ��Ϊ��
//����ֵ2���������������
//����ֵ3����������
long  GetImgMean(double *imgBuffer,int bandSum,int* bandNo,int xsize,
													int ysize,double* meanValue);

//����ÿ������Ӱ��ı�׼��
//imgBuffer		:������ε�Ӱ�����鰴BSQ�洢
//bandSum		:����Ӱ��Ĳ�����
//bandNo		:����Ӱ��Ĳ������
//xsize			:Ӱ��һ����Ԫ����Ŀ
//ysize			:Ӱ��һ����Ԫ����Ŀ
//meanValue		:����Ĳ��ξ�ֵ
//stdv			:ÿ������Ӱ��ı�׼��
//����ֵ1�������ָ��Ϊ��
//����ֵ2���������������
//����ֵ3����������
long  GetImgStdCovarianve(double *imgBuffer,int bandSum,int* bandNo,
								int xsize,int ysize,double *meanValue,double* stdv);


//�����Ӧ���ε�Э�������
//imgBuffer		:������ε�Ӱ�����鰴BSQ�洢
//bandSum		:����Ӱ��Ĳ�����
//bandNo		:����Ӱ��Ĳ������
//xsize			:Ӱ��һ����Ԫ����Ŀ
//ysize			:Ӱ��һ����Ԫ����Ŀ
//covarianceMat	:�����õĲ��μ��Э�������
//����ֵ1�������ָ��Ϊ��
//����ֵ2���������������
//����ֵ3����������
long  GetCovarianceMatrix(double *imgBuffer,int bandSum,int* bandNo,
									int xsize,int ysize,double** covarianceMat);


//����ÿ�����ε�����Ӱ��
//imgBuffer		:������ε�Ӱ�����鰴BSQ�洢
//bandSum		:����Ӱ��Ĳ�����
//bandNo		:����Ӱ��Ĳ������
//sampleX		:������X�������
//sampleY		:������Y�������
//xsize			:Ӱ��һ����Ԫ����Ŀ
//ysize			:Ӱ��һ����Ԫ����Ŀ
//imgNoise		:����Ӱ����BSQ��ʽ�洢
//����ֵ1�������ָ��Ϊ��
//����ֵ2���������������
//����ֵ3����������
long  GetNoiseImg(double *imgBuffer,int bandSum,int* bandNo,int sampleX,
								int sampleY,int xsize,int ysize,double* imgNoise);
//��ȡ�߹���Ӱ��ĳһ�еĺ��������Ӱ��
//const char*pathIn		������Ӱ��·��
//int lineIdx			��Ӱ���������
//const char* pathOut	�����Ӱ��·��
long  GetCutLineImg(const char*pathIn,int lineIdx,const char* pathOut);

//��ȡ�߹���Ӱ��ĳһ�еĺ��������Ӱ��
//const char*pathIn		������Ӱ��·��
//const char* path_plant_coeff:���ʿ����Ի�ϵ��·��
//int lineIdx			��Ӱ���������
//const char* pathOut	�����Ӱ��·��
long  GetCutLineImg(const char*pathIn,const char* path_plant_coeff,int lineIdx,const char* pathOut);

/********************************************************************************************************/
/*						                        ������                                                 */
/********************************************************************************************************/
//����ȥ���Ļ�
//imgBufferIn	:���������ε�Ӱ�����鰴BSQ�洢
//bandSum		:����Ӱ��Ĳ�����
//bandNo		:����Ӱ��Ĳ������
//xsize			:Ӱ��һ����Ԫ����Ŀ
//ysize			:Ӱ��һ����Ԫ����Ŀ
long  Decentered(double *imgBufferIn,int bandSum,int *bandNo,int xsize,int ysize,double *imgBufferOut);


//���ݰ׻�
//ȥ������֮�䲨�ε������
//imgBufferIn	:���������ε�Ӱ�����鰴BSQ�洢
//bandSum		:����Ӱ��Ĳ�����
//bandNo		:����Ӱ��Ĳ������
//xsize			:Ӱ��һ����Ԫ����Ŀ
//ysize			:Ӱ��һ����Ԫ����Ŀ
long  Derelated(double *imgBufferIn,int bandSum,int *bandNo,int xsize,int ysize,double *imgBufferOut);


//������
//imgBufferIn	:���������ε�Ӱ�����鰴BSQ�洢
//bandSum		:����Ӱ��Ĳ�����
//bandNo		:����Ӱ��Ĳ������
//xsize			:Ӱ��һ����Ԫ����Ŀ
//ysize			:Ӱ��һ����Ԫ����Ŀ
void  SphereTrans(double *imgBufferIn,int bandSum,int *bandNo,int xsize,int ysize,double *imgBufferOut);


//���GDALӰ���д��ʱֱ����·����װP������
//����PCA�任
//pathSrc		:����Ӱ��·��
//pathDst		:���Ӱ��·��
//pathDecenter  :���ȥ���Ļ�Ӱ��
//bandNo		:ѡȡ���б任�Ĳ���
void  SphereTrans(char* pathSrc,char* pathDst,char* pathDecenter,int* bandNo,int num);

/********************************************************************************************************/
/*						                        MNF�任                                                 */
/********************************************************************************************************/
//���MNF����
//covmatNoise	:����Ӱ���Э�������
//covmatImg		:Ӱ���Э�������
//mnfOperator	:MNF�任����
//size			:����Ľ���
//����ֵ1�������ָ��Ϊ��
//����ֵ2���������������
//����ֵ3����������
long  GetMNFOperator(double **covmatNoise,double **covmatImg,double** mnfOperator,int size);


//�������еľ������MNF�任
//imgBufferIn	:���������ε�Ӱ�����鰴BSQ�洢
//bandSum		:����Ӱ��Ĳ�����
//bandNo		:����Ӱ��Ĳ������
//xsize			:Ӱ��һ����Ԫ����Ŀ
//ysize			:Ӱ��һ����Ԫ����Ŀ
//mnfOperator	:MNF�任����
//imgBufferOut	:�����Ӱ��BSQ�洢
//path			:����任�����ļ��е�λ��
//����ֵ1�������ָ��Ϊ��
//����ֵ2���������������
//����ֵ3����������
long  MNF(double *imgBufferIn,int bandSum,int *bandNo,int xsize,int ysize,double *imgBufferOut,char* path);


//�������еľ������MNF���任
//imgBufferIn	:���������ε�Ӱ�����鰴BSQ�洢
//bandSum		:����Ӱ��Ĳ�����
//bandNo		:����Ӱ��Ĳ������
//xsize			:Ӱ��һ����Ԫ����Ŀ
//ysize			:Ӱ��һ����Ԫ����Ŀ
//mnfOperator	:MNF�任����
//imgBufferOut	:�����Ӱ��BSQ�洢
//path			:����任�����ļ��е�λ��
//����ֵ1�������ָ��Ϊ��
//����ֵ2���������������
//����ֵ3����������
long  InvMNF(double* imgBufferIn,int bandSum,int *bandNo,int xsize,int ysize,double *imgBufferOut,char *path);

//���GDALӰ���д��ʱֱ����·����װMNF�任��MNF���任
//����MNF�任
//pathSrc		:����Ӱ��·��
//pathDst		:���Ӱ��·��
//bandNo		:ѡȡ���б任�Ĳ���
//pathMartix	:�任����·��
void  MNFTrans(char* pathSrc,char* pathDst,int* bandNo,int num,char* pathMartix);

//����PCA���任
//pathSrc		:����Ӱ��·��
//pathDst		:���Ӱ��·��
//bandNo		:ѡȡ���з��任�Ĳ���
//pathMartix	:�任����·��
void  InvMNFTrans(char* pathSrc,char* pathDst,int* bandNo,int num,char* pathMartix);

/********************************************************************************************************/
/*						                        PCA�任                                                 */
/********************************************************************************************************/
//���PCA����
//covmatNoise	:����Ӱ���Э�������
//covmatImg		:Ӱ���Э�������
//mnfOperator	:MNF�任����
//size			:����Ľ���
//����ֵ1�������ָ��Ϊ��
//����ֵ2���������������
//����ֵ3����������
long  GetPCAOperator(double **covmatImg,double **pcaOperator,int size);


//�������еľ������PCA�任
//imgBufferIn	:���������ε�Ӱ�����鰴BSQ�洢
//bandSum		:����Ӱ��Ĳ�����
//bandNo		:����Ӱ��Ĳ������
//xsize			:Ӱ��һ����Ԫ����Ŀ
//ysize			:Ӱ��һ����Ԫ����Ŀ
//pcaOperator	:PCA�任����
//imgBufferOut	:�����Ӱ��BSQ�洢
//path			:����任����
//����ֵ1�������ָ��Ϊ��
//����ֵ2���������������
//����ֵ3����������
long  PCA(double *imgBufferIn,int bandSum,int *bandNo,
				int xsize,int ysize,double *imgBufferOut,char* path);


//�������еľ������PCA���任�任
//imgBufferIn	:���������ε�Ӱ�����鰴BSQ�洢
//bandSum		:����Ӱ��Ĳ�����
//bandNo		:����Ӱ��Ĳ������
//xsize			:Ӱ��һ����Ԫ����Ŀ
//ysize			:Ӱ��һ����Ԫ����Ŀ
//pcaOperator	:PCA�任����
//imgBufferOut	:�����Ӱ��BSQ�洢
//path			:����任����
//����ֵ1�������ָ��Ϊ��
//����ֵ2���������������
//����ֵ3����������
long  InvPCA(double *imgBufferIn,int bandSum,int *bandNo,
					int xsize,int ysize,double *imgBufferOut,char* path);

//���GDALӰ���д��ʱֱ����·����װPCA�任��PCA���任
//����PCA�任
//pathSrc		:����Ӱ��·��
//pathDst		:���Ӱ��·��
//bandNo		:ѡȡ���б任�Ĳ���
//pathMartix	:�任����·��
void  PCATrans(char* pathSrc,char* pathDst,int* bandNo,int num,char* pathMartix);

//����PCA���任
//pathSrc		:����Ӱ��·��
//pathDst		:���Ӱ��·��
//bandNo		:ѡȡ���з��任�Ĳ���
//pathMartix	:�任����·��
void  InvPCATrans(char* pathSrc,char* pathDst,int* bandNo,int num,char* pathMartix);

/********************************************************************************************************/
/*						                        RXD�㷨                                                 */
/********************************************************************************************************/
//RX�㷨���Ӱ����쳣
//imgBufferIn	:����Ӱ��
//imgBufferOut	:���Ӱ��
//xsize			:Ӱ��x�����С
//ysize			:Ӱ��y�����С
//bgWndsize		:�������ڴ�С
//tWndsize		:Ŀ�괰�ڴ�С
//bands			:Ӱ��Ĳ�����
void  RXD(double *imgBufferIn,double* imgBufferOut,int xsize,int ysize,int bands,int bgWndsize,int tWndsize);

//pathSrc		:����Ӱ��·��
//pathDst		:���Ӱ��·��
//bgwndsize		:�������ڴ�С
//tWndsize		:Ŀ�괰�ڵĴ�С
//һ�㱳�����ڱ�Ŀ�괰�ڴ�ö�
void  RXD(char* pathSrc,char* pathDst,int bgWndsize,int tWndsize);


//////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/
/*						                     FastICA�㷨                                                */
/********************************************************************************************************/
//�����Ժ���ת��
//tan ���� a1Ϊ�����Ĳ���
long g_tanhFunction(double* w,double* x_imput,int bands,int xsize,int ysize,double* x_output);
long g_derivative_tanh(double* w,double* x_imput,int bands,int xsize,int ysize,double* x_output);
//exp ָ������
long g_expFunction(double* w,double* x_imput,int bands,int xsize,int ysize,double* x_output);
long g_derivative_exp(double* w,double* x_imput,int bands,int xsize,int ysize,double* x_output);
//�����ݺ���
long g_power3Function(double* w,double* x_imput,int bands,int xsize,int ysize,double* x_output);
long g_derivative_power3(double* w,double* x_imput,int bands,int xsize,int ysize,double* x_output);

//���ڸ�����󻯵�FastICA�㷨
//imgBufferIn	����Ӱ��
//xsize ysize	Ӱ��Ĵ�С
//bandSum		Ӱ�񲨶���
//IpNum			�����ɷָ���
//imgBufferOut	������
long  FastICA(double* imgBufferIn,int xsize,int ysize,int bandSum,int IpNum,double* imgBufferOut);

//GDAL��װ��FastICA�㷨
//pathSrc		����Ӱ��·��
//pathDst		���Ӱ��·��
//IpNum			�����ɷָ���
void  FastICA(char* pathSrc,char* pathDst,int IpNum);


/********************************************************************************************************/
/*						                             FastFinder�㷨                                     */
/********************************************************************************************************/
//���㵥�������
long  SingleVloumeFunc(double* data,double*dataCal,double* dataTra,double* dataMul,int bands,int number,double&det);
//ͨ��FastFinder�㷨��ȡӰ���Ԫ
//�㷨ͬ����ͨ��������������ȡ��Ԫ
long  FastFinder(double* imgBuffer,int xsize,int ysize,int bandSum,int endMemberNum);

//GDAL��װ
void  FastFinder(char* pathImg,int endMemberNum,char* pathOut);

/********************************************************************************************************/
/*						                   SVDӰ��ֽ��㷨                                              */
/********************************************************************************************************/
//svd�ֽ�����ݿռ�̫�� ��ʱ��ȥʵ��
long  SVD(double *dataIn,int xsize,int ysize,double *dataOut,char* pathS,char* pathV,char* pathD);

//GDAL��װӰ��SVD�ֽ�
long  SVDTrans(char* pathIn,int *bandNo,char* pathS,char* pathV,char* pathD);

/********************************************************************************************************/
/*						                   OSPӰ��ͶӰ����                                              */
/********************************************************************************************************/
//����ͶӰ������Ӱ�����ͶӰ
//transMat��ͶӰ����
//matx maty��ͶӰ����Ĵ�С
//dataOut���������
long  OSP(double* dataIn,int xsize,int ysize,int bands,double* transMat,int matx,int maty,double* dataOut);

//ʹ�������ӿռ�ͶӰ�ķ������й��׽��
//double* dataIn							�����Ӱ������
//int int xsize,int ysize,int bands			Ӱ��Ĵ�С��Ӱ�񲨶���
//double *endMat,int endNumber,int bands	��Ԫ���ף���Ԫ��Ŀ�Ͷ�Ԫ������
//double *dataout							������
long  OSPUnmix(double* dataIn,int xsize,int ysize,int bands,double *endMat,int endNumber,double *dataout);
//GDAL��װ�������ӿռ�ͶӰ�Ĺ��׽�췽��
long  OSPUnmix(const char* pathImgIn,const char* pathImgOut,const char* endFile,int endnumber,int bands);

#endif
