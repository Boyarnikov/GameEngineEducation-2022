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
#include "InputHandler.h"

#include <vector>

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
    InputHandler* inputHandler = new InputHandler();

    MSG msg = { 0 };

    timer.Start();
    timer.Reset();

    const int N = 100;
    CubeGameObject* cubes[N];
    std::srand(std::time(nullptr));

    std::vector<CubeGameObject*> inputSubscribers;

    for (int i = 0; i < N; i++) {
        int rand = std::rand() % 3;

        if (rand == 0) 
            cubes[i] = new MoveCube(timer.TotalTime());
        else if (rand == 1)
            cubes[i] = new JumpCube(timer.TotalTime());
        else {
            cubes[i] = new ControlCube(timer.TotalTime());
            inputSubscribers.push_back(cubes[i]);
        }
        renderThread->EnqueueCommand(RC_CreateCubeRenderObject, cubes[i]->GetRenderProxy());

        cubes[i]->SetPosition(4.0f * (i % 20) - 40.f, -50.0f, (i / 20) * 4.f);
    }


    float newPositionX = 0.0f;

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
            inputHandler->Update();
            
            float t = 0;
            timer.Tick();
            
            if (inputHandler->GetInputState().test(eIC_GoLeft))
                for (auto& cube : cubes) {
                    cube->Event(timer.TotalTime(), timer.DeltaTime(), eIC_GoLeft);
                }
            if (inputHandler->GetInputState().test(eIC_GoRight))
                for (auto& cube : cubes) {
                    cube->Event(timer.TotalTime(), timer.DeltaTime(), eIC_GoRight);
                }

            for (auto& cube : cubes) {
                cube->Update(timer.TotalTime(), timer.DeltaTime());
            }

            renderThread->OnEndFrame();
        }
    }

    return (int) msg.wParam;
}
