#pragma once

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "light.h"
#include "material.h"
#include "objects.h"
#include "ray.h"
#include "shape.h"

class TestScene {
  public:
    TestScene();

    std::optional<SurfaceIntersection> hit(const Ray& ray) const;

    std::optional<SurfaceIntersection> hit(const Ray& ray, double tmin, double tmax) const;

    std::vector<std::shared_ptr<Geometry>> get_objects() const { return objects; }

    std::vector<std::shared_ptr<Light>> get_lights() const { return lights; }

    size_t object_count() const { return objects.size(); }

    size_t light_count() const { return lights.size(); }

    void add(const std::shared_ptr<Geometry>& object) { add_geometry(object); }

    void add(const std::shared_ptr<Light>& light) { add_light(light); }

    void add_geometry(const std::shared_ptr<Geometry>& object) { objects.push_back(object); }

    void add_light(const std::shared_ptr<Light>& light) { lights.push_back(light); }

    bool mutually_visible(const Vec3& p, const Vec3& q) const;

    void load_scene1() {
        clear();
        init_scene1();
    }

    void load_scene2() {
        clear();
        init_scene2();
    }

    void load_scene3() {
        clear();
        init_scene3();
    }

  private:
    void clear() {
        objects.clear();
        lights.clear();
    }

    void init_scene1();
    void init_light1();
    void init_geometry1();

    void init_scene2();
    void init_light2();
    void init_geometry2();

    void init_scene3();
    void init_light3();
    void init_geometry3();

    std::vector<std::shared_ptr<Geometry>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    /// ------------- Predefined materials ------------
    std::shared_ptr<MaterialDiffuse> diffuse_white =
        std::make_shared<MaterialDiffuse>(Color::white, 0.8);

    std::shared_ptr<MaterialDiffuse> diffuse_red =
        std::make_shared<MaterialDiffuse>(Color::red, 0.8);

    std::shared_ptr<MaterialDiffuse> diffuse_green =
        std::make_shared<MaterialDiffuse>(Color::green, 0.8);

    std::shared_ptr<MaterialDiffuse> diffuse_skyblue =
        std::make_shared<MaterialDiffuse>(Color::skyblue, 0.8);

    std::shared_ptr<Glass> glass = std::make_shared<Glass>();

    std::shared_ptr<PerfectMirror> perfect_mirror = std::make_shared<PerfectMirror>();
};

std::shared_ptr<Light> get_random_light(const TestScene& scene);