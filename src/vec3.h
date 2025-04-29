#pragma once

#include "commons.h"

typedef class Vec3
{
    public:
    Vec3() : m_e{0.0, 0.0, 0.0} {}
    Vec3(f64 e0, f64 e1, f64 e2) : m_e{e0, e1, e2} {}

    f64 x() const { return m_e[0]; }
    f64 y() const { return m_e[1]; }
    f64 z() const { return m_e[2]; }

    Vec3 operator-() const { return Vec3(-m_e[0], -m_e[1], -m_e[2]); }
    f64 operator[](size_t i) const { return m_e[i]; }
    f64& operator[](size_t i) { return m_e[i]; }

    Vec3& operator+=(const Vec3 &v);
    Vec3& operator*=(f64 t);
    Vec3& operator/=(f64 t);
    f64 length() const;
    f64 length_squared() const;

    static Vec3 random() 
    {
        return Vec3(random_double(), random_double(), random_double());
    }

    static Vec3 random(f64 min, f64 max) 
    {
        return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    bool near_zero() const 
    {
        f64 s = 1e-8;
        return ((std::fabs(m_e[0]) < s) && (std::fabs(m_e[1]) < s) && (std::fabs(m_e[2]) < s));
    }

    private:
    f64 m_e[3];
} Vec3, Point3;

inline std::ostream &operator<<(std::ostream &out, const Vec3 &vec)
{
    return out << vec.x() << ' ' << vec.y() << ' ' << vec.z();
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline Vec3 operator*(const Vec3 &u, f64 t)
{
    return Vec3(u.x() * t, u.y() * t, u.z() * t);
}

inline Vec3 operator*(f64 t, const Vec3 &u)
{
    return u * t;;
}

inline Vec3 operator/(const Vec3 &u, f64 t)
{
    return u * (1 / t);
}

inline Vec3 operator/(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.x() / v.x(), u.y() / v.y(), u.z() / v.z());
}

inline f64 dot(const Vec3 &u, const Vec3 &v)
{
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.y() * v.z() - u.z() * v.y(),
                u.z() * v.x() - u.x() * v.z(),
                u.x() * v.y() - u.y() * v.x());
}

inline Vec3 unit_vector(const Vec3 &vec)
{
    return vec / vec.length();
}

inline Vec3 random_unit_vector()
{
    while(true)
    {
        Vec3 vec = Vec3::random(-1,1);
        f64 len_sq = vec.length_squared();
        if(len_sq > 1e-160 && len_sq <= 1) { return vec / std::sqrt(len_sq); }
    }
}

inline Vec3 random_on_hemisphere(const Vec3 &normal)
{
    Vec3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere, normal) > 0.0) return on_unit_sphere; // unit vector in the same hemisphere as normal
    else return -on_unit_sphere;
}

inline Vec3 random_on_unit_disk()
{
    while (true)
    {
        Vec3 v = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if(v.length_squared() < 1) { return v; }
    }
    
}

inline Vec3 reflect(const Vec3 &incident, const Vec3 &normal)
{
    return incident - 2 * dot(incident, normal) * normal;
}

inline Vec3 refract(const Vec3 &uv, const Vec3 &normal, f64 etai_over_etat)
{
    auto cos_theta = std::fmin(dot(-uv, normal), 1.0);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * normal);
    Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * normal;
    return r_out_perp + r_out_parallel;
}