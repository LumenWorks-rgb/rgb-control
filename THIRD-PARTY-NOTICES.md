# Сторонние компоненты

RGB Control использует следующие компоненты. Все они встроены без изменений; тексты лицензий также лежат в папке «Лицензии» рядом с установленной программой.

| Компонент | Для чего | Лицензия | Исходники |
|---|---|---|---|
| NAudio 1.10.0 (© Mark Heath) | захват и обработка звука микрофона | MIT | https://github.com/naudio/NAudio |
| LibreHardwareMonitorLib 0.9.6 | датчики, вентиляторы и помпа, шина памяти | MPL-2.0 | https://github.com/LibreHardwareMonitor/LibreHardwareMonitor |
| RAMSPDToolkit-NDD, DiskInfoToolkit, BlackSharp.Core 1.0.7 | зависимости LibreHardwareMonitor | MPL-2.0 | https://www.nuget.org (страницы пакетов) |
| HidSharp | зависимость LibreHardwareMonitor | Apache-2.0 | https://www.zer7.com/software/hidsharp |
| System.Memory, System.Buffers, System.Numerics.Vectors, System.Runtime.CompilerServices.Unsafe, System.Security.AccessControl, System.Security.Principal.Windows, System.Threading.AccessControl | библиотеки .NET | MIT | https://github.com/dotnet/runtime |
| PawnIO 2.2.0 — установщик драйвера (namazso) | доступ к вентиляторам и шине памяти | GPL-2.0 | https://github.com/namazso/PawnIO, https://github.com/namazso/PawnIO.Setup |
| Модули PawnIO | то же | LGPL-2.1-or-later | https://github.com/namazso/PawnIO.Modules |
| Каталог устройств (выжимка из списка устройств OpenRGB) | распознавание комплектующих: названия, коды USB/PCI, протокол | GPL-2.0 (проект OpenRGB) | https://openrgb.org/devices.html |

Установщик PawnIO распространяется в неизменном виде (SHA256 `1F519A22E47187F70A1379A48CA604981C4FCF694F4E65B734AAA74A9FBA3032`); устанавливается только по выбору пользователя.

## Прошивка Cidoo QK75

Прошивка для клавиатуры Cidoo QK75 собрана на основе QMK Firmware (форк [aliveoutside/qmk_firmware](https://github.com/aliveoutside/qmk_firmware), ветка `qk61`) и распространяется по лицензии **GNU GPL v2**. Полные исходники — в папке [`firmware-qk75`](firmware-qk75).
