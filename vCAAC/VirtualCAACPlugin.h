#pragma once
#include "pch.h"

#include "EuroScopePlugIn.h"
#include "VirtualCAACHelper.h"
#include "vCAACHandlers.h"

#include <cmath>
#include <string>
#include <tuple>
#include <vector>

#define MY_PLUGIN_NAME "The vCAAC"
#define MY_PLUGIN_VERSION "0.0.1"
#define MY_PLUGIN_DEVELOPER "Yi Zhang"
#define MY_PLUGIN_COPYRIGHT "GPL v3 (Tentative)"
#define MY_PLUGIN_VIEW_AVISO "vCAAC Tag Family"

#define VCAAC_LIST_SIZE 54

using namespace std;
using namespace EuroScopePlugIn;

class GroundData {
public:
    string m_Callsign;
    string m_Status;
};

class CVirtualCAACPlugin : public EuroScopePlugIn::CPlugIn {
//protected:
   // EuroScopePlugIn::CFlightPlanList m_GroundList;
   // vector<GroundData> m_GroundData;

public:
    CVirtualCAACPlugin();

    virtual ~CVirtualCAACPlugin();

    //virtual void OnControllerPositionUpdate(CController Controller);

    //virtual void OnControllerDisconnect(CController Controller);

    //virtual void OnRadarTargetPositionUpdate(CRadarTarget RadarTarget);

    //virtual void OnFlightPlanDisconnect(CFlightPlan FlightPlan);

    //virtual void OnFlightPlanFlightPlanDataUpdate(CFlightPlan FlightPlan);

	//virtual void OnAirportRunwayActivityChanged(void);

    //virtual void CVirtualCAACPlugin::OnFlightPlanControllerAssignedDataUpdate(
    //    EuroScopePlugIn::CFlightPlan FlightPlan, int DataType);

    virtual bool OnCompileCommand(const char* sCommandLine);

    virtual void OnFunctionCall(int FunctionId, const char* sItemString, POINT Pt,
        RECT Area);

    virtual void CVirtualCAACPlugin::OnGetTagItem(
        EuroScopePlugIn::CFlightPlan FlightPlan,
        EuroScopePlugIn::CRadarTarget RadarTarget, int ItemCode, int TagData,
        char sItemString[16], int* pColorCode, COLORREF* pRGB, double* pFontSize);

    void VersionCheck();
};
