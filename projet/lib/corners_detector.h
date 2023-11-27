#ifndef CORNERS_DECTECTOR_H
#define CORNERS_DECTECTOR_H

#include <line_sensor.h>
#include <master_navigation.h>
#include <event_timer.h>

enum class ScanState : uint8_t
{
    FIRST = 0,
    SECOND = 3,
    OVER = 6
};

class CornersDetector
{
public:
    CornersDetector(MasterNavigation *navigation);

    const char *run();

private:
    void scan();
    const char *detect();
    // const char *findCorner();
    // const char *comeBack();

private:
    bool _isDetecting;
    uint8_t _detector;
    uint8_t _scan;
    LineStructure _intersection;
    MasterNavigation *_navigation;
    LineSensor *_lineSensor;
};
#endif