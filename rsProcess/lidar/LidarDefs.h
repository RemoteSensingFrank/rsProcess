#pragma once
#include<time.h>
#include<vector>
using namespace std;
#include"Geometry.h"
#include"..\RTree.h"

//�����ȡ���ݵ�����ڴ���Ϊ512MB
#define LargestMemoryToRead 536870912
#define BlockPointNumbers 2048
//================================================lasͷ�ļ�����=========================================================
// ��������
enum  LAS_CLASSIFICATION_TYPE
{
	CLASSIFICATION_NULL = 0,	// δ�������
	CLASSIFICATION_UNCLASSIFIED = 1,
	CLASSIFICATION_GROUND = 2,
	CLASSIFICATION_LOW_VEGETATION = 3,
	CLASSIFICATION_MEDIUM_VEGETATION = 4,
	CLASSIFICATION_HIGH_VEGETATION = 5,
	CLASSIFICATION_BUILDING = 6,
	CLASSIFICATION_LOW_POINT_NOISE = 7,
	CLASSIFICATION_MEDEL_KEYPOINT = 8,
	CLASSIFICATION_WATER = 9,
	CLASSIFICATION_OVERLAP_POINTS2 = 12,
	CLASSIFICATION_reserved						// ����ʹ��
};

#pragma pack(1)/*ʹ���ֽڶ���*/
class LASHeader
{
public:

	// LASHeader���ļ��еĴ�Сdpw
	// sizeof(LASHeader)����Ľ�����ܱ������
	// ��Ϊ���������ܻ���һЩ�Ż�(��߽�����) 
	// ������ɫ��չ
	enum { HEADER_SIZE = 227 };

public:												// ���� ˵��		Ĭ��ֵ	�ֽڱ��
	char file_signature[4];							// *	�ļ���־	"LASF"	1-4
													//unsigned short file_source_id;				// *	�ļ����	0		
	/*
	* This data field is reserved and must be zero filled by generating generating_software.
	* 1.0��las�ļ�Ϊ4�ֽڣ�1.1��Ϊ2�ֽڣ��������File_Source_ID����4���ֽ�
	*/
	unsigned long  reserved;						// -	����ʹ��	5-8		5-8
	//unsigned short filesourceid;
	unsigned long  project_ID_GUID_data_1;			// -	4�ֽ�		0		9-12
	unsigned short project_ID_GUID_data_2;			// -	2�ֽ�		0		13-14
	unsigned short project_ID_GUID_data_3;			// -	2�ֽ�		0		15-16
	unsigned char project_ID_GUID_data_4[8];		// -	8�ֽ�		0		17-24
	unsigned char version_major;					// *	���汾��			25
	unsigned char version_minor;					// *	���汾��			26
	unsigned char system_id[32];					// *	ϵͳ��ʶ			27-58
	char generating_software[32];					// *	�������			59-90
	unsigned short file_creation_day;				// -	����ʱ��			91-92
	unsigned short file_creation_year;				// -						93-94
	unsigned short header_size;						// *	Head��С			95-96
	unsigned int offset_to_point_data;				// *	���ݵ�ַ			97-100
	unsigned int number_of_variable_length_records; // *	�䳤��¼��Ŀ		101-104
	unsigned char point_data_format;				// *	�����ݸ�ʽGPS		105
	unsigned short point_data_record_length;		// *	�����ݳ���			106-107
	unsigned long number_of_point_records;			// *	�����Ŀ			108-111
	unsigned long number_of_points_by_return[5];	// *	�ز�������			112-131

	/*
	* Xcoordinate = (Xrecord * x_scale_factor) + x_offset
	* Ycoordinate = (Yrecord * y_scale_factor) + y_offset
	* Zcoordinate = (Zrecord * z_scale_factor) + z_offset
	*/
	double x_scale_factor;							// *	����ϵ��			132-139
	double y_scale_factor;							// *	����ϵ��			140-147
	double z_scale_factor;							// *	����ϵ��			148-155

	double x_offset;								// *	���ƫ��			156-163	
	double y_offset;								// *	���ƫ��			170-177
	double z_offset;								// *	���ƫ��			164-169

	double max_x;									// *	x���ֵ				178-195
	double min_x;									// *	x��Сֵ				186-193
	double max_y;									// *						194-201			
	double min_y;									// *						202-209
	double max_z;									// *						210-217
	double min_z;									// *						218-225

public:
	/*
		���������
	*/
	LASHeader();
	LASHeader(const LASHeader& header);
	LASHeader& operator=(const LASHeader& header);

	/*
		���ݼ�ID
	*/
	unsigned short GetFile_Source_ID() { return (version_minor == 1 && version_minor == 1) ? reserved >> 16 : 0; }
	void SetFile_Source_ID(unsigned short id);

	/*
		���ݼ�����λ
	*/
	unsigned int Getreserved() { return (version_minor == 1 && version_minor == 1) ? reserved &= 0x0000FFFF : reserved; }
	void Setreserved(unsigned int reservedi);

	bool HasPoint()		   const
	{
		if (point_data_record_length >= 12)
			return true;
		else
			return false;
	}
	bool HasIntensity()	   const
	{
		if (point_data_record_length >= 14)
			return true;
		else
			return false;
	}
	bool HasReturnNumber() const
	{
		if (point_data_record_length >= 15)
			return true;
		else
			return false;
	}
	bool HasNumberofReturn()const
	{
		if (point_data_record_length >= 15)
			return true;
		else
			return false;
	}
	bool HasScanDirectionFlag() const
	{
		if (point_data_record_length >= 15)
			return true;
		else
			return false;
	}
	bool HasEdgeofFlightLine()const
	{
		if (point_data_record_length >= 15)
			return true;
		else
			return false;
	}
	bool HasScanAgnleRank()	const
	{
		if (point_data_record_length >= 17)
			return true;
		else
			return false;
	}
	bool HasFlightLine()const
	{
		if (point_data_record_length >= 17)
			return true;
		else
			return false;
	}
	bool HasGPSTime()const;

	bool HasLASColorExt4() const;	//�Ƿ�����ɫ ���ֽڶ���
	bool HasLASColorExt6() const;	//�Ƿ�����ɫ ���ֽڶ���
	int  HasLASPointExt()  const;	//���ݸ�ʽ�Ƿ�����չ
	bool HasPointDataStartfile_signature() const { return version_minor == 0 && version_minor == 1; }
	tm*  GetDataTime() {
		time_t timer;
		time(&timer);
		return localtime(&timer);
	};				//��ȡʱ��

	void LASHeader_Read(FILE *fs);		//��д����ͷ�ļ�
	void LASHeader_Write(FILE *fs) const;

	const static unsigned short Data_Record_Length_of_Format0; // 20������չ�����ṹʱ��0��ʽ���׼����
	const static unsigned short Data_Record_Length_of_Format1; // 28������չ�����ṹʱ��1��ʽ���׼����
	const static unsigned short Data_Record_Length_of_Format2; // 26��1.2����չ�����ṹʱ��1��ʽ���׼����
	const static unsigned short Data_Record_Length_of_Format3; // 34��1.2����չ�����ṹʱ��1��ʽ���׼����
	const static unsigned short HeaderSize_Def;				   // 227����׼ͷ�ļ�����

	const static char ErrorPointCnt[];			 // "number of point is more than LasFileHead.Number_of_point_records";
	const static char ErrorVarLenRecordCnt[];	 // "number of variable Length Record is more than LasFileHead.Number_of_variable_length_records";
	const static char ErrorOffsettoData[];		 // "number of bytes of Variable Length Record is more than LasFileHead.Offset_to_data";
};

// �䳤��¼ͷ��
#pragma pack(1)
#define min_las_record_after_header 54;
class  LASVariableRecord
{
public:											// ����	����	Ĭ��ֵ
	LASVariableRecord() {
		reserved = 0xAABB;
		memset(user_id, 0, sizeof(char) * 16);
		record_id = 0;
		record_length_after_header = min_las_record_after_header;
		memset(description, 0, sizeof(char));
		record_buffer = NULL;
	}
	~LASVariableRecord()
	{
		if (record_buffer != NULL)
			delete[]record_buffer;
	}
	unsigned short reserved;					// -	����	0
	char user_id[16];							// *			LASF_Specl/LASF_Projection
	unsigned short record_id;					// *	���
	unsigned short record_length_after_header;	// *	������¼��С
	char description[32];						// -	����˵��
	unsigned char* record_buffer;

	void LASVariableRecord_Read(FILE* fs);
	void LASVariableRecord_Write(FILE* fs) const;

	//Two bytes after the last variable length record, and before the point data
	const static unsigned short Min_Record_Length_After_Header;  // 54
	const static short Point_Data_Start_Signature;
};

// �ٷ�����ı䳤��¼�ṹ
// User ID: LASF_Projection
// Record ID: 34735
class  LASVariable_header_geo_keys
{
public:
	unsigned short key_directory_version;
	unsigned short key_revision;
	unsigned short minor_revision;
	unsigned short number_of_keys;

	LASVariable_header_geo_keys()
	{
		key_directory_version = 1;	// Always
		key_revision = 1;	// Always
		minor_revision = 0;	// Always
	}
};
class  LASVariable_header_key_entry
{
public:
	unsigned short key_id;
	unsigned short tiff_tag_location;
	unsigned short count;
	unsigned short value_offset;
};



//=======================================================las xyz���ļ�========================================================
#ifndef COLORREF
#define COLORREF long
#endif

#ifndef RGB
#define RGB(r,g,b)          ((COLORREF)(((unsigned char)(r)|((unsigned long)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(b))<<16)))
#endif

typedef RTree<int, double, 2, double, 4>  LASBlockTree;

/*����ز��ź�*/
enum  eLASEcho
{
	eLidarEchoOnly = 0,
	eLidarEchoFirst = 1,
	eLidarEchoMidian = 2,
	eLidarEchoLast = 3
};
/*���Ƶ����*/
#pragma pack(1)
enum  eLASClassification
{
	elcCreated = 0,	// �����ģ�û�б������
	elcUnclassified = 1,	// �����ģ����޷�ʶ�����ĵ�
	elcGround = 2,	// �����
	elcLowVegetation = 3,	// ����ֲ��
	elcMediumVegetation = 4,	// �еȸ߶ȵ�ֲ��
	elcHighVegetation = 5,	// �ߵ�ֲ��
	elcBuilding = 6,	// ������
	elcLowPoint = 7,	// ���ڵر�ĵ㣨������
	elcModelKeyPoint = 8,	// ���Ƶ�
	elcWater = 9,	// ˮ
	elcOverlapPoint = 12,	// �����ص���
	elcDeletedPoint = -1	// ��ɾ���ĵ�
};
#pragma pack()
static eLASClassification GetLidarClassification(unsigned char clsType)
{
	return (eLASClassification)clsType;
}
/*Las1.2��ɫ��չ*/
struct LASColorExt
{
	unsigned short Red;
	unsigned short Green;
	unsigned short Blue;
};

/*����XYZ�ļ�*/
#pragma pack(1)
struct LAS_XYZ
{
	double x;
	double y;
	double z;
};

//las���ļ�
#pragma pack(1)/*�ֽڶ���*/
struct LASPoint
{
public:
	LAS_XYZ			m_vec3d;
	unsigned short  m_intensity;
	unsigned char   m_rnseByte;
	char			m_classify;
	char			m_scanAngle;
	unsigned char	m_userdata;
	unsigned short  m_flightID;
	double			m_gpsTime;
	LASColorExt		m_colorExt;
};

//las���ļ��������ļ��ֿ�õ�������
class LASRectangle
{
public:
	LASRectangle() { m_lasPoints = NULL; m_point_index = NULL; m_lasPoints_numbers = 0; }
	~LASRectangle(){
		if (m_lasPoints != NULL)
			delete[]m_lasPoints;
		m_lasPoints = NULL;
		m_lasPoints_numbers = 0;
	}

	//�����ڴ�
	void LASRect_AllocateMemory(int lasPoints,bool inMemory,Rect rect);
public:
	Rect m_Rectangle;
	LASPoint* m_lasPoints;
	long long m_lasPoints_numbers;
	int *m_point_index;
};

//�����las�ļ�
class LASSet
{
public:
	LASSet() { m_lasRectangles = NULL; m_lasBlockTree.RemoveAll(); m_numRectangles = 0; }
	~LASSet(){
		printf("�����ͷ��ڴ�...\n");
		if (m_lasRectangles != NULL)
			delete[]m_lasRectangles;
		m_lasRectangles = NULL;
		m_lasBlockTree.RemoveAll();
	}

	//�ҵ�ƥ��ľ��ε�id������id��ȡ���ĸ�������
	bool LASRect_Search(int rectID, Rect searchRect, vector<int> &rects);
	bool LASRect_Search(int rectID, LAS_XYZ searchPnt, vector<int> &rects);

	//����R���Ĺ���
	long LASRect_BuildTree();

	//�����ڴ�
	void LASRect_AllocateMemory(int lasRects, bool inMemory);

public:
	LASRectangle* m_lasRectangles;
	LASBlockTree  m_lasBlockTree;
	int			  m_numRectangles;
};