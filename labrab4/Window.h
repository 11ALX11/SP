#pragma once

#include <shlobj_core.h>

class Window
{
private:
    int wLeft;
    int wTop;

protected:
    int id;
    HWND window;
    int height;
    int width;
    int memoryNeeded;
    bool areAdministratorRightsGranted;
    bool isShown;

public:
    Window(int id, HWND window)
        : id(id), window(window), memoryNeeded(0)
        , areAdministratorRightsGranted(IsUserAnAdmin()), isShown(true)
    {
        ShowWindow(window, isShown);
        
        ClassGetWindowSize();
    }
    Window(int id, HWND window, int width, int height)
        : Window(id, window)
    {
        this->height = height;
        this->width = width;
        SetWindowSize(height, width);
    }

    void doWork(int memoryNeeded) {
        this->memoryNeeded += memoryNeeded;
    }

    void showOrHide() {
        isShown = !isShown;
        ShowWindow(window, isShown);
    }

    // Функция для изменения размеров окна
    void SetWindowSize(int height, int width) {
        SetWindowPos(window, NULL, wLeft, wTop, width, height, SWP_NOZORDER);
    }

    bool getAreAdministratorRightsGranted() {
        return areAdministratorRightsGranted;
    }

private:
    // Функция для определения размеров окна (переменных w_left, w_top, width и height)
    void ClassGetWindowSize() {
        RECT rect;
        GetWindowRect(window, &rect);
        wLeft = rect.left;
        wTop = rect.top;
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }
};