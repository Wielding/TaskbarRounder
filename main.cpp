#include <Windows.h>

#include <dwmapi.h>
#include <string>
#include <vector>

std::vector<HWND> FindWindowsWithClass(const std::wstring& className, HWND parent)
{
    std::vector<HWND> result;
    HWND hWnd;

    if ((hWnd = FindWindowEx(parent, nullptr, className.c_str(), L""))!=nullptr) {
        result.emplace_back(hWnd);
    }

    return result;
}

// This recursion is overkill for this situation. I copied it from another
// app I wrote, so it is already tested.  Why mess with what is not broken.
std::vector<HWND> FindWindowsWithClassList(const std::vector<std::wstring>& classList, HWND parent = nullptr)
{
    std::vector<HWND> result;

    if (classList.empty()) {
        return result;
    }

    std::vector<std::wstring> newList{};
    newList.insert(newList.begin(), classList.begin()+1, classList.end());

    auto handleList = FindWindowsWithClass(*classList.begin(), parent);

    if (newList.empty()) {
        return handleList;
    }

    for (auto* handle: handleList) {
        auto children = FindWindowsWithClassList(newList, handle);
        result.insert(result.begin(), children.begin(), children.end());
    }

    return result;
}

INT WINAPI wWinMain(
        _In_ HINSTANCE hInstance,
        _In_opt_ HINSTANCE hPrevInstance,
        _In_ LPWSTR lpCmdLine,
        _In_ int nShowCmd
)
{

    const std::vector<std::wstring> classes = {L"Shell_TrayWnd"};

    const auto foundWindows = FindWindowsWithClassList(classes);

    // We did not find it so just give up.  Why was it not found? I don't know.
    if (foundWindows.empty()) {
        return 8;
    }

    DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_DEFAULT;

    DwmGetWindowAttribute(foundWindows[0], DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(preference));

    preference = preference != DWMWCP_ROUND ? DWMWCP_ROUND : DWMWCP_DEFAULT;

    DwmSetWindowAttribute(foundWindows[0], DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(preference));


    return 0;
}