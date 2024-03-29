#include "corners_detector.h"

/**
 * /fichier corners_detector.cpp
 * /auteurs Thierry Champion, Nikolai Olekhnovitch, Raisse Oumarou Petitot, Rym Touati
 * /date    05 décembre 2023
 * Créé le 10 novembre
 * 
 * Classe représentant le mode de détermination du coin. Un encodage est utilisé pour 
 * identifier quelles intersections ont été détectées et quels ont été les distances.
 * 
 * Les deux bits significatfis de l'encodage représente la distance avec la première
 * intersection. Une unité donne une valeure de 00 et deux unité donne une valeure de 11.
 * Les trois bits les moins significatifs représente la nature de la première intersection.
 * Pour un virage a gauche, les bits sont 100 et pour un virage a droite, les bits sont 010.
 * S'il est aussi possible d'aller tout droit, le premier bit est mit à 1.
 * Les trois bits restants représente la nature de la deuxième interseciton, s'il y en a une.
 * 
 * Ex: (4, 7) nord = Long Gauche et tout droit => 11 000 101
 */

const uint16_t DELAY_HIGH_NOTE = 1000;
const uint8_t CENTERING_DELAY = 42;

CornersDetector::CornersDetector()
    : _isDetecting(false),
      _detector(0),
      _scan(0),
      _intersection(LineStructure::NONE)
{
}

const char *CornersDetector::run(MasterNavigation *navigation, Piezo *piezo)
{
    findCorner(navigation);

    piezo->play(Note::G, Octave::TOP);
    _delay_ms(DELAY_HIGH_NOTE);
    piezo->stop();

    EventTimer::setToggling(false, LedColor::GREEN);
    comeBack(navigation);
    return getCornerName();
}

void CornersDetector::findCorner(MasterNavigation *navigation)
{
    EventTimer::setToggling(true);
    _scan = 0;
    _detector = 0;
    _isDetecting = true;
    EventTimer::resetNavigationCounter();
    navigation->jumpStart();

    LineSensor *lineSensor = navigation->getLineSensor();

    while (_isDetecting)
    {
        navigation->goStraight();
        lineSensor->updateDetection();
        if (lineSensor->getStructure() == LineStructure::RIGHT ||
            lineSensor->getStructure() == LineStructure::LEFT)
        {
            _intersection = lineSensor->getStructure();
            while (lineSensor->detectsSimpleIntersection())
            {
                lineSensor->updateDetection();
            }
            uint16_t count = EventTimer::getNavigationCounter();
            navigation->driveDistance(CENTERING_DELAY);
            lineSensor->updateDetection();
            scanIntersection(navigation, count);
        }
        PRINT(_detector);
    }
}

void CornersDetector::comeBack(MasterNavigation *navigation)
{
    PRINT("FIRST UTURN");
    navigation->stop();
    _delay_ms(200);

    if (_intersection == LineStructure::RIGHT)
    {
        navigation->executeMovementCode(MovementCode::LEFT);
    }
    else
    {
        navigation->executeMovementCode(MovementCode::RIGHT);
    }
    if (_detector == 0b010011 || _detector == 0b011011)
        navigation->driveToIntersection();
    navigation->driveToIntersection();

    PRINT("SECOND UTURN");
    navigation->stop();
    _delay_ms(200);
    if (_intersection == LineStructure::RIGHT)
    {
        navigation->executeMovementCode(MovementCode::RIGHT);
    }
    else
    {
        navigation->executeMovementCode(MovementCode::LEFT);
    }
}
void CornersDetector::scanIntersection(MasterNavigation *navigation, uint16_t count)
{

    _isDetecting = false;
    _detector |= (_detector == 0 &&
                  count > (navigation->getUnitCount() + (navigation->getUnitCount() >> 2)))
                     ? (0b11 << 6)
                     : 0;

    LineStructure detection = _intersection;

    if (navigation->getLineSensor()->getStructure() == LineStructure::FORWARD)
    {
        if (_intersection == LineStructure::RIGHT)
            detection = LineStructure::RIGHT_FORWARD;
        else if (_intersection == LineStructure::LEFT)
            detection = LineStructure::LEFT_FORWARD;
    }

    switch (detection)
    {
        case LineStructure::RIGHT:

            _detector |= 0b010 << _scan;
            break;
        case LineStructure::RIGHT_FORWARD:
            _detector |= 0b011 << _scan;
            _isDetecting = true;
            _scan += 3;
            if (_detector == 0b11000011)
            {
                _isDetecting = false;
            }
            break;
        case LineStructure::LEFT:
            _detector |= 0b100 << _scan;
            break;
        case LineStructure::LEFT_FORWARD:
            _detector |= 0b101 << _scan;

            break;
        default:
            break;
    }
    if (_scan == 6)
    {
        _isDetecting = false;
    }

    PRINT(structureToString(detection));
}

const char *CornersDetector::getCornerName()
{
    const char *corner = "CORNER NOT DETECTED";
    switch (_detector) // 3 derniers bits pour encodage de la 1er detection
    {                  // 3 bits du milieu pour la 2eme
        case 0b11000011: // RIGHT_FORWARD Long
            corner = "(1,1)           EAST";
            break;
        case 0b100: // LEFT
            corner = "(1,1)           SOUTH";
            break;
        case 0b11000100: // LEFT long
            corner = "(4,1)           EAST";
            break;
        case 0b010: // RIGHT
            corner = "(4,1)           NORTH";
            break;
        case 0b011011: // RIGHT_FORWARD  and RIGHT_FORWARD
            corner = "(4,7)           WEST";
            break;
        case 0b11000101: // LEFT_FORWARD long
            corner = "(4,7)           NORTH";
            break;
        case 0b010011: // RIGHT_FORWARD and RIGHT
            corner = "(1,7)           SOUTH";
            break;
        case 0b101: // LEFT_FORWARD
            corner = "(1,7)           WEST";
            break;
        default:
            break;
    }

    PRINT(corner);
    return corner;
}