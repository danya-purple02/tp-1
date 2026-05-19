This README generated with the assistance of AI.

# MarkdownParser

Console application written in C++ for parsing Markdown files and extracting selected markup elements.

The program receives a Markdown file path through command-line arguments and outputs the extracted content according to the selected mode.

## Project information

**Author:** danya_purple02  
**Creation date:** 05.03.2026  
**Repository on GitHub:** [TP-1](https://github.com/danya-purple02/TP-1.git)

## Description

This project was created as part of a laboratory work on C++ programming.

The application works with Markdown documents and can extract:

- headers;
- paragraphs;
- ordered one-level lists.

Command-line arguments are processed using the CLI11 library.

## Project structure

```text
MarkdownParser/
│
├─ .vscode/
│  ├─ c_cpp_properties.json
│  └─ tasks.json
│
├─ build/
│  └─ main.exe
│
├─ data/
│  └─ test Markdown files
│
├─ src/
│  └─ main.cpp
│
├─ third_party/
│  └─ CLI11/
│
├─ README.md
├─ README_RU.md
└─ README_EN.md
```

## Requirements

To build and run the project, you need:

- Windows;
- Visual Studio Code;
- C/C++ extension for VS Code;
- MinGW-w64 / WinLibs GCC;
- CLI11 library.

The project uses the following compiler:

```text
G++ 16.1.0
```

## Build

The project is built through the configured VS Code build task.

Press:

```text
Ctrl + Shift + B
```

After the build, the executable file will be created in the following directory:

```text
build/main.exe
```

Manual build command example:

```powershell
C:\drivers\mingw64\bin\g++.exe -fdiagnostics-color=always -g -std=c++20 -I third_party\CLI11\include src\main.cpp -o build\main.exe
```

## Run

Example launch command:

```powershell
.\build\main.exe --mode all --file ".\data\markdown_mixed.md"
```

## Command-line arguments

```text
--mode    parsing mode
--file    path to the Markdown file
```

Example:

```powershell
.\build\main.exe --mode all --file ".\data\test.md"
```

## Used libraries

### CLI11

This project uses the [CLI11](https://github.com/CLIUtils/CLI11) library for command-line argument parsing.

CLI11 is a header-only library, so it is enough to download its source files into the project and add the include directory to the build configuration. The project uses the following include directive:

```cpp
#include <CLI/CLI.hpp>
```

To install CLI11 into the project, run the following command from the repository root directory:

```powershell
git clone https://github.com/CLIUtils/CLI11.git third_party\CLI11
```

After running the command, the following file should appear in the project:

```text
third_party\CLI11\include\CLI\CLI.hpp
```


## Notes

The project is configured to use UTF-8 encoding for correct processing and output of Russian text in the terminal.