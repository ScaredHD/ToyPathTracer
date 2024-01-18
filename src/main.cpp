#include "logger.h"
#include "pathtracer.h"
#include "renderer.h"
#include "scene.h"
#include "timer.h"

int main() {
    constexpr bool small_img = true;
    constexpr int image_w    = small_img ? 300 : 600;
    constexpr int image_h    = small_img ? 200 : 400;

    constexpr double aspect_ratio = static_cast<double>(image_w) / image_h;

    auto cam1 = create_camera({0, 4, 6}, {0, 0, -1});
    cam1->set_aspect_ratio(aspect_ratio);
    cam1->focus_on_point({0, 0, 0});
    cam1->set_vfov(60);

    size_t spp     = 16;
    auto p_sampler = std::make_shared<PixelSampler>();
    PathTracer renderer{image_w, image_h, p_sampler, spp};

    auto scene = std::make_shared<TestScene>();
    renderer.load_scene(scene);

    size_t render_time{};
    Timer timer;

    // ------------ Scene 1 ------------

    std::cout << "render scene1:\n";
    scene->load_scene1();

    timer.reset();
    renderer.render(*cam1);
    render_time = timer.reset();

    renderer.save_output("../../results/scene1.png");
    std::cout << "render time for scene 1: " << format_time(render_time) << "\n";

    // ------------ Scene 2 ------------

    std::cout << "\nrender scene2:\n";
    scene->load_scene2();

    timer.reset();
    renderer.render(*cam1);
    render_time = timer.reset();

    renderer.save_output("../../results/scene2.png");
    std::cout << "render time for scene 2: " << format_time(render_time) << "\n";

    // ------------ Scene 3 ------------

    std::cout << "\nrender scene3:\n";
    scene->load_scene3();

    timer.reset();
    renderer.render(*cam1);
    render_time = timer.reset();

    renderer.save_output("../../results/scene3.png");
    std::cout << "render time for scene 3: " << format_time(render_time) << "\n";
}