#include "artery/traci/VehicleController.h"
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/systems/si/acceleration.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/si/velocity.hpp>

namespace si = boost::units::si;

namespace traci
{

VehicleController::VehicleController(const std::string& id, traci::LiteAPI& api) :
    m_id(id), m_api(api), m_boundary(api.simulation().getNetBoundary()), m_type(getTypeId(), api)
{
}

const std::string& VehicleController::getVehicleId() const
{
    return m_id;
}

std::string VehicleController::getTypeId() const
{
    return m_api.vehicle().getTypeID(m_id);
}

const VehicleType& VehicleController::getVehicleType() const
{
    return m_type;
}

Position VehicleController::getPosition() const
{
    return traci::position_cast(m_boundary, m_api.vehicle().getPosition(m_id));
}

auto VehicleController::getGeoPosition() const -> GeoPosition
{
    TraCIPosition traci_pos = m_api.vehicle().getPosition(m_id);
    TraCIGeoPosition traci_geo = m_api.convertGeo(traci_pos);
    GeoPosition geo;
    geo.latitude = traci_geo.latitude * boost::units::degree::degree;
    geo.longitude = traci_geo.longitude * boost::units::degree::degree;
    return geo;
}

auto VehicleController::getHeading() const -> Angle
{
    using namespace traci;
    return angle_cast(TraCIAngle { m_api.vehicle().getAngle(m_id) });
}

auto VehicleController::getSpeed() const -> Velocity
{
    return m_api.vehicle().getSpeed(m_id) * si::meter_per_second;
}

auto VehicleController::getMaxSpeed() const -> Velocity
{
    return m_api.vehicle().getMaxSpeed(m_id) * si::meter_per_second;
}

void VehicleController::setMaxSpeed(Velocity v)
{
    m_api.vehicle().setMaxSpeed(m_id, v / si::meter_per_second);
}

void VehicleController::setSpeed(Velocity v)
{
    m_api.vehicle().setSpeed(m_id, v / si::meter_per_second);
}

auto VehicleController::getLength() const -> Length
{
    return m_type.getLength();
}

auto VehicleController::getWidth() const -> Length
{
    return m_type.getWidth();
}

void VehicleController::changeTarget(const std::string& edge)
{
    m_api.vehicle().changeTarget(m_id, edge);
}

} // namespace traci
