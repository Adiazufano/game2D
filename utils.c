/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 10:55:19 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/07/02 11:29:44 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	free_memory(char **map, int **visited_map, int saltos)
{
	int	i;

	if (map)
	{
		i = 0;
		while (map[i])
			free(map[i++]);
		free(map);
	}
	if (visited_map)
	{
		i = 0;
		while (i < saltos)
			free(visited_map[i++]);
		free(visited_map);
	}
}

void	cleanup_mlx_resources(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	if (game->player_img)
		mlx_delete_image(game->mlx, game->player_img);
	if (game->wall_img)
		mlx_delete_image(game->mlx, game->wall_img);
	if (game->exit_img)
		mlx_delete_image(game->mlx, game->exit_img);
	if (game->collectionable_img)
		mlx_delete_image(game->mlx, game->collectionable_img);
	mlx_terminate(game->mlx);
}

int	height(char *map, t_game *game)
{
	int		map_fd;
	char	*line;
	int		saltos;

	saltos = 0;
	map_fd = open(map, O_RDONLY);
	map_errors(map_fd, game);
	line = get_next_line(map_fd);
	while (line != NULL)
	{
		saltos++;
		free(line);
		line = get_next_line(map_fd);
	}
	close(map_fd);
	return (saltos);
}

int	get_player_pos(char **map, t_point *pos)
{
	int	x;
	int	y;

	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == 'P')
			{
				pos -> x = x;
				pos -> y = y;
				return (1);
			}
			x++;
		}
		y++;
	}
	return (0);
}

int	**visited_map(t_game *game)
{
	int	y;
	int	x;
	int	**visited;

	visited = (int **)malloc(sizeof(int *) * game -> map_height);
	if (!visited)
		ft_error_and_exit(game);
	y = 0;
	while (y < game -> map_height)
	{
		visited[y] = (int *)malloc(sizeof(int) * game -> map_width);
		if (!visited[y])
		{
			while (y > 0)
				free(visited[--y]);
			free(visited);
			ft_error_and_exit(game);
		}
		x = 0;
		while (x < game -> map_width)
			visited[y][x++] = 0;
		y++;
	}
	return (visited);
}
