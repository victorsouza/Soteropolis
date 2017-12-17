#include "stdafx.h"

#include "Voronoi2D.hpp"

#include "Utils.hpp"

#include <glm/geometric.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

Voronoi2D::Voronoi2D() { colors.push_back(glm::vec3(0, 0, 0)); }

Voronoi2D::~Voronoi2D() {}

void Voronoi2D::setBoundary(int w, int h) {
  bounds = Iso_rectangle_2(0.0, 0.0, (double)w, (double)h);
  boundpoints[0] = T.insert(Point_2(0.5 * w - 10.0 * w, 0.5 * h - 10.0 * h));
  boundpoints[1] = T.insert(Point_2(0.5 * w + 10.0 * w, 0.5 * h - 10.0 * h));
  boundpoints[2] = T.insert(Point_2(0.5 * w - 10.0 * w, 0.5 * h + 10.0 * h));
  boundpoints[3] = T.insert(Point_2(0.5 * w + 10.0 * w, 0.5 * h + 10.0 * h));
  boundpoints[0]->info() = 0;
  boundpoints[1]->info() = 0;
  boundpoints[2]->info() = 0;
  boundpoints[3]->info() = 0;
}

bool Voronoi2D::isBoundPoint(Triangulation::Vertex_handle v) {
  for (int i = 0; i < 4; ++i) {
    if (v == boundpoints[i])
      return true;
  }
  return false;
}

void Voronoi2D::addPoint(glm::vec2 p, unsigned id) {
  auto vh = T.insert(Point_2(p.x, p.y));
  vh->info() = id;
}

size_t Voronoi2D::addColor(glm::vec3 col) {
  colors.push_back(col);
  return colors.size() - 5;
}

void Voronoi2D::setInterestPoint(glm::vec2 p, double dist) {
  closest = T.nearest_vertex(Point_2(p.x, p.y));
  isclose = (CGAL::compare_squared_distance(closest->point(), Point_2(p.x, p.y),
                                            dist * dist) == CGAL::SMALLER);
}

bool Voronoi2D::drag_select_if_closer(glm::vec2 p, double dist) {
  dragpoint = T.nearest_vertex(Point_2(p.x, p.y));
  return (CGAL::compare_squared_distance(dragpoint->point(), Point_2(p.x, p.y),
                                         dist * dist) == CGAL::SMALLER);
}

void Voronoi2D::drag_to(glm::vec2 p) {
  unsigned id = dragpoint->info();
  T.remove(dragpoint);
  dragpoint = T.insert(Point_2(p.x, p.y));
  dragpoint->info() = id;
}

void Voronoi2D::render_vertices() {
  // Draw Vertices of Triangulation
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glPointSize(5.0);
  glColor3f(1.0f, 65.0f / 255.0f, 54.0f / 255.0f);
  glBegin(GL_POINTS);
  for (auto v_it = T.vertices_begin(); v_it != T.vertices_end(); ++v_it) {
    if (closest == Triangulation::Vertex_handle(v_it)) {
      if (isclose)
        glColor3f(1.0f, 220.0f / 255.0f, 0.0f);
      glVertex2d(v_it->point().hx(), v_it->point().hy());
      glColor3f(1.0f, 65.0f / 255.0f, 54.0f / 255.0f);
    } else {
      glVertex2d(v_it->point().hx(), v_it->point().hy());
    }
  }
  glEnd();
}

void Voronoi2D::render_triangulation() {
  // Draw Edges of Triangulation
  glColor3f(127.0f / 255.0f, 219.0f / 255.0f, 255.0f / 255.0f);
  glBegin(GL_LINES);
  for (auto e_it = T.finite_edges_begin(); e_it != T.finite_edges_end();
       ++e_it) {
    auto face = *(e_it->first);
    int i = e_it->second;
    auto v1 = face.vertex(face.cw(i));
    auto v2 = face.vertex(face.ccw(i));
    if (isBoundPoint(v1) || isBoundPoint(v2))
      continue;
    auto segment = T.segment(e_it);
    glVertex2d(segment.point(0).hx(), segment.point(0).hy());
    glVertex2d(segment.point(1).hx(), segment.point(1).hy());
  }
  glEnd();
}

void Voronoi2D::render_cells() {
  // Draw Voronoi Cells Colored
  // glColor3f(121.0f / 255.0f, 121.0f / 255.0f, 121.0f / 255.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  for (auto v_it = T.vertices_begin(); v_it != T.vertices_end(); ++v_it) {
	if (isBoundPoint(v_it))
	  continue;
	auto col = colors[v_it->info()];
	if (closest == Triangulation::Vertex_handle(v_it)) {
	  glColor3f(col.x * 0.8f, col.y * 0.8f, col.z * 0.8f);
	}
	else {
	  glColor3f(col.x, col.y, col.z);
	}
	Triangulation::Edge_circulator circ = v_it->incident_edges();
	Triangulation::Edge_circulator done(circ);
	do {
	  auto e = *circ;
	  if (T.is_infinite(e))
		continue;
	  auto o = T.dual(e);
	  // Finite Edges
	  if (CGAL::object_cast<K::Segment_2>(&o)) {
		auto segment = CGAL::object_cast<K::Segment_2>(o);
		glVertex2d(v_it->point().hx(), v_it->point().hy());
		glVertex2d(segment.point(0).hx(), segment.point(0).hy());
		glVertex2d(segment.point(1).hx(), segment.point(1).hy());
	  }
	} while (++circ != done);
  }
  glEnd();
}

void Voronoi2D::render_cells_edges() {
  // Draw Voronoi Cell Edges
  // glColor3f(1.0f / 255.0f, 255.0f / 255.0f, 112.0f / 255.0f);
  glColor3f(0, 0, 0);
  glBegin(GL_LINES);
  for (auto e_it = T.edges_begin(); e_it != T.edges_end(); ++e_it) {
    auto o = T.dual(e_it);
    // Finite Edges
    if (CGAL::object_cast<K::Segment_2>(&o)) {
      auto segment = CGAL::object_cast<K::Segment_2>(o);
      glVertex2d(segment.point(0).hx(), segment.point(0).hy());
      glVertex2d(segment.point(1).hx(), segment.point(1).hy());
    }
    // Infinite Edges
    else if (CGAL::object_cast<K::Ray_2>(&o)) {
      auto ray = CGAL::object_cast<Ray_2>(o);
      auto inter = CGAL::intersection(ray, bounds);
      if (inter) {
        if (auto segment = boost::get<Segment_2>(&*inter)) {
          glVertex2d(segment->point(0).hx(), segment->point(0).hy());
          glVertex2d(segment->point(1).hx(), segment->point(1).hy());
        }
      }
    }
  }
  glEnd();
}
