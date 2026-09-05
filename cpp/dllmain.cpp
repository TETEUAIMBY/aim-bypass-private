#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <thread>
#include "menu/Menu.h"
#include "features/Aimbot.h"
#include "features/ESP.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Variáveis globais
HWND g_hwnd = nullptr;
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

bool g_MenuOpen = false;
bool g_bInitialized = false;
bool g_bRunning = true;

// Instâncias dos módulos
Menu* g_pMenu = nullptr;
Aimbot* g_pAimbot = nullptr;
ESP* g_pESP = nullptr;

// Forward declarations
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool CreateHiddenWindow();
bool InitializeDX11();
void SelfDelete();
void ClearLogs();

// Criar janela oculta para renderização
bool CreateHiddenWindow()
{
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = L"AimBypassMenu";

    if (!RegisterClassEx(&wc))
        return false;

    g_hwnd = CreateWindowEx(
        WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        wc.lpszClassName,
        L"Aim Bypass Private",
        WS_POPUP,
        0, 0, 1920, 1080,
        nullptr, nullptr, wc.hInstance, nullptr
    );

    if (!g_hwnd)
        return false;

    SetLayeredWindowAttributes(g_hwnd, RGB(0, 0, 0), 0, LWA_ALPHA);
    SetWindowPos(g_hwnd, HWND_TOPMOST, 0, 0, 1920, 1080, SWP_NOACTIVATE);

    return true;
}

// Inicializar DirectX 11
bool InitializeDX11()
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 1920;
    sd.BufferDesc.Height = 1080;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[3] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
    };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        featureLevelArray,
        3,
        D3D11_SDK_VERSION,
        &sd,
        &g_pSwapChain,
        &g_pd3dDevice,
        &featureLevel,
        &g_pd3dDeviceContext
    );

    if (FAILED(hr))
        return false;

    ID3D11Texture2D* pBackBuffer = nullptr;
    if (FAILED(g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer))))
        return false;

    if (FAILED(g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView)))
    {
        pBackBuffer->Release();
        return false;
    }

    pBackBuffer->Release();
    g_bInitialized = true;
    return true;
}

// Message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_INSERT)  // INSERT para abrir/fechar menu
            g_MenuOpen = !g_MenuOpen;
        if (wParam == VK_ESCAPE)
            g_MenuOpen = false;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Self Delete - Fechar cheat, manter FiveM, limpar logs
void SelfDelete()
{
    // Limpar logs primeiro
    ClearLogs();

    // Fechar todos os processos do cheat EXCETO FiveM
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32))
    {
        do
        {
            // NÃO fechar FiveM.exe
            if (wcscmp(pe32.szExeFile, L"FiveM.exe") == 0)
                continue;

            // NÃO fechar svchost, explorer, etc
            if (wcscmp(pe32.szExeFile, L"explorer.exe") == 0 ||
                wcscmp(pe32.szExeFile, L"svchost.exe") == 0 ||
                wcscmp(pe32.szExeFile, L"SearchIndexer.exe") == 0)
                continue;

            // Fechar processos do cheat
            if (wcscmp(pe32.szExeFile, L"injector.exe") == 0 ||
                wcscmp(pe32.szExeFile, L"loader.bat") == 0 ||
                wcscmp(pe32.szExeFile, L"AimBypassPrivate.dll") == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                if (hProcess)
                {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                }
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);

    // Encerrar thread principal
    g_bRunning = false;
    PostQuitMessage(0);
}

// Limpar logs do cheat
void ClearLogs()
{
    // Limpar arquivo de log do cheat
    char logPath[MAX_PATH];
    GetTempPathA(MAX_PATH, logPath);
    strcat_s(logPath, MAX_PATH, "AimBypassPrivate.log");
    DeleteFileA(logPath);

    // Limpar diretório temporário do cheat
    char tempDir[MAX_PATH];
    GetTempPathA(MAX_PATH, tempDir);
    strcat_s(tempDir, MAX_PATH, "AimBypass\\");

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((std::string(tempDir) + "*.*").c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
            {
                std::string filePath = tempDir + std::string(findData.cFileName);
                DeleteFileA(filePath.c_str());
            }
        } while (FindNextFileA(hFind, &findData));

        FindClose(hFind);
        RemoveDirectoryA(tempDir);
    }
}

// Thread principal
DWORD WINAPI MainThread(LPVOID lpParam)
{
    // Criar janela
    if (!CreateHiddenWindow())
        return 1;

    // Inicializar DirectX
    if (!InitializeDX11())
        return 1;

    // Criar instâncias dos módulos
    g_pMenu = new Menu();
    g_pAimbot = new Aimbot();
    g_pESP = new ESP();

    if (!g_pMenu || !g_pAimbot || !g_pESP)
        return 1;

    // Loop principal
    MSG msg;
    while (g_bRunning)
    {
        if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                break;
        }
        else
        {
            // Atualizar features
            if (g_pAimbot)
                g_pAimbot->Update();
            if (g_pESP)
                g_pESP->Update();

            // Renderizar menu
            if (g_MenuOpen && g_pMenu)
            {
                float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
                g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);

                g_pMenu->Render();

                if (FAILED(g_pSwapChain->Present(1, 0)))
                    break;
            }
            else
            {
                Sleep(1);
            }
        }
    }

    // Cleanup
    if (g_pMenu) delete g_pMenu;
    if (g_pAimbot) delete g_pAimbot;
    if (g_pESP) delete g_pESP;

    if (g_mainRenderTargetView) g_mainRenderTargetView->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pd3dDeviceContext) g_pd3dDeviceContext->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();

    if (g_hwnd)
    {
        DestroyWindow(g_hwnd);
        UnregisterClassW(L"AimBypassMenu", GetModuleHandle(nullptr));
    }

    return 0;
}

// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(nullptr, 0, MainThread, nullptr, 0, nullptr);
        if (hThread)
            CloseHandle(hThread);
    }
    return TRUE;
}

// Função exportada para self-delete
extern "C" void __declspec(dllexport) ExecuteSelfDelete()
{
    SelfDelete();
}