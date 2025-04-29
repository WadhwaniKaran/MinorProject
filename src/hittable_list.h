#pragma once

#include "commons.h"
#include "hittable.h"

class HittableList : public Hittable
{
    public:
    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { m_objects.clear(); }

    void add(shared_ptr<Hittable> object) { m_objects.push_back(object); }

    bool hit(const Ray &ray, Interval ray_t, HitRecord &record) const override
    {
        HitRecord temp_rec;
        bool hit_anything = false;
        f64 closest_so_far = ray_t.m_max;

        for(const auto &object : m_objects)
        {
            if(object.get()->hit(ray, Interval(ray_t.m_min, closest_so_far), temp_rec))
            {
                record = temp_rec;
                hit_anything = true;
                closest_so_far = temp_rec.t;
            }
        }

        return hit_anything;
    } 

    private:
    std::vector<shared_ptr<Hittable>> m_objects;
};