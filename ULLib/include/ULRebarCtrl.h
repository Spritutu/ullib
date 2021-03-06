///\file ULRebarCtrl.h
///\brief ������������ ���� ������ ������ ������������ �� ��������� ��������(31.08.2008)
#pragma once
#ifndef __UL_ULREBARCTRL_H__
#define __UL_ULREBARCTRL_H__
#include "ULRebar.h"
#include "ULWndCtrl.h"
namespace ULWnds
{
	namespace ULWndCtrls
	{
		///\class CULRebarCtrl
		///\brief ����� ������ ������������ �� ��������� ��������(31.08.2008)
		class CULRebarCtrl :
			public CULWndCtrl
		{
		private:
			bool m_fAutoSize;
		protected:
			///\brief ��������������� ��� ������
			ULBars::CULRebar m_Rebar;
			///\brief ������� ����
			static LRESULT ClientWndProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam);
			///\brief ������� ���� ����������� ��� ���������������� ��������� ����������
			WNDPROC m_lpSubClassWndProc;
		public:
			//================================================
			///\brief �����������
			//================================================
			CULRebarCtrl(void);
				///\brief ����������� �����������
			CULRebarCtrl(CULRebarCtrl&);
			///\brief ����������
			virtual ~CULRebarCtrl(void);
			///\brief �������� �����������
			void operator=(CULRebarCtrl&);
			///\brief ������� ���������� ������ �� ������
			inline ULBars::CULRebar& GetRebar(){return m_Rebar;};
			///\brief ������� �������� ��������������
			///\param hParentWnd - ����� ��������(��������)
			///\param nXPos,nYPos - ������� �������������� ��� �������� �
			///	��������� ���������
			///\param dwDockedState - ���������� �������� �� ������ ��������
			BOOL CreateRebarCtrl(HWND hParentWnd,
							int nXPos,
							int nYPos,
							DWORD dwDockedState);
		protected:
			///\brief ���������� WM_SIZE
			virtual LRESULT OnSize(WPARAM nType,LPARAM point);
		};
	}
} 
#endif //__UL_ULREBARCTRL_H__
