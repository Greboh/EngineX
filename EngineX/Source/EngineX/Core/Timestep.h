#pragma once

namespace EngineX
{
    class Timestep
    {
    public:
        Timestep(const float time = 0.0f) : m_Time(time) {}

        operator float() const;

    private:
        float m_Time;
    };

    inline Timestep::operator float() const
    { return m_Time; }
}
