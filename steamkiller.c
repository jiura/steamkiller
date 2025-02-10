// TODO: 
// - Get rid of SteamWebHelper
// - This thing is eating up 1MB constantly, even when doing nothing. Maybe I could lower this.

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <shellapi.h>
#include <tlhelp32.h>

#define ONE_MINUTE 60000 // 1 minute in milliseconds
#define IDI_ICON1 101
#define WM_TRAYICON (WM_APP + 100)

enum WmCommandParams {
	WM_COMMAND_EXIT = 1
};

BOOL kill_process(const wchar_t* process_name) {
	PROCESSENTRY32 pe32;
	HANDLE h_process_snap;
	HANDLE h_process;
	BOOL result = FALSE;

	h_process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h_process_snap == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(h_process_snap, &pe32)) {
		do {
			if (_wcsicmp(pe32.szExeFile, process_name) == 0) {
				h_process = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
				if (h_process) {
					result = TerminateProcess(h_process, 0);
					CloseHandle(h_process);
					break;
				}
			}
		} while (Process32Next(h_process_snap, &pe32));
	}

	CloseHandle(h_process_snap);
	return result;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

NOTIFYICONDATA nid;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	const wchar_t CLASS_NAME[] = L"SteamKillerWndClass";

	// Register window class
	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create hidden window
	HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"", 0, 0, 0, 0, 0, 0, 0, hInstance, NULL);
	if (hwnd == NULL) {
		return 0;
	}

	// Initialize tray icon
	nid.cbSize = sizeof(nid);
	nid.hWnd = hwnd;
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = WM_TRAYICON;
	nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcscpy(nid.szTip, L"Steam Killer");

	HICON h_icon = (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	if (h_icon) {
		nid.hIcon = h_icon;
	}

	Shell_NotifyIcon(NIM_ADD, &nid);

	// Run message loop
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Cleanup
	Shell_NotifyIcon(NIM_DELETE, &nid);

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_TRAYICON:
			if (lParam == WM_LBUTTONUP) {
				PostMessage(hwnd, WM_COMMAND, WM_COMMAND_EXIT, 0);
			} else if (lParam == WM_RBUTTONUP) {
				POINT p;
				GetCursorPos(&p);
				SetForegroundWindow(hwnd);

				HMENU hmenu = CreatePopupMenu();
				AppendMenu(hmenu, MF_STRING, WM_COMMAND_EXIT, L"Exit");

				TrackPopupMenu(hmenu, TPM_RIGHTBUTTON, p.x, p.y, 0, hwnd, NULL);
				DestroyMenu(hmenu);
			}

			break;

		case WM_COMMAND:
			if (LOWORD(wParam) == WM_COMMAND_EXIT) {
				BOOL result = kill_process(L"steam.exe"); // TODO: Use this result for something?
				PostQuitMessage(0);
			}
			return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
