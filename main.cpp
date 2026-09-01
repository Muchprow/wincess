#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <string>

// 1. Admin check
BOOL IsRunAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin;
}

// 2. Disable Watermark (svsvc Start = 4)
bool RemoveWatermark() {
    HKEY hKey;
    DWORD startType = 4;
    LPCSTR subkey = "SYSTEM\\CurrentControlSet\\Services\\svsvc";

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subkey, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, "Start", 0, REG_DWORD, (const BYTE*)&startType, sizeof(startType));
        RegCloseKey(hKey);
        return true;
    }
    return false;
}

// 3. Change Wallpaper via WinAPI
bool SetWallpaper(const std::string& path) {
    return SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (void*)path.c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
}

// 4. Toggle System & App Theme (0 = Dark, 1 = Light)
void SetSystemTheme(bool isLight) {
    HKEY hKey;
    DWORD value = isLight ? 1 : 0;
    LPCSTR subkey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";

    if (RegOpenKeyExA(HKEY_CURRENT_USER, subkey, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, "AppsUseLightTheme", 0, REG_DWORD, (const BYTE*)&value, sizeof(value));
        RegSetValueExA(hKey, "SystemUsesLightTheme", 0, REG_DWORD, (const BYTE*)&value, sizeof(value));
        RegCloseKey(hKey);
    }
}

int main(int argc, char* argv[]) {
    // Silent mode for Task Scheduler
    if (argc > 1 && std::string(argv[1]) == "--silent") {
        if (IsRunAsAdmin()) RemoveWatermark();
        return 0;
    }

    std::cout << "===========================================" << std::endl;
    std::cout << "             W I N C E S S                 " << std::endl;
    std::cout << "  Alternative Personalization & Watermark  " << std::endl;
    std::cout << "===========================================\n" << std::endl;

    if (!IsRunAsAdmin()) {
        std::cerr << "[!] Please run Wincess as Administrator!" << std::endl;
        std::cin.get();
        return 1;
    }

    while (true) {
        std::cout << "\n--- WINCCESS MENU ---" << std::endl;
        std::cout << "[1] Remove Watermark & Enable Auto-Check" << std::endl;
        std::cout << "[2] Change Wallpaper (Enter Image Path)" << std::endl;
        std::cout << "[3] Set Dark Theme" << std::endl;
        std::cout << "[4] Set Light Theme" << std::endl;
        std::cout << "[0] Exit" << std::endl;
        std::cout << "\nSelect an option: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            RemoveWatermark();
            system("schtasks /create /tn \"WincessAutoCheck\" /tr \"\\\""
                   "%~f0\\\" --silent\" /sc onlogon /rl highest /f >nul 2>&1");
            std::cout << "[+] Watermark disabled! Auto-check installed." << std::endl;
        }
        else if (choice == 2) {
            std::cout << "Enter full path to image (e.g. C:\\image.jpg): ";
            std::string path;
            std::cin.ignore();
            std::getline(std::cin, path);

            if (SetWallpaper(path)) {
                std::cout << "[+] Wallpaper updated successfully!" << std::endl;
            } else {
                std::cout << "[-] Failed to set wallpaper. Check path." << std::endl;
            }
        }
        else if (choice == 3) {
            SetSystemTheme(false);
            std::cout << "[+] Dark Theme applied!" << std::endl;
        }
        else if (choice == 4) {
            SetSystemTheme(true);
            std::cout << "[+] Light Theme applied!" << std::endl;
        }
        else if (choice == 0) {
            break;
        }
    }

    return 0;
}
