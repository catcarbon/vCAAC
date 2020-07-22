#pragma once
#include "pch.h"

#include "EuroScopePlugIn.h"
#include "VirtualCAACHelper.h"
#include "vCAACHandlers.h"

#define MY_PLUGIN_NAME "The vCAAC"
#define MY_PLUGIN_VERSION "0.0.2"
#define MY_PLUGIN_DEVELOPER "Yi Zhang"
#define MY_PLUGIN_COPYRIGHT "GPL v3 (Tentative)"
#define MY_PLUGIN_VIEW_AVISO "vCAAC Tag Family"

using namespace std;
using namespace EuroScopePlugIn;

class CVirtualCAACPlugin : public EuroScopePlugIn::CPlugIn {
public:
    CVirtualCAACPlugin();

    virtual ~CVirtualCAACPlugin();

    virtual bool OnCompileCommand(const char* sCommandLine);

    virtual void OnFunctionCall(int FunctionId, const char* sItemString, POINT Pt,
        RECT Area);

    virtual void CVirtualCAACPlugin::OnGetTagItem(
        EuroScopePlugIn::CFlightPlan FlightPlan,
        EuroScopePlugIn::CRadarTarget RadarTarget, int ItemCode, int TagData,
        char sItemString[16], int* pColorCode, COLORREF* pRGB, double* pFontSize);

    void VersionCheck();
};
