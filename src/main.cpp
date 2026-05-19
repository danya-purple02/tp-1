#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <clocale>
#include <iterator>

#include <CLI/CLI.hpp>

using namespace std;
using namespace CLI;

struct ParseResults 
{
    vector<string> headers;
    vector<string> paragraphs;
    vector<string> o_lists;
};

class CmdArgsParser 
{
private:
	int modeInt = 0; //1 - all, 2 - headers, 3 - paragraphs, 4 - o_lists
    string fileName;

    
    int makeModeInt(string mode)
    {
        if (mode == "all")
        {
            return 1;
        }
        else if (mode == "headers")
        {
            return 2;
        }
        else if (mode == "paragraphs")
        {
            return 3;
        }
        else if (mode == "o_lists")
        {
            return 4;
        }
        else return 0;
    }

public:
    bool parseArgs(int argc, char** argv)
    {
        string modeStr;
        App config{ "parser v1.0 by purple" };

        config.add_option("--mode", modeStr,
            "Режимы: \n  | all - все режимы в одном |\n"
            "   | headers - выводит только заголовки |\n"
            "   | paragraphs - выводит только параграфы |\n"
            "   | o_lists - парсит только упорядоченные одноуровневые списки |")
            ->required()
            ->check(IsMember({ "all", "headers", "paragraphs", "o_lists" }));

        config.add_option("--file", fileName, "Путь к файлу для парсинга")
            ->required();

        try
        {
            config.parse(argc, argv);

            modeInt = makeModeInt(modeStr);
            if (!modeInt)
            {
                return false;
            }

            return true;
        }
        catch (const ParseError& e)
        {
            return config.exit(e);
        }
    }

    string getFileName()
    {
        return fileName;
    }

    int getMode()
    {
        return modeInt;
    }
};

class MarkdownParser {
private:
    regex re_header = regex(R"(^\s{0,3}(#{1,6})\s+(.*)$)");
    regex re_olist = regex(R"(^\s*(\d+)\.\s+(.*)$)");

    vector<string> read_lines(const string& filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            throw runtime_error("Не удалось открыть файл: " + filename);
        }

        vector<string> lines;
        string line;

        while (getline(file, line))
            lines.push_back(line);

        return lines;
    }

    void parseHeaders(const vector<string>& lines, vector<string>& result)
    {
        for (const auto& line : lines) {
            smatch m;
            if (regex_match(line, m, re_header)) {
                result.push_back(m[2].str());
            }
        }
    }

    void parseParagraphs(const vector<string>& lines, vector<string>& result)
    {
        for (const auto& line : lines) {
            if (line.empty()) continue;

            if (regex_match(line, re_header)) continue;
            if (regex_match(line, re_olist)) continue;

            if (!line.empty())
                result.push_back(line);
        }
    }

    void parseOLists(const vector<string>& lines, vector<string>& result)
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
                            result.push_back(itemLine);
                        }
                        currentList.clear();
                    }
                }
            }
        }

        if (!currentList.empty())
        {
            if (isOrderedOneLevelList(currentList))
            {
                for (const auto& itemLine : currentList)
                {
                    result.push_back(itemLine);
                }
                currentList.clear();
            }
        }
    }

    bool isOrderedOneLevelList(const vector<string>& lines)
    {
        if (lines.empty())
            return false;

        int expected = 1;

        for (const auto& line : lines)
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

public:
    void print(int mode, struct ParseResults& res)
    {
        switch (mode)
        {
            case 1: 
            {
                cout << "заголовки: " << endl;
                for (const auto& header : res.headers)
                {
                    cout << header << endl;
                }

                cout << endl << endl << endl << "параграфы: " << endl;
                for (const auto& paragraph : res.paragraphs)
                {
                    cout << paragraph << endl;
                }

                cout << endl << endl << endl << "упорядоченные одноуровневые списки: " << endl;
                for (const auto& o_list : res.o_lists)
                {
                    cout << o_list << endl;
                }
                break;
            }
            case 2: 
            {
                cout << "заголовки: " << endl;
                for (const auto& header : res.headers)
                {
                    cout << header << endl;
                }
                break;
			}
            case 3:
            {
                cout << "параграфы: " << endl;
                for (const auto& paragraph : res.paragraphs)
                {
                    cout << paragraph << endl;
                }
                break;
            }
            case 4:
            {
                cout << "упорядоченные одноуровневые списки: " << endl;
                for (const auto& o_list : res.o_lists)
                {
                    cout << o_list << endl;
                }
                break;
            }
        }
    }

    struct ParseResults parse(const int& mode, const string& fileName)
    {
        struct ParseResults res;
		vector<string> lines = read_lines(fileName);

        switch (mode)
        {
        case 1:
        {
            parseHeaders(lines, res.headers);
            parseParagraphs(lines, res.paragraphs);
            parseOLists(lines, res.o_lists);
            break;
        }
        case 2:
        {
            parseHeaders(lines, res.headers);
            break;
        }
        case 3:
        {
            parseParagraphs(lines, res.paragraphs);
            break;
        }
        case 4:
        {
            parseOLists(lines, res.o_lists);
            break;
        }
        default:
        {
            throw runtime_error("Неверный режим: " + to_string(mode));
        }
        }
        return res;
    }
};

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian");

    CmdArgsParser config;
	if(!config.parseArgs(argc, argv)) return 99;

    MarkdownParser Mark;

    try 
    {
        struct ParseResults Results = Mark.parse(config.getMode(), config.getFileName());

        Mark.print(config.getMode(), Results);
    }
    catch (const exception& ex) 
    {
        cerr << ex.what() << "\n";
    }

    return 0;
}