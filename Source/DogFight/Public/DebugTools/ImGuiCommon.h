// Dog Fight Game Code By CYM.
#pragma once

#ifdef IMGUI_API
#define WITH_IMGUI 1
#else
#define WITH_IMGUI 0
#endif

#if WITH_IMGUI
#include "ImGuiModule.h"
#include "ImGuiDelegates.h"
#include "imgui.h"
#endif
