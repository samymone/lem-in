#include "lemin.h"

void			second_rooms(t_room *room, t_main *map)
{
	t_path		*tmp;

	if (!map->paths)
	{
		if (!(map->paths = (t_path *)ft_memalloc(sizeof(t_path))))
			ft_error("malloc failed\n");
		map->paths->current = room;
	}
	else
	{
		tmp = map->paths;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = (t_path *)ft_memalloc(sizeof(t_path));
		tmp = tmp->next;
		tmp->current = room;
	}
}

void            possible_ways(t_room *room, t_main *map)
{
	t_path     *tmp;

	if (!map->endway)
	{
		if (!(map->endway = (t_toend*)ft_memalloc(sizeof(t_toend)))
			|| !(map->endway->path = (t_path*)ft_memalloc(sizeof(t_path))))
			ft_error("malloc failed\n");
		map->endway->path->current = room;
	}
	else
	{
		tmp = map->endway->path;
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = (t_path*)ft_memalloc(sizeof(t_path));
		tmp = tmp->next;
		tmp->current = room;
	}
}

static void		path_is_found(t_room *room, t_main *map)
{
	while (room->from != map->start)
		room = room->from;
	second_rooms(room, map);
}