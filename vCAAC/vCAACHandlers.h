#pragma once
#include "pch.h"

#include "EuroScopePlugIn.h"

#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace EuroScopePlugIn;

class vCAACHandlers {
public:
    static string GetAltitudeDisplay(CFlightPlan fp, const int trans_alt, const int type);
    static string GetCFLDisplay(CFlightPlan fp, const int trans_alt, const int type);
    static string GetFlightPlanAltitudeDisplay(CFlightPlan fp, const int type);
    static string GetLanguage(CFlightPlan fp);
    static string GetRAMWithOffset(CFlightPlan fp);
    static string GetSpeedRestriction(CFlightPlan fp);

    static vector<tuple<string, string, bool>> GetCFLList(CFlightPlan fp);
    static vector<tuple<string, string>> GetLanguageList(CFlightPlan fp);

    static bool SetCFL(CFlightPlan fp, const char* altitude_meter_display);
    static bool SetLanguage(CFlightPlan fp, const char* language_type);

private:
    static const size_t TAG_BUFFER_SIZE = 10;
    static vector<int> levels_mixed_meter;
    static vector<int> levels_mixed_feet;
    static vector<string> tag_func_display_format;
    static map<char, string> communication_type;
    static map<string, char> communication_type_rev;
};