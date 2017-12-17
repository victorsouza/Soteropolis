#include "stdafx.h"

#include "Engine.hpp"

Engine::Engine() {
  // ** Initialize Random ** //
  rng.gen.seed(rng.device());
  rng.gen.discard(1000);

  // ** Initialize State ** //
  std::normal_distribution<> normal_x(screen.width * 0.5, 100.0f);
  std::normal_distribution<> normal_y(screen.height * 0.5, 100.0f);
  std::uniform_real_distribution<> unif_x(0, screen.width);
  std::uniform_real_distribution<> unif_y(0, screen.height);
  state.voronoi.setBoundary(screen.width, screen.height);
  std::uniform_real_distribution<> col(0.0f, 1.0f);
  for (int i = 0; i < 25; ++i) {
    // state.voronoi.addPoint(glm::vec2(unif_x(rng.gen), unif_y(rng.gen)),i+1);
    state.voronoi.addPoint(glm::vec2(normal_x(rng.gen), normal_y(rng.gen)),
                           i + 1);
    state.voronoi.addColor(glm::vec3(col(rng.gen), col(rng.gen), col(rng.gen)));
  }
}

Engine::~Engine() {
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Engine::start() {
  // Init SDL
  SDL_Init(sdl_systems_flags);
  SDL_LogSetOutputFunction(_log, nullptr);
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

  // Init Graphics
  window = SDL_CreateWindow(screen.title, screen.pos_x, screen.pos_y,
                            screen.width, screen.height, screen.sdl_flags);
  _assert_fatal(window, "Window Creation");

  glcontext = SDL_GL_CreateContext(window);
  _assert_fatal(glcontext, "OpenGl Context");

  // Init Timers
  time.delta = 0;
  time.begin = SDL_GetPerformanceCounter();
  time.now = time.begin;
  time.last_frame = time.begin - 1;

  running = true;
}

void Engine::update_time() {
  // Update Timers
  time.last_frame = time.now;
  time.now = SDL_GetPerformanceCounter();
  time.delta = ((time.now - time.last_frame) * 1000.0 /
                (GLdouble)SDL_GetPerformanceFrequency());
  // Calculate FPS Here
}

void Engine::loop() {
  while (running) {
    // Time Handling
    update_time();
    // Event Handling
    event_handle();

    // Logic Handling

    // Rendering
    render();
    SDL_GL_SwapWindow(window);
  }
}

void Engine::render() {
  glMatrixMode(GL_PROJECTION | GL_MODELVIEW);
  glLoadIdentity();
  glOrtho(0.0f, screen.width, screen.height, 0.0f, -1.0f, 1.0f);

  glClearColor(0.0f, 31.0f / 255.0f, 63.0f / 255.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  if (state.render_cells) {
    state.voronoi.render_cells();
  }
  if (state.render_triangulation) {
    state.voronoi.render_triangulation();
  }
  if (state.render_edges) {
	state.voronoi.render_cells_edges();
  }
  if (state.render_points) {
	state.voronoi.render_vertices();
  }
}

void Engine::event_handle() {
  // Mouse Position
  int x, y;
  SDL_GetMouseState(&x, &y);
  mouse_pos.x = (GLfloat)x;
  mouse_pos.y = (GLfloat)y;
  state.voronoi.setInterestPoint(mouse_pos, 5.0);

  if (state.dragging) {
    state.voronoi.drag_to(mouse_pos);
    std::cout << "DRAG" << std::endl;
  }

  // Even Pool
  while (SDL_PollEvent(&event)) {
	
    switch (event.type) {
    // Mouse Events
    case SDL_MOUSEBUTTONDOWN:
      switch (event.button.button) {
      case SDL_BUTTON_LEFT:
        if (!state.dragging) {
          if (state.voronoi.drag_select_if_closer(mouse_pos, 5.0) && state.render_points) {
            state.dragging = true;
            std::cout << "START DRAG" << std::endl;
          } else {
            std::uniform_real_distribution<> col(0.0f, 1.0f);
            unsigned id = state.voronoi.addColor(
                glm::vec3(col(rng.gen), col(rng.gen), col(rng.gen)));
            state.voronoi.addPoint(glm::vec2(event.motion.x, event.motion.y),
                                   id);
            std::cout << "ADD" << std::endl;
          }
        }
        break;
      }
      break;
    case SDL_MOUSEBUTTONUP:
      switch (event.button.button) {
      case SDL_BUTTON_LEFT:
        if (state.dragging) {
          state.dragging = false;
          std::cout << "STOP DRAG" << std::endl;
        }
        break;
      }
      break;
    // Keyboard Events
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_t:
        state.render_triangulation = !state.render_triangulation;
        break;
      case SDLK_v:
        state.render_cells = !state.render_cells;
        break;
	  case SDLK_e:
		state.render_edges = !state.render_edges;
		break;
	  case SDLK_p:
		state.render_points = !state.render_points;
		break;
      }
      break;
    // Quitting
    case SDL_QUIT:
      running = false;
      break;
    }
  }
}
