#pragma once
class CVMachineLearningTrain
{
public:
	//BP������ѵ��MNIST�Ⲣ��ѵ��������
	void CV_ANN_BP_TrainMnist(const char* pathMnist, const char* pathLabel, const char* pathNet);
};