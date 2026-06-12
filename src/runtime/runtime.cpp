#include "runtime.h"

#include <regex>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

static const std::regex re_header(R"(^(#{1,6})\s+(.+)$)");
static const std::regex re_olist(R"(^\s*(\d+)\.\s+(.+)$)");

static bool isOrderedOneLevelList(
    const std::vector<std::string>& listItems
)
{
    if (listItems.empty())
        return false;

    int expected = 1;

    for (const auto& line : listItems)
    {
        smatch m;
        if (!regex_match(line, m, re_olist))
             return false;

        int num = stoi(m[1].str());

        if (num != expected)
            return false;

        expected++;
    }
    return true;
}

extern "C" void parseHeaders(
    const std::vector<std::string>& lines,
    std::vector<std::string>& headers
)
{
    for (const auto& line : lines) {
        smatch m;
        if (regex_match(line, m, re_header)) {
            headers.push_back(m[2].str());
        }
    }
}

extern "C" void parseParagraphs(
    const std::vector<std::string>& lines,
    std::vector<std::string>& paragraphs
)
{
    for (const auto& line : lines) 
    {
        if (line.empty()) continue;

        if (regex_match(line, re_header)) continue;
        if (regex_match(line, re_olist)) continue;

        paragraphs.push_back(line);
    }
}

extern "C" void parseOLists(
    const std::vector<std::string>& lines,
    std::vector<std::string>& o_lists
)
{
    vector<string> currentList;

    for (auto it = lines.cbegin(); it != lines.cend(); ++it)
    {
        const string& line = *it;
        if (regex_match(line, re_olist))
        {
            currentList.push_back(line);
        }
        else
        {
            if (!currentList.empty())
            {
                if (isOrderedOneLevelList(currentList))
                {
                    for (const auto& itemLine : currentList)
                    {
                        o_lists.push_back(itemLine);
                    }
                }

                currentList.clear();
            }
        }
    }

    if (!currentList.empty())
    {
        if (isOrderedOneLevelList(currentList))
        {
            for (const auto& itemLine : currentList)
            {
                o_lists.push_back(itemLine);
            }
        }

        currentList.clear();
    }
}