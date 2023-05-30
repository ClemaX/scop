#include "logger.h"
#include <map.h>

#include <string.h>
#include <unistd.h>

int	map_init(map *map)
{
	bzero(map, sizeof(*map));

	return 0;
}

void	map_destroy(map *map)
{
	(void)map;
}

void	map_point_symbol_set(map *map, point_id point_id, symbol symbol)
{
	map->points[point_id].symbol = symbol;
}

void	map_point_position_set(map *map, point_id point_id, const int position[2])
{
	map->points[point_id].position[x] = position[x];
	map->points[point_id].position[y] = position[y];
}

void	map_draw(const map *map)
{
	char	canvas[51 * 50];

	memset(canvas, ' ', sizeof(canvas));

	for (unsigned i = 0; i < POINT_COUNT; ++i)
	{
		const int	pos[2] = {
			map->points[i].position[x] / 2 + 50 / 2,
			map->points[i].position[y] / 2 + 50 / 2,
		};

		printf("point_pos: %d, %d\n", pos[x], pos[y]);

		canvas[51 * pos[y] + pos[x]] = map->points[i].symbol;
	}

	for (unsigned i = 0; i < 50; ++i)
	{
		canvas[51 * i + 50] = '\n';
	}

	write(STDERR_FILENO, canvas, sizeof(canvas));
}
