#include "pins.h"
#include <corners_detector.h>
int main()
{
    MasterNavigation navigation;
    CornersDetector detector(&navigation);
    const char *corner = detector.run();
    PRINT(corner);
}
