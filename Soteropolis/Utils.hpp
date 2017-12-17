#pragma once

bool _point_in_triangle_2d(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c);

bool _point_in_circumcircle(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c);

void _log(void *userdata, int category, SDL_LogPriority priority,
          const char *message);

void _assert_fatal(void *ptr, const char *str);

void _assert(void *ptr, const char *str);
