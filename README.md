# Wincess (Windows Access & Personalization Control)

**Wincess** is a lightweight, open-source C++ command-line utility designed to disable Windows activation watermarks and bypass standard personalization limits without modifying system core files or using KMS activators.

## Features
* **Watermark Remover:** Disables the `svsvc` background service responsible for the "Activate Windows" overlay.
* **Alternative Personalization:** Directly changes wallpapers via WinAPI (`SystemParametersInfoA`) and toggles System/Apps themes via Windows Registry.
* **Persistence Agent:** Includes an optional background task (`--silent` mode) via Windows Task Scheduler to maintain tweaks after major OS updates.
* **100% Safe:** Zero system patching, no external DLL dependencies (statically linked).

## How to Build (Code::Blocks / MinGW)
1. Open project in Code::Blocks.
2. Ensure `advapi32` is added to **Linker Settings**.
3. Add static linking flags: `-static-libgcc -static-libstdc++ -static`.
4. Compile and run as Administrator.

## Usage
Run `wincess.exe` as Administrator to access the main CLI menu:
- Option `1`: Apply tweaks & enable startup auto-check.
- Option `2`: Apply custom wallpaper by path.
- Option `3/4`: Toggle Light/Dark system themes.

## Support the Project ☕
If you find Wincess useful and want to support its development, you can send a crypto donation via MetaMask (EVM networks: Ethereum, BNB Chain, Polygon, Arbitrum, etc.):

**MetaMask (EVM Address):**
`0x4B4d23cdeD10F5585B219066127422c62D8D09C6`

---

## License
This project is licensed under the **GNU General Public License v3.0 (GPL-3.0)**. See the [LICENSE](LICENSE) file for details.

> **Disclaimer:** Wincess is created for customization and educational purposes only. It does not generate genuine Microsoft licenses.
