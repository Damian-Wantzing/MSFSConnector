// LandingTrainer2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <format>

#include <Windows.h>
#include <SimConnect.h>

#include "Gui.h"

#include "Facilities.h"
#include "Approach.h"
#include "RunwayDesignator.h"
#include "ApproachType.h"

HANDLE sim = NULL;

char inputBuffer[5] = "";
std::vector<std::string> approachNames;
static int currentItem = 0;

void render()
{
    ImGui::Text("Airport");
    ImGui::PushItemWidth(100);
    if (ImGui::InputText("##Airport", inputBuffer, IM_ARRAYSIZE(inputBuffer)))
    {
        for (int i = 0; inputBuffer[i]; i++) {
            inputBuffer[i] = toupper(inputBuffer[i]);
        }

        if (strlen(inputBuffer) == 4)
        {
            std::vector<MSFSConnector::Approach> airportApproaches = MSFSConnector::Facilities::getApproaches(sim, inputBuffer);
            approachNames.resize(airportApproaches.size());
            for (size_t i = 0; i < airportApproaches.size(); i++)
            {
                approachNames[i] = std::format("{}{}{}", MSFSConnector::approachTypeToString(MSFSConnector::ApproachType(airportApproaches[i].type)), airportApproaches[i].runwayNumber, MSFSConnector::RunwayDesignatorToString(MSFSConnector::RunwayDesignator(airportApproaches[i].runwayDesignator)));
            }
        }
    }
    ImGui::PopItemWidth();

    std::vector<const char*> approaches;
    for (const auto& name : approachNames)
    {
        approaches.push_back(name.c_str());
    }

    ImGui::Text("Approach");
    ImGui::Combo("##Approach", &currentItem, approaches.data(), approaches.size());
}

int main()
{
    SimConnect_Open(&sim, "Landing Trainer", NULL, 0, 0, 0);

    Gui gui("test", 800, 600, false);

    gui.render(
        render, 
        ImVec4(0.45f, 0.55f, 0.60f, 1.00f),
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar | 
        ImGuiWindowFlags_NoScrollWithMouse
    );
    
    SimConnect_Close(sim);

    return 0;
}

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    return main();
}