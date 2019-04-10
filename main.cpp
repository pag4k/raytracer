#include <ctime>
#include <iostream>
#include <string>

#include "Scene.h"

int main() {

    std::string files[] = {
        "scene1", "scene2",      "scene3",      "scene4",
        "scene5", "mesh_scene1", "mesh_scene2",
        //                           "evaluate_scene1",
        //                           "evaluate_scene2", "evaluate_scene3",
        //                           "evaluate_scene4",
        //                           "evaluate_scene5",
        //                           "evaluate_scene6"
        //                           "scene1b", "scene2b", "scene3b", "scene4b",
        //                           "scene5b", "scene6b", "scene7b"
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