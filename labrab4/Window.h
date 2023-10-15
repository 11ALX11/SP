#pragma once

class Window
{
protected:
    int id;
    int height;
    int width;
    int memoryNeeded;
    bool areAdministratorRightsGranted;
    bool isShown;

public:
    Window(int id, int height, int width, bool areAdminRightsGranted)
        : id(id), height(height), width(width), memoryNeeded(0),
        areAdministratorRightsGranted(areAdminRightsGranted), isShown(false) {}

    void doWork(int memoryNeeded) {
        this->memoryNeeded += memoryNeeded;
    }

    void showOrHide() {
        isShown = !isShown;
    }
};