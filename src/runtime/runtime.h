#pragma once

#include <string>
#include <vector>

typedef void (*ParseHeadersFunc)(
    const std::vector<std::string>& lines,
    std::vector<std::string>& headers
);

typedef void (*ParseParagraphsFunc)(
    const std::vector<std::string>& lines,
    std::vector<std::string>& paragraphs
);

typedef void (*ParseOListsFunc)(
    const std::vector<std::string>& lines,
    std::vector<std::string>& o_lists
);