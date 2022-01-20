#include "GLaDOS.h"

using namespace GLaDOS;

class MainScene : public Scene {
  public:
    bool onInit() override {
        Platform::getInstance().setClearColor({0, 0, 0, 1});
        // clang-format off
        float quad[] = {
            0.5, -0.5, 0.0,     1.0, 0.0, 0.0, 1.0,
            -0.5, -0.5, 0.0,    0.0, 1.0, 0.0, 1.0,
            -0.5,  0.5, 0.0,    0.0, 0.0, 1.0, 1.0,

            0.5,  0.5, 0.0,     1.0, 1.0, 0.0, 1.0,
            0.5, -0.5, 0.0,     1.0, 0.0, 0.0, 1.0,
            -0.5, 0.5, 0.0,     0.0, 0.0, 1.0, 1.0,
        };
        // clang-format on
        VertexBuffer* vertexData = Platform::getRenderer().createVertexBuffer(VertexFormatDescriptor().position().color(), 6);
        vertexData->copyBufferData(quad);
        Mesh* mesh = Platform::getRenderer().createMesh(vertexData, nullptr);
        if (mesh == nullptr) {
            return false;
        }
        shaderProgram = Platform::getRenderer().createShaderProgramFromFile("basicVertex", "basicFragment");
        if (shaderProgram == nullptr) {
            return false;
        }
        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);
        RasterizerDescription desc;
        desc.mCullMode = CullMode::None;
        shaderProgram->setRasterizerState(desc);

        rectObject = createGameObject("rectObject");
        auto* meshRenderer = rectObject->addComponent<MeshRenderer>(mesh, material);

        Input::addAxis("Horizontal", NEW_T(InputHandler(KeyCode::KEY_D, KeyCode::KEY_A, 0.01)));
        Input::addAxis("Vertical", NEW_T(InputHandler(KeyCode::KEY_W, KeyCode::KEY_S, 0.01)));

        return true;
    }

    void onUpdate(real deltaTime) override {
        if (Input::isKeyDown(KeyCode::KEY_ESCAPE)) {
            Platform::getInstance().quit();
        }
        shaderProgram->setUniform("brightness", 1.f);
        shaderProgram->setUniform("model", rectObject->transform()->localToWorldMatrix());
        shaderProgram->setUniform("view", Mat4<real>::lookAt(cameraPosition, Vec3::normalize(cameraPosition + Vec3::forward), UVec3::up));
        shaderProgram->setUniform("projection", Mat4<real>::perspective(Math::toRadians(Deg{60.f}), 1024 / 800, 0.1f, 5000.f));
        Vec3 moveDir = Vec3{Input::getAxisRaw("Horizontal"), Input::getAxisRaw("Vertical")}.makeNormalize();
        rectObject->transform()->translate(moveDir * deltaTime);
        rectObject->transform()->rotate({0, deltaTime * 100, 0});
    }

  private:
    ShaderProgram* shaderProgram = nullptr;
    GameObject* rectObject = nullptr;
    Vec3 cameraPosition = {0, 0, 2};
};

bool init() {
    PlatformParams params{1024, 800, "01-setup", "GLaDOS", false};
    if (!Platform::getInstance().initialize(params)) {
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    if (!init()) {
        return -1;
    }

    SceneManager::getInstance().setActiveScene(SceneManager::getInstance().createScene<MainScene>("testScene"));
    while (Platform::getInstance().isRunning()) {
        Platform::getInstance().update();
    }

    DestructionManager::getInstance().destroyObjects();
    dumpMemory();
    return 0;
}