================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : ChatCpp Project Overview
===============================================================================

This ChatCpp application has created by Mikhail Makurin. 

A detailed description of the chat in Russian can be seen at the end of this document.

This file contains a summary of what you will find in each of the files that
make up your ChatCpp application.

ChatCpp.vcproj
    This is the main project file for VC++ projects generated using an application wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    application wizard.

ChatCpp.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CChatCppApp application class.

ChatCpp.cpp
    This is the main application source file that contains the application
    class CChatCppApp.

ChatCpp.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

res\ChatCpp.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file ChatCpp.rc.

res\ChatCpp.rc2
    This file contains resources that are not edited by Microsoft 
    Visual C++. You should place all resources not editable by
    the resource editor in this file.

/////////////////////////////////////////////////////////////////////////////

The application wizard creates one dialog class:
ChatCppDlg.h, ChatCppDlg.cpp - the dialog
    These files contain your CChatCppDlg class.  This class defines
    the behavior of your application's main dialog.  The dialog's template is
    in ChatCpp.rc, which can be edited in Microsoft Visual C++.
/////////////////////////////////////////////////////////////////////////////

Other Features:

ActiveX Controls
    The application includes support to use ActiveX controls.

Printing and Print Preview support
    The application wizard has generated code to handle the print, print setup, and print preview
    commands by calling member functions in the CView class from the MFC library.
/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named ChatCpp.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

ChatCpp.manifest
	Application manifest files are used by Windows XP to describe an applications 
	dependency on specific versions of Side-by-Side assemblies. The loader uses this 
	information to load the appropriate assembly from the assembly cache or private 
	from the application. The Application manifest  maybe included for redistribution 
	as an external .manifest file that is installed in the same folder as the application 
	executable or it may be included in the executable in the form of a resource. 
/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is in a 
language other than the operating system's current language, you will need 
to copy the corresponding localized resources MFC70XXX.DLL from the Microsoft
Visual C++ CD-ROM under the Win\System directory to your computer's system or 
system32 directory, and rename it to be MFCLOC.DLL.  ("XXX" stands for the 
language abbreviation.  For example, MFC70DEU.DLL contains resources 
translated to German.)  If you don't do this, some of the UI elements of 
your application will remain in the language of the operating system.

/////////////////////////////////////////////////////////////////////////////


Чат написан на языке С++ на базе библиотеки MFC. Для визуализации построения пользовательского интерфейса выбран тип приложения на основе диалоговых окон.
Количество подключаемых к чату клиентов не ограничивается. Для удобного хранения сокетов подключенных клиентов используется шаблонный класс стандартной библиотеки С++ class vector. Класс vector представляет собой динамический массив и поддерживает быстрый доступ к любому элементу.
Работу сетевого приложения чата обеспечивают сокеты. Сетевое соединение устанавливается посредством объектов класса CSock. Класс CSock произведен от высокоуровневого класса асинхронных сокетов CAsyncSocket. Асинхронная работа сокетов исключает блокировку пользовательского интерфейса во время установления соединения и отправки-получения сообщений по сети. Класс CAsyncSocket входит в состав богатейшей библиотеки MFC, облегчающей рутинный труд С++ программистов. CAsyncSocket - интуитивно понятная реализация в виде класса для низкоуровневого интерфейса Windows Sockets API. Дочерний класс CSock инкапсулирует в себе достоинства асинхронной работы с сокетами и небольшим дополнительным программным кодом обеспечивает полноценную сетевую работу чата.

Листинг реализации класса CSock:
// Событие подключения на стороне клиентского приложения.
void CSock::OnConnect(int nErrorCode)
{
    // Данные в родительское окно для индикации процесса соединения.
    CChatCppDlg* pDlg = (CChatCppDlg*)m_pParent;
    nErrorCode == 0 ? pDlg->OnConnect(FALSE) : pDlg->OnConnect(TRUE);
	
    CAsyncSocket::OnConnect(nErrorCode);
}

// Событие отключения от сети
void CSock::OnClose(int nErrorCode)
{
    Beep(2000, 300);
	
    CAsyncSocket::OnClose(nErrorCode);
}

// Событие получения сообщений.
void CSock::OnReceive(int nErrorCode)
{
    // Данные в родительское окно для визуальности приема сообщений.
    CChatCppDlg* pDlg = (CChatCppDlg*)m_pParent;
    pDlg->OnReceive();

    CAsyncSocket::OnReceive(nErrorCode);
}

// Запрос на подключение, направляемый клиентом серверу.
// Происходит на стороне серверного приложения.
void CSock::OnAccept(int nErrorCode)
{
    // Данные в родительское окно для индикации процесса соединения.
    CChatCppDlg* pDlg = (CChatCppDlg*)m_pParent;
    pDlg->OnAccept();

    CAsyncSocket::OnAccept(nErrorCode);
}
Подключение клиентов
При установлении сетевого соединения для каждого клиента создаётся отдельный сокет. Сокеты клиентов хранятся в динамическом массиве std::vector m_vecSockets. В момент акцептирования (принятия) клиентов в чате рассылается информация о количестве подключенных пользователей. Успешное подключение к сети индицирует заголовок приложения.
// Принимаем запросы на подключения
void CChatCppDlg::OnAccept(void)
{
    CSock* pSock = new CSock;
    pSock->m_pParent = this;

    // Если все в порядке добавим рабочий сокет в список 
    // подключенных рабочих сокетов.
    if(m_mainSocket.Accept(*pSock) == TRUE)
    {
        m_vecSockets.push_back(pSock);
        m_ButtonSend.EnableWindow(TRUE);
        SendCountPeople();

        SetWindowText("Сеть работает!");
    }
    else 
    {
        delete pSock;
    } }
Сортировка сетевых сообщений
Поскольку сетевые сообщения могут содержать информацию любого типа (строки символов, числа, двоичные файлы) возникает необходимость создания сортировки сообщений внутри приложения. Исходник содержит один из способов сортировки типов сообщений. Для этого используется специальная структура для передачи по сети, содержащая в себе необходимые переменные.
Структура для передачи сетевых сообщений:
struct SENDBUFFER
{
    SENDBUFFER() 
    {
        typemessage = 0; 
        countpeople = 0;
        stopchat = false;
        ZeroMemory(name, sizeof(TCHAR)*14); 
        ZeroMemory(buffer, sizeof(TCHAR)*202);
    }

    int typemessage;
    int countpeople;
    bool stopchat;
    TCHAR name[14];
    TCHAR buffer[202];};
Сортировку получаемых сообщений удобно производить с помощью оператора swicth:
m_mainSocket.Receive(&sb, sizeof(SENDBUFFER));
switch(sb.typemessage)
{
case m_TypeMessage::tmCountPeople:
{
...
}
break;
case m_TypeMessage::tmChat:
{
...
}
break;
case m_TypeMessage::tmDisconnect:
{
..
}
break;
default:
AfxMessageBox("Неизвестное сообщение!");
break;
}
Макросы WINVER и _WIN32_WINNT для разных версий
Исходный код приложения сетевого чата можно использовать в разных версиях Windows, как более ранних, так и в современных.
Для работы в ранних версиях операционной системы Windows требуется закомментировать отмеченные в листинге строки кода.
// Макросы для работы в ранних версиях операционной системы Windows
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER   // Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400  // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif
#ifndef _WIN32_WINNT // Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400 // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif
/*
// Макросы для новых версий Windows
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
*/
Для работы в новых версиях Windows требуется закомментировать макросы WINVER и _WIN32_WINNT для ранних версий.
/*
// Макросы для работы в ранних версиях операционной системы Windows
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER   // Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400   // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif
#ifndef _WIN32_WINNT    // Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400   // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif
*/
// Макросы для новых версий Windows
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

Примечание:
Среда программирования MS Visual Studio 2019, обязательна установка MFC библиотеки версии 142 и выше.

/////////////////////////////////////////////////////////////////////////////
