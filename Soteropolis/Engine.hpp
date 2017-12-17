#pragma once

#include "stdafx.h"

#include "Voronoi2D.hpp"

#include <glm/vec2.hpp>

class Engine {
  bool running = false;
  Uint32 sdl_systems_flags =
      SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS;

  // ** Graphics ** //
  SDL_Window *window = nullptr;
  SDL_GLContext glcontext = {0};

  struct window_data {
    int width = 800;
    int height = 600;
    int pos_x = SDL_WINDOWPOS_CENTERED;
    int pos_y = SDL_WINDOWPOS_CENTERED;
    Uint32 sdl_flags =
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    const char *title = "Engine 1";
  };
  window_data screen;

  // ** Time ** //
  struct time_data {
    Uint64 begin;
    Uint64 now;
    Uint64 last_frame;
    GLdouble delta;
    GLdouble fps;
    GLdouble seconds;
  };
  time_data time;

  // ** Random ** //
  struct random_data {
    std::random_device device;
    std::mt19937 gen;
  };
  random_data rng;

  // ** Events ** //
  SDL_Event event = {0};
  glm::vec2 mouse_pos;

  // ** State ** //
  struct state_data {
    Voronoi2D voronoi;
    bool render_cells = false;
    bool render_triangulation = true;
	bool render_points = true;
	bool render_edges = true;
    bool dragging = false;
  };
  state_data state;

public:
  Engine();
  ~Engine();

  void start();
  void update_time();
  void loop();
  void render();
  void event_handle();
};
