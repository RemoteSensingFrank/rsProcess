#pragma once

//������������ȡ�������
#ifndef _LAS_PROGRESS_
#define _LAS_PROGRESS_
#include"AuxiliaryFunction.h"
#include <string>
using namespace std;
/**
* @brief ����������
*
* �ṩ����������ӿڣ�����ӳ��ǰ�㷨�Ľ���ֵ ģ��GDAL������
*/
class  GDALProcessBase
{
public:
	/**
	* @brief ���캯��
	*/
	GDALProcessBase()
	{
		m_dPosition = 0.0;
		m_iStepCount = 100;
		m_iCurStep = 0;
		m_bIsContinue = true;
	}

	/**
	* @brief ��������
	*/
	virtual ~GDALProcessBase() {}

	/**
	* @brief ���ý�����Ϣ
	* @param pszMsg			������Ϣ
	*/
	virtual void SetMessage(const char* pszMsg) = 0;

	/**
	* @brief ���ý���ֵ
	* @param dPosition		����ֵ
	* @return �����Ƿ�ȡ����״̬��trueΪ��ȡ����falseΪȡ��
	*/
	virtual bool SetPosition(double dPosition) = 0;

	/**
	* @brief ������ǰ��һ��������true��ʾ������false��ʾȡ��
	* @return �����Ƿ�ȡ����״̬��trueΪ��ȡ����falseΪȡ��
	*/
	virtual bool StepIt() = 0;

	/**
	* @brief ���ý��ȸ���
	* @param iStepCount		���ȸ���
	*/
	virtual void SetStepCount(int iStepCount)
	{
		ReSetProcess();
		m_iStepCount = iStepCount;
	}

	/**
	* @brief ��ȡ������Ϣ
	* @return ���ص�ǰ������Ϣ
	*/
	string GetMessage()
	{
		return m_strMessage;
	}

	/**
	* @brief ��ȡ����ֵ
	* @return ���ص�ǰ����ֵ
	*/
	double GetPosition()
	{
		return m_dPosition;
	}

	/**
	* @brief ���ý�����
	*/
	void ReSetProcess()
	{
		m_dPosition = 0.0;
		m_iStepCount = 100;
		m_iCurStep = 0;
		m_bIsContinue = true;
	}

	/*! ������Ϣ */
	string m_strMessage;
	/*! ����ֵ */
	double m_dPosition;
	/*! ���ȸ��� */
	int m_iStepCount;
	/*! ���ȵ�ǰ���� */
	int m_iCurStep;
	/*! �Ƿ�ȡ����ֵΪfalseʱ��ʾ����ȡ�� */
	bool m_bIsContinue;
};

/**
* @brief ����̨��������
*
* �ṩ����̨����Ľ�������ӿڣ�����ӳ��ǰ�㷨�Ľ���ֵ
*/
class  GDALConsoleProcess : public GDALProcessBase
{
public:
	/**
	* @brief ���캯��
	*/
	GDALConsoleProcess()
	{
		m_dPosition = 0.0;
		m_iStepCount = 100;
		m_iCurStep = 0;
	};

	/**
	* @brief ��������
	*/
	~GDALConsoleProcess()
	{
		//remove(m_pszFile);
	};

	/**
	* @brief ���ý�����Ϣ
	* @param pszMsg			������Ϣ
	*/
	void SetMessage(const char* pszMsg)
	{
		m_strMessage = pszMsg;
		printf("%s\n", pszMsg);
	}

	/**
	* @brief ���ý���ֵ
	* @param dPosition		����ֵ
	* @return �����Ƿ�ȡ����״̬��trueΪ��ȡ����falseΪȡ��
	*/
	bool SetPosition(double dPosition)
	{
		m_dPosition = dPosition;
		TermProgress(m_dPosition);
		m_bIsContinue = true;
		return true;
	}

	/**
	* @brief ������ǰ��һ��
	* @return �����Ƿ�ȡ����״̬��trueΪ��ȡ����falseΪȡ��
	*/
	bool StepIt()
	{
		m_iCurStep++;
		m_dPosition = m_iCurStep*1.0 / m_iStepCount;

		TermProgress(m_dPosition);
		m_bIsContinue = true;
		return true;
	}

private:
	void TermProgress(double dfComplete)
	{
		static int nLastTick = -1;
		int nThisTick = (int)(dfComplete * 40.0);

		nThisTick = min(40, max(0, nThisTick));

		// Have we started a new progress run?  
		if (nThisTick < nLastTick && nLastTick >= 39)
			nLastTick = -1;

		if (nThisTick <= nLastTick)
			return;

		while (nThisTick > nLastTick)
		{
			nLastTick++;
			if (nLastTick % 4 == 0)
				fprintf(stdout, "%d", (nLastTick / 4) * 10);
			else
				fprintf(stdout, ".");
		}

		if (nThisTick == 40)
			fprintf(stdout, " - done.\n");
		else
			fflush(stdout);
	}
};
#ifndef STD_API
#define STD_API __stdcall
#endif

/*****************************************************************************
* @brief : ������������GDAL�㷨�еĽ��ȵ��������
* @author : W.W.Frank
* @date : 2015/12/3 21:29
* @version : version 1.0
* @inparam :
* @outparam :
*****************************************************************************/
int STD_API ALGTremProgress(double dfComplete, const char* pszMessage, void* pProgress);

#endif