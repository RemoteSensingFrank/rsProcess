#pragma once
#include"../opencv/opencv2/ml.hpp"
#include "../opencv/opencv2/core/core.hpp"
#include"../opencv\opencv2\imgproc.hpp"
#include "../opencv/opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;
using namespace ml;

//���ݼ�����
enum DatasetTypes
{
	DATASET_MNIST = 0,
	ENVI_IMAGEROI = 1
	
};

//����ѧϰѵ���㷨
class CVMachineLearningTrain
{
public:
	//BP������ѵ��MNIST�Ⲣ��ѵ��������
	void CV_ANN_BP_Train(const char* pathDataset, const char* pathLabelSet, const char* pathNet, DatasetTypes datasetType);

	//SVM������
	void CV_SVM_Train(const char* pathDataset,double C, const char* pathLabelSet, const char* pathSVM, DatasetTypes datasetType);

	//logistic regression�߼��ع�ѵ����
	void CV_LogisticRegression_Train(const char* pathDataset, const char* pathLabelSet, const char* pathLogisticRegression, DatasetTypes datasetType);

private:
	//��ȡѵ�����ݼ�
	void CV_GetMnistTrainData(const char* pathMnist,const char* pathLabel,Mat &trianMat, Mat &labelMat);
};

//����ѧϰԤ���㷨
class CVMachineLearningPredict
{
public:
	//�����㷨��Mnist���ݽ���Ԥ��
	void CV_ANN_BP_PredictMnist(const char* pathPredict, const char* pathNet, const char* pathLabel = NULL);
	void CV_SVM_PredictMnist(const char* pathPredict, const char* pathSVM,const char* pathLabel=NULL);
	void CV_LogisticRegression_PredictMnist(const char* pathPredict, const char* pathLogisticRegression, const char* pathLabel = NULL);
};