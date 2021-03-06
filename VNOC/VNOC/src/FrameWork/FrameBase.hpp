#pragma once
#include "IFrameWork.h"
#include <atlcomcli.h>
#include <list>
#include <vector>

#define HR_NOTPROCESSED	MAKE_HRESULT(1,0x590c,0x590c)

struct XMessage : public IRefCountImpl
{
	virtual VOID GetID(CString& id) = 0;

	XMessage()
		: destModule(module_Any)
		, srcModule(module_Any)
		, result(HR_NOTPROCESSED)
	{
	}

	VOID MarkProcessed(HRESULT result)
	{
		ATLASSERT(result != HR_NOTPROCESSED);
		this->result = result;
	}
	BOOL IsProcessed()
	{
		return result != HR_NOTPROCESSED;
	}

	FrameModule	destModule;
	FrameModule srcModule;

	HRESULT	result;
};

class CFrameBase : public IFrameModule
{
public:
	CFrameBase(FrameModule module)
		: m_moduleType(module)
	{
	}
	virtual ~CFrameBase()
	{
	}
	VOID SendXMessage(XMessage* msg)
	{
		ATLASSERT(msg);
		CComPtr<IFrameWork> pFrameWork;
		Global->GetIFrameModule(&pFrameWork);
		Global->PtrAssert(pFrameWork);
		pFrameWork->SendXMessage(msg);
	}
	FrameModule GetModuleType()
	{
		return m_moduleType;
	}
	virtual BOOL IsView()
	{
		return GetModuleType()>module_ViewBegin && GetModuleType()<module_ViewEnd;
	}
	STDMETHOD( Initialize(IModule* UpperFrame=NULL){return S_OK;} );
	STDMETHOD( UnInitialize(){return S_OK;} );
	STDMETHOD( Run(){return S_OK;} );
	STDMETHOD( Terminate(){return S_OK;} );
protected:
	FrameModule	m_moduleType;
};

#define CarelessXMessage(_class) \
	virtual VOID _class::ProcessXMessage(XMessage* pmsg,XMessage* pMsgGetList=NULL) \
	{}

#define Begin_XMessage(_class)	\
	virtual VOID _class::ProcessXMessage(XMessage* pmsg,XMessage* pMsgGetList=NULL) \
	{ \
		CString msgID; \
		XMessage_GetListenList* pListMsg=NULL; \
		if (!pMsgGetList) \
		{ \
			if (!pmsg) \
				return; \
			pmsg->GetID(msgID); \
		} \
		else \
		{ \
			pListMsg = dynamic_cast<XMessage_GetListenList*>(pMsgGetList); \
			if (!pListMsg) \
			{ \
				return; \
			} \
		} \

#define OnXMessage(_msg,_proc)	\
		if (pListMsg) \
		{ \
			CString id; \
			_msg::Static_GetID(id); \
			pListMsg->msgIDList.push_back(id); \
		} \
		if ( pmsg && _msg::Static_IsTypeOf(msgID) ) \
		{ \
			_proc((_msg*)pmsg); \
		}

#define Chain_XMessage(_class)	\
		{} \
		{ \
			_class::ProcessXMessage(pmsg); \
		}

#define End_XMessage()	 \
		{} \
	}

#define ImTheMessageOf(_msg)	\
public: \
	virtual VOID GetID(CString& id) \
	{ \
		Static_GetID(id); \
	} \
	static VOID Static_GetID(CString& id) \
	{ \
		id = #_msg; \
	} \
	static BOOL Static_IsTypeOf(LPCTSTR id) \
	{ \
		CString tempID; \
		Static_GetID(tempID); \
		return (tempID.CompareNoCase(id)==0)? TRUE: FALSE; \
	}

//////////////////////////////////////////////////////////////////////////
// Messages

// 特殊消息，获取模块所监听的消息列表
struct XMessage_GetListenList : public XMessage
{
	ImTheMessageOf(XMessage_GetListenList);

	std::list<CString>	msgIDList;
};

//////////////////////////////////////////////////////////////////////////
// 登陆相关

struct XMessage_Login : public XMessage
{
	ImTheMessageOf(XMessage_Login);

	CString username;
	CString pwd;
};

struct XMessage_LoginError : public XMessage
{
	ImTheMessageOf(XMessage_LoginError);
	
	ResultCode result;
};

struct XMessage_Login_Result : public XMessage
{
	ImTheMessageOf(XMessage_Login_Result);

	BOOL success;
	DWORD	userToken;
	BYTE	guid[16];
};

struct XMessage_ShowLogin : public XMessage
{
	ImTheMessageOf(XMessage_ShowLogin);
};

//////////////////////////////////////////////////////////////////////////
// 房间相关

struct XMessage_ShowRoomListWnd : public XMessage
{
	ImTheMessageOf(XMessage_ShowRoomListWnd);
};


struct XMessage_ShowRoomListWnd_Result : public XMessage
{
    ImTheMessageOf(XMessage_ShowRoomListWnd_Result);
};

struct XMessage_GetRoomList : public XMessage
{
    ImTheMessageOf(XMessage_GetRoomList);
};

struct XMessage_GetRoomList_Result : public XMessage
{
    ImTheMessageOf(XMessage_GetRoomList_Result);

    std::vector<int> roomID;
};

struct XMessage_ShowClassroom : public XMessage
{
	ImTheMessageOf(XMessage_ShowClassroom);
};

//////////////////////////////////////////////////////////////////////////
// Framework

struct XMessage_FrameStart : public XMessage
{
	ImTheMessageOf(XMessage_FrameStart);
};

struct XMessage_StartTest : public XMessage
{
	ImTheMessageOf(XMessage_StartTest);
};