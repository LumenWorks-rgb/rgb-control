# RGB Control

**English** · [Русский](README.md)

A lightweight Windows app for RGB lighting, fan & pump curves and microphone processing — all in one small window.

**[⬇ Download the latest version](../../releases/latest)** — `RGB Control Setup X.Y.exe`

![RGB Control — main window](docs/screenshot-main.png)

> The interface is currently in Russian. An English UI is planned.

---

## Features

- **Lighting** — effects (rainbow, wave, breathing, fire, meteors and more), custom colors, all devices in sync. Fans on the motherboard can be mapped one by one.
- **Cooling** — fan and pump curves by CPU temperature, fixed speed or "as in BIOS". On overheating all fans jump to 100% automatically.
- **Sound** — OBS-style microphone processing: noise suppression, noise gate, EQ, compressor, limiter. The processed voice can go to Discord and games via VB-Audio Virtual Cable.
- **Setup check** — on first launch the app scans your PC against a catalog of ~2,500 RGB devices and shows what it controls and what is not supported yet.
- **Stays out of the way** — almost no CPU load, lives in the tray, starts with Windows.

| Cooling | Sound |
|---|---|
| ![Cooling](docs/screenshot-cooling.png) | ![Sound](docs/screenshot-sound.png) |

## Supported hardware

### ✅ Tested

| Type | Models |
|---|---|
| Motherboards (ARGB headers) | ASRock Polychrome USB — tested on ASRock B550M Pro RS and B550M Pro SE |
| Cooling | Fans and pump via Nuvoton chips (tested on NCT6796D-R) |
| Graphics cards | MSI RTX 5060 Gaming Trio |
| RAM | ENE DIMM_LED-0103 controllers (ADATA XPG) |
| Keyboards | Cidoo QK75 (per-key lighting with the unofficial firmware from [`firmware-qk75`](firmware-qk75), at your own risk) |
| Mice | Attack Shark X11 |
| Mixers | FIFINE AmpliGame SC3 (lighting needs the unofficial [RGB+](https://github.com/wopln/FIFINE-SC3-RGB-Plus) firmware, at your own risk) |
| Microphones | FIFINE AmpliGame AM9 |

### 🟡 Expected to work, not tested yet

- **RAM** with other ENE controllers — ASUS, G.Skill Trident Z, TeamGroup T-Force, Patriot Viper and others.
- **Graphics cards** — other MSI RTX 40/50 series.
- **Cooling** on other motherboards supported by LibreHardwareMonitor.

If you have such hardware, please try it and tell us whether it works.

### ❌ Not supported yet

ASUS / MSI / Gigabyte motherboards, graphics cards from other vendors, Corsair and Kingston Fury RAM, Razer / Logitech / SteelSeries peripherals and much more. Support is added based on reports — see below.

## Installation

1. Download `RGB Control Setup X.Y.exe` from [Releases](../../releases/latest) and run it.
2. Windows may show "Windows protected your PC" — the installer is not code-signed yet. Click **More info → Run anyway**.
3. The app installs for the current user, no admin rights needed. Optionally the installer:
   - installs the **PawnIO** driver — needed for fans, pump and RAM lighting;
   - enables **"Always run as administrator"** — cooling and RAM only work with admin rights; Windows asks once.
4. New versions install on top; settings are kept. Uninstall via Settings → Apps.

**Requirements:** Windows 10 or 11 (64-bit). To send the processed voice to other apps — the free [VB-Audio Virtual Cable](https://vb-audio.com/Cable/).

## Something doesn't work?

The easiest way is right from the app: the **«Не работает?»** ("Not working?") button on a device screen, or **⚙ → «Сообщить о проблеме»** ("Report a problem"). Describe the issue in a few words and press send. The author receives your text, hardware info and the error log — no personal data; you can preview everything before sending.

Or open an [issue](../../issues/new/choose) and attach the compatibility report: **⚙ → «Сохранить отчёт совместимости»** ("Save compatibility report") — it appears on your desktop.

Want your device supported? Send the report as well — it shows how the device works.

## Safety

- The app only touches lighting and fans. It never writes to RAM SPD or BIOS settings.
- Unknown devices never receive commands — the app only reads what they report about themselves.
- On exit, devices return to their own effects and fans go back to BIOS control.

> [!WARNING]
> **Device firmware is unofficial, optional and at your own risk.** Manufacturers did not release or support it, warranty may be void, and LumenWorks is not responsible for your devices.
> - **Cidoo QK75** — the stock firmware is not included (you can request it from Cidoo support). Don't flash unless you are sure.
> - **FIFINE SC3** — the [RGB+](https://github.com/wopln/FIFINE-SC3-RGB-Plus) firmware (by wopln) is not part of RGB Control: download it and its tool from the author and follow their instructions.
>
> Everything else in RGB Control works without this firmware.

## Author and license

© 2026 **LumenWorks**. Free but closed-source — see [LICENSE.txt](LICENSE.txt). Third-party components and their licenses: [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md).

The Cidoo QK75 firmware source is open under GPL-2.0 — see [`firmware-qk75`](firmware-qk75).
