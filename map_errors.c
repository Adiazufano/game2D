/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 10:51:07 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/07/02 11:29:55 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

char	**get_map(char *map_path, int saltos, t_game *game)
{
	int		map_fd;
	char	*line;
	char	**map;
	int		i;
	int		len;

	i = 0;
	map = malloc(sizeof(char *) * (saltos + 1));
	map_fd = open(map_path, O_RDONLY);
	map_errors(map_fd, game);
	while (i < saltos)
	{
		line = get_next_line(map_fd);
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		map[i] = line;
		i++;
	}
	map[i] = NULL;
	get_next_line(-1);
	close(map_fd);
	return (map);
}

static void	count_items(t_game *game, int *index, int *k)
{
	if (game -> map[*index][*k] == 'C')
		game -> total_collecionables++;
	if (game -> map[*index][*k] == 'P')
		game -> total_player++;
	if (game -> map[*index][*k] == 'E')
		game -> total_exits++;
}

static void	quantity(t_game *game, int *index, int *k)
{
	int	line_len;

	line_len = (int) ft_strlen(game -> map[*index]);
	while (game -> map[*index][*k])
	{
		if (*index == 0 || *index == game -> map_height - 1)
		{
			if (game -> map[*index][*k] != '1')
				ft_error_and_exit(game);
		}
		count_items(game, index, k);
		if (*index > 0 && *index < game -> map_height - 1)
		{
			if (game -> map[*index][0] != '1'
				|| game -> map[*index][line_len - 1] != '1')
				ft_error_and_exit(game);
		}
		if (game -> map[*index][*k] != '1'
			&& game -> map[*index][*k] != '0'
			&& game -> map[*index][*k] != 'C'
			&& game -> map[*index][*k] != 'P'
			&& game -> map[*index][*k] != 'E')
			ft_error_and_exit(game);
		(*k)++;
	}
}

void	is_valid_map(t_game *game)
{
	int	k;
	int	index;

	k = 0;
	index = 0;
	while (game -> map[index])
	{
		k = 0;
		if (ft_strlen(game -> map[index]) < 3 || game -> map_height < 3)
			ft_error_and_exit(game);
		quantity(game, &index, &k);
		index++;
	}
	if (game -> total_collecionables <= 0
		|| game -> total_exits != 1
		|| game -> total_player != 1)
		ft_error_and_exit(game);
}

void	ft_map_form(t_game *game)
{
	int		i;
	size_t	width;

	i = 0;
	width = ft_strlen(game -> map[0]);
	if (game -> map_height == (int) width)
		ft_error_and_exit(game);
	while (game -> map[i])
	{
		if (ft_strlen(game -> map[i]) != width)
			ft_error_and_exit(game);
		i++;
	}
}
