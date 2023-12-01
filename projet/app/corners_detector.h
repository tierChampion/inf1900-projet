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
    CornersDetector();

    const char *run(MasterNavigation *navigation, Piezo *piezo);

private:
    void scanIntersection(MasterNavigation* navigation, uint16_t counter);
    const char *getCornerName();
    void findCorner(MasterNavigation* navigation);
    void comeBack(MasterNavigation* navigation);

private:
    bool _isDetecting;
    uint8_t _detector;
    uint8_t _scan;
    LineStructure _intersection;
};
#endif