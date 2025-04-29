#include "vec3.h"

Vec3 &Vec3::operator+=(const Vec3 &v)
{
    m_e[0] += v.m_e[0];
    m_e[1] += v.m_e[1];
    m_e[2] += v.m_e[2];
    return *this;
}

Vec3 &Vec3::operator*=(f64 t)
{
    m_e[0] *= t;
    m_e[1] *= t;
    m_e[2] *= t;
    return *this;
}

Vec3 &Vec3::operator/=(f64 t)
{
    m_e[0] /= t;
    m_e[1] /= t;
    m_e[2] /= t;
    return *this;
}

f64 Vec3::length() const
{
    return std::sqrt(length_squared());
}

f64 Vec3::length_squared() const
{
    return m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2];
}