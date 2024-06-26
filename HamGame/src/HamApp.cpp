#include "Ham.h"
#include "Ham/Core/EntryPoint.h"
#include "HamLayer.h"

namespace Ham {

class HamApp : public Application {
 public:
  HamApp(const ApplicationSpecification &specification) : Application(specification)
  {
    m_Layer = std::make_shared<HamLayer>(this);
    PushLayer(m_Layer.get());
  }

 private:
  std::shared_ptr<HamLayer> m_Layer;
};

Application *CreateApplication(ApplicationCommandLineArgs args)
{
  // create app specification
  ApplicationSpecification spec;
  spec.Name = "Ham Engine Demo";
  // spec.Width = 600;
  // spec.Height = 600;
  spec.CommandLineArgs = args;

  return new HamApp(spec);
}

}  // namespace Ham