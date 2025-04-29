#pragma once

#include "commons.h"

struct Interval
{
    // default is empty
    Interval() : m_min(+infinity), m_max(-infinity) {}

    Interval(f64 min, f64 max) : m_min(min), m_max(max) {}

    f64 size() const { return m_max - m_min; }
    
    f64 contains(f64 x) const
    {
        if(x >= m_min && x <= m_max) return true;
        return false;
    }

    f64 surrounds(f64 x) const
    {
        if(x > m_min && x < m_max) return true;
        return false;
    }

    f64 clamp(f64 x) const
    {
        if(x < m_min) return m_min;
        if(x > m_max) return m_max;
        return x;
    }

    static const Interval empty, universe;

    f64 m_min, m_max;
};