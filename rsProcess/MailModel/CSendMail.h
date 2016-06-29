#pragma once  
#include <winsock2.h>
#include<stdio.h>
using namespace std;
struct SMailInfo //�ʼ���Ϣ  
{
	char*   m_pcUserName;//�û���¼���������  
	char*   m_pcUserPassWord;//�û���¼���������  
	char*   m_pcSenderName;//�û�����ʱ��ʾ������  
	char*   m_pcSender;//�����ߵ������ַ  
	char*   m_pcReceiver;//�����ߵ������ַ  
	char*   m_pcTitle;//�������  
	char*   m_pcBody;//�ʼ��ı�����  
	char*   m_pcIPAddr;//��������IP  
	char*   m_pcIPName;//�����������ƣ�IP�����ƶ�ѡһ������ȡ���ƣ�  
	SMailInfo() { memset(this, 0, sizeof(SMailInfo)); }
};
class CSendMail
{
public:
	CSendMail(void);
	~CSendMail(void);

public:
	bool SendMail(SMailInfo &smailInfo);//�����ʼ�����Ҫ�ڷ��͵�ʱ���ʼ���ʼ���Ϣ  
protected:
	void Char2Base64(unsigned char* pBuff64, unsigned char* pSrcBuff, int iLen);//��char����ת����Base64����  
	bool  CReateSocket(SOCKET &sock);//����socket����  
	bool Logon(SOCKET &sock);//��¼���䣬��Ҫ���з��ʼ�ǰ��׼������  

	bool SendHead(SOCKET &sock);//�����ʼ�ͷ  
	bool SendTextBody(SOCKET &sock);//�����ʼ��ı�����  
	bool SendEnd(SOCKET &sock);//�����ʼ���β  
protected:
	SMailInfo m_sMailInfo;
	char m_cReceiveBuff[1024];
	char  m_cSendBuff[4096];//���ͻ�����  
};
