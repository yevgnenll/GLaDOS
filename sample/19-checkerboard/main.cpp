#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        Camera* camera = getMainCamera();
        camera->gameObject()->transform()->setLocalPosition({0, 0, 2});

        GameObject* cube = createGameObject("cube");
        Mesh* mesh = MeshGenerator::generateTexturedCube();
        shaderProgram = Platform::getRenderer().createShaderProgramFromFile("checkerVertex", "checkerFragment");
        if (shaderProgram == nullptr) {
            return false;
        }
        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);
        cube->addComponent<MeshRenderer>(mesh, material);
        cubeTransform = cube->transform();
        Platform::getInstance().setClearColor(Color::orange);

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }

        shaderProgram->setUniform("size", 16.0f);
        shaderProgram->setUniform("color0", Color::purple);
        shaderProgram->setUniform("color1", Color::red);

        if (Input::isMousePress(MouseButton::MOUSE_LEFT)) {
            Vec3 mouseDelta = Input::mouseDeltaPosition();
            real rotationX = mouseDelta.y * sensitivity * deltaTime;
            real rotationY = mouseDelta.x * sensitivity * deltaTime;
            cubeTransform->rotate(cubeTransform->right(), Deg{rotationX});
            cubeTransform->rotate(cubeTransform->up(), Deg{-rotationY});
        }
    }

  private:
    real sensitivity = 15;
    Transform* cubeTransform = nullptr;
    ShaderProgram* shaderProgram;
};

int main(int argc, char** argv) {
    {
        PlatformParams params{1024, 800, "19-checkerboard", "GLaDOS", false};
        if (!Platform::getInstance().initialize(params)) {
            return -1;
        }

        SceneManager::getInstance().setActiveScene(SceneManager::getInstance().createScene<MainScene>("testScene"));
        while (Platform::getInstance().isRunning()) {
            Platform::getInstance().update();
        }
    }

    DestructionManager::getInstance().destroyObjects();
    dumpMemory();
    return 0;
}