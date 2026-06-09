#pragma once

#include <string>
#include <vector>

#ifdef BUILDING_LOADTIME_DLL
#define LOADTIME_API __declspec(dllexport)
#else
#define LOADTIME_API __declspec(dllimport)
#endif

LOADTIME_API void openDocument(
    const std::string& fileName,
    std::vector<std::string>& lines
);

LOADTIME_API void closeDocument(
    std::vector<std::string>& lines
);