#pragma once
#include <glad/glad.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <any>
#include <array>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <codecvt>
#include <concepts>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <vector>

#define constrain(x, y, z) (x < y ? y : x > z ? z : x)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>
#include <unknwn.h>
#include <windowsx.h>
#include <ShlObj_core.h>

// Rarely updated local headers
#include "GuiCode2/Vec.hpp"

#define M_PI 3.14159265359

#define DEBUG 1
#define FPS 1
#if DEBUG == 1
#define LOG(x) std::cout << x << std::endl
# if FPS == 1
#define MEASURE_FPS static std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); \
static std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); \
end = std::chrono::steady_clock::now(); \
long long diff = \
std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count(); \
float fps = 1000000000.0 / diff; \
LOG("FPS = " << fps); \
begin = std::chrono::steady_clock::now();
# else
#define MEASURE_FPS
# endif
#else
#define LOG(x)
#define MEASURE_FPS
#endif

#define LIMIT_FPS(x) \
static auto endl = std::chrono::steady_clock::now(); \
static auto beginl = std::chrono::steady_clock::now(); \
endl = std::chrono::steady_clock::now(); \
int64_t diffl = \
std::chrono::duration_cast<std::chrono::nanoseconds>(endl - beginl).count(); \
int64_t ns = 1000000000.0 / x; \
int64_t sleepfor = ns - diffl; \
if (sleepfor > 0) \
std::this_thread::sleep_for(std::chrono::nanoseconds(sleepfor)); \
beginl = std::chrono::steady_clock::now();
