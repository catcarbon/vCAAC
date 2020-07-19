#include "pch.h"

#include "VirtualCAACPlugin.h"

#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace EuroScopePlugIn;

vector<int> vCAACHandlers::levels_mixed_meter{
    14900, 14300, 13700, 13100, 12500, 12200, 11900, 11600, 11300, 11000, 10700,
    10400, 10100, 9800, 9500, 9200, 8900, 8400, 8100, 7800, 7500, 7200,
    6900, 6600, 6300, 6000, 5700, 5400, 5100, 4800, 4500, 4200, 3900,
    3600, 3300, 3000, 2700, 2400, 2100, 1800, 1500, 1200, 900,
    850, 800, 750, 700, 650, 600, 550, 500, 450, 300, 100
};
vector<int> vCAACHandlers::levels_mixed_feet{
    48900, 46900, 44900, 43000, 41100, 40100, 39100, 38100, 37100, 36100, 35100,
    34100, 33100, 32100, 31100, 30100, 29100, 27600, 26600, 25600, 24600, 23600,
    22600, 21700, 20700, 19700, 18700, 17700, 16700, 15700, 14800, 13800, 12800,
    11800, 10800, 9800, 8900, 7900, 6900, 5900, 4900, 3900, 3000,
    2800, 2600, 2500, 2300, 2100, 2000, 1800, 1600, 1500, 1000, 300
};
vector<string> vCAACHandlers::tag_func_display_format{
    "----", "%04d/F%03d", "%04d", "%04d"
};

map<char, string> vCAACHandlers::communication_type{
    { '0', "  " }, { 'v', "  " }, { 'r', "/EN" }, { 't', "/NORDO" }
};

map<string, char> vCAACHandlers::communication_type_rev{
    { "  ", 'v' }, { "/EN", 'r' }, { "/NORDO", 't' }
};

/* TODO: implement imperial altitude
vector<int> CFLImperialMeter{
    14930, 14330, 13720, 13110, 12500, 12190, 11890, 11580, 11280, 10970,
    10670, 10360, 10060, 9750, 9950, 9140, 8840, 8530, 8230, 7920,
    7620, 7320, 7010, 6700, 6400, 6100, 5970, 5490, 5180, 4880,
    4570, 4270, 3960, 3660, 3350, 3050, 2740, 2440, 2130, 1830,
    1520, 1220, 910, 760, 610, 460, 300, 20
};

vector<int> CFLImperialFeet{
    49000, 47000, 45000, 43000, 41000, 40000, 39000, 38000, 37000, 36000,
    35000, 34000, 33000, 32000, 31000, 30000, 29000, 28000, 27000, 26000,
    25000, 24000, 23000, 22000, 21000, 20000, 19000, 18000, 17000, 16000,
    15000, 14000, 13000, 12000, 11000, 10000, 9000, 8000, 7000, 6000,
    5000, 4000, 3000, 2500, 2000, 1500, 1000, 500
};
*/

string vCAACHandlers::GetAltitudeDisplay(CFlightPlan fp, const int trans_alt, const int type)
{
    int alt = fp.GetCorrelatedRadarTarget().GetPosition().GetPressureAltitude();
    if (alt > trans_alt) {
        alt = fp.GetCorrelatedRadarTarget().GetPosition().GetFlightLevel();
    }

    char buf[16];
    sprintf_s(buf, 16, tag_func_display_format[type % 10].c_str(),
        (int)round(alt * 0.03048),
        (int)round(alt / 100.0));

    return string(buf);
}

string vCAACHandlers::GetCFLDisplay(CFlightPlan fp, const int trans_alt, const int type)
{
    int altitude_radar = fp.GetCorrelatedRadarTarget().GetPosition().GetPressureAltitude();
    if (altitude_radar > trans_alt) {
        altitude_radar = fp.GetCorrelatedRadarTarget().GetPosition().GetFlightLevel();
    }
    // directly converted feet value
    int altitude_cfl = fp.GetControllerAssignedData().GetClearedAltitude();

    if (altitude_cfl && altitude_cfl <= 2) {
        return string(1, (altitude_cfl == 1) ? 0xAE : 0xA9);
    }

    if (abs(altitude_radar - altitude_cfl) <= 150 || altitude_cfl == 0) {
        return string("");
    }

    char buf[16];
    sprintf_s(buf, 16, tag_func_display_format[type % 10].c_str(),
        (int)round(altitude_cfl / 10.0 * DIV_TO_FEET),
        (int)round(altitude_cfl / 100.0));

    return string(buf);
}

string vCAACHandlers::GetFlightPlanAltitudeDisplay(CFlightPlan fp, const int type)
{
    int altitude_planned = fp.GetFlightPlanData().GetFinalAltitude();

    // do not display planned altitude less than 3000 ft for unmitigated rounding error.
    if (altitude_planned <= 3000) {
        return string("");
    }

    char buf[16];
    sprintf_s(buf, 16, tag_func_display_format[type % 10].c_str(),
        (int)round(altitude_planned / 10.0 * DIV_TO_FEET),
        (int)round(altitude_planned / 100.0));

    return string(buf);
}

string vCAACHandlers::GetLanguage(CFlightPlan fp)
{
    char comm_type = fp.GetControllerAssignedData().GetCommunicationType();
    auto ct_iter = communication_type.find(comm_type);
    if (ct_iter != communication_type.end()) {
        return communication_type[comm_type];
    }
	// could be out of my control
    //fp.GetControllerAssignedData().SetCommunicationType('V');
    return string("  ");
}

string vCAACHandlers::GetRAMWithOffset(CFlightPlan fp)
{
    return string("");
}

string vCAACHandlers::GetSpeedRestriction(CFlightPlan fp)
{
    int assigned_speed = fp.GetControllerAssignedData().GetAssignedSpeed();

    char buf[16];
    if (assigned_speed == 0) {
        assigned_speed = fp.GetControllerAssignedData().GetAssignedMach();
        if (assigned_speed == 0) {
            return string("");
        }
        sprintf_s(buf, 16, "M%.2f", assigned_speed / 100.0);
    } else {
        sprintf_s(buf, 16, "S%03d", assigned_speed);
    }

    return string(buf);
}

// Functions

vector<tuple<string, string, bool>> vCAACHandlers::GetCFLList(CFlightPlan fp)
{
    // Must use GetControllerAssignedData(), or GetClearedAltitude() will give FPL cruise alt when neither CFL nor RFL is set.
    int altitude_reference = fp.GetControllerAssignedData().GetClearedAltitude();
    if (altitude_reference <= 2) {
        altitude_reference = fp.GetCorrelatedRadarTarget().GetPosition().GetPressureAltitude();
    }

    vector<tuple<string, string, bool>> ret;

    char meter_field[8], feet_field[8];
    bool selected = false;
    for (unsigned int i = 0; i < levels_mixed_meter.size(); i++) {
        snprintf(meter_field, 8, "%04d", levels_mixed_meter[i] / 10);
        snprintf(feet_field, 8, "/F%03d", levels_mixed_feet[i] / 100);
        ret.push_back(std::make_tuple(
            string(meter_field), string(feet_field), !selected && levels_mixed_feet[i] <= altitude_reference));
        selected = levels_mixed_feet[i] <= altitude_reference;
    }

    return ret;
}

vector<tuple<string, string>> vCAACHandlers::GetLanguageList(CFlightPlan fp)
{
    vector<tuple<string, string>> ret;
    ret.push_back(make_tuple(string("  "), string(" Chinese")));
    ret.push_back(make_tuple(string("/EN"), string(" English")));
    ret.push_back(make_tuple(string("/NORDO"), string(" No Radio")));

    return ret;
}

// setters

bool vCAACHandlers::SetCFL(CFlightPlan fp, const char* altitude_meter_display)
{
    int altitude_meter = atoi(altitude_meter_display);

    if (altitude_meter == 0) {
        if (strncmp(altitude_meter_display, "    ", 4) == 0) {
            return fp.GetControllerAssignedData().SetClearedAltitude(0);
        }
        if (strncmp(altitude_meter_display, "IAPP", 4) == 0) {
            return fp.GetControllerAssignedData().SetClearedAltitude(1);
        }
        if (strncmp(altitude_meter_display, "VAPP", 4) == 0) {
            return fp.GetControllerAssignedData().SetClearedAltitude(2);
        }
    }

    // altitude_meter_display is by 10-meter, i.e. "0980" means 9800 meters
    double altitude_feet = atoi(altitude_meter_display) * 10.0 / DIV_TO_FEET;

    // produces direct convertion from metric to feet
    // this conversion is correct except for the following RVSM altitudes
    // 1250/F411, 1220/F401, 1190/F391 -> round down 100 feet
    // 0890/F291, 0920/F301, 0950/F311, 0980/F321 -> round up 100 feet
    return fp.GetControllerAssignedData().SetClearedAltitude((int)round(altitude_feet));
}

bool vCAACHandlers::SetLanguage(CFlightPlan fp, const char* language_type)
{
    string language_type_str = string(language_type);
    auto ctr_iter = communication_type_rev.find(language_type_str);
    if (ctr_iter != communication_type_rev.end()) {
        return fp.GetControllerAssignedData().SetCommunicationType(communication_type_rev[language_type_str]);
	}
    return fp.GetControllerAssignedData().SetCommunicationType('v');
}
