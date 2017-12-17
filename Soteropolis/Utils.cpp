#include "stdafx.h"

#include "Utils.hpp"

#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/vec2.hpp>

bool _point_in_triangle_2d(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c) {
  // Compute vectors
  auto v0 = c - a;
  auto v1 = b - a;
  auto v2 = p - a;

  // Compute dot products
  auto dot00 = glm::dot(v0, v0);
  auto dot01 = glm::dot(v0, v1);
  auto dot02 = glm::dot(v0, v2);
  auto dot11 = glm::dot(v1, v1);
  auto dot12 = glm::dot(v1, v2);

  // Compute barycentric coordinates
  auto invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
  auto u = (dot11 * dot02 - dot01 * dot12) * invDenom;
  auto v = (dot00 * dot12 - dot01 * dot02) * invDenom;

  // Check if point is in triangle
  return (u >= 0) && (v >= 0) && (u + v < 1);
}

bool _point_in_circumcircle(glm::vec2 p, glm::vec2 a, glm::vec2 b,
                            glm::vec2 c) {
  auto dA = glm::length2(a); // a.x * a.x + a.y * a.y;
  auto dB = glm::length2(b); // b.x * b.x + b.y * b.y;
  auto dC = glm::length2(c); // c.x * c.x + c.y * c.y;

  auto aux1 = (dA * (c.y - b.y) + dB * (a.y - c.y) + dC * (b.y - a.y));
  auto aux2 = -(dA * (c.x - b.x) + dB * (a.x - c.x) + dC * (b.x - a.x));
  auto div = (2 * (a.x * (c.y - b.y) + b.x * (a.y - c.y) + c.x * (b.y - a.y)));

  if (div == 0)
    return false;

  glm::vec2 center = glm::vec2(aux1 / div, aux2 / div);
  auto radius2 =
      (center.x - a.x) * (center.x - a.x) + (center.y - a.y) * (center.y - a.y);

  return (glm::length2(p - center) <= radius2);
}

void _log(void *userdata, int category, SDL_LogPriority priority,
          const char *message) {
  switch (category) {
  case SDL_LOG_CATEGORY_APPLICATION:
    // std::cout << "[APP] ";
    break;
  case SDL_LOG_CATEGORY_ERROR:
    std::cout << "[ERROR] ";
    break;
  case SDL_LOG_CATEGORY_ASSERT:
    std::cout << "[ASSERT] ";
    break;
  case SDL_LOG_CATEGORY_SYSTEM:
    std::cout << "[SYSTEM] ";
    break;
  case SDL_LOG_CATEGORY_AUDIO:
    std::cout << "[AUDIO] ";
    break;
  case SDL_LOG_CATEGORY_VIDEO:
    std::cout << "[VIDEO] ";
    break;
  case SDL_LOG_CATEGORY_RENDER:
    std::cout << "[RENDER] ";
    break;
  case SDL_LOG_CATEGORY_INPUT:
    std::cout << "[INPUT] ";
    break;
  case SDL_LOG_CATEGORY_TEST:
    std::cout << "[TEST] ";
    break;
  case SDL_LOG_CATEGORY_CUSTOM:
    break;
  }
  switch (priority) {
  case SDL_LOG_PRIORITY_VERBOSE:
    std::cout << "[v] ";
    break;
  case SDL_LOG_PRIORITY_DEBUG:
    std::cout << "[d] ";
    break;
  case SDL_LOG_PRIORITY_INFO:
    break;
  case SDL_LOG_PRIORITY_WARN:
    std::cout << "[W] ";
    break;
  case SDL_LOG_PRIORITY_ERROR:
    std::cout << "[E] ";
    break;
  case SDL_LOG_PRIORITY_CRITICAL:
    std::cout << "[!] ";
    break;
  }
  std::cout << message << std::endl;
}

void _assert_fatal(void *ptr, const char *str) {
  if (ptr == nullptr) {
    SDL_Log("FATAL ERROR : %s\n", str);
    const char *error = SDL_GetError();
    if (*error) {
      SDL_Log("> SDL ERROR : %s\n", error);
      SDL_ClearError();
    }
    std::cin.get();
    std::exit(EXIT_FAILURE);
  }
}

void _assert(void *ptr, const char *str) {
  if (ptr == nullptr) {
    SDL_Log("      ERROR : %s\n", str);
    const char *error = SDL_GetError();
    if (*error) {
      SDL_Log("> SDL ERROR : %s\n", error);
      SDL_ClearError();
    }
  }
}
