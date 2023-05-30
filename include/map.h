#pragma once

#include "vector.h"
#include <stdint.h>
#include <GL/gl.h>

typedef enum	e_point_id
{
	POINT_TARGET,
	POINT_CAMERA,
	POINT_COUNT,
}				point_id;

typedef char	symbol;

typedef struct	t_point
{
	int		position[2];
	symbol	symbol;
}				point;

typedef struct	s_map
{
	point	points[POINT_COUNT];
}				map;

int		map_init(map *map);
void	map_destroy(map *map);

void	map_point_symbol_set(map *map, point_id point_id, symbol symbol);
void	map_point_position_set(map *map, point_id point_id, const int position[2]);

void	map_draw(const map *map);
