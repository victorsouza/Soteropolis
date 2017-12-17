#pragma once

#include "stdafx.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<unsigned, K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<unsigned, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef K::Point_2 Point_2;
typedef K::Segment_2 Segment_2;
typedef K::Ray_2 Ray_2;
typedef K::Line_2 Line_2;
typedef K::Iso_rectangle_2 Iso_rectangle_2;

class Voronoi2D {
  Triangulation T;
  Iso_rectangle_2 bounds;

  bool isclose = false;
  Triangulation::Vertex_handle closest;
  Triangulation::Vertex_handle dragpoint;
  Triangulation::Vertex_handle boundpoints[4];

  bool isBoundPoint(Triangulation::Vertex_handle v);

  std::vector<glm::vec3> colors;

public:
  Voronoi2D();
  ~Voronoi2D();
  void setBoundary(int w, int h);
  void addPoint(glm::vec2 p, unsigned id);
  size_t addColor(glm::vec3 col);
  void setInterestPoint(glm::vec2 p, double dist);
  bool drag_select_if_closer(glm::vec2 p, double dist);
  void drag_to(glm::vec2 p);

  void render_vertices();
  void render_triangulation();
  void render_cells();
  void render_cells_edges();
};
