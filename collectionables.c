/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collectionables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 11:01:19 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/07/02 11:28:59 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	draw_c(t_game *game, int x, int y)
{
	if (mlx_image_to_window(game -> mlx,
			game -> collectionable_img, (x * 64), (y * 64)) < 0)
		ft_error_and_exit(game);
	game->initial_collectibles_drawn++;
}

void	draw_collectionables(t_game *game)
{
	int				x;
	int				y;
	mlx_texture_t	*texture;

	texture = get_texture("./temp/queso.png");
	if (!texture)
		ft_error_and_exit(game);
	game -> collectionable_img = get_image(game->mlx, texture);
	if (!game->collectionable_img)
		ft_error_and_exit(game);
	resize_image(game -> collectionable_img, game, 64, 64);
	y = 0;
	while (game -> map[y])
	{
		x = 0;
		while (game -> map[y][x])
		{
			if (game -> map[y][x] == 'C')
				draw_c(game, x, y);
			x++;
		}
		y++;
	}
}

static void	hide_collected_collectionable(t_game *game,
			int collected_x, int collected_y)
{
	int	i;

	i = 0;
	while (i < game -> initial_collectibles_drawn)
	{
		if (game -> collectionable_img -> instances[i].x / 64 == collected_x
			&& game -> collectionable_img -> instances[i].y / 64 == collected_y)
		{
			game -> collectionable_img -> instances[i].enabled = false;
		}
		i++;
	}
}

void	is_collectionable(t_game *game, int new_x, int new_y)
{
	if (game -> map[new_y][new_x] == 'C')
	{
		game -> map[new_y][new_x] = '0';
		game->total_collecionables--;
		ft_printf("Coleccionables restantes: %d\n", game->total_collecionables);
		hide_collected_collectionable(game, new_x, new_y);
	}
}
