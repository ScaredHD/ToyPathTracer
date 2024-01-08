#include "scene.h"

#include <random>


TestScene::TestScene() {
    init_scene3();
}

std::optional<SurfaceIntersection> TestScene::hit(const Ray& ray) const {
    return hit(ray, 1.0e-6, inf);
}

std::optional<SurfaceIntersection> TestScene::hit(const Ray& ray, double tmin, double tmax) const {
    std::optional<SurfaceIntersection> closest;
    // ----------- Intersection with Geometry -----------
    for (const auto& object : objects) {
        if (auto rec = object->hit(ray, tmin, tmax); rec.has_value()) {
            closest           = rec;
            tmax              = rec->t;
            // closest->geometry = object;
            // closest->light    = nullptr;
            closest->intersection = object;
        }
    }
    // ----------- Intersection with Light -----------
    for (const auto& light : lights) {
        if (auto rec = light->hit(ray, tmin, tmax); rec.has_value()) {
            closest           = rec;
            tmax              = rec->t;
            // closest->light    = light;
            // closest->geometry = nullptr;
            closest->intersection = light;
        }
    }
    return closest;
}

bool TestScene::mutually_visible(const Vec3& p, const Vec3& q) const {
    Ray r{p, q - p};  // Must not normalize (q - p)
    return !hit(r, 0.000001, 0.999999);
}

static std::mt19937 g_Gen{};

std::shared_ptr<Light> get_random_light(const TestScene& scene) {
    auto lights     = scene.get_lights();
    int light_count = static_cast<int>(scene.light_count());
    auto idx        = random_int(0, light_count - 1);
    return lights[idx];
}

void TestScene::init_scene1() {
    init_light1();
    init_geometry1();
}

void TestScene::init_light1() {
    add(create_area_light({0, 3, 0}, {0, 0, 0}, Vec3::all(3)));
    add(create_area_light({-4, 0, 2}, {0, 0, 90}, Vec3::all(10)));
    add(create_area_light({2, 0.5, 2}, {90, 0, 0}, Vec3::all(1), Color::white, 3.0));
}

void TestScene::init_geometry1() {
    add(create_geometry(primitives.rect_xz, diffuse_white, {0, 0, 0}, {0, 0, 0}, Vec3::all(1000)));

    add(create_geometry(primitives.sphere, diffuse_red, {0, 1, 0}, {0, 0, 0}, Vec3::all(1)));
    add(create_geometry(primitives.sphere, diffuse_white, {2, 1, 0}, {0, 0, 0}, Vec3::all(0.8)));
    add(create_geometry(primitives.sphere, diffuse_skyblue, {-2, 1, 2}, {0, 0, 0}, Vec3::all(1)));

    add(create_geometry(primitives.sphere, glass, {0, 1, 3}, {0, 0, 0}, Vec3::all(1)));
}

void TestScene::init_scene2() {
    init_light2();
    init_geometry2();
}

void TestScene::init_light2() {
    add(create_area_light(Vec3{0, 4, 0}, Vec3{0, 0, 0}, Vec3::all(10)));
}

void TestScene::init_geometry2() {
    add(create_geometry(primitives.rect_xz, perfect_mirror, {0, -2, 0}, {0, 0, 0}, Vec3::all(1000)));
    // add(create_geometry(primitives.sphere, glass, {0, 0, 0}, {0, 0, 0}, Vec3::all(2)));
    add(create_geometry(primitives.sphere, diffuse_red, {2, 0, -6}, {0, 0, 0}, Vec3::all(2)));
    add(create_geometry(primitives.sphere, diffuse_green, {-4, 0, -2}, {0, 0, 0}, Vec3::all(2)));
    add(create_geometry(primitives.sphere, diffuse_white, {4, 0, -2}, {0, 0, 0}, Vec3::all(1)));
    add(create_geometry(primitives.sphere, diffuse_skyblue, {3, -1, 2}, {0, 0, 0}, Vec3::all(1)));
}

void TestScene::init_scene3() {
    init_light3();
    init_geometry3();
}

void TestScene::init_light3() {
    add(create_area_light({0, 4, 2}, {0, 0, 0}, Vec3::all(4), Color::white, 2.0));
    // add(create_area_light({0, 0, 2}, {0, 0, 0}, Vec3::all(4)));
    // add(create_area_light({0, 2, 0}, {90, 0, 0}, Vec3::all(4)));
    // add(create_area_light({-2, 2, 2}, {0, 0, 90}, Vec3::all(4)));
    // add(create_area_light({2, 2, 2}, {0, 0, 90}, Vec3::all(4)));
}

void TestScene::init_geometry3() {
    add(create_geometry(primitives.rect_xz, diffuse_white, {0, 0, 2}, {0, 0, 0}, Vec3::all(4)));
    add(create_geometry(primitives.rect_xz, diffuse_skyblue, {0, 2, 0}, {90, 0, 0}, Vec3::all(4)));
    add(create_geometry(primitives.rect_xz, diffuse_red, {-2, 2, 2}, {0, 0, 90}, Vec3::all(4)));
    add(create_geometry(primitives.rect_xz, diffuse_green, {2, 2, 2}, {0, 0, 90}, Vec3::all(4)));
    add(create_geometry(primitives.sphere, glass, {0, 1.2, 2}, {0, 0, 0}, Vec3::all(1.2)));
}
