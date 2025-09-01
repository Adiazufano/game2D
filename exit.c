/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 11:02:21 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/07/02 11:29:10 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	draw_exit(t_game *game)
{
	int				x;
	int				y;
	mlx_texture_t	*texture;

	texture = get_texture("./temp/exit.png");
	if (!texture)
		ft_error_and_exit(game);
	game -> exit_img = get_image(game->mlx, texture);
	if (!game->exit_img)
		ft_error_and_exit(game);
	resize_image(game -> exit_img, game, 64, 64);
	y = 0;
	while (game -> map[y])
	{
		x = 0;
		while (game -> map[y][x])
		{
			if (game -> map[y][x] == 'E'
				&& mlx_image_to_window(game -> mlx,
					game -> exit_img, (x * 64), (y * 64)) < 0)
				ft_error_and_exit(game);
			x++;
		}
		y++;
	}
}

void	is_exit(t_game *game, int new_x, int new_y)
{
	if (game->map[new_y][new_x] == 'E')
	{
		if (game->total_collecionables == 0)
		{
			ft_printf("Felicidades has escapado!!\n");
			mlx_close_window(game->mlx);
			return ;
		}
		else
		{
			ft_printf("!Aun quedan coleccionables por recoger!\n");
			return ;
		}
	}
}
