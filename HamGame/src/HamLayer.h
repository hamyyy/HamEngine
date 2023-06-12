#pragma once

#include "Ham.h"

namespace Ham
{
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

        Entity GetActiveCamera() { return m_Scene.GetActiveCamera(); }

    private:
        Application *m_App;
        Scene &m_Scene;
        std::unique_ptr<Shader> shader;
    };

}