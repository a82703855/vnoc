#pragma once
#include "../../BKWin/wtlhelper/whwindow.h"
#include "../../net/INet.h"
#include "../FrameBase.hpp"

struct ColumnData
{
	ColumnData()
		: strCRoomOrder("�����б�"), strCTeacher("��ʦ"),
		  strCPeople("��������"), strCTime("�ڿ�ʱ��")	
	{}

	CString strCRoomOrder, strCTeacher, strCPeople, strCTime;
};

struct ListItemData
{
	ListItemData()
		: strIRoomOrder("NULL"), strITeacher("NULL"),
		  strIPeople("NULL"), strITime("NULL")	
	{}

	CString strIRoomOrder, strITeacher, strIPeople, strITime;
};


class CRoomListWnd : public CBkDialogImpl<CRoomListWnd>
	,public CFrameBase
{
public:
	CRoomListWnd::CRoomListWnd() : CBkDialogImpl<CRoomListWnd>(BKDlg_RoomListWin),CFrameBase(module_RoomListWin)
	{
	}

public:
	void	OnBkBtnClose();
	VOID	OnShowWnd(XMessage* pmsg);
	LRESULT OnInitDialog(HWND hWnd, LPARAM lparam);
	LRESULT OnListItemClick(int idRealWnd, LPNMHDR pnmh, BOOL& bHandled);

protected:	
	BOOL	ColumnInit(ColumnData* pColData);
	int		AppendItem(ListItemData* pItemData);

protected:
	CListViewCtrlEx m_wndListCtrl;

private:

public:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(DlgControl_RoomListWin_ImgBtn_Close,	OnBkBtnClose)
	BK_NOTIFY_MAP_END()
	
	BEGIN_MSG_MAP_EX_DECLARE(CRoomListWnd)
		Begin_XMessage(CRoomListWnd)
		OnXMessage(XMessage_ShowRoomList,OnShowWnd)
	End_XMessage()
};