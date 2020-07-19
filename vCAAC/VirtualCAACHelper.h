#pragma once
#include "pch.h"

#include "EuroScopePlugIn.h"

const int TAG_ITEM_ALT_MM = 1, TAG_ITEM_ALT_MI = 2, TAG_ITEM_ALT_MO = 3,
          TAG_ITEM_CFL_MM = 11, TAG_ITEM_CFL_MI = 12, TAG_ITEM_CFL_MO = 13,
          TAG_ITEM_RFL_MM = 21, TAG_ITEM_RFL_MI = 22, TAG_ITEM_RFL_MO = 23,
          FPL_DATA_FINAL_MM = 31, FPL_DATA_FINAL_MI = 32, FPL_DATA_FINAL_MO = 33,
          TAG_ITEM_GND_STS = 101;

// experimental
const int TAG_ITEM_LANGUAGE = 997;
const int TAG_ITEM_RAM_OFFSET = 998;
const int TAG_ITEM_SPEED_RESTRICTION = 999;

const int TAG_FUNC_CFL_MM = 1, TAG_FUNC_CFL_MI = 2, TAG_FUNC_CFL_MO = 3, TAG_FUNC_CFL_SET = 4,
          TAG_FUNC_RFL_MM = 11, TAG_FUNC_RFL_MI = 12, TAG_FUNC_RFL_MO = 13, TAG_FUNC_RFL_SET = 14;

// experimental
const int TAG_FUNC_LANGUAGE = 101, TAG_FUNC_SET_LANGUAGE = 102;
const int TAG_FUNC_RAM_OFFSET = 111, TAG_FUNC_SET_OFFSET = 112;
const int TAG_FUNC_SPEED_RESTRICTION = 999;

const double DIV_TO_FEET = 0.3048;

inline static bool
startsWith(const char* pre, const char* str)
{
    size_t lenpre = strlen(pre), lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}
