#pragma once

#include "Ham.h"

namespace Ham
{
    struct VertexData
    {
        glm::vec3 position;
        glm::vec3 normal;
        // glm::vec2 texCoords;
    };

    class HamLayer : public Layer
    {
    public:
        HamLayer(Application *app);
        ~HamLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(TimeStep deltaTime) override;
        virtual void OnUIRender(TimeStep deltaTime) override;
        // virtual void OnEvent(Event &event) override;

        Entity GetCamera() { return m_Scene.GetActiveCamera(); }

    private:
        Application *m_App;
        Scene &m_Scene;
        std::unique_ptr<Shader> shader;
        std::vector<VertexData> vertices;
        std::vector<uint32_t> indices;
        unsigned int cubeVBO, cubeVAO;

        glm::mat4 transform;
    };

}