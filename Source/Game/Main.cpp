#include "Common.h"

#include "Cube/Cube.h"
#include "Cube/RotatingCube.h"
#include "Light/RotatingPointLight.h"
#include "Model/Model.h"
#include "Game/Game.h"

INT WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nCmdShow)
{

    std::unique_ptr<Game> game = std::make_unique<Game>(L"GGP : Lighting Cube with OOD");

    // Phong Lighting Shaders
    std::shared_ptr<VertexShader> vertexShader = std::make_shared<VertexShader>(L"Shaders/Shader.fx", "VS", "vs_5_0");
    if (FAILED(game->GetRenderer()->AddVertexShader(L"VS", vertexShader)))
        return 0;

    std::shared_ptr<PixelShader> pixelShader = std::make_shared<PixelShader>(L"Shaders/Shader.fx", "PS", "ps_5_0");
    if (FAILED(game->GetRenderer()->AddPixelShader(L"PS", pixelShader)))
        return 0;

    std::shared_ptr<PixelShader> pixelSolidShader = std::make_shared<PixelShader>(L"Shaders/Shader.fx", "PSSolid", "ps_5_0");
    if (FAILED(game->GetRenderer()->AddPixelShader(L"PSSolid", pixelSolidShader)))
        return 0;

    // Point Light
    XMFLOAT4 color;
    XMStoreFloat4(&color, Colors::AntiqueWhite);
    color.x *= 20;
    color.y *= 20;
    color.z *= 20;

    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>(XMFLOAT4(-5.77f, 5.77f, -5.77f, 1.0f), color);

    if (FAILED(game->GetRenderer()->AddPointLight(0u, pointLight)))
        return 0;

    // Point Light Cube
    std::shared_ptr<Cube> lightCube = std::make_shared<Cube>(color);
    lightCube->Translate(XMVectorSet(-5.77f, 5.77f, -5.77f, 0.0f));
    if (FAILED(game->GetRenderer()->AddRenderable(L"LightCube", lightCube)))
        return 0;
    if (FAILED(game->GetRenderer()->SetVertexShaderOfRenderable(L"LightCube", L"VS")))
        return 0;
    if (FAILED(game->GetRenderer()->SetPixelShaderOfRenderable(L"LightCube", L"PSSolid")))
        return 0;

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
    TODO1: Rotating Point Light
    -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    XMFLOAT4 rpcolor; // red
    XMStoreFloat4(&rpcolor, Colors::Red);
    rpcolor.x *= 0.2f;
    rpcolor.y *= 0.2f;
    rpcolor.z *= 0.2f;

    // Assuming RotatingPointLight constructor is (centerOffset, color, radius, speed)
    // The centerOffset is where the orbit is centered, e.g., origin or model position.
    std::shared_ptr<RotatingPointLight> rotatingPointLight = std::make_shared<RotatingPointLight>(
        XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),   // Center of rotation (e.g., origin)
        rpcolor
    );

    // Add the rotating point light to the renderer
    // Use a different index (e.g., 1u) than the static light (0u)
    if (FAILED(game->GetRenderer()->AddPointLight(1u, rotatingPointLight)))
        return 0;



    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
    TODO2: Rotating Point Light Cube
    -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    std::shared_ptr<RotatingCube> rotatingLightCube =
        std::make_shared<RotatingCube>(rpcolor);     // same color as the light

    rotatingLightCube->Translate(XMVectorSet(0.0f, 0.0f, -6.0f, 0.0f));

    if (FAILED(game->GetRenderer()->AddRenderable(L"RotatingLightCube", rotatingLightCube)))
        return 0;
    if (FAILED(game->GetRenderer()->SetVertexShaderOfRenderable(L"RotatingLightCube", L"VS")))
        return 0;
    if (FAILED(game->GetRenderer()->SetPixelShaderOfRenderable(L"RotatingLightCube", L"PSSolid")))
        return 0;


    // Golem
    std::shared_ptr<Model> golem = std::make_shared<Model>(L"../../Data/Stone/Stone.obj");
    if (FAILED(game->GetRenderer()->AddRenderable(L"Golem", golem)))
        return 0;
    if (FAILED(game->GetRenderer()->SetVertexShaderOfRenderable(L"Golem", L"VS")))
        return 0;
    if (FAILED(game->GetRenderer()->SetPixelShaderOfRenderable(L"Golem", L"PS")))
        return 0;


    if (FAILED(game->Initialize(hInstance, nCmdShow)))
        return 0;

    return game->Run();
}