/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:28:52 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/07/02 11:17:22 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"
#include "so_long.h"

void	init_game_s(t_game *game)
{
	game->mlx = NULL;
	game->map = NULL;
	game->visited_map = NULL;
	game->wall_img = NULL;
	game->exit_img = NULL;
	game->collectionable_img = NULL;
	game->player_img = NULL;
	game->moves = 0;
	game->map_height = 0;
	game->map_width = 0;
	game->total_collecionables = 0;
	game->total_exits = 0;
	game->total_player = 0;
	game->player_pos.x = -1;
	game->player_pos.y = -1;
	game -> initial_collectibles_drawn = 0;
}

void	load_game_data(t_game *game, char *map_path)
{
	game -> map_height = height(map_path, game);
	game -> map = get_map(map_path, game -> map_height, game);
	game -> map_width = ft_strlen(game -> map[0]);
	ft_map_form(game);
	is_valid_map(game);
	get_player_pos(game -> map, &game -> player_pos);
	game -> visited_map = visited_map(game);
	flood_fil(&game->player_pos,
		game->map_height, game -> map, game -> visited_map);
	check_acces(game);
}

int	main(int argc, char *argv[])
{
	t_game	game;

	init_game_s(&game);
	if (argc != 2
		|| ft_strncmp(&argv[1][ft_strlen(argv[1]) - 4], ".ber", 4) != 0)
	{
		ft_error_and_exit(&game);
	}
	load_game_data(&game, argv[1]);
	game.mlx = mlx_init(game.map_width * 64,
			game.map_height * 64, "so_long", false);
	if (!game.mlx)
		ft_error_and_exit(&game);
	draw_start_map(&game);
	game.player_img = draw_player(game.mlx, game.player_pos, &game);
	mlx_key_hook(game.mlx, &key_hook, &game);
	mlx_loop(game.mlx);
	free_memory(game.map, game.visited_map, game.map_height);
	cleanup_mlx_resources(&game);
	return (0);
}
