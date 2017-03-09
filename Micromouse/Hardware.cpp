#include "Hardware.h"

Hardware::Hardware()
{
}

Hardware::~Hardware()
{
// TODO 
}

unsigned Hardware::moveForward(unsigned mm, bool keepGoing, bool useCaution)
{
    return 0;
}

void Hardware::rotate(int deg)
{
}

void Hardware::checkWall(Relation relation)
{
}

bool Hardware::isWall(Relation relation)
{
    return false;
}

void Hardware::setSpeed(unsigned mmps)
{
    speed = mmps;
}

void Hardware::calibrateMotors()
{
}

void Hardware::calibrateRangeFinders()
{
}
