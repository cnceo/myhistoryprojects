#include "stdafx.h"
#include "MainDialog.h"
#include <Commdlg.h>
#include <shellapi.h>

bool g_bUpdateDir = false;


void MainDialog::Init()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
    m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
    m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
    m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
	m_pClose = static_cast<CButtonUI*>(m_pm.FindControl(_T("close")));
	CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("tree")));
	CControlUI* pControl = m_pm.FindControl(_T("drag"));
	pTree->m_pDragingCtrl = pControl;

	CTreeNodeUI* pNodeRoot = new CTreeNodeUI;
	pNodeRoot->SetAttribute(_T("folderattr"),_T("padding=\"0,1,0,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16'\" hotimage=\"file='treeview_b.png' source='36,0,72,16'\" selectedimage=\"file='treeview_a.png' source='0,0,36,16' selectedhotimage=\"file='treeview_a.png' source='36,0,72,16'\""));
	pTree->Add(pNodeRoot);
	CDuiString strNode = "";
	strNode.Format("根节点");
	pNodeRoot->SetItemText(strNode);
	pNodeRoot->SetExpand(true);
	for (int i = 1; i < 7; i++)
	{
		int nn = pNodeRoot->GetCountChild();
		CTreeNodeUI* pNodeParent = new CTreeNodeUI;
		pNodeRoot->AddChildNode(pNodeParent);
		pNodeParent->SetName(strNode);
		pNodeParent->SetAttribute(_T("folderattr"),_T("padding=\"0,1,0,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16'\" hotimage=\"file='treeview_b.png' source='36,0,72,16'\" selectedimage=\"file='treeview_a.png' source='0,0,36,16' selectedhotimage=\"file='treeview_a.png' source='36,0,72,16'\""));
		CDuiString strNode = "";
		strNode.Format("%d", i);
		pNodeParent->SetItemText(strNode);
		pNodeParent->SetName(strNode);
		pNodeParent->SetVisibleTag(false);
		for (int j = 1; j< 7; j++)
		{
			CTreeNodeUI* pNodeChild = new CTreeNodeUI(true);	//表示是叶子节点 
			CDuiString strNode = "";
			strNode.Format("%d_%d", i, j);
			pNodeChild->SetItemText(strNode);
			pNodeChild->SetName(strNode);
			pNodeParent->AddChildNode(pNodeChild);
			
		}
	}	
}
void MainDialog::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) 
		OnPrepare(msg);
	else if( msg.sType == _T("click") ) 
	{		
		if( msg.pSender == m_pCloseBtn ) 
		{
			PostQuitMessage(0);
			return; 
		}
		else if( msg.pSender == m_pMinBtn ) 
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return; 
		}
		else if( msg.pSender == m_pMaxBtn ) 
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; 
		}
		else if( msg.pSender == m_pRestoreBtn ) 
		{ 
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; 
		}
		else if (msg.pSender->GetName() == _T("change"))
		{
			CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("tree")));
			CTreeNodeUI* dstParent = (CTreeNodeUI*)pTree->GetItemAt(1);
			//::MessageBox(NULL, dstParent->GetItemText(), "", 0);
			CTreeNodeUI* pNode = (CTreeNodeUI*)pTree->GetItemAt(6);
			//::MessageBox(NULL, pNode->GetItemText(), "", 0);
			pTree->Move(dstParent, pNode);
		}
		else if (msg.pSender->GetName() == _T("change1"))
		{
			RECT rt;
			rt.left=msg.ptMouse.x;
			rt.top = msg.ptMouse.y - 60;
			rt.right = rt.left + 70;
			rt.bottom = rt.top + 20;

			
			/*
			CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("tree")));
			CTreeNodeUI* pNodeRoot = (CTreeNodeUI*)pTree->GetItemAt(0);

			CTreeNodeUI* pNode4 = pNodeRoot->GetChildNode(2);
			CTreeNodeUI* dstParent = pNodeRoot->GetChildNode(0);
			pTree->Move(dstParent, pNode4);

			*/

		}
		else if (msg.pSender == m_pClose)
		{
			
		}
	}
	
	else if(msg.sType==_T("headerclick"))
	{
		CControlUI* pHeader = static_cast<CControlUI*>(msg.pSender);
		CListUI* pList = static_cast<CListUI*>(pHeader->GetParent()->GetParent());
		
		CListBodyUI* pBody = (CListBodyUI*)pList->GetList();
//		pBody->SortItems(pList->GetItemIndex(pHeader));
		//MessageBox(NULL,,_T("test"),MB_OK);
	}
	else if( msg.sType == _T("itemexpand") ) 
	{
		CTreeNodeUI* pNowNode = static_cast<CTreeNodeUI*>(msg.pSender);
		pNowNode->Select(true);
		
		CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_pm.FindControl(_T("tree")));
		CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("file_list")));
		g_bUpdateDir = false;
		if (!pNowNode->IsHasChild())
		{
			g_bUpdateDir = true;
		}

		int nnn = pNowNode->GetCountChild();
// 		for (int i = 0; i< nnn; i++)
// 		{
// 			pNowNode->GetItemAt(i)->SetVisible(!pNowNode->GetItemAt(i)->IsVisible());
// 			break;
// 		}

		//pList->RemoveAll();
	
		WIN32_FIND_DATA FindFileData;
		HANDLE hListFile;
		TCHAR szFilePath[MAX_PATH];


// 
// 		lstrcpy(szFilePath,pNowNode->GetUserData());
// 		lstrcat(szFilePath, _T("\\*"));
// 
// 		hListFile = FindFirstFile(szFilePath,&FindFileData);
// 
// 		if(hListFile==INVALID_HANDLE_VALUE)
// 		{
// 			return;
// 		}
// 		else
// 		{        
// 			do
// 			{
// 				if(lstrcmp(FindFileData.cFileName,TEXT("."))==0||
// 					lstrcmp(FindFileData.cFileName,TEXT(".."))==0)
// 				{
// 					continue;
// 				}
// 	
// 
// 
// 				SYSTEMTIME  st;
// 				FileTimeToSystemTime(&FindFileData.ftLastWriteTime,&st);
// 				DWORDLONG dwlSize = (DWORDLONG)(FindFileData.nFileSizeHigh * (MAXDWORD+1)) + FindFileData.nFileSizeLow;
// 				DWORD dwSize = (DWORD)(dwlSize / 1024);
// 
// 				if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
// 				{	
// 					if (g_bUpdateDir)
// 					{
// 						//MessageBox(NULL,FindFileData.cFileName,L"test",MB_OK);
// 						CTreeNodeUI* pNode = new CTreeNodeUI;
// 						pNode->SetAttribute(_T("folderattr"),_T("padding=\"0,1,0,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16'\" hotimage=\"file='treeview_b.png' source='36,0,72,16'\" selectedimage=\"file='treeview_a.png' source='0,0,36,16' selectedhotimage=\"file='treeview_a.png' source='36,0,72,16'\""));
// 						pNowNode->AddChildNode(pNode);
// 						pNode->SetItemText(FindFileData.cFileName);
// 						CDuiString szUserData = pNowNode->GetUserData() + _T("\\") + pNode->GetItemText();
// 						pNode->SetUserData(szUserData);
// 					}
// 					
// 				}
// 				else
// 				{
// 					CListTextElementUI* pLine = new CListTextElementUI;
// 					pList->Add(pLine);
// 					CDuiString szNewName(FindFileData.cFileName);
// 					
// 					int pos = szNewName.ReverseFind('.');
// 					CDuiString szExt = szNewName.Right(szNewName.GetLength() - pos);
// 					SHFILEINFO shfi;
// 					memset(&shfi,0,sizeof(shfi));
// 					SHGetFileInfo(szExt,
// 						FILE_ATTRIBUTE_NORMAL,
// 						&shfi,
// 						sizeof(shfi),
// 						SHGFI_ICON|SHGFI_USEFILEATTRIBUTES);
// 
// 					pLine->SetIcon(0,shfi.hIcon);
// 				
// 					pLine->SetText(0,szNewName);
// 
// 					SYSTEMTIME  st;
// 					FileTimeToSystemTime(&FindFileData.ftLastWriteTime,&st);
// 					CDuiString szTime;
// 					szTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
// 					pLine->SetText(1,szTime);
// 
// 					DWORDLONG dwlSize = (DWORDLONG)(FindFileData.nFileSizeHigh * (MAXDWORD+1)) + FindFileData.nFileSizeLow;
// 					CDuiString szSize;
// 					if (dwlSize <1024)
// 					{
// 						szSize.Format(_T("%ldB"),dwlSize);
// 					}
// 					else
// 					{
// 						szSize.Format(_T("%ldK"),dwlSize/1024);
// 					}
// 
// 					pLine->SetText(2,szSize);
// 
// 					CDuiString szArrtib;
// 					szArrtib.Format(_T("%d"),FindFileData.dwFileAttributes);
// 					pLine->SetText(3,szArrtib);
// 					
// 				}
// 			}
// 			while(FindNextFile(hListFile, &FindFileData));
// 		
// 		}			
// 	

	}
	
}

LRESULT MainDialog::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return FALSE;

}
LRESULT MainDialog::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	//--
	
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(4);
	m_WndShadow.SetPosition(0, 0);
	//--
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0, this, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	Init();
	return 0;
}
LRESULT MainDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}
LRESULT MainDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);
	bHandled = FALSE;
	return 0;
}
LRESULT MainDialog::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}
LRESULT MainDialog::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
LRESULT MainDialog::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
LRESULT MainDialog::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(*this, &pt);

    RECT rcClient;
    ::GetClientRect(*this, &rcClient);
	
    if( !::IsZoomed(*this) ) {
        RECT rcSizeBox = m_pm.GetSizeBox();
        if( pt.y < rcClient.top + rcSizeBox.top ) {
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
            return HTTOP;
        }
        else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
            return HTBOTTOM;
        }
        if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
        if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
    }
	
    RECT rcCaption = m_pm.GetCaptionRect();
    if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
        && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
	{
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
            if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
                _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
                _tcscmp(pControl->GetClass(), _T("TextUI")) != 0
				)
                return HTCAPTION;
    }

    return HTCLIENT;
}
LRESULT MainDialog::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		HRGN hRgn = ::CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right + 1, rcClient.bottom + 1, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}
LRESULT MainDialog::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-rcWork.left, -rcWork.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT MainDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( wParam == SC_CLOSE )
	{
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = __super::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed )
	{
		if( !bZoomed )
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}
	return lRes;
}

LRESULT MainDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) 
	{
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		
		default:
			bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}