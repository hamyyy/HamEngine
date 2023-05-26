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

    private:
        Application *m_App;
    };

}