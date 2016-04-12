#pragma once
#include"LidarDefs.h"

/*��ȡlidar�ļ�����*/
class LidarReader
{
public:
	LidarReader() { m_lasFile = NULL; m_lasvarHeader = NULL; m_isDatasetOpen = false; }
	~LidarReader() {
		if (m_lasFile != NULL)
		{
			fclose(m_lasFile);
			m_lasFile = NULL;
		}
		if (m_lasvarHeader != NULL)
		{
			delete[]m_lasvarHeader;
			m_lasvarHeader = NULL;
		}
		m_isDatasetOpen = false;
	}

	//1.��las�ļ�
	virtual long LidarReader_Open(const char* pathLidar) = 0;
	//2.��ȡlas�ļ�
	virtual long LidarReader_Read(bool inMemory,int sample) = 0;
	//3.��las�ļ�д��
	virtual long LidarReader_Write(const char* pathLidar) = 0;

public:
	LASHeader			m_lasHeader;
	LASVariableRecord*	m_lasvarHeader;
	LASVariable_header_geo_keys m_lasgeokeyHeader;
	LASVariable_header_key_entry m_lasgeoentryHeader;
	LASSet				m_lasDataset;
	//����Ϊlas�ļ����ݼ�
	FILE				*m_lasFile;
	bool                m_isDatasetOpen;
};

/*LAS��ʽ�ļ���ȡ����*/
class LASLidarReader : public LidarReader
{
public:
	//1.��las�ļ�����ȡlas�ļ�ͷ�ļ���ȡͷ�ļ���Ϣ
	long LidarReader_Open(const char* pathLidar);
	//2.��ȡ���ݣ��ж��Ƿ��ȡ�������ڴ棬�������ڴ���ֻ��ȡindex��������������
	long LidarReader_Read(bool inMemory, int readSkip =1);
	//3.��las�ļ�д��
	long LidarReader_Write(const char* pathLidar);
};