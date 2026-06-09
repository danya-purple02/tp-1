#include "loadtime.h"

#include <fstream>
#include <stdexcept>

void openDocument(
    const std::string& fileName,
    std::vector<std::string>& lines
)
{
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        throw std::runtime_error("Не удалось открыть файл: " + fileName);
    }

    lines.clear();

    std::string line;

    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
}

void closeDocument(
    std::vector<std::string>& lines
)
{
    lines.clear();
}