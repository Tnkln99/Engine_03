#include "Engine.h"
#include "core/Coordinator.h"

#include <cstdlib>
#include <iostream>

#include "components/Camera.h"
#include "components/Light.h"
#include "components/Model.h"
#include "components/Transform.h"

int main() {
    zt::Engine engine{};
    zt::core::Coordinator coordinator{};

    engine.init(coordinator);


    std::shared_ptr model = engine.graphicEngine.loadModel("models/flat_vase.obj");

    zt::core::Entity vase = coordinator.createEntity();
    coordinator.addComponent<zt::component::Transform>(
        vase,
        { { -.5f, .5f, 0.0f }, { 3.f, 1.5f, 3.f } }
    );
    coordinator.addComponent<zt::component::Model>(
        vase,
        { model }
    );

    zt::core::Entity vase2 = coordinator.createEntity();
    coordinator.addComponent<zt::component::Transform>(
        vase2,
        { { .5f, .5f, 0.0f }, { 3.f, 1.5f, 3.f } }
    );
    coordinator.addComponent<zt::component::Model>(
        vase2,
        { model }
    );

    engine.graphicEngine.camera = coordinator.createEntity();
    zt::component::Transform cameraTransform{};
    cameraTransform.translation.z = -2.5f;
    coordinator.addComponent(
        engine.graphicEngine.camera,
        cameraTransform
    );
    zt::component::Camera cameraComp{};
    coordinator.addComponent(
        engine.graphicEngine.camera,
        cameraComp
    );

    zt::core::Entity light = coordinator.createEntity();
    zt::component::Light compLight{};
    compLight.color = glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
    zt::component::Transform lightTransform{};

    coordinator.addComponent(
        light,
        compLight
    );
    coordinator.addComponent(
        light,
        lightTransform
    );
    

    try {
        engine.run(coordinator);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
