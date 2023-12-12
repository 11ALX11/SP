// WindowsForms.cpp : Defines the entry point for the application.
//

#include <vector>
#include <string>
#include <locale>
#include <codecvt>

#include "framework.h"
#include "Windows.h"
#include "WindowsForms.h"
#include "HospitalLib.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

std::vector<std::reference_wrapper<Patient>> patientsQueue;
std::vector<AdultPatient> adultPatients;
std::vector<ChildPatient> childPatients;
std::vector<std::unique_ptr<Doctor>> doctors;
std::vector<std::thread> threads;

std::mutex launchButtonMtx;

HWND patientNameTextBox;
HWND patientValueNumericUpDown;

HWND addAdultPatientButton;
HWND addChildPatientButton;
#define ADDADULTPATIENTBUTTON 200
#define ADDCHILDPATIENTBUTTON 201

HWND doctorNameTextBox;
HWND doctorMaxTime;

HWND addDoctorButton;
HWND addAdultDoctorButton;
HWND addPediatricianButton;
#define ADDDOCTORBUTTON 202
#define ADDADULTDOCTORBUTTON 203
#define ADDPEDIATRICIANBUTTON 204

HWND launchButton;
#define LAUNCHBUTTON 205

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSFORMS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSFORMS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSFORMS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSFORMS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

std::string WStringToString(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wstr);
}

std::string GetWindowTextString(HWND hWnd)
{
    int length = GetWindowTextLength(hWnd);
    if (length == 0)
        return "";

    std::wstring buffer(length + 1, '\0');
    GetWindowText(hWnd, &buffer[0], length + 1);

    return WStringToString(buffer);
}

int GetWindowTextInt(HWND hWnd)
{
    std::string text = GetWindowTextString(hWnd);
    if (text == "") return 0;
    return std::stoi(text);
}

void AddAdultPatient(const std::string& name, int value)
{
    AdultPatient adultPatient(name, value);
    adultPatients.push_back(adultPatient);
}

void AddChildPatient(const std::string& name, int value)
{
    ChildPatient childPatient(name, value);
    childPatients.push_back(childPatient);
}

void AddDoctor(const std::string& name, int maxTime)
{
    doctors.push_back(std::make_unique<Doctor>(name, maxTime));
}

void AddAdultDoctor(const std::string& name, int maxTime)
{
    doctors.push_back(std::make_unique<AdultDoctor>(name, maxTime));
}

void AddPediatrician(const std::string& name, int maxTime)
{
    doctors.push_back(std::make_unique<Pediatrician>(name, maxTime));
}

void InitializePatientControls(HWND hWnd)
{
    // Создание элементов управления для пациентов
    CreateWindowEx(0, L"STATIC", L"Patient Name:", WS_CHILD | WS_VISIBLE, 10, 10, 120, 20, hWnd, NULL, hInst, NULL);

    patientNameTextBox = CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE, 150, 10, 120, 20, hWnd, NULL, hInst, NULL);

    CreateWindowEx(0, L"STATIC", L"Patient Value:", WS_CHILD | WS_VISIBLE, 10, 40, 120, 20, hWnd, NULL, hInst, NULL);

    patientValueNumericUpDown = CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE, 150, 40, 120, 20, hWnd, NULL, hInst, NULL);

    addAdultPatientButton = CreateWindowEx(0, L"BUTTON", L"Add Adult Patient", WS_CHILD | WS_VISIBLE, 300, 10, 150, 30, hWnd, (HMENU) ADDADULTPATIENTBUTTON, hInst, NULL);
    addChildPatientButton = CreateWindowEx(0, L"BUTTON", L"Add Child Patient", WS_CHILD | WS_VISIBLE, 460, 10, 150, 30, hWnd, (HMENU) ADDCHILDPATIENTBUTTON, hInst, NULL);
}

void InitializeDoctorControls(HWND hWnd)
{
    // Создание элементов управления для врачей
    CreateWindowEx(0, L"STATIC", L"Doctor Name:", WS_CHILD | WS_VISIBLE, 10, 80, 120, 20, hWnd, NULL, hInst, NULL);

    doctorNameTextBox = CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE, 150, 80, 120, 20, hWnd, NULL, hInst, NULL);

    CreateWindowEx(0, L"STATIC", L"Doctor Max Time:", WS_CHILD | WS_VISIBLE, 10, 110, 120, 20, hWnd, NULL, hInst, NULL);

    doctorMaxTime = CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE, 150, 110, 120, 20, hWnd, NULL, hInst, NULL);

    addDoctorButton = CreateWindowEx(0, L"BUTTON", L"Add Doctor", WS_CHILD | WS_VISIBLE, 300, 80, 120, 30, hWnd, (HMENU) ADDDOCTORBUTTON, hInst, NULL); addDoctorButton = CreateWindowEx(0, L"BUTTON", L"Add Doctor", WS_CHILD | WS_VISIBLE, 300, 80, 120, 30, hWnd, (HMENU)ADDDOCTORBUTTON, hInst, NULL);
    addAdultDoctorButton = CreateWindowEx(0, L"BUTTON", L"Add Adult Doctor", WS_CHILD | WS_VISIBLE, 430, 80, 150, 30, hWnd, (HMENU) ADDADULTDOCTORBUTTON, hInst, NULL);
    addPediatricianButton = CreateWindowEx(0, L"BUTTON", L"Add Pediatrician", WS_CHILD | WS_VISIBLE, 590, 80, 150, 30, hWnd, (HMENU) ADDPEDIATRICIANBUTTON, hInst, NULL);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            InitializePatientControls(hWnd);
            InitializeDoctorControls(hWnd);
            launchButton = CreateWindowEx(0, L"BUTTON", L"Launch", WS_CHILD | WS_VISIBLE, 10, 140, 100, 30, hWnd, (HMENU) LAUNCHBUTTON, hInst, NULL);
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            // Parse the menu selections:
            switch (wmId)
            {
            case LAUNCHBUTTON:
                if (launchButtonMtx.try_lock()) {

                    // Вызов функции AllocConsole() для создания консольного окна
                    AllocConsole();

                    // Перенаправление стандартных потоков ввода/вывода/ошибок в консоль
                    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
                    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
                    freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);

                    for (auto& adultPatient : adultPatients) {
                        patientsQueue.push_back(std::ref(adultPatient));
                    }
                    for (auto& childPatient : childPatients) {
                        patientsQueue.push_back(std::ref(childPatient));
                    }

                    for (auto& doctor : doctors) {
                        threads.emplace_back(&Doctor::work, doctor.get(), std::ref(patientsQueue));
                    }

                    for (auto& thread : threads) {
                        thread.join();
                    }

                    std::cout << "All patients have been treated. Simulation completed.\n";
                    launchButtonMtx.unlock();
                }
                break;
            case ADDADULTPATIENTBUTTON:
            {
                std::string name = GetWindowTextString(patientNameTextBox);
                int value = GetWindowTextInt(patientValueNumericUpDown);
                AddAdultPatient(name, value);
            }
            break;

            case ADDCHILDPATIENTBUTTON:
            {
                std::string name = GetWindowTextString(patientNameTextBox);
                int value = GetWindowTextInt(patientValueNumericUpDown);
                AddChildPatient(name, value);
            }
            break;

            case ADDDOCTORBUTTON:
            {
                std::string name = GetWindowTextString(doctorNameTextBox);
                int maxTime = GetWindowTextInt(doctorMaxTime);
                AddDoctor(name, maxTime);
            }
            break;

            case ADDADULTDOCTORBUTTON:
            {
                std::string name = GetWindowTextString(doctorNameTextBox);
                int maxTime = GetWindowTextInt(doctorMaxTime);
                AddAdultDoctor(name, maxTime);
            }
            break;

            case ADDPEDIATRICIANBUTTON:
            {
                std::string name = GetWindowTextString(doctorNameTextBox);
                int maxTime = GetWindowTextInt(doctorMaxTime);
                AddPediatrician(name, maxTime);
            }
            break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
