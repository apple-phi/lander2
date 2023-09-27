#include <glm/vec3.hpp>
namespace Physics
{

    enum class LanderStatus
    {
        LANDED,
        FLYING,
        CRASHED
    };
    enum class ChuteStatus
    {
        NOT_DEPLOYED,
        DEPLOYED,
        LOST
    };
    struct Parachute
    {
        ChuteStatus status = ChuteStatus::NOT_DEPLOYED;
        double size = 2.0; // m
        double dragCoeff = 2.0;
        Parachute() = default;
        double area() const;
        glm::dvec3 drag(glm::dvec3 vel, double atmosphericDensity) const;
    };

    struct Lander
    {
        glm::dvec3 pos, vel, facing;
        Parachute chute;
        LanderStatus status = LanderStatus::FLYING;
        double size = 1.0;    // m
        double mass = 1000.0; // kg
        double dragCoeff = 0.5;
        Lander() = default;
        Lander(const glm::dvec3 &pos, const glm::dvec3 &vel);
        void updateDynamics(double dt);
        double area() const;
        double massAccountingForFuel() const;
        glm::dvec3 weight() const;
        glm::dvec3 thrust() const;
        glm::dvec3 bodyDrag(glm::dvec3 vel, double atmosphericDensity) const;
        glm::dvec3 netDrag(glm::dvec3 vel, double atmosphericDensity) const;
        glm::dvec3 netForce() const;
        glm::dvec3 netAccel() const;
        glm::dvec3 groundVelocity() const;
        glm::dvec3 groundPosition() const;
        double altitude() const;
        glm::dvec3 climbRate() const;
        // double effectiveMass(/*std::optional<double> _fuel = std::nullopt*/) const;
        // glm::dvec3 netDrag(const glm::dvec3 &x, const glm::dvec3 &v) const;
        // glm::dvec3 weight(const glm::dvec3 &x) const;
        // glm::dvec3 netForce(const glm::dvec3 &x, const glm::dvec3 &v) const;
        // glm::dvec3 netAccel(const glm::dvec3 &x, const glm::dvec3 &v) const;
        // void recalculateAttributes();
        // void checkIfLanded();
        // bool parachuteSafe() const;
        // void updateThrottleAndFuel();
        // double landerDrag() const;
        // double chuteDrag() const;

    private:
        glm::dvec3 prevPos, prevVel;
    };

    struct State
    {
        float time = 0.0f;
        bool paused = false;
        Lander lander;
        static State &ref();

    private:
        State();
        State(const State &) = delete;
        State &operator=(const State &) = delete;
    };
}