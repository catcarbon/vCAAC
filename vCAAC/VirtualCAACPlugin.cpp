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
    RegisterTagItemType("ALT Mixed Metric", TAG_ITEM_ALT_MM);
    // RegisterTagItemType("ALT Mixed Imperial", TAG_ITEM_ALT_MI);
    RegisterTagItemType("ALT Metric Only", TAG_ITEM_ALT_MO);

    RegisterTagItemType("CFL Mixed Metric", TAG_ITEM_CFL_MM);
    // RegisterTagItemType("CFL Mixed Imperial", TAG_ITEM_CFL_MI);
    RegisterTagItemType("CFL Metric Only", TAG_ITEM_CFL_MO);

    RegisterTagItemType("RFL Mixed Metric", TAG_ITEM_RFL_MM);
    // RegisterTagItemType("RFL Mixed Imperial", TAG_ITEM_RFL_MI);
    RegisterTagItemType("RFL Metric Only", TAG_ITEM_RFL_MO);

    RegisterTagItemFunction("Modify CFL Mixed Metric", TAG_FUNC_CFL_MM);
    //  RegisterTagItemFunction("Modify CFL Mixed Imperial", TAG_FUNC_CFL_MI);
    RegisterTagItemFunction("Modify CFL Metric Only", TAG_FUNC_CFL_MO);

    //RegisterTagItemFunction("RFL edit", TAG_FUNC_VCAAC_RFL);

    RegisterTagItemType("FP Final Altitude Mixed Metric", FPL_DATA_FINAL_MM);
    //RegisterTagItemType("FP Final Altitude Mixed Imperial", FPL_DATA_FINAL_MM);
    RegisterTagItemType("FP Final Altitude Metric Only", FPL_DATA_FINAL_MO);

	RegisterTagItemType("Communication Type", TAG_ITEM_LANGUAGE);
    RegisterTagItemFunction("Modify Communication Type", TAG_FUNC_LANGUAGE);

    RegisterTagItemType("Assigned Speed (if set)", TAG_ITEM_SPEED_RESTRICTION);

    /*
    m_GroundList = RegisterFpList("Ground List");

    if (m_GroundList.GetColumnNumber() == 0) {
        m_GroundList.AddColumnDefinition("C/S", 6, false,
            NULL, EuroScopePlugIn::TAG_ITEM_TYPE_CALLSIGN,
            NULL, EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
            NULL, EuroScopePlugIn::TAG_ITEM_FUNCTION_NO);
        m_GroundList.AddColumnDefinition("STS", 6, false,
            NULL, TAG_ITEM_GND_STS,
            NULL, TAG_FUNC_GND_STS,
            NULL, EuroScopePlugIn::TAG_ITEM_FUNCTION_NO);
    }
	*/
}

CVirtualCAACPlugin::~CVirtualCAACPlugin() {}

/*
void CVirtualCAACPlugin::OnAirportRunwayActivityChanged(void)
{
    DisplayUserMessage(MY_PLUGIN_NAME, "Info", "Runway changed", true, true, false, false, false);
}
*/

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

	char buf[100];
    string str;

    switch (ItemCode) {
    case TAG_ITEM_ALT_MM:
    //case TAG_ITEM_ALT_MI:
    case TAG_ITEM_ALT_MO:
        str = vCAACHandlers::GetAltitudeDisplay(FlightPlan, GetTransitionAltitude(), ItemCode);
        //sprintf_s(sItemString, 16, "%s", str.c_str());
        break;
    case TAG_ITEM_CFL_MM:
    //case TAG_ITEM_CFL_MI:
    case TAG_ITEM_CFL_MO:
        str = vCAACHandlers::GetCFLDisplay(FlightPlan, GetTransitionAltitude(), ItemCode);
        //sprintf_s(sItemString, 16, "%s", str.c_str());
        break;
    case FPL_DATA_FINAL_MM:
        //case FPL_DATA_FINAL_MI:
    case FPL_DATA_FINAL_MO:
        str = vCAACHandlers::GetFlightPlanAltitudeDisplay(FlightPlan, ItemCode);
        //sprintf_s(sItemString, 16, "%s", str.c_str());
        break;
    case TAG_ITEM_LANGUAGE:
        str = vCAACHandlers::GetLanguage(FlightPlan);
        //sprintf_s(buf, 100, "Comm type: %c", FlightPlan.GetControllerAssignedData().GetCommunicationType());
        //DisplayUserMessage(MY_PLUGIN_NAME, "Info", buf, true, true, false, false, false);
        //sprintf_s(sItemString, 16, "%s", str.c_str());
        break;
    case TAG_ITEM_SPEED_RESTRICTION:
        str = vCAACHandlers::GetSpeedRestriction(FlightPlan);
        //sprintf_s(sItemString, 16, "%s", str.c_str());
        break;
    }

	sprintf_s(sItemString, 16, "%s", str.c_str());
}

void CVirtualCAACPlugin::OnFunctionCall(int FunctionId, const char* sItemString,
    POINT Pt, RECT Area)
{
    EuroScopePlugIn::CFlightPlan fp;

    // flight plan must be valid
    fp = FlightPlanSelectASEL();
    if (!fp.IsValid())
        return;

    switch (FunctionId) {
    case TAG_FUNC_CFL_MM:
        //case TAG_FUNC_CFL_MI:
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

void CVirtualCAACPlugin::VersionCheck(void) {}
