// GameEngine.cpp : Defines the entry point for the application.
//

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "GameEngine.h"
#include "RenderEngine.h"
#include "RenderThread.h"
#include "CubeGameObject.h"
#include "GameTimer.h"
#include "INIReader.h"


const char convertToVk(const std::string& s, const std::map <std::string, int>& inputMapping) {
    if (s.length() == 1) {
        char in = s[0];
        if (in >= '0' && in <= '9')
            return (char)in;
        if (in >= 'a' && in <= 'z')
            return toupper(in);
        if (in >= 'A' && in <= 'Z')
            return (char)in;
    }
    else {
        if (inputMapping.find(s) == inputMapping.end()) {
            return 0x07; // Undefined
        }
        else {
            return inputMapping.at(s);
        }
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // Enable run-time memory check for debug builds.
#if defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    GameTimer timer;

    RenderEngine* renderEngine = new RenderEngine(hInstance);
    RenderThread* renderThread = renderEngine->GetRT();

    GameObject* cube = new CubeGameObject();
    renderThread->EnqueueCommand(RC_CreateCubeRenderObject, cube->GetRenderProxy());
    cube->SetPosition(0.0f, -3.0f, 0.0f);

    MSG msg = { 0 };

    
    INIReader r("../actionmap.ini");
    
    if (r.ParseError() < 0) {
        return 1;
    }
    
    std::map <std::string, int> inputMapping;

    // не нашёл общеё функции чтобы перегонять из строк названий VkKeys в чары, поэтому так)
    inputMapping["VK_LEFT"] = VK_LEFT;
    inputMapping["VK_UP"] = VK_UP;
    inputMapping["VK_RIGHT"] = VK_RIGHT;
    inputMapping["VK_DOWN"] = VK_DOWN;
    inputMapping["VK_SPACE"] = VK_SPACE;
    // ну и ещё куча параметров если сильно хочется

    const char inputUp = convertToVk(r.Get("Keyboard", "GoUp", "W"), inputMapping);
    const char inputDown = convertToVk(r.Get("Keyboard", "GoDown", "S"), inputMapping);
    const char inputLeft = convertToVk(r.Get("Keyboard", "GoLeft", "A"), inputMapping);
    const char inputRight = convertToVk(r.Get("Keyboard", "GoRight", "D"), inputMapping);
        /*
    const char inputUp = 'W';
    const char inputDown = 'S';
    const char inputLeft = 'A';
    const char inputRight = 'D';
        */
    
    float acceleration = 16.0f;
    float friction = 8.0f;
    float speedCap = 10.0f;
    float movement[2] = { 0.0f, 0.0f };

    timer.Start();
    timer.Reset();

    // Main message loop:
    while (msg.message != (WM_QUIT | WM_CLOSE))
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            float t = 0;
            
            timer.Tick();
            t = sin(timer.TotalTime())*2;

            // The value 0x8000 contains the bit flag that tests whether the key is currently pressed.
            if (GetAsyncKeyState(inputLeft) & 0x8000) {
                movement[0] -= timer.DeltaTime() * acceleration;
            }
            if (GetAsyncKeyState(inputRight) & 0x8000) {
                movement[0] += timer.DeltaTime() * acceleration;
            }

            if (GetAsyncKeyState(inputDown) & 0x8000) {
                movement[1] -= timer.DeltaTime() * acceleration;
            }
            if (GetAsyncKeyState(inputUp) & 0x8000) {
                movement[1] += timer.DeltaTime() * acceleration;
            }

            movement[0] = max(-speedCap, min(speedCap, movement[0]));
            movement[1] = max(-speedCap, min(speedCap, movement[1]));
            cube->AddPosition(movement[0] * timer.DeltaTime(), 0.0f, movement[1] * timer.DeltaTime());
            
            
            if (movement[0] > timer.DeltaTime() * friction)
                movement[0] -= timer.DeltaTime() * friction;
            else if (movement[0] < -timer.DeltaTime() * friction)
                movement[0] += timer.DeltaTime() * friction;
            else movement[0] = 0;

            if (movement[1] > timer.DeltaTime() * friction)
                movement[1] -= timer.DeltaTime() * friction;
            else if (movement[1] < -timer.DeltaTime() * friction)
                movement[1] += timer.DeltaTime() * friction;
            else movement[1] = 0;

            //cube->AddPosition(0.0f,0.100f * timer.DeltaTime(), 0.0f);
            //cube->SetPosition(t, 0.0f, 0.0f);

            renderThread->OnEndFrame();
        }
    }

    return (int) msg.wParam;
}
