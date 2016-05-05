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
	void BSBCImageInpaintingProcess(const char* pathImgIn, const char* pathImgMask, const char* pathImgRepair);
private:
	void DiffuseLoop(int* data, int* img, int x, int y, int xsize, int ysize);

	void RepaireLoop(int* data, int* img, double delta, int x, int y, int xsize, int ysize);

	double RepairChange(int* img, int x, int y, int xsize, int ysize);

	double Laplace(int* img, int x, int y, int xsize, int ysize);

	bool TerminateBSCBCondition(int* preData, int* afterData, int *maskData, int xsize, int ysize);
};
