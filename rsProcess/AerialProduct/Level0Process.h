#pragma once
#include<vector>
using namespace std;
/*
	0�����ݲ�Ʒ�������
	1.0������ת��ΪBSQ��ʽ���ݣ�ԭʼ����ΪBIL��ʽ�������д��ץ��ΪBSQ�󷽱㴦��
	2.0��Ӱ�����ݷǾ�����У��������ԭʼӰ��ķǾ�����У����������򵥵�У��������Ŀǰ�����ڲ�ͬ������������ͬ�Ĵ�����
	3.����0�����ݿ��ܰ���������Ϣ����Ҫ�޳�������Ϣֻ��ȡ��������
	4.��ԭʼ�����н����Eventʱ������
*/
class Level0Process
{
public:
	//ԭʼ����ת��ΪBSQ��ʽ����
	virtual long Level0Proc_RawToBSQ(const char* pathRawBIL, const char* pathBSQ);

	//����ʵ���Ҷ������ݺͰ��������ݽ��д������Ǿ�����У��
	virtual long Level0Proc_Nonuniform(const char* pathBSQ, const char* pathNonuniform,const char* pathCalibFile= 0, const char* pathDarkFile=0);

	//��ȡ���õ����ݣ���ȥ�������õ����ݺͲ���
	virtual long Level0Proc_GetUsefulData(const char* pathBSQ, const char* pathUseful, int leftunuse, int rightunuse, vector<int> usefulBands);

	//����õ�Event�ļ�����
	virtual long Level0Proc_ExtractEvent(const char* pathRawBIL, const char* pathEvent, const int nEventOffset);
private:
	//��ȡ�Ǿ�����У��������ͨ��ʵ���Ҷ����ļ��Ͱ�����
	void GetNonuniformParameters(const char* pathCalibFile, const char* pathDarkFile, float* params);
};

class QPDLevel0Process : public Level0Process
{
public:
	//�޸Ķ̲�����Event��Ϣ�����ڶ���ȫ�׶����ݶ̲�Event�İ�΢������ȷ�ģ�����������쳣
	//�����Ҫ��ȫ�׶ζ̲����ݽ��д���
	long Level0Proc_ModifySWIREvent(const char* pathEvent,const char* pathExEvent, float fTime);
};


//ȫ�׶�����0�����ݲ�Ʒ�������Ժ���
void QPDLevel0ProcessUnitTestFunc();