#include "stdafx.h"
#include <string>
#include "ConnInfoSubWhd.h"
#include "ConnInfoUI.h"
#include "CharacterSet.h"
#include "UserMessage.h"


DUI_BEGIN_MESSAGE_MAP(ConnInfoSubWhd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_END_MESSAGE_MAP()

ConnInfoSubWhd::ConnInfoSubWhd(TDicTables* pDic,OperatorType type, int celIdx)
{
	ASSERT(pDic);
	m_pdicServerInfo = pDic;
	m_celIdx = celIdx;
	m_type = type;
}

ConnInfoSubWhd::~ConnInfoSubWhd(void)
{
}

void ConnInfoSubWhd::InitWindow()
{
	TDicTables& dicObj = *m_pdicServerInfo;
	m_pEditName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_server_name")));
	m_pEditHost = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_server_ip")));
	m_pEditPort = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_server_port")));
	m_pEditAuth = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edt_server_passwd")));
	CLabelUI* pLabelAdd =  static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_conn_add")));
	CLabelUI* pLabelAlt =  static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_conn_alt")));
	if (m_type == Type_Add)
	{
		pLabelAdd->SetVisible(true);
		pLabelAlt->SetVisible(false);
	}
	else 
	{
		m_pEditName->SetText(CharacterSet::ANSIToUnicode(dicObj[ConnInfoUI::kServerNameIndex][m_celIdx]).c_str());
		m_pEditHost->SetText(CharacterSet::ANSIToUnicode(dicObj[ConnInfoUI::kServerIpIndex][m_celIdx]).c_str());
		m_pEditPort->SetText(CharacterSet::ANSIToUnicode(dicObj[ConnInfoUI::kServerPortIndex][m_celIdx]).c_str());
		m_pEditAuth->SetText(CharacterSet::ANSIToUnicode(dicObj[ConnInfoUI::kServerAuthIndex][m_celIdx]).c_str());
		pLabelAdd->SetVisible(false);
		pLabelAlt->SetVisible(true);
	}
}



LPCTSTR ConnInfoSubWhd::GetWindowClassName() const
{
	return _T("UIConnInfoSubWhd");
}


UINT ConnInfoSubWhd::GetClassStyle() const
{
	return UI_CLASSSTYLE_DIALOG;
}


void ConnInfoSubWhd::OnFinalMessage( HWND hWnd)
{
	delete this;
}


DuiLib::UILIB_RESOURCETYPE ConnInfoSubWhd::GetResourceType() const
{
	return UILIB_FILE;
}

DuiLib::CDuiString ConnInfoSubWhd::GetSkinFolder()
{
	return _T("skin\\");
}

DuiLib::CDuiString ConnInfoSubWhd::GetSkinFile()
{
	return _T("ConnSubInfo.xml");
}

LRESULT ConnInfoSubWhd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

void ConnInfoSubWhd::OnClick( TNotifyUI& msg )
{
	if(msg.pSender->GetName() == _T("btn_conn_cancel"))
	{
		Close();
		return; 
	}
	else if (msg.pSender->GetName() == _T("btn_conn_save"))
	{
		bool retVal = false;
		if (m_type == Type_Add)
		{
			retVal = OnAddInfo();
		}
		else 
		{
			retVal = OnAltInfo();
		}	
		if (retVal) Close();
	}
}

bool ConnInfoSubWhd::OnAddInfo()
{
	std::string name = CharacterSet::UnicodeToANSI(m_pEditName->GetText().GetData());
	std::string ip =  CharacterSet::UnicodeToANSI(m_pEditHost->GetText().GetData());
	std::string port =  CharacterSet::UnicodeToANSI(m_pEditPort->GetText().GetData());
	std::string auth =  CharacterSet::UnicodeToANSI(m_pEditAuth->GetText().GetData());
	if (name.empty())
	{
		CDuiString CtlName = m_pEditName->GetName();
		CtlName.Replace(_T("edt"), _T("lbl") );
		CLabelUI* pLbl = static_cast<CLabelUI*> (m_PaintManager.FindControl(CtlName));
		UserMessageBox(GetHWND(), 10011, pLbl->GetText(), MB_ICONINFORMATION);
		return false;
	}
	if (ip.empty())
	{
		CDuiString CtlName = m_pEditHost->GetName();
		CtlName.Replace(_T("edt"), _T("lbl") );
		CLabelUI* pLbl = static_cast<CLabelUI*> (m_PaintManager.FindControl(CtlName));
		UserMessageBox(GetHWND(), 10011, pLbl->GetText(), MB_ICONINFORMATION);
		return false;
	}
	if (port.empty())
	{
		CDuiString CtlName = m_pEditPort->GetName();
		CtlName.Replace(_T("edt"), _T("lbl") );
		CLabelUI* pLbl = static_cast<CLabelUI*> (m_PaintManager.FindControl(CtlName));
		UserMessageBox(GetHWND(), 10011, pLbl->GetText(), MB_ICONINFORMATION);
		return false;
	}
	TDicTables& dicObj = *m_pdicServerInfo;
	for (std::size_t idx=0; idx<dicObj[ConnInfoUI::kServerNameIndex].size(); ++idx)
	{
		if (dicObj[ConnInfoUI::kServerNameIndex][idx] == name)
		{
			UserMessageBox(GetHWND(), 10015, NULL, MB_ICONWARNING);
			return false;
		}
	}
	dicObj[ConnInfoUI::kServerNameIndex].push_back(name);
	dicObj[ConnInfoUI::kServerIpIndex].push_back(ip);
	dicObj[ConnInfoUI::kServerPortIndex].push_back(port);
	dicObj[ConnInfoUI::kServerAuthIndex].push_back(auth);
	return true;
}

bool ConnInfoSubWhd::OnAltInfo()
{
	std::string name = CharacterSet::UnicodeToANSI(m_pEditName->GetText().GetData());
	std::string ip =  CharacterSet::UnicodeToANSI(m_pEditHost->GetText().GetData());
	std::string port =  CharacterSet::UnicodeToANSI(m_pEditPort->GetText().GetData());
	std::string auth =  CharacterSet::UnicodeToANSI(m_pEditAuth->GetText().GetData());
	TDicTables& dicObj = *m_pdicServerInfo;
	for (std::size_t idx=0; idx<dicObj[ConnInfoUI::kServerNameIndex].size(); ++idx)
	{
		if (dicObj[ConnInfoUI::kServerNameIndex][idx] == name && idx!=m_celIdx)
		{
			UserMessageBox(GetHWND(), 10015, NULL, MB_ICONWARNING);
			return false;
		}
	}
	dicObj[ConnInfoUI::kServerNameIndex][m_celIdx] = name;
	dicObj[ConnInfoUI::kServerIpIndex][m_celIdx] = ip;
	dicObj[ConnInfoUI::kServerPortIndex][m_celIdx] = port;
	dicObj[ConnInfoUI::kServerAuthIndex][m_celIdx] = auth;
	return true;
}

