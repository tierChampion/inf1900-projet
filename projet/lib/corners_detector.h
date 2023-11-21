#ifndef CORNERS_DECTECTOR_H
#define CORNERS_DECTECTOR_H
#include <line_sensor.h>
#include <master_navigation.h>
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
    void intRoutineTimer1();

private:
    void scan(LineSensor lineSensor);
    const char *detect();
    uint16_t _distanceCounter;
    bool _isDetecting;
    uint8_t _detector;
    ScanState _scanState;
};
#endif