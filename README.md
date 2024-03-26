# NZ.JQ GUI 2

## Описание

Графический интерфейс для jq.

### Возможности

- Окно ввода JSON.
- Окно ввода запроса.
- Окно вывода результата работы JQ.

## Системные требования

### Исполнение

- Windows 10 / 11
- https://jqlang.github.io/jq/

### Разработка

- cmake
- Windows 10 / 11
- Visual Studio 2022
- Windows SDK 10.0
- NAppGUI
- https://jqlang.github.io/jq/

## Сборка

### Debug

1. ./generate.cmd       - генерация файлов проекта для Visual Studio
2. ./make-debug.cmd     - компиляция

### Release

1. ./generate.cmd       - генерация файлов проекта для Visual Studio
2. ./make-release.cmd   - компиляция
3. ./publish.ps1        - создание пакета

## Запуск

1. Открыть в проводнике `.\bind\Debug\bin`
2. Запустить `NZJqGui.exe`

## Дистрибуция

- NZJqGui.exe
- jq-windows-amd64.exe

Оба файла должны лежать в одном каталоге.
