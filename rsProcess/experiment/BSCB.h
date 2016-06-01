#pragma once
//BSCBӰ���޸�����
/*BSCBӰ���޸���������������Ϊ��
1.��Ϣ����
2.��Ϣ��ɢ
3.��ֹ����
*/
class BSCB
{
public:
	void BSCBImageInpaintingProcess(const char* pathImgIn, const char* pathImgMask, const char* pathImgRepair);
private:
	void InterpolationInit(float* data, float *maskData, int xsize, int ysize);

	void DiffuseLoop(float* data, float* img, int x, int y, int xsize, int ysize);

	void RepaireLoop(float* data, float* img, double delta, int x, int y, int xsize, int ysize);

	double RepairChange(float* img, int x, int y, int xsize, int ysize);

	double Laplace(float* img, int x, int y, int xsize, int ysize);

	bool TerminateBSCBCondition(float* preData, float* afterData, float *maskData, int xsize, int ysize);
};
