#pragma once

//Ŀǰ���д�����1����ȷ���������0����������������ٶԴ��������е���

//��־���·��
#define _log_path_ "c:\\process_log\\rsProcess.log";

//����ͷ��Ϣ
typedef struct stDataInfo
{
	unsigned int	nSensorType;
	unsigned int	nSensorOrder;
	unsigned int	nWidths;
	unsigned int	nSamples;
	unsigned int	nLines;
	unsigned int	nBands;
	unsigned int	nYear;
	unsigned int	nMonth;
	unsigned int	nDay;
	unsigned int	nSecond;
	unsigned int	nMode;
	__int64 nHeadOffset;
	__int64 nEofOffset;
}DINFO;