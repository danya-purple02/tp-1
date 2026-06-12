#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <clocale>
#include <stdexcept>

#include <CLI/CLI.hpp>
#include "loadtime.h"
#include "runtime.h"

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
            config.exit(e);
            return false;
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
    vector<string> read_lines(const string& filename)
    {
        vector<string> lines;

        openDocument(filename, lines);

        return lines;
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

        HMODULE runtimeDll = LoadLibraryA("runtime.dll");

        if (!runtimeDll)
        {
            runtimeDll = LoadLibraryA("build\\runtime.dll");
        }

        if (!runtimeDll)
        {
            throw runtime_error("Не удалось загрузить runtime.dll");
        }

        ParseHeadersFunc parseHeadersDll =
            reinterpret_cast<ParseHeadersFunc>(
                GetProcAddress(runtimeDll, "parseHeaders")
            );

        ParseParagraphsFunc parseParagraphsDll =
            reinterpret_cast<ParseParagraphsFunc>(
                GetProcAddress(runtimeDll, "parseParagraphs")
            );

        ParseOListsFunc parseOListsDll =
            reinterpret_cast<ParseOListsFunc>(
                GetProcAddress(runtimeDll, "parseOLists")
            );

        if (!parseHeadersDll || !parseParagraphsDll || !parseOListsDll)
        {
            FreeLibrary(runtimeDll);
            throw runtime_error("Не удалось получить функции из runtime.dll");
        }

        switch (mode)
        {
        case 1:
        {
            parseHeadersDll(lines, res.headers);
            parseParagraphsDll(lines, res.paragraphs);
            parseOListsDll(lines, res.o_lists);
            break;
        }
        case 2:
        {
            parseHeadersDll(lines, res.headers);
            break;
        }
        case 3:
        {
            parseParagraphsDll(lines, res.paragraphs);
            break;
        }
        case 4:
        {
            parseOListsDll(lines, res.o_lists);
            break;
        }
        default:
        {
            FreeLibrary(runtimeDll);
            throw runtime_error("Неверный режим: " + to_string(mode));
        }
        }

        FreeLibrary(runtimeDll);
        closeDocument(lines);
        return res;
    }
};

int main(int argc, char** argv)
{
    //setlocale(LC_ALL, "Russian");

    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");

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