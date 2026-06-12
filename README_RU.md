Этот README сгенерирован при помощи искусственного интеллекта.
# MarkdownParser

Консольное приложение на C++ для извлечения элементов из Markdown-документа.

Проект выполнен в рамках лабораторной работы по дисциплине «Технология программирования».

## Возможности

Программа извлекает из Markdown-файла:

- заголовки;
- абзацы;
- упорядоченные одноуровневые списки.

Поддерживаемые режимы работы:

```text
all         вывести все найденные элементы
headers     вывести только заголовки
paragraphs  вывести только абзацы
o_lists     вывести только упорядоченные одноуровневые списки
```

## Особенности реализации

Проект использует две динамически подключаемые библиотеки:

```text
loadtime.dll
```

Первая библиотека подключается неявно во время загрузки программы.

Она отвечает за работу с Markdown-документом:

- открытие файла;
- чтение строк файла;
- закрытие документа / очистку данных после обработки.

```text
runtime.dll
```

Вторая библиотека подключается явно во время выполнения программы с помощью Windows API:

- `LoadLibraryA`;
- `GetProcAddress`;
- `FreeLibrary`.

Она отвечает за поиск элементов Markdown-документа:

- `parseHeaders`;
- `parseParagraphs`;
- `parseOLists`.

## Структура проекта

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

## Назначение основных файлов

### `src/main.cpp`

Основной файл программы.

Отвечает за:

- обработку аргументов командной строки;
- вызов функций из `loadtime.dll`;
- явную загрузку `runtime.dll`;
- получение адресов функций через `GetProcAddress`;
- вызов функций поиска элементов;
- вывод результата;
- обработку исключительных ситуаций.

### `src/loadtime/loadtime.h`

Заголовочный файл первой DLL.

Содержит объявления функций:

```text
openDocument
closeDocument
```

Также содержит макрос `LOADTIME_API`, который используется для экспорта и импорта функций DLL.

### `src/loadtime/loadtime.cpp`

Реализация первой DLL.

Функция `openDocument` открывает Markdown-файл, считывает его строки и передаёт их основной программе.

Функция `closeDocument` очищает контейнер со строками документа после завершения обработки.

### `src/runtime/runtime.h`

Заголовочный файл второй DLL.

Содержит типы указателей на функции:

```text
ParseHeadersFunc
ParseParagraphsFunc
ParseOListsFunc
```

Эти типы используются в `main.cpp` после вызова `GetProcAddress`.

### `src/runtime/runtime.cpp`

Реализация второй DLL.

Содержит функции:

```text
parseHeaders
parseParagraphs
parseOLists
```

Также содержит вспомогательную функцию `isOrderedOneLevelList`, которая используется внутри `parseOLists`.

### `src/runtime/runtime.def`

Файл экспорта функций второй DLL.

Содержит список функций, которые должны быть доступны из `runtime.dll`:

```text
parseHeaders
parseParagraphs
parseOLists
```

## Зависимости

Проект использует библиотеку CLI11 для обработки аргументов командной строки.

Если папки `third_party/CLI11` нет, её можно добавить командой:

```bash
git clone --depth 1 https://github.com/CLIUtils/CLI11.git third_party/CLI11
```

## Сборка проекта

Проект собирается в Visual Studio Code через задачи из файла `.vscode/tasks.json`.

Порядок сборки:

```text
Terminal → Run Task... → Build loadtime dll
Terminal → Run Task... → Build runtime dll
Terminal → Run Task... → Build main
```

После сборки в папке `build` должны появиться файлы:

```text
main.exe
loadtime.dll
libloadtime.a
runtime.dll
```

## Запуск программы

Пример запуска с выводом всех элементов:

```powershell
./build/main.exe --mode all --file "./data/markdown_mixed.md"
```

Вывод только заголовков:

```powershell
./build/main.exe --mode headers --file "./data/markdown_mixed.md"
```

Вывод только абзацев:

```powershell
./build/main.exe --mode paragraphs --file "./data/markdown_mixed.md"
```

Вывод только упорядоченных одноуровневых списков:

```powershell
./build/main.exe --mode o_lists --file "./data/markdown_mixed.md"
```

## Обработка ошибок

В программе предусмотрена обработка исключительных ситуаций:

- неверный режим работы;
- невозможность открыть Markdown-файл;
- невозможность загрузить `runtime.dll`;
- невозможность получить адреса функций из `runtime.dll`.

## Используемые технологии

- C++;
- GCC / MinGW;
- Visual Studio Code;
- Windows API;
- динамически подключаемые библиотеки;
- load-time dynamic linking;
- run-time dynamic linking;
- регулярные выражения;
- CLI11.

## Автор

author — danya_purple02  
creation date — 05.03.2026

Repository on GitHub — https://github.com/danya-purple02/TP-1.git