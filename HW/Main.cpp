#include <windows.h>
#include <ctime>
#include <tchar.h>
#include <commctrl.h>
#include <windowsX.h>
#include "resource.h"
#include <string>

#pragma comment(lib,"comctl32")

class CMultithreadDlg
{
public:
    CMultithreadDlg(void);
public:
    ~CMultithreadDlg(void);
    static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
    static CMultithreadDlg* ptr;
    void Cls_OnClose(HWND hwnd);
    BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
    void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
    HWND hPlay1, hPlay2, hPlay3;
    HANDLE Th1, Th2, Th3;
    bool IsThreadSuspended(HANDLE hThread);
    bool IsPrime(int num);
};


int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
    INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX) };
    icc.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icc);
    CMultithreadDlg dlg;
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, reinterpret_cast<DLGPROC>(CMultithreadDlg::DlgProc));
}

CMultithreadDlg* CMultithreadDlg::ptr = NULL;

CMultithreadDlg::CMultithreadDlg(void)
{
    ptr = this;
}

CMultithreadDlg::~CMultithreadDlg(void)
{

}

void CMultithreadDlg::Cls_OnClose(HWND hwnd)
{
    TerminateThread(Th1, 0);
    TerminateThread(Th2, 0);
    TerminateThread(Th3, 0);
    CloseHandle(Th1);
    CloseHandle(Th2);
    CloseHandle(Th3);
    EndDialog(hwnd, 0);
}

DWORD WINAPI ThreadPrime(LPVOID lp)
{
    HWND hListBox = (HWND)lp;
    CMultithreadDlg* dlgPtr = reinterpret_cast<CMultithreadDlg*>(lp); 
    if (dlgPtr == nullptr) {
        
        return 0;
    }

    for (int num = 2;; ++num) {
        if (dlgPtr->IsPrime(num)) { 
            
            std::wstring numStr = std::to_wstring(num);
           
            ListBox_AddString(hListBox, numStr.c_str());
            
            ListBox_SetCurSel(hListBox, ListBox_GetCount(hListBox) - 1);
        }
        Sleep(250);
    }
    return 0;
}

DWORD WINAPI ThreadEven(LPVOID lp)
{
    HWND hListBox = (HWND)lp;
    for (int num = 0;; num += 2) {
     
        std::wstring numStr = std::to_wstring(num);
     
        ListBox_AddString(hListBox, numStr.c_str());
       
        ListBox_SetCurSel(hListBox, ListBox_GetCount(hListBox) - 1);
        Sleep(250);
    }
    return 0;
}

DWORD WINAPI ThreadRange(LPVOID lp)
{
    HWND hListBox = (HWND)lp;
    for (int num = 0; num <= 20000; ++num) {
       
        std::wstring numStr = std::to_wstring(num);
       
        ListBox_AddString(hListBox, numStr.c_str());
       
        ListBox_SetCurSel(hListBox, ListBox_GetCount(hListBox) - 1);
        Sleep(250);
    }
    return 0;
}
BOOL CMultithreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    srand(time(0));

    Th1 = CreateThread(NULL, 0, ThreadPrime, GetDlgItem(hwnd, IDC_LIST1), CREATE_SUSPENDED, NULL);
    Th2 = CreateThread(NULL, 0, ThreadEven, GetDlgItem(hwnd, IDC_LIST2), CREATE_SUSPENDED, NULL);
    Th3 = CreateThread(NULL, 0, ThreadRange, GetDlgItem(hwnd, IDC_LIST3), CREATE_SUSPENDED, NULL);

    hPlay1 = GetDlgItem(hwnd, IDC_BUTTON2);
    hPlay2 = GetDlgItem(hwnd, IDC_BUTTON3);
    hPlay3 = GetDlgItem(hwnd, IDC_BUTTON4);

    return TRUE;
}

void CMultithreadDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    if (id == IDC_BUTTON2)
    {
        if (Th1) {
            if (IsThreadSuspended(Th1)) {
                ResumeThread(Th1);
                SetWindowText(hPlay1, TEXT("Пауза"));
            }
            else {
                SuspendThread(Th1);
                SetWindowText(hPlay1, TEXT("Пуск"));
            }
        }
    }
    else if (id == IDC_BUTTON3)
    {
        if (Th2) {
            if (IsThreadSuspended(Th2)) {
                ResumeThread(Th2);
                SetWindowText(hPlay2, TEXT("Пауза"));
            }
            else {
                SuspendThread(Th2);
                SetWindowText(hPlay2, TEXT("Пуск"));
            }
        }
    }
    else if (id == IDC_BUTTON4)
    {
        if (Th3) {
            if (IsThreadSuspended(Th3)) {
                ResumeThread(Th3);
                SetWindowText(hPlay3, TEXT("Пауза"));
            }
            else {
                SuspendThread(Th3);
                SetWindowText(hPlay3, TEXT("Пуск"));
            }
        }
    }
    else if (id == IDC_BUTTON5)
    {
        TerminateThread(Th1, 0);
        CloseHandle(Th1);
        EnableWindow(hPlay1, FALSE);
    }
    else if (id == IDC_BUTTON6)
    {
        TerminateThread(Th2, 0);
        CloseHandle(Th2);
        EnableWindow(hPlay2, FALSE);
    }
    else if (id == IDC_BUTTON7)
    {
        TerminateThread(Th3, 0);
        CloseHandle(Th3);
        EnableWindow(hPlay3, FALSE);
    }
}

BOOL CALLBACK CMultithreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
    }
    return FALSE;
}

bool CMultithreadDlg::IsThreadSuspended(HANDLE hThread)
{
    DWORD dwExitCode;
    GetExitCodeThread(hThread, &dwExitCode);
    return dwExitCode == STILL_ACTIVE;
}

bool CMultithreadDlg::IsPrime(int num)
{
    if (num <= 1)
        return false;
    if (num <= 3)
        return true;
    if (num % 2 == 0 || num % 3 == 0)
        return false;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
    }
    return true;
}
