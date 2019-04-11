#include <ctime>
#include <iostream>
#include <string>

#include "Scene.h"

int main() {

    // Files to render.
    std::string files[] = {
        "scene1", "scene2",      "scene3",      "scene4",
        "scene5", "mesh_scene1", "mesh_scene2",
    };

    double totalTime = 0;
    for (auto &file : files) {
        std::clock_t clock_start = std::clock();
        Scene scene(file);
        scene.Render(file);
        double time = ((std::clock() - clock_start) / (double)CLOCKS_PER_SEC);
        totalTime += time;
        std::cout << "Render time: " << time << " seconds" << std::endl;
    }
    std::cout << "Total time: " << totalTime << " seconds" << std::endl;

    return 0;
}