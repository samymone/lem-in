/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   best_room_definition.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samymone <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 18:36:18 by samymone          #+#    #+#             */
/*   Updated: 2020/03/11 18:36:21 by samymone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

int				compare_paths(t_room *best, t_room *current)
{
	int			old;
	int			new;
	t_room		*tmp;

	old = 0;
	new = 0;
	tmp = best->where;
	while (tmp)
	{
		old++;
		tmp = tmp->where;
	}
	tmp = current;
	while (tmp)
	{
		new++;
		tmp = tmp->where;
	}
	return (old - new);
}

void			remake_paths2(t_room *best, t_room *current, t_main *map)
{
	if (reach_end(current, map->end) == 1)
	{
		free_path(best->where, map);
		best->where = current;
		current->from = best;
		start_searching(map->end, map);
	}

}

t_room			*best_variant4(t_room *current, t_room *variant, t_main *map)
{
	t_link *link;

	link = map->all_links_here;
	if (variant->level > current->level && variant->inputs > 1)
		return (variant);
	while (link)
	{
		if (link->first_room == current && link->second_room != variant
		&& !link->second_room->is_dead_end && link->second_room->inputs > 1
		&& link->first_room->level < link->second_room->level)
		{
			if (link->second_room->where && compare_paths(link->second_room, current) > 0)
				remake_paths2(link->second_room, current, map);
			else if (!link->second_room->where)
				return (link->second_room);
		}
		else if (link->second_room == current && link->first_room != variant
		&& !link->first_room->is_dead_end && link->first_room->inputs > 1
		&& link->first_room->level > link->second_room->level)
		{
			if (link->first_room->where && compare_paths(link->first_room, current) > 0)
				remake_paths2(link->first_room, current, map);
			else if (!link->first_room->where)
				return (link->first_room);
		}
		link = link->next;
	}
    return (variant);
}



t_room			*best_variant3(t_room *current, t_room *variant, t_main *map)
{
	t_link		*link;

	link = map->all_links_here;
	if (variant->level == current->level && variant->outputs <= 1)
		return (variant);
	while (link)
	{
		if (link->first_room == current && link->second_room != variant
		&& link->second_room->outputs <= 1 && !link->second_room->is_dead_end
		&& link->first_room->level == link->second_room->level)
		{
			if (link->second_room->where && compare_paths(link->second_room, current) > 0)
				remake_paths2(link->second_room, current, map);
			else if (!link->second_room->where)
				return (link->second_room);
		}
		else if (link->second_room == current && link->first_room != variant
		&& link->first_room->outputs <= 1 && !link->first_room->is_dead_end &&
		link->first_room->level == link->second_room->level)
		{
			if (link->first_room->where && compare_paths(link->first_room, current) > 0)
				remake_paths2(link->first_room, current, map);
			else if (!link->first_room->where)
				return (link->first_room);
		}
		link = link->next;
	}
    return (best_variant4(current, variant, map));
}

t_room			*best_variant2(t_room *current, t_room *variant, t_main *map)
{
	t_link		*link;

	link = map->all_links_here;
	if (variant->level < current->level || variant == map->start)
		return (variant);
	while (link)
	{
		if (link->first_room == current && link->second_room != variant && link->second_room != map->end
		&& !link->second_room->is_dead_end && link->second_room != map->end
		&& link->first_room->level > link->second_room->level)
		{
			if (link->second_room->where && compare_paths(link->second_room, current) > 0)
				remake_paths2(link->second_room, current, map);
			else if (!link->second_room->where)
				return (link->second_room);
		}
		else if (link->second_room == current && link->first_room != variant && link->first_room != map->end
		&& !link->first_room->is_dead_end && link->first_room != map->end
		&& link->first_room->level < link->second_room->level)
		{
			if (link->first_room->where && compare_paths(link->first_room, current) > 0)
				remake_paths2(link->first_room, current, map);
			else if (!link->first_room->where)
				return (link->first_room);
		}
		link = link->next;
	}
    return (best_variant3(current, variant, map));
}

int				check_best_variant(t_room *room, t_room *variant)
{
    if (room != variant && room->outputs == 1 && !room->is_dead_end
        && !room->where)
        return (1);
    return (0);
}

t_room			*best_variant(t_room *current, t_room *variant, t_main *map)
{
	t_link		*link;

	if ((variant->outputs == 1 && variant->level < current->level)
	|| variant == map->start)
		return (variant);
	else
	{
		link = map->all_links_here;
		while (link)
		{
			if (link->first_room == current && link->second_room != map->end &&
			check_best_variant(link->second_room, variant) &&
			link->first_room->level > link->second_room->level)
				return (link->second_room);
			else if (link->second_room == current &&
			link->first_room->level < link->second_room->level &&
			link->first_room != map->end && check_best_variant(link->first_room, variant))
				return (link->first_room);
			link = link->next;
		}
	}
	return (best_variant2(current, variant, map));
}