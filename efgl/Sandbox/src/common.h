#pragma once

#include "scene/Camera.h"
#include "Window.h"
#include "geometry/Model.h"
#include "Shader.h"
#include "util/Profile.h"
#include "geometry/prim/Quad.h"
#include "scene/Light.h"
#include "Application.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include <functional>

using namespace efgl;

static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 800;

static bool firstMouse = true;
static float lastX = SCREEN_WIDTH / 2.0f;
static float lastY = SCREEN_HEIGHT / 2.0f;
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

