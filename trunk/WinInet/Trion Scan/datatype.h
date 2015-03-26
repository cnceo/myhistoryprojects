#pragma once
#include <afxmt.h>
#include <afxtempl.h>
#include <map>
using namespace std;
#define BMPFILE_PATH "checkcodenew.bmp"
#include "RasDialMgr.h"
typedef struct _RegisterPack
{
	CString strAcc;		//��������
	CString strPwd;		//����ʱ��
	CString strMail;	//��Ϣ���
	int iStatus;
	CString strMemo;
	CString strIntroMan;
	CString strNickName;
	CString strIp;
	_RegisterPack()
	{
		strAcc.Empty();
		strPwd.Empty();
		strMail.Empty();
		iStatus = 0;	//0δע��,1�ɹ�,2ʧ��
		strMemo.Empty();
		strIntroMan.Empty();
		strNickName.Empty();
		strIp.Empty();
	}
}RegisterPack;

typedef CArray<RegisterPack, RegisterPack> RegisterPackList;

extern HANDLE  g_hEvent;			//�¼����
extern CString g_strCheckCode;
extern bool g_ExitFlag;
extern RegisterPackList g_RegisterList;
extern CListCtrl g_AccList;
extern int g_iDeleteFlag;
extern int g_iShowWidth;
extern int g_iShowHeight;
extern int g_iAotuChangeIp;
extern int g_iAotuCheckCode;
extern int g_iIpRegNum;
extern int g_iIpChangeType;
extern int g_iCheckIpFlag;		//0�����IP�Ƿ�仯��1ͨ��RAS���IP�仯��2ͨ��POST��ʽ���
extern int g_iReCheckIpCount;
extern int g_iTimeOut;
extern int g_iCheckCodeType;
extern int g_iCodeErrorNum;
extern CRasDialMgr g_RasMgr;
extern CString g_strCurrIP;
extern CString g_strDirect;		//ͼ�񱣴�Ŀ¼
extern CString g_strFileName;
extern int g_iRegWebFlag;
extern int g_iIsChangeMac;
extern int g_iRedialStop;
extern int GetRegisterPack(RegisterPack &submitPack);
extern void AddRegisterPack(RegisterPack &submitPack);
extern void AddRegisterPackEx(RegisterPack &submitPack);
extern void WriteLog(CString srcStr,int iFlag);
extern void DeleteRegisterPack(RegisterPack &rgPack);
extern void AddToShowList(RegisterPack rgPack,int iLastFlag);
extern void ShowBmp(CString srcPath,HDC hdc);


typedef struct _VpnInfoStr
{
	CString strServer;
	CString strDes;
	CString strUser;
	CString strPwd;
}VpnInfoStr;
extern CArray<VpnInfoStr,VpnInfoStr> gl_ProxyStruList;
extern HWND gl_hwnd;
extern map<CString,int> gl_mapOldAcc;