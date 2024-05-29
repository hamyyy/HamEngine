#pragma once

// libraries
#include <imgui.h>
#include <ImGuizmo.h>
#include <glad/gl.h>
#include <Ham/Core/Math.h>
#include "Ham/Util/ImGuiExtra.h"

// scripting
//

//  Ham Engine Core
#include "Ham/Core/Macros.h"
#include "Ham/Core/Base.h"
#include "Ham/Core/Log.h"
#include "Ham/Core/FileSystem.h"
#include "Ham/Core/Assert.h"
#include "Ham/Debug/Profiler.h"

#include "Ham/Core/Application.h"
#include "Ham/Core/Layer.h"
#include "Ham/Core/Log.h"
#include "Ham/Core/Assert.h"

#include "Ham/Renderer/Shader.h"
#include "Ham/Renderer/Buffer.h"

#include "Ham/Util/Random.h"
#include "Ham/Util/TimeStep.h"
#include "Ham/Util/UUID.h"
#include "Ham/Util/File.h"

#include "Ham/Input/Input.h"
#include "Ham/Input/KeyCodes.h"

#include "Ham/Scene/Scene.h"
#include "Ham/Scene/Entity.h"
#include "Ham/Scene/Component.h"
#include "Ham/Scene/Systems.h"

// #include "Ham/Script/NativeScript.h"

//  stdlib
#include <vector>
#include <string>
#include <filesystem>
#include <cstdint>
