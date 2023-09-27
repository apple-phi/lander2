
#include <cmath>
#include <numbers>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include "physics/state.h"

namespace Physics
{
    const double G = 6.67430E-11;
    namespace Mars
    {
        double mass = 6.42E23; // kg
        glm::dvec3 pos = glm::dvec3(0, 0, 0);
        double radius = 3389500; // m at datum
        double angularVel = 1.059E-07;
        double atmosphericDensity(double altitude)
        {
            return altitude < 0.0 ? 0.0 : 0.017 * std::exp(-altitude / 11000.0 /* km */);
        }
        glm::dvec3 gravityAcceleration(const glm::dvec3 &x)
        {
            return -glm::normalize(x) * G * Mars::mass / glm::dot(x, x);
        }
    };
    namespace Settings
    {
        double dt = 0.1;
    }

    double Parachute::area() const
    {
        return status == ChuteStatus::DEPLOYED ? 5.0 * size * size : 0;
    }
    glm::dvec3 Parachute::drag(glm::dvec3 vel, double atmosphericDensity) const
    {
        return -0.5 * atmosphericDensity * dragCoeff * area() * glm::length(vel) * vel;
    }

    Lander::Lander(const glm::dvec3 &pos, const glm::dvec3 &vel) : pos(pos), vel(vel)
    {
        facing = -glm::normalize(pos);
        prevPos = pos - vel * Settings::dt;
        prevVel = vel - netAccel() * Settings::dt;
    }
    void Lander::updateDynamics(double dt)
    {
        prevPos = pos;
        prevVel = vel;
        pos += vel * dt;
        vel += netAccel() * dt;
        if (altitude() < 0.0)
        {
            status = LanderStatus::LANDED;
        }
    }
    double Lander::area() const
    {
        return std::numbers::pi * size * size;
    }
    double Lander::massAccountingForFuel() const
    {
        return mass; // TODO: implement
    }
    glm::dvec3 Lander::weight() const
    {
        return massAccountingForFuel() * Mars::gravityAcceleration(pos);
    }
    glm::dvec3 Lander::thrust() const
    {
        return glm::dvec3(0, 0, 0); // TODO: implement
    }
    double Lander::altitude() const
    {
        return glm::length(pos) - Mars::radius;
    }
    glm::dvec3 Lander::climbRate() const
    {
        return glm::dot(vel, glm::normalize(pos)) * glm::normalize(pos);
    }
    glm::dvec3 Lander::groundVelocity() const
    {
        return vel - climbRate();
    }
    glm::dvec3 Lander::bodyDrag(glm::dvec3 vel, double atmosphericDensity) const
    {
        return -0.5 * atmosphericDensity * dragCoeff * area() * glm::length(vel) * vel;
    }
    glm::dvec3 Lander::netDrag(glm::dvec3 vel, double atmosphericDensity) const
    {
        return bodyDrag(vel, atmosphericDensity) + chute.drag(vel, atmosphericDensity) * std::fabs(glm::dot(glm::normalize(vel), glm::normalize(facing)));
    }
    glm::dvec3 Lander::netForce() const
    {
        return weight() + thrust() + netDrag(vel, Mars::atmosphericDensity(altitude()));
    }
    glm::dvec3 Lander::netAccel() const
    {
        return netForce() / massAccountingForFuel();
    }

    State::State() {}
    State &State::ref()
    {
        static State instance;
        return instance;
    }
}