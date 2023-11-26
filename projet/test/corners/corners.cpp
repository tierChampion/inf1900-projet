#include "pins.h"
#include <corners_detector.h>
int main()
{
    LineSensor sensor;
    MasterNavigation navigation;
    CornersDetector detector;
    const char *corner = detector.detectCorner(navigation, sensor);
    PRINT(corner);
}
