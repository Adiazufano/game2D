/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 10:46:57 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/07/02 11:30:38 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	move(t_game *game, int new_x, int new_y)
{
	is_collectionable(game, new_x, new_y);
	is_exit(game, new_x, new_y);
	if (game -> total_collecionables == 0 && game -> map[new_y][new_x] == 'E')
		return ;
	game->player_pos.x = new_x;
	game->player_pos.y = new_y;
	game->player_img->instances[0].x = game->player_pos.x * 64;
	game->player_img->instances[0].y = game->player_pos.y * 64;
	game->moves++;
	ft_printf("Movimientos: %d\n", game->moves);
}

static int	keys(mlx_key_data_t keydata, t_game *game, t_point *target_pos_out)
{
	target_pos_out->x = game->player_pos.x;
	target_pos_out->y = game->player_pos.y;
	if (keydata.action != MLX_PRESS)
		return (0);
	if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_UP)
		target_pos_out -> y--;
	else if (keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_DOWN)
		target_pos_out -> y++;
	else if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_RIGHT)
		target_pos_out -> x++;
	else if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_LEFT)
		target_pos_out -> x--;
	else if (keydata.key == MLX_KEY_ESCAPE)
	{
		mlx_close_window(game -> mlx);
		return (0);
	}
	return (1);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;
	t_point	temp_player_pos;
	int		is_move;

	game = (t_game *) param;
	is_move = keys(keydata, game, &temp_player_pos);
	if (is_move)
	{
		if (temp_player_pos.x >= 0 && temp_player_pos.x < game -> map_width
			&& temp_player_pos.y >= 0 && temp_player_pos.y < game -> map_height
			&& game -> map[temp_player_pos.y][temp_player_pos.x] != '1')
		{
			if (temp_player_pos.x != game->player_pos.x
				|| temp_player_pos.y != game->player_pos.y)
				move(game, temp_player_pos.x, temp_player_pos.y);
		}
	}
}
