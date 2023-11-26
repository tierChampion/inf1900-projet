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
    CornersDetector();
    const char *detectCorner(MasterNavigation navigation, LineSensor lineSensor);

private:
    void scan(LineSensor lineSensor);
    const char *detect();

private:
    bool _isDetecting;
    uint8_t _detector;
    uint8_t _scan;
    LineStructure _intersection;
};
#endif