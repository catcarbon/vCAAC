#pragma once
#include "pch.h"

#include "VirtualCAACPlugin.h"

using namespace std;
using namespace EuroScopePlugIn;

//string version_url = "https://raw.githubusercontent.com/catcarbon/vCAAC/master/pluginversion.txt";
//string version_url_alt = "https://git.sinofsx.com/vCAAC/master/pluginversion.txt";

CVirtualCAACPlugin::CVirtualCAACPlugin()
    : EuroScopePlugIn::CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
        MY_PLUGIN_NAME, MY_PLUGIN_VERSION, MY_PLUGIN_DEVELOPER, MY_PLUGIN_COPYRIGHT)
{
    /**
        Current altitude display.
    */
    RegisterTagItemType("Current Altitude Mixed Metric", TAG_ITEM_ALT_MM);
    RegisterTagItemType("Current Altitude Metric Only", TAG_ITEM_ALT_MO);

    /**
        Controller assigned temporary altitude display.
    */
    RegisterTagItemType("Temp Altitude Mixed Metric", TAG_ITEM_CFL_MM);
    RegisterTagItemType("Temp Altitude Metric Only", TAG_ITEM_CFL_MO);
    RegisterTagItemFunction("List Temp Altitudes Mixed Metric", TAG_FUNC_CFL_MM);
    RegisterTagItemFunction("List Temp Altitudes Metric Only", TAG_FUNC_CFL_MO);

    /**
        Cruising altitude entered in flight plan. 
    */
    RegisterTagItemType("FP Cruise Mixed Metric", TAG_ITEM_FP_CRUISE_MM);
    RegisterTagItemType("FP Cruise Metric Only", TAG_ITEM_FP_CRUISE_MO);

    /**
        Communication type assigned by controllers. 
        /v/ -> voice using Chinese
        /r/ -> voice using English
        /t/ -> No Radio (NORDO)
    */
    RegisterTagItemType("Communication Type", TAG_ITEM_LANGUAGE);
    RegisterTagItemFunction("List Communication Types", TAG_FUNC_LANGUAGE);

    /**
        Assigned speed or mach. Speed is always in knots regardless of ES config.   
    */
    RegisterTagItemType("Assigned Speed/Mach (if set)", TAG_ITEM_SPEED_RESTRICTION);
}

CVirtualCAACPlugin::~CVirtualCAACPlugin() { }

bool CVirtualCAACPlugin::OnCompileCommand(const char* sCommandLine)
{
    CString cmd(sCommandLine);
    if (cmd.Left(6) == CString(".vcaac")) {
        char buf[100];
        sprintf_s(buf, 100, "%s Plugin for SinoFSX, version %s. Developed by %s, %s.",
            MY_PLUGIN_NAME, MY_PLUGIN_VERSION, MY_PLUGIN_DEVELOPER, MY_PLUGIN_COPYRIGHT);
        DisplayUserMessage(MY_PLUGIN_NAME, "Info", buf, true, true, false, false, false);
    } else {
        return false;
    }

    return true;
}

void CVirtualCAACPlugin::OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan,
    EuroScopePlugIn::CRadarTarget RadarTarget,
    int ItemCode, int TagData,
    char sItemString[16], int* pColorCode,
    COLORREF* pRGB, double* pFontSize)
{
    // only for valid flight plans
    if (!FlightPlan.IsValid())
        return;

    string str;

    switch (ItemCode) {
    case TAG_ITEM_ALT_MM:
    case TAG_ITEM_ALT_MO:
        str = vCAACHandlers::GetAltitudeDisplay(FlightPlan, GetTransitionAltitude(), ItemCode);
        break;
    case TAG_ITEM_CFL_MM:
    case TAG_ITEM_CFL_MO:
        str = vCAACHandlers::GetCFLDisplay(FlightPlan, GetTransitionAltitude(), ItemCode);
        break;
    case TAG_ITEM_FP_CRUISE_MM:
    case TAG_ITEM_FP_CRUISE_MO:
        str = vCAACHandlers::GetFlightPlanAltitudeDisplay(FlightPlan, ItemCode);
        break;
    case TAG_ITEM_LANGUAGE:
        str = vCAACHandlers::GetLanguage(FlightPlan);
        break;
    case TAG_ITEM_SPEED_RESTRICTION:
        str = vCAACHandlers::GetSpeedRestriction(FlightPlan);
        break;
    }

    sprintf_s(sItemString, 16, "%s", str.c_str());
}

void CVirtualCAACPlugin::OnFunctionCall(int FunctionId, const char* sItemString,
    POINT Pt, RECT Area)
{
    EuroScopePlugIn::CFlightPlan fp;

    // flight plan must be valid for selected aircraft
    fp = FlightPlanSelectASEL();
    if (!fp.IsValid())
        return;

    switch (FunctionId) {
    case TAG_FUNC_CFL_MM:
        CPlugIn::OpenPopupList(Area, "Cleared Altitude", 2);
        for (const auto& tup : vCAACHandlers::GetCFLList(fp)) {
            AddPopupListElement(get<0>(tup).c_str(), get<1>(tup).c_str(), TAG_FUNC_CFL_SET, get<2>(tup));
        }
        AddPopupListElement("    ", "    ", TAG_FUNC_CFL_SET, false, POPUP_ELEMENT_NO_CHECKBOX, false, true);
        AddPopupListElement("IAPP", "    ", TAG_FUNC_CFL_SET, false, POPUP_ELEMENT_NO_CHECKBOX, false, true);
        AddPopupListElement("VAPP", "    ", TAG_FUNC_CFL_SET, false, POPUP_ELEMENT_NO_CHECKBOX, false, true);
        break;
    case TAG_FUNC_CFL_MO:
        CPlugIn::OpenPopupList(Area, "Cleared Altitude", 2);
        for (const auto& tup : vCAACHandlers::GetCFLList(fp)) {
            AddPopupListElement(get<0>(tup).c_str(), "  ", TAG_FUNC_CFL_SET, get<2>(tup));
        }
        AddPopupListElement("    ", "  ", TAG_FUNC_CFL_SET, false, POPUP_ELEMENT_NO_CHECKBOX, false, true);
        AddPopupListElement("IAPP", "  ", TAG_FUNC_CFL_SET, false, POPUP_ELEMENT_NO_CHECKBOX, false, true);
        AddPopupListElement("VAPP", "  ", TAG_FUNC_CFL_SET, false, POPUP_ELEMENT_NO_CHECKBOX, false, true);
        break;
    case TAG_FUNC_LANGUAGE:
        CPlugIn::OpenPopupList(Area, "Communication", 2);
        for (const auto& tup : vCAACHandlers::GetLanguageList(fp)) {
            AddPopupListElement(get<0>(tup).c_str(), get<1>(tup).c_str(), TAG_FUNC_SET_LANGUAGE);
        }
        break;
    case TAG_FUNC_CFL_SET:
    case TAG_FUNC_RFL_SET:
        // ignore bool returned from ES
        vCAACHandlers::SetCFL(fp, sItemString);
        break;
    case TAG_FUNC_SET_LANGUAGE:
        // ignore bool returned from ES
        vCAACHandlers::SetLanguage(fp, sItemString);
        break;
    }
}

void CVirtualCAACPlugin::VersionCheck(void) { }
