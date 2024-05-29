#pragma once

#include <vector>

#include "Ham/Core/Base.h"
#include "Ham/Core/Layer.h"

namespace Ham {

class LayerStack {
 public:
  LayerStack() = default;
  ~LayerStack();

  void PushLayer(Layer *layer);
  void PushLayerUnique(Layer *layer);
  void PushOverlay(Layer *overlay);
  void PushOverlayUnique(Layer *overlay);
  void PopLayer(Layer *layer);
  void PopOverlay(Layer *overlay);
  const size_t GetSize() const { return m_Layers.size(); }

  std::vector<Layer *>::iterator begin() { return m_Layers.begin(); }
  std::vector<Layer *>::iterator end() { return m_Layers.end(); }
  std::vector<Layer *>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
  std::vector<Layer *>::reverse_iterator rend() { return m_Layers.rend(); }

  std::vector<Layer *>::const_iterator begin() const { return m_Layers.begin(); }
  std::vector<Layer *>::const_iterator end() const { return m_Layers.end(); }
  std::vector<Layer *>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
  std::vector<Layer *>::const_reverse_iterator rend() const { return m_Layers.rend(); }

  auto operator[](size_t index) { return m_Layers.at(index); }
  const auto operator[](size_t index) const { return m_Layers.at(index); }

 private:
  std::vector<Layer *> m_Layers;
  unsigned int m_LayerInsertIndex = 0;
};

}  // namespace Ham