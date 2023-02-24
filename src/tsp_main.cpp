/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 15:25
 */

#include "tsp_application.hpp"

int main()
{
    tsp::Application* App = tsp::Application::GetApplication();
    App->Init();
    App->Update();
    App->Exit();
    return (0);
}
