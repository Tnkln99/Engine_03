#include "Engine.h"
#include "core/Coordinator.h"

#include "components/Camera.h"
#include "components/Light.h"
#include "components/Model.h"
#include "components/Transform.h"
#include "utilities/Timer.h"

#include <cstdlib>
#include <iostream>


int main() {
    zt::Engine engine{};
    zt::core::Coordinator coordinator{};

    engine.init(coordinator);

    auto cameraControllerSystem = coordinator.registerSystem<zt::system::CameraControllerSystem>();
    {
        zt::core::Signature signature;
        signature.set(coordinator.getComponentType<zt::component::Camera>());
        signature.set(coordinator.getComponentType<zt::component::Transform>());
        coordinator.setSystemSignature<zt::system::CameraControllerSystem>(signature);
    }
    cameraControllerSystem->init();

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
        while(!engine.shouldCloseWindow())
        {
            engine.update(coordinator);
            cameraControllerSystem->update(coordinator, engine.graphicEngine.getAspectRatio(), zt::utilities::Timer::getDeltaTime());
        }
        engine.terminate();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
