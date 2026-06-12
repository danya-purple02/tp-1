This README generated with the assistance of AI.

# MarkdownParser

A C++ console application for extracting elements from a Markdown document.

The project was developed as part of a laboratory work for the course “Technology of Programming”.

## Features

The program extracts the following elements from a Markdown file:

- headings;
- paragraphs;
- single-level ordered lists.

Supported modes:

```text
all         print all found elements
headers     print headings only
paragraphs  print paragraphs only
o_lists     print single-level ordered lists only
```

## Implementation details

The project uses two dynamically linked libraries:

```text
loadtime.dll
```

The first library is loaded implicitly when the program starts.

It is responsible for working with the Markdown document:

- opening the file;
- reading file lines;
- closing the document / clearing data after processing.

```text
runtime.dll
```

The second library is loaded explicitly at runtime using Windows API:

- `LoadLibraryA`;
- `GetProcAddress`;
- `FreeLibrary`.

It is responsible for searching Markdown elements:

- `parseHeaders`;
- `parseParagraphs`;
- `parseOLists`.

## Project structure

```text
MarkdownParser/
│
├─ .vscode/
│  └─ tasks.json
│
├─ build/
│  ├─ main.exe
│  ├─ loadtime.dll
│  ├─ libloadtime.a
│  └─ runtime.dll
│
├─ data/
│  └─ markdown_mixed.md
│
├─ reports/
│
├─ src/
│  ├─ main.cpp
│  │
│  ├─ loadtime/
│  │  ├─ loadtime.h
│  │  └─ loadtime.cpp
│  │
│  └─ runtime/
│     ├─ runtime.h
│     ├─ runtime.cpp
│     └─ runtime.def
│
├─ third_party/
│  └─ CLI11/
│
├─ README.md
├─ README_RU.md
└─ README_EN.md
```

## Main files

### `src/main.cpp`

The main application file.

It is responsible for:

- parsing command-line arguments;
- calling functions from `loadtime.dll`;
- explicitly loading `runtime.dll`;
- getting function addresses using `GetProcAddress`;
- calling Markdown element parsing functions;
- printing results;
- handling exceptional situations.

### `src/loadtime/loadtime.h`

Header file of the first DLL.

It declares the following functions:

```text
openDocument
closeDocument
```

It also contains the `LOADTIME_API` macro used for DLL function export and import.

### `src/loadtime/loadtime.cpp`

Implementation of the first DLL.

The `openDocument` function opens a Markdown file, reads its lines, and passes them to the main program.

The `closeDocument` function clears the container with document lines after processing.

### `src/runtime/runtime.h`

Header file of the second DLL.

It contains function pointer types:

```text
ParseHeadersFunc
ParseParagraphsFunc
ParseOListsFunc
```

These types are used in `main.cpp` after calling `GetProcAddress`.

### `src/runtime/runtime.cpp`

Implementation of the second DLL.

It contains the following functions:

```text
parseHeaders
parseParagraphs
parseOLists
```

It also contains the helper function `isOrderedOneLevelList`, which is used inside `parseOLists`.

### `src/runtime/runtime.def`

Module-definition file for exporting functions from the second DLL.

It contains the list of functions exported from `runtime.dll`:

```text
parseHeaders
parseParagraphs
parseOLists
```

## Dependencies

The project uses CLI11 for command-line argument parsing.

If the `third_party/CLI11` folder is missing, it can be added with:

```bash
git clone --depth 1 https://github.com/CLIUtils/CLI11.git third_party/CLI11
```

## Build

The project is built in Visual Studio Code using tasks from `.vscode/tasks.json`.

Build order:

```text
Terminal → Run Task... → Build loadtime dll
Terminal → Run Task... → Build runtime dll
Terminal → Run Task... → Build main
```

After building, the `build` directory should contain:

```text
main.exe
loadtime.dll
libloadtime.a
runtime.dll
```

## Run

Run with all elements:

```powershell
./build/main.exe --mode all --file "./data/markdown_mixed.md"
```

Print headings only:

```powershell
./build/main.exe --mode headers --file "./data/markdown_mixed.md"
```

Print paragraphs only:

```powershell
./build/main.exe --mode paragraphs --file "./data/markdown_mixed.md"
```

Print single-level ordered lists only:

```powershell
./build/main.exe --mode o_lists --file "./data/markdown_mixed.md"
```

## Error handling

The program handles the following exceptional situations:

- invalid operating mode;
- inability to open the Markdown file;
- inability to load `runtime.dll`;
- inability to get function addresses from `runtime.dll`.

## Technologies

- C++;
- GCC / MinGW;
- Visual Studio Code;
- Windows API;
- dynamic-link libraries;
- load-time dynamic linking;
- run-time dynamic linking;
- regular expressions;
- CLI11.

## Author

author — danya_purple02  
creation date — 05.03.2026

Repository on GitHub — https://github.com/danya-purple02/TP-1.git