#ifndef CORNERS_DECTECTOR_H
#define CORNERS_DECTECTOR_H

#include <line_sensor.h>
#include <master_navigation.h>
#include <event_timer.h>
#include <piezo.h>

enum class ScanState : uint8_t
{
    FIRST = 0,
    SECOND = 3,
    OVER = 6
};

class CornersDetector
{
public:
    CornersDetector(MasterNavigation *navigation, Piezo *piezo);

    const char *run();

private:
    void scanIntersection();
    const char *detect();
    void findCorner();
    void comeBack();

private:
    MasterNavigation *_navigation;
    Piezo *_piezo;
    bool _isDetecting;
    uint8_t _detector;
    uint8_t _scan;
    LineStructure _intersection;
};
#endif