/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_items.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 10:59:39 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/07/02 11:09:51 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	draw_walls(t_game *game)
{
	int				x;
	int				y;
	mlx_texture_t	*texture;

	texture = get_texture("./temp/pared.png");
	if (!texture)
		ft_error_and_exit(game);
	game -> wall_img = get_image(game->mlx, texture);
	if (!game->wall_img)
		ft_error_and_exit(game);
	resize_image(game -> wall_img, game, 64, 64);
	y = 0;
	while (game -> map[y])
	{
		x = 0;
		while (game -> map[y][x])
		{
			if (game -> map[y][x] == '1'
				&& mlx_image_to_window(game -> mlx,
					game -> wall_img, (x * 64), (y * 64)) < 0)
				ft_error_and_exit(game);
			x++;
		}
		y++;
	}
}

mlx_image_t	*draw_player(mlx_t *mlx, t_point player_pos, t_game *game)
{
	mlx_texture_t	*texture;
	mlx_image_t		*img;

	texture = get_texture("./temp/player.png");
	if (!texture)
	{
		ft_error_and_exit(game);
		return (NULL);
	}
	img = get_image(mlx, texture);
	if (!img)
	{
		ft_error_and_exit(game);
		return (NULL);
	}
	resize_image(img, game, 64, 64);
	if (mlx_image_to_window(mlx, img,
			(player_pos.x * 64), (player_pos.y * 64)) < 0)
	{
		ft_error_and_exit(game);
		return (NULL);
	}
	return (img);
}

static void	background(t_game *game)
{
	mlx_texture_t	*texture;

	texture = get_texture("./temp/background.png");
	if (!texture)
		ft_error_and_exit(game);
	game -> background = get_image(game->mlx, texture);
	if (!game->background)
		ft_error_and_exit(game);
	resize_image(game -> background, game,
		(game -> map_width * 64), (game -> map_height * 64));
	if (mlx_image_to_window(game -> mlx,
			game -> background, 0, 0) < 0)
		ft_error_and_exit(game);
}

void	draw_start_map(t_game *game)
{
	background(game);
	draw_walls(game);
	draw_exit(game);
	draw_collectionables(game);
}
