#pragma once
#include <afxsock.h>		// MFC socket extensions

//====================================================
// Описание работы классов и методов исходника на:
// https://www.interestprograms.ru
// Исходные коды программ и игр
// Автор исходных кодов Клочков Павел
//====================================================


// Класс сокетов производный от класса 
// асинхронных сокетов CAsyncSocket.
class CSock : public  CAsyncSocket
{
public:
	CSock();
	virtual ~CSock();
	
	// События для возможности обработки
	// сообщений в асинхронном режиме.
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnAccept(int nErrorCode);

	// Переменная для "связи" с родительским окном.
	CWnd* m_pParent;
};


