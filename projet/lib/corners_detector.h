#ifndef CORNERS_DECTECTOR_H
#define CORNERS_DECTECTOR_H
#include <line_sensor.h>
#include <navigation.h>
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
    char *detectCorner();

private:
    void scan();
    char *detect();
    void intRoutineTimer1();
    uint16_t _distanceCounter;
    bool _isDetecting;
    uint8_t _detector;
    ScanState _scanState;
};
#endif