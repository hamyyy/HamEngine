#pragma once

// libraries
#include <imgui.h>
#include <ImGuizmo.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include "Ham/Util/GlmExtra.h"

//  Ham Engine Core
#include "Ham/Core/Base.h"
#include "Ham/Core/Log.h"
#include "Ham/Core/Assert.h"
#include "Ham/Debug/Profiler.h"

#include "Ham/Core/Application.h"
#include "Ham/Core/Layer.h"
#include "Ham/Core/Log.h"
#include "Ham/Core/Assert.h"

#include "Ham/Renderer/Shader.h"

#include "Ham/Util/TimeStep.h"
#include "Ham/Input/Input.h"
#include "Ham/Input/KeyCodes.h"

#include "Ham/Scene/Scene.h"
#include "Ham/Scene/Entity.h"
#include "Ham/Scene/Component.h"

//  stdlib
#include <vector>
#include <string>
#include <filesystem>
#include <cstdint>
#include <format>
