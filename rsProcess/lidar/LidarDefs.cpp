#include"LidarDefs.h"

const unsigned short LASHeader::Data_Record_Length_of_Format0 = 20;
const unsigned short LASHeader::Data_Record_Length_of_Format1 = 28;
//��������Ϊ1.2��ʽ��Ӧ�ĵ��ʽ
const unsigned short LASHeader::Data_Record_Length_of_Format2 = 26;
const unsigned short LASHeader::Data_Record_Length_of_Format3 = 34;
const unsigned short LASHeader::HeaderSize_Def = 227;
//==========================================las�ļ�ͷ�ļ�======================================================================
LASHeader::LASHeader()
{
	const char tmp[] = "LASF";
	for (int i = 0; i < 4; i++)
		file_signature[i] = tmp[i];
	//strcpy_s(file_signature,"LASF");
	// File_Source_ID = 0;
	reserved = 0;
	project_ID_GUID_data_1 = 0;
	project_ID_GUID_data_2 = 0;
	project_ID_GUID_data_3 = 0;
	memset(project_ID_GUID_data_4, 0, 8);
	version_major = 1;
	version_minor = 0;

	memset(system_id, 0, 32);
	strcpy_s(generating_software, "W.W_Frank");

	memset(generating_software, 0, 32);
	strcpy_s(generating_software, "RAMS"); // ��չColor�����к����generating_software����RAMS����TerraSolid��ȡ��Ȼ����

										   //DateTime dt = DateTime::Now();
	file_creation_day = 1;
	file_creation_year = 2015;

	header_size = HeaderSize_Def;
	offset_to_point_data = header_size + 2; // 1.0�汾ͨ���ڿɱ䳤��¼����һ��������0xccdd��1.1ȡ�������������
	number_of_variable_length_records = 0;
	point_data_format = 0;
	point_data_record_length = Data_Record_Length_of_Format0;// + 4;
	number_of_point_records = 0;

	for (int i = 0; i < 5; i++)
		number_of_points_by_return[i] = 0;

	x_scale_factor = 1;
	y_scale_factor = 1;
	z_scale_factor = 1;
	x_offset = 0;
	y_offset = 0;
	z_offset = 0;
	min_x = 0;
	max_x = 0;
	min_y = 0;
	max_y = 0;
	min_z = 0;
	max_z = 0;
}
LASHeader::LASHeader(const LASHeader& header)
{
	memcpy(this, &header, HeaderSize_Def);
}
LASHeader& LASHeader::operator=(const LASHeader& header)
{
	memcpy(this, &header, HeaderSize_Def);
	return *this;
}
void LASHeader::SetFile_Source_ID(unsigned short id)
{
	unsigned int t = id;
	if (version_major == 1 && version_minor == 1)
	{
		reserved &= 0x0000FFFF;
		reserved |= (t << 16);
	}
}
void LASHeader::Setreserved(unsigned int reservedi)
{
	if (version_major == 1 && version_minor == 1)
	{
		reserved &= 0xFFFF0000;
		reserved |= (reserved & 0x0000FFFF);
	}
	else
		reserved = reservedi;
}
bool LASHeader::HasGPSTime() const
{
	if (version_major == 1 && (version_minor == 0 || version_minor == 1))
	{
		if (point_data_format == 1)
			return true;
		else
			return false;
	}
	else if (version_major == 1 && version_minor == 2)//1.2
	{
		if (point_data_format == 1 || point_data_format == 3)
			return true;
		else
			return false;
	}
	else
		exit(1);
}
bool LASHeader::HasLASColorExt4() const
{
	// color: 4 unsigned char
	// lyf modified [20100522] �����˶�1.2��ʽ��֧��
	// ����Ҫ������������ǣ�1��1.0����1.1��ʽ��չ������ɫ��Ϣ(4���ֽ�)
	// 2.1.2��ʽ�Դ�rgb������ɫֵ(3���ֽ�)
	if (version_major == 1 && (version_minor == 0 || version_minor == 1))//1.0 1.1��ʽ
	{
		if (point_data_format == 0)
			return point_data_record_length == Data_Record_Length_of_Format0 + 4 ? true : false;
		else if (point_data_format == 1)
			return point_data_record_length == Data_Record_Length_of_Format1 + 4 ? true : false;
		else
			exit(0);
	}
	else if (version_major == 1 && version_minor == 2)//1.2��ʽ
	{
		if (point_data_format == 0)
			return point_data_record_length == Data_Record_Length_of_Format0 + 4 ? true : false;
		else if (point_data_format == 1)
			return point_data_record_length == Data_Record_Length_of_Format1 + 4 ? true : false;
		else if (point_data_format == 2)
			return point_data_record_length == Data_Record_Length_of_Format2 + 4 ? true : false;
		else if (point_data_format == 3)
			return point_data_record_length == Data_Record_Length_of_Format3 + 4 ? true : false;
		else
			exit(0);
	}
	return false;
}
bool LASHeader::HasLASColorExt6() const
{
	if (version_major == 1 && version_minor == 2)
	{
		if (point_data_format == 2 || point_data_format == 3)
			return true;
		else if (point_data_format == 0 || point_data_format == 1)
			return false;
		else
			exit(0);
	}
	else
		return false;
}
int  LASHeader::HasLASPointExt() const
{
	int len = 0;
	if (point_data_format == 0)
		len = point_data_record_length - LASHeader::Data_Record_Length_of_Format0;
	else if (point_data_format == 1)
		len = point_data_record_length - LASHeader::Data_Record_Length_of_Format1;
	// lyf modified [20100522] �����˶�1.2��ʽ��֧��
	else if (point_data_format == 2)
		len = point_data_record_length - LASHeader::Data_Record_Length_of_Format2;
	else if (point_data_format == 3)
		len = point_data_record_length - LASHeader::Data_Record_Length_of_Format3;
	else
		exit(0);
	if (len >= 0)
		return len;
	else
		exit(1);
}
void LASHeader::LASHeader_Read(FILE *fs)
{
	//��ȡ��׼�ļ�ͷ����
	//����ֽڲ�����Ļ���ֻ�ܰ������·�ʽ��ȡ
	//fseek(fs, 0, SEEK_SET);
	//fread((void*)(file_signature), 4, 1, fs);
	//fread((void*)(&reserved), 4, 1, fs);
	//fread((void*)(&project_ID_GUID_data_1), 4, 1, fs);
	//fread((void*)(&project_ID_GUID_data_2), 2, 1, fs);
	//fread((void*)(&project_ID_GUID_data_3), 2, 1, fs);
	//fread((void*)(project_ID_GUID_data_4),  8, 1, fs);
	//fread((void*)(&version_major), 1, 1, fs);
	//fread((void*)(&version_minor), 1, 1, fs);
	//fread((void*)(&system_id), 32, 1, fs);
	//fread((void*)(&generating_software), 32, 1, fs);
	//fread((void*)(&file_creation_day), 2, 1, fs);
	//fread((void*)(&file_creation_year), 2, 1, fs);
	//fread((void*)(&header_size), 2, 1, fs);
	//fread((void*)(&offset_to_point_data), 4, 1, fs);
	//fread((void*)(&number_of_variable_length_records), 4, 1, fs);
	//fread((void*)(&point_data_format), 1, 1, fs);
	//fread((void*)(&point_data_record_length), 2, 1, fs);
	//fread((void*)(&number_of_point_records), 4, 1, fs);
	//fread((void*)(&number_of_points_by_return), 4, 5, fs);
	//
	//fread((void*)(&x_scale_factor), 8, 1, fs);
	//fread((void*)(&y_scale_factor), 8, 1, fs);
	//fread((void*)(&z_scale_factor), 8, 1, fs);
	//fread((void*)(&x_offset), 8, 1, fs);
	//fread((void*)(&y_offset), 8, 1, fs);
	//fread((void*)(&z_offset), 8, 1, fs);

	//fread((void*)(&max_x), 8, 1, fs);
	//fread((void*)(&min_x), 8, 1, fs);
	//fread((void*)(&max_y), 8, 1, fs);
	//fread((void*)(&min_y), 8, 1, fs);
	//fread((void*)(&max_z), 8, 1, fs);
	//fread((void*)(&min_z), 8, 1, fs);
	//int num = sizeof(LASHeader);
	fread((void*)(this), HeaderSize_Def, 1, fs);
	strcpy_s(generating_software, "W.W_Frank");

	//��ȡ�ļ�����
	int curPos = ftell(fs);
	fseek(fs, 0, SEEK_END);
	int size = ftell(fs);
	fseek(fs, curPos, SEEK_SET);

	// �����ļ�ͷ��¼�ĵ�����ʵ�ʵ�������
	unsigned int ptsCnt;
	int len = size;
	if ((unsigned int)len <= offset_to_point_data)
		ptsCnt = 0;
	else
		ptsCnt = (len - offset_to_point_data);

	ptsCnt /= point_data_record_length;
	number_of_point_records = ptsCnt;
	if (point_data_record_length == Data_Record_Length_of_Format0)
	{
		point_data_format = 0;
	}
	else if (point_data_record_length == Data_Record_Length_of_Format1)
	{
		point_data_format = 1;
	}
	else if (point_data_record_length == Data_Record_Length_of_Format2)
	{
		point_data_format = 2;
	}
	else if (point_data_record_length == Data_Record_Length_of_Format3)
	{
		point_data_format = 3;
	}
	else
		exit(1);
}
void LASHeader::LASHeader_Write(FILE *fs) const
{
	fwrite((void*)(this), HeaderSize_Def, 1, fs);
}
//==========================================las�䳤�ֶζ�ȡ=====================================================================
const unsigned short LASVariableRecord::Min_Record_Length_After_Header = min_las_record_after_header;
const short LASVariableRecord::Point_Data_Start_Signature = (short)0xccdd;

void LASVariableRecord::LASVariableRecord_Read(FILE* fs)
{
	LASVariableRecord& record = (*this);
	fread(&record.reserved, sizeof(unsigned short), 1, fs);
	fread(record.user_id, sizeof(char), 16, fs);
	fread(&record.record_id, sizeof(unsigned short), 1, fs);
	fread(&record.record_length_after_header, sizeof(unsigned short), 1, fs);
	fread(record.description, sizeof(char), 32, fs);

	if (record.record_buffer != NULL)
	{
		delete[] record.record_buffer;
		record.record_buffer = NULL;
	}
	int len = record.record_length_after_header;
	if (len > 0)
	{
		record.record_buffer = new unsigned char[len];
		fread(record.record_buffer, sizeof(char), len, fs);
	}
}
void LASVariableRecord::LASVariableRecord_Write(FILE* fs) const
{
	fwrite(&reserved, sizeof(unsigned short), 1, fs);
	fwrite(user_id, sizeof(char), 16, fs);
	fwrite(&record_id, sizeof(unsigned short), 1, fs);
	fwrite(&record_length_after_header, sizeof(unsigned short), 1, fs);
	fwrite(description, sizeof(char), 32, fs);

	int len = record_length_after_header;
	if (len > 0)
	{
		if (record_buffer == NULL)
			exit(1);
		else
			fwrite(record_buffer, sizeof(char), len, fs);
	}
}
//==============================================================================================================================
//���ݿ麯��
void LASRectangle::LASRect_AllocateMemory(int lasPoints, bool inMemory, Rect rect)
{
	m_lasPoints_numbers = lasPoints;
	try
	{
		if (inMemory)
		{
			m_lasPoints = new LASPoint[lasPoints];
			m_point_index = new int[lasPoints];
		}
		else
		{
			m_point_index = new int[lasPoints];
		}
		m_Rectangle.setRect(rect.min[0], rect.min[1], rect.max[0], rect.max[1]);
	}
	catch (bad_alloc &e)
	{
		printf("%s\n", e.what());
		exit(-1);
	}
}
//LASSet ���ݼ�����
bool SearchRectCallback(int id, void* arg)
{
	//cout << "Hit data rect " << id << "\n";
	vector<int> *rect = (vector<int>*)arg;
	rect->push_back(id);
	return true; // keep going
}
bool LASSet::LASSet_Search(int rectID, Rect searchRect, vector<int> &rects)
{
	if (m_lasBlockTree.Count() == 0)
		return false;
	else
		m_lasBlockTree.Search(searchRect.min, searchRect.max, SearchRectCallback, &rects);
	return true;
}
bool LASSet::LASSet_Search(int rectID, LAS_XYZ searchPnt, vector<int> &rects)
{
	Rect searchRect(searchPnt.x, searchPnt.y, searchPnt.x, searchPnt.y);
	if (m_lasBlockTree.Count() == 0)
		return false;
	else
		m_lasBlockTree.Search(searchRect.min, searchRect.max, SearchRectCallback, &rects);
	return true;
}
//����˳�δ����ҵ���Ӧ�ĵ�
bool LASSet::LasSet_Search(int pointID, LAS_XYZ &searchPnt)
{
	int pntBegIdz=0, pntEndIdx=0;
	for (int i = 0; i < m_numRectangles; ++i)
	{
		//��ʼ����ֹ���
		pntBegIdz = pntEndIdx;
		pntEndIdx += m_lasRectangles[i].m_lasPoints_numbers - 1;
		if (pointID >= pntBegIdz&&pointID <= pntEndIdx)
		{
			memcpy(&searchPnt, &m_lasRectangles[i].m_lasPoints[pointID - pntBegIdz].m_vec3d, sizeof(LAS_XYZ));
			return true;
		}
	}
	return false;
}
long LASSet::LASSet_BuildTree()
{
	if (m_lasRectangles == NULL|| m_numRectangles == 0)
	{
		printf("no data to build tree\n");
		return -1;
	}
	//�����ݹ���R���ṹ
	for (int i = 0; i < m_numRectangles; ++i)
		m_lasBlockTree.Insert(m_lasRectangles[i].m_Rectangle.min, m_lasRectangles[i].m_Rectangle.max, i);
	return 0;
}
void LASSet::LASSet_AllocateMemory(int lasRects, bool inMemory)
{
	m_numRectangles = lasRects;
	try
	{
		if (inMemory)
		{
			m_lasRectangles = new LASRectangle[lasRects];
		}
	}
	catch (bad_alloc &e)
	{
		printf("%s\n", e.what());
		exit(-1);
	}
}
void LASSet::LASSet_Trim()
{
	//��ȡ���ݵ�����λ��
	THREEDPOINT pntLasSet;
	memset(&pntLasSet, 0, sizeof(THREEDPOINT));
	for (int i = 0; i < m_numRectangles; ++i)
	{
		THREEDPOINT pntRectSet;
		memset(&pntRectSet, 0, sizeof(THREEDPOINT));
		for (int j = 0; j < m_lasRectangles[i].m_lasPoints_numbers; ++j)
		{
			pntRectSet.dX += m_lasRectangles[i].m_lasPoints[j].m_vec3d.x / m_lasRectangles[i].m_lasPoints_numbers;
			pntRectSet.dY += m_lasRectangles[i].m_lasPoints[j].m_vec3d.y / m_lasRectangles[i].m_lasPoints_numbers;
			pntRectSet.dZ += m_lasRectangles[i].m_lasPoints[j].m_vec3d.z / m_lasRectangles[i].m_lasPoints_numbers;
			memcpy(&m_lasRectangles[i].m_rectCenter, &pntRectSet, sizeof(THREEDPOINT));
		}
		pntLasSet.dX += pntRectSet.dX / m_numRectangles;
		pntLasSet.dY += pntRectSet.dY / m_numRectangles;
		pntLasSet.dZ += pntRectSet.dZ / m_numRectangles;
		memcpy(&m_SetCenter, &pntLasSet, sizeof(THREEDPOINT));
	}
}