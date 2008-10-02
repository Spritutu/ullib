///\file ULToolBar.cpp
///\brief cpp ���� ������ �������(11.08.2007)
///\bug ���������� OnDropDown
#include "..\..\ULLib\Include\ULToolbar.h"
#include "..\..\ULLib\Include\ULMenu.h"
#include "..\..\ULLib\Include\ULRes.h"
namespace ULWnds
{
	namespace ULBars
	{
		CULToolBar::CULToolBar(void):CULSubClass(),
			m_dxBitmap(0),
			m_dyBitmap(0)
		{
			MessageMap.AddMessage<CULToolBar>(WM_WINDOWPOSCHANGING,&CULToolBar::OnWindowPosChanging);
			MessageMap.AddReflectNotify<CULToolBar>(TBN_DROPDOWN,&CULToolBar::OnDropDown);
		}
		CULToolBar::CULToolBar(CULToolBar& toolBar):CULSubClass(toolBar),
			m_dxBitmap(toolBar.m_dxBitmap),
			m_dyBitmap(toolBar.m_dyBitmap),
			m_TBToolTip(toolBar.m_TBToolTip),
			m_pInfoButtons(toolBar.m_pInfoButtons),
			m_afFlag(toolBar.m_afFlag),
			m_tbStyle(toolBar.m_tbStyle)
		{
		}

		CULToolBar::~CULToolBar(void)
		{
		}

		void CULToolBar::operator=(CULToolBar& toolBar)
		{
			m_dxBitmap=toolBar.m_dxBitmap;
			m_dyBitmap=toolBar.m_dyBitmap;
			m_TBToolTip=toolBar.m_TBToolTip;
			m_pInfoButtons=toolBar.m_pInfoButtons;
			m_afFlag=toolBar.m_afFlag;
			m_tbStyle=toolBar.m_tbStyle;
			ULWnds::CULSubClass::operator=(toolBar);
		}

		HWND CULToolBar::Create(HWND hParentWnd,
						UINT tbID,//ID �������
						int dxButton,//������ ������
						int dyButton,//������ ������
						int dxBitmap,//������ �������� ��� ������
						int dyBitmap,//������ �������� ��� ������
						UINT afFlag,
						DWORD tbStyle,
						DWORD tbExStyle)
		{
			m_tbStyle=tbStyle;
			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
			icex.dwICC  = ICC_BAR_CLASSES;
			InitCommonControlsEx(&icex);
		//==================================
			m_dxBitmap=dxBitmap;
			m_dyBitmap=dyBitmap;
			m_hParentWnd=hParentWnd;
			m_afFlag=afFlag;
			tbStyle|=afFlag;

			m_hWnd = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, 
				tbStyle, 0, 0, dxButton, dyButton, hParentWnd, 
				(HMENU)(INT_PTR)tbID, ULOther::ULGetResourceHandle(), NULL); 
			if(m_hWnd!=NULL)
			{
				SendMessage(TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof(TBBUTTON), 0); 
				SendMessage(TB_SETEXTENDEDSTYLE , 0, tbExStyle ); 
			}
			//���������� ������
			m_TBToolTip.m_pULToolBar=this;		
			m_TBToolTip.Attach((HWND)SendMessage(TB_GETTOOLTIPS));
			//������������� ������
			return (Attach(m_hWnd))?m_hWnd:NULL;
		};

		int CULToolBar::AddButton(int idCommand,
						BYTE btnState,
						BYTE btnStyle,
						TCHAR* szTTip,
						TCHAR* pszStr,
						int nBitmap,
						UINT_PTR nID,
						HINSTANCE hInst,
						HMENU hMenu)//����� ������������ ���� ��� ������(���� btnStyle==BTNS_DROPDOWN)
		{
		//========================���������� � ������ m_InfoButtons===============
			m_pInfoButtons.Resize(m_pInfoButtons.GetSize()+1);
			if((btnStyle&BTNS_SEP)==0)
			{
				if((btnStyle&BTNS_DROPDOWN)!=0)
					m_pInfoButtons[m_pInfoButtons.GetSize()-1].hMenu=hMenu;
				else
					m_pInfoButtons[m_pInfoButtons.GetSize()-1].hMenu=0;
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].szToolTip=szTTip;
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].idCommand=idCommand;
			}
			else
			{
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].hMenu=0;
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].szToolTip[0]=0;
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].idCommand=0;
			}
		//=========================================================================	
			btnState|=(m_afFlag&CCS_VERT)?TBSTATE_WRAP:0;
			TBBUTTON tbButtonsAdd={nBitmap, idCommand, 
				btnState,
				btnStyle,
		#if defined(_WIN32) | defined(_WIN64)
				{0},
		#endif
				/*dwData*/0, (INT_PTR)pszStr};
			TBADDBITMAP tb;
			int  stdidx;
			tb.hInst=(nID>IDB_HIST_LARGE_COLOR)?ULOther::ULGetResourceHandle():hInst;
			tb.nID = nID;

			if(nID>IDB_HIST_LARGE_COLOR)
			{
				HIMAGELIST himl=(HIMAGELIST)SendMessage(TB_GETIMAGELIST,0,0);
				stdidx=ImageList_AddMasked(himl,(HBITMAP)nID,RGB(192,192,192));
				SendMessage(TB_SETIMAGELIST,0,LPARAM(himl));
			}
			else
			{
				stdidx = (int)SendMessage(TB_ADDBITMAP, nBitmap, (LPARAM)&tb);
			}
			tbButtonsAdd.iBitmap +=stdidx;

			SendMessage(TB_SETBUTTONSIZE,0,(LPARAM)MAKELONG(m_dxBitmap,m_dyBitmap));
			SendMessage(TB_SETBITMAPSIZE,0,(LPARAM)MAKELONG(m_dxBitmap,m_dyBitmap));
			return (BOOL)SendMessage (TB_ADDBUTTONS,1,(LPARAM) &tbButtonsAdd);
		};

		int CULToolBar::AddButton(int idCommand,//ID ��������(������)
						BYTE btnState,//��������� ������
						BYTE btnStyle,//����� ������
						TCHAR* szTTip,//������ ��� ������
						TCHAR* pszStr,//������� ��� ������
						int nBitmap,//����� �������� ��� ������ ������� 
						HBITMAP hBitmap,//ID ������� ��� ��������
						HMENU hMenu)//ID ������������ ���� ��� ������(���� btnStyle==BTNS_DROPDOWN)
		{
			return AddButton(idCommand,btnState,btnStyle,szTTip,pszStr,nBitmap,(INT_PTR)hBitmap,
				NULL,hMenu);
		}

		int CULToolBar::InsertButton(int nInto,//���� ��������
						int idCommand,
						BYTE btnState,
						BYTE btnStyle,
						TCHAR* szTTip,
						TCHAR* pszStr,
						int nBitmap,
						UINT_PTR nID,
						HINSTANCE hInst,
						HMENU hMenu)
		{
		//========================������� � ������ m_InfoButtons===============
			ULOther::CULArr<tagInfoButtons> tmppInfoButtons(m_pInfoButtons.GetSize());
			m_pInfoButtons.Resize(m_pInfoButtons.GetSize()+1);
			if((btnStyle&BTNS_SEP)==0)
			{
				if((btnStyle&BTNS_DROPDOWN)!=0)
					m_pInfoButtons[nInto].hMenu=hMenu;
				m_pInfoButtons[nInto].szToolTip=szTTip;
				m_pInfoButtons[nInto].idCommand=idCommand;
			}
			else
			{
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].hMenu=0;
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].szToolTip[0]=0;
				m_pInfoButtons[m_pInfoButtons.GetSize()-1].idCommand=0;
			}
			memcpy(m_pInfoButtons+nInto+1,tmppInfoButtons+nInto,(m_pInfoButtons.GetSize()-nInto-1)*sizeof(tagInfoButtons));
		//===========================================================
			btnState|=(m_afFlag&CCS_VERT)?TBSTATE_WRAP:0;
			TBBUTTON tbButtonsAdd={nBitmap, idCommand, 
				btnState,
				btnStyle,
		#if defined(_WIN32) | defined(_WIN64)
				{0},
		#endif
				/*dwData*/0, (INT_PTR)pszStr};
			if((btnStyle&BTNS_SEP)==0)
			{
				TBADDBITMAP tb;
				int  stdidx;
				tb.hInst=(nID>IDB_HIST_LARGE_COLOR)?ULOther::ULGetResourceHandle():hInst;
				tb.nID = nID;

				if(nID>IDB_HIST_LARGE_COLOR)
				{
					HIMAGELIST himl=(HIMAGELIST)SendMessage(TB_GETIMAGELIST,0,0);
					stdidx=ImageList_AddMasked(himl,(HBITMAP)nID,RGB(192,192,192));
					SendMessage(TB_SETIMAGELIST,0,LPARAM(himl));
				}
				else
				{
					stdidx = (int)SendMessage (TB_ADDBITMAP, nBitmap, (LPARAM)&tb);
				}
				tbButtonsAdd.iBitmap +=stdidx;
			}
			SendMessage(TB_SETBUTTONSIZE,0,(LPARAM)MAKELONG(m_dxBitmap,m_dyBitmap));
			SendMessage(TB_SETBITMAPSIZE,0,(LPARAM)MAKELONG(m_dxBitmap,m_dyBitmap));
			SendMessage(TB_INSERTBUTTON,nInto,LPARAM(&tbButtonsAdd));
			return 0;
		};

		int CULToolBar::InsertButton(int nInto,//���� ��������
						int idCommand,
						BYTE btnState,
						BYTE btnStyle,
						TCHAR* szTTip,
						TCHAR* pszStr,
						int nBitmap,//����� �������� ��� ������ ������� 
						HBITMAP hBitmap,//ID ������� ��� ��������
						HMENU hMenu)
		{
			return InsertButton(nInto,idCommand,btnState,btnStyle,szTTip,pszStr,nBitmap,
				(INT_PTR)hBitmap,NULL,hMenu);
		}

		int CULToolBar::DeleteButton(int nButton)
		{
		//========================�������� �� ������� m_InfoButtons===============
			ULOther::CULArr<tagInfoButtons> tmppInfoButtons(m_pInfoButtons.GetSize());
			memcpy(tmppInfoButtons,m_pInfoButtons,(m_pInfoButtons.GetSize())*sizeof(tagInfoButtons));
			m_pInfoButtons.Resize(m_pInfoButtons.GetSize()-1);
			int counter=0;
			for(int i=0;i<signed(m_pInfoButtons.GetSize()+1);i++)
				if(i!=nButton)
				{
					memcpy(m_pInfoButtons+counter,tmppInfoButtons+i,sizeof(tagInfoButtons));
					counter++;
				};
		//=========================================================================	
			SendMessage(TB_DELETEBUTTON,nButton,0);
			return 0;
		};


		void CULToolBar::AutoSize()//�������� � ���������� WM_SIZE ���� ��������
		{
			if((m_afFlag&CCS_VERT)!=CCS_VERT)
				SendMessage(TB_AUTOSIZE); 
		};

		LRESULT CULToolBar::OnWindowPosChanging(WPARAM,LPARAM lParam)
		{
			if((m_tbStyle&TBSTYLE_AUTOSIZE)==0)
			{
				WINDOWPOS* pWP=(WINDOWPOS*)lParam;
				RECT rcTBWidth;
				SendMessage(TB_GETITEMRECT,
					GetButtonCount()-1,(LPARAM)&rcTBWidth);
				if((pWP->cx>rcTBWidth.right))
						pWP->cx=rcTBWidth.right;
		}
			return FALSE;
		};

		LRESULT CULToolBar::OnDropDown(LPARAM lParam)
		{
			#define lpnm   ((LPNMHDR)lParam)
			#define lpnmTB ((LPNMTOOLBAR)lParam)
			RECT rc;
			TPMPARAMS tpm;
			::SendMessage(lpnmTB->hdr.hwndFrom, TB_GETRECT,
				(WPARAM)lpnmTB->iItem, (LPARAM)&rc);
				MapWindowPoints(lpnmTB->hdr.hwndFrom,
				HWND_DESKTOP, (LPPOINT)&rc, 2);                         
			tpm.cbSize = sizeof(TPMPARAMS);
			tpm.rcExclude = rc;
			for(unsigned int i=0;i<m_pInfoButtons.GetSize();i++)
				if(lpnmTB->iItem==(signed)m_pInfoButtons[i].idCommand)
				{
					ULControls::CULMenu PopupMenu(m_pInfoButtons[i].hMenu);
					POINT pt={rc.left, rc.bottom};
					PopupMenu.TrackPopupMenuEx(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,               
						pt, m_hParentWnd, &tpm); 	
					PopupMenu.Detach();
				}
			return TBDDRET_DEFAULT;
		}
		CULToolBar::CTBToolTip::CTBToolTip():ULControls::CULToolTip()
		{
			MessageMap.AddReflectNotify<CTBToolTip>(TTN_GETDISPINFO,&CTBToolTip::OnGetDispInfo);
		};

		CULToolBar::CTBToolTip::~CTBToolTip(){};
		LONG CULToolBar::CTBToolTip::OnGetDispInfo(LPARAM lParam)
		{
			#define lpnm   ((LPNMHDR)lParam)
			#define lpttt ((LPTOOLTIPTEXT)lParam)
			lpttt->hinst = ULOther::ULGetResourceHandle(); 
			UINT_PTR idButton = lpttt->hdr.idFrom; 
			for(unsigned int i=0;i<m_pULToolBar->m_pInfoButtons.GetSize();i++)
				if(idButton==m_pULToolBar->m_pInfoButtons[i].idCommand)
				{
					lpttt->lpszText = m_pULToolBar->m_pInfoButtons[i].szToolTip;
					break;
				}
			return TRUE;
		};
	}
}