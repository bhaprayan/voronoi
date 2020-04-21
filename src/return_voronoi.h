#ifndef RETURN_VORONOI_H
#define RETURN_VORONOI_H

#include <stdint.h>
#include <stdio.h> // printf
#include <stdlib.h>

#if defined(_MSC_VER)
#include <malloc.h>
#define alloca _alloca
#else
#include <alloca.h>
#endif

#ifndef JC_VORONOI_IMPLEMENTATION
#define JC_VORONOI_IMPLEMENTATION
// If you wish to use doubles
//#define JCV_REAL_TYPE double
//#define JCV_FABS fabs
//#define JCV_ATAN2 atan2
#include "jc_voronoi.h"
#endif

#ifndef JC_VORONOI_CLIP_IMPLEMENTATION
#define JC_VORONOI_CLIP_IMPLEMENTATION
#include "jc_voronoi_clip.h"
#endif

#ifdef HAS_MODE_FASTJET
#include "../test/fastjet/voronoi.h"
#include <vector>
#endif

static void relax_points(const jcv_diagram *diagram, jcv_point *points) {
  const jcv_site *sites = jcv_diagram_get_sites(diagram);
  for (int i = 0; i < diagram->numsites; ++i) {
    const jcv_site *site = &sites[i];
    jcv_point sum = site->p;
    int count = 1;

    const jcv_graphedge *edge = site->edges;

    while (edge) {
      sum.x += edge->pos[0].x;
      sum.y += edge->pos[0].y;
      ++count;
      edge = edge->next;
    }

    points[site->index].x = sum.x / count;
    points[site->index].y = sum.y / count;
  }
}

jcv_diagram return_voronoi() {
  // Number of sites to generate
  int count = 1000;
  // Image dimension
  int width = 512;
  int height = 512;
  int numrelaxations = 0;

  jcv_point *points = 0;
  jcv_rect *rect = 0;

  points = (jcv_point *)malloc(sizeof(jcv_point) * (size_t)count);

  int pointoffset = 10; // move the points inwards, for aestetic reasons

  srand(0);

  for (int i = 0; i < count; ++i) {
    points[i].x = (float)(pointoffset + rand() % (width - 2 * pointoffset));
    points[i].y = (float)(pointoffset + rand() % (height - 2 * pointoffset));
  }

  printf("Width/Height is %d, %d\n", width, height);
  printf("Count is %d, num relaxations is %d\n", count, numrelaxations);

  jcv_clipper *clipper = 0;

  for (int i = 0; i < numrelaxations; ++i) {
    jcv_diagram diagram;
    memset(&diagram, 0, sizeof(jcv_diagram));
    jcv_diagram_generate(count, (const jcv_point *)points, rect, clipper,
                         &diagram);

    relax_points(&diagram, points);

    jcv_diagram_free(&diagram);
  }

  jcv_diagram diagram;
  memset(&diagram, 0, sizeof(jcv_diagram));
  jcv_diagram_generate(count, (const jcv_point *)points, rect, clipper,
                       &diagram);

  return diagram;
}

#endif
