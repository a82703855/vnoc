#include "StdAfx.h"
#include "LoginImpl.h"
#include "../../FrameWork/Dialogs/BKWinLogin.h"
#include "../../FrameWork/Dialogs/BKWinRoomList.h"

CLoginImpl::CLoginImpl(void) : CFrameBase(module_LoginData)
{
	m_frame = NULL;
}

CLoginImpl::~CLoginImpl(void)
{
}

HRESULT CLoginImpl::Run()
{
	netHelper.AddFilter(MSG_ALI_TYPE,this);
	netHelper.StartListen();
	return S_OK;
}

HRESULT CLoginImpl::Terminate()
{
	netHelper.StopListen();
	return S_OK;
}

HRESULT CLoginImpl::SetAccount( LPCTSTR userName )
{
	return S_OK;
}

HRESULT CLoginImpl::Initialize( IModule* UpperFrame )
{
	m_frame = dynamic_cast<IFrameWork*>(UpperFrame);
	ATLASSERT(m_frame);
	m_frame->AddRef();
	return S_OK;
}

HRESULT CLoginImpl::UnInitialize()
{
	if (m_frame)
	{
		m_frame->Release();
	}
	return S_OK;
}

HRESULT CLoginImpl::Logout( LPCTSTR username/*=NULL*/ )
{
	return S_OK;
}

HRESULT CLoginImpl::GetCurrentUser(CString& username,CString& cookie)
{
	return E_NOTIMPL;
}

VOID CLoginImpl::OnLogin( XMessage_Login* pMsg )
{
	if (CheckLoginRequest(pMsg))
	{
		Login(pMsg->username,pMsg->pwd);
	}
}

BOOL CLoginImpl::CheckLoginRequest( XMessage_Login* pMsg )
{
	CString& userName = pMsg->username;
	CString& pwd = pMsg->pwd;
	XMessage_LoginError error;
	if (userName.GetLength() == 0)
	{
		error.result = Result_UserName_LengthError;
	}
	else if (userName.Find(_T("sparta")))
	{
		error.result = Result_UserName_InvalidChar;
	}
	else if (pwd.GetLength() == 0)
	{
		error.result = Result_Pwd_LengthError;
	}

	SendXMessage(&error);
	return ResultSucceeded(error.result);
}

HRESULT CLoginImpl::Login( LPCTSTR username,LPCTSTR pwd )
{
	INetCenter* pNetCenter = NULL;
	Global->GetINetCenter(&pNetCenter);
	if (pNetCenter)
	{
		MSG_RLI netMsg;
		netMsg.SetAccountNumber((const byte*)username,_tcslen(username)*sizeof(TCHAR));
		netMsg.SetPassword((const byte*)pwd,_tcslen(pwd)*sizeof(TCHAR));
		pNetCenter->SendServer(netMsg);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CLoginImpl::OnNetMessage( const CMessage& msg )
{
	switch(msg.GetMessageType())
	{
	case MSG_ALI_TYPE:
		const MSG_ALI* msgReal = dynamic_cast<const MSG_ALI*>(&msg);
		if (msgReal)
		{
			XMessage_Login_Result loginResult;
			loginResult.success = msgReal->GetLoginResult()?FALSE:TRUE;
			loginResult.userToken = msgReal->GetToken();
			BYTE* pGuid = (byte *)msgReal->GetATLGUID();
			if (pGuid && 
				msgReal->GetATLGUIDLen()==sizeof(loginResult.guid)/sizeof(loginResult.guid[0]))
			{
				memcpy(loginResult.guid,pGuid,sizeof(loginResult.guid)/sizeof(loginResult.guid[0]));
			}
			SendXMessage(&loginResult);
		}
		break;
	}
	return S_OK;
}

