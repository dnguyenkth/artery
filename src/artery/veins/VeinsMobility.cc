#include "artery/veins/VeinsMobility.h"
#include <cmath>

Define_Module(VeinsMobility)

void VeinsMobility::initialize(int stage)
{
    BaseMobility::initialize(stage);
    if (stage == 0) {
        WATCH(mVehicleId);
        WATCH(mPosition);
        WATCH(mDirection);
        WATCH(mSpeed);
    } else if (stage == 1) {
        mPosition = move.getStartPos();
        mDirection = move.getDirection();
        mSpeed = move.getSpeed();
    }
}

void VeinsMobility::initialize(const Position& pos, Angle heading, double speed)
{
    using boost::units::si::meter;
    mPosition.x = pos.x / meter;
    mPosition.y = pos.y / meter;
    move.setStart(mPosition);

    mSpeed = speed;
    move.setSpeed(mSpeed);

    mDirection = Coord { cos(heading.radian()), -sin(heading.radian()) };
    move.setDirectionByVector(mDirection);
}

void VeinsMobility::update(const Position& pos, Angle heading, double speed)
{
    initialize(pos, heading, speed);

    BaseMobility::updatePosition(); // emits update signal for Veins
    // assert there is no identical signal emitted twice
    ASSERT(BaseMobility::mobilityStateChangedSignal != MobilityBase::stateChangedSignal);
    emit(MobilityBase::stateChangedSignal, this);
}
