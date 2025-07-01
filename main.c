/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:28:52 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/07/01 16:42:30 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"
#include "so_long.h"

void	free_memory(char **fill_lines, int **visited_map, int saltos)
{
	int	i;

	if (fill_lines)
	{
		i = 0;
		while (fill_lines[i])
			free(fill_lines[i++]);
		free(fill_lines);
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

void	ft_error_and_exit(t_game *game)
{
	printf("Error\n");
	free_memory(game -> map, game -> visited_map, game -> map_height);
	cleanup_mlx_resources(game);
	get_next_line(-1);
	exit(1);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*u_s1;	
	unsigned char	*u_s2;
	size_t			i;

	u_s1 = (unsigned char *)s1;
	u_s2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (u_s1[i] != u_s2[i])
			return (u_s1[i] - u_s2[i]);
		if (u_s1[i] == '\0' || u_s2[i] == '\0')
			break ;
		i++;
	}
	return (0);
}

void	map_errors(int map_fd, t_game *game)
{
	if (map_fd < 0)
		ft_error_and_exit(game);
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

char	**fill(char *map, int saltos, t_game *game)
{
	int		map_fd;
	char	*line;
	char	**fill_lines;
	int		i;
	int		len;

	i = 0;
	fill_lines = malloc(sizeof(char *) * (saltos + 1));
	map_fd = open(map, O_RDONLY);
	map_errors(map_fd, game);
	while (i < saltos)
	{
		line = get_next_line(map_fd);
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		fill_lines[i] = line;
		i++;
	}
	fill_lines[i] = NULL;
	get_next_line(-1);
	close(map_fd);
	return (fill_lines);
}

void	count_items(t_game *game, int *index, int *k)
{
	if (game -> map[*index][*k] == 'C')
		game -> total_collecionables++;
	if (game -> map[*index][*k] == 'P')
		game -> total_player++;
	if (game -> map[*index][*k] == 'E')
		game -> total_exits++;
}

void	quantity(t_game *game, int *index, int *k)
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
		while (x < game -> map_height)
			visited[y][x++] = 0;
		y++;
	}
	return (visited);
}

void	flood_fil(t_point *pos, int saltos,
			char **fill_lines, int **visited_map)
{
	int		len;
	t_point	next_pos;

	len = ft_strlen(fill_lines[0]);
	if (pos -> x < 0 || pos -> x >= len
		|| pos -> y < 0 || pos -> y >= saltos
		|| fill_lines[pos -> y][pos -> x] == '1'
		|| visited_map[pos -> y][pos -> x] == 1)
		return ;
	visited_map[pos -> y][pos -> x] = 1;
	next_pos.x = pos -> x + 1;
	next_pos.y = pos -> y;
	flood_fil(&next_pos, saltos, fill_lines, visited_map);
	next_pos.x = pos -> x - 1;
	next_pos.y = pos -> y;
	flood_fil(&next_pos, saltos, fill_lines, visited_map);
	next_pos.x = pos -> x;
	next_pos.y = pos -> y + 1;
	flood_fil(&next_pos, saltos, fill_lines, visited_map);
	next_pos.x = pos -> x;
	next_pos.y = pos -> y - 1;
	flood_fil(&next_pos, saltos, fill_lines, visited_map);
}

void	check_acces(t_game *game)
{
	int	len;
	int	y;
	int	x;

	len = ft_strlen(game -> map[0]);
	y = 0;
	while (y < game -> map_height)
	{
		x = 0;
		while (x < len)
		{
			if (game -> map[y][x] == 'P'
				&& game -> visited_map[y + 1][x] == 0
				&& game -> visited_map[y - 1][x]
				&& game -> visited_map[y][x + 1]
				&& game -> visited_map[y][x - 1])
				ft_error_and_exit(game);
			if ((game -> map[y][x] == 'C'
				|| game -> map[y][x] == 'E')
				&& game -> visited_map[y][x] == 0)
				ft_error_and_exit(game);
			x++;
		}
		y++;
	}
}

mlx_texture_t	*get_texture(char *path)
{
	mlx_texture_t	*texture;

	texture = mlx_load_png(path);
	if (!texture)
		return (NULL);
	return (texture);
}

mlx_image_t	*get_image(mlx_t *mlx, mlx_texture_t *texture)
{
	mlx_image_t	*img;

	if (!texture)
		return (NULL);
	img = mlx_texture_to_image(mlx, texture);
	mlx_delete_texture(texture);
	if (!img)
	{
		printf("error");
		return (NULL);
	}
	return (img);
}

void	resize_image(mlx_image_t *img, t_game *game, int width, int height)
{
	if (mlx_resize_image(img, width, height) == false)
		ft_error_and_exit(game);
}

void	draw_walls(t_game *game)
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

void	draw_c(t_game *game, int x, int y)
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

void	background(t_game *game)
{
	mlx_texture_t	*texture;

	texture = get_texture("./temp/background.png");
	if (!texture)
		ft_error_and_exit(game);
	game -> background = get_image(game->mlx, texture);
	if (!game->background)
		ft_error_and_exit(game);
	resize_image(game -> background, game,(game -> map_width * 64),  (game -> map_height * 64));
	if ( mlx_image_to_window(game -> mlx,
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

int	total_collectionables(char **fill_lines)
{
	int	x;
	int	y;
	int	total;

	total = 0;
	y = 0;
	while (fill_lines[y])
	{
		x = 0;
		while (fill_lines[y][x])
		{
			if (fill_lines[y][x] == 'C')
				total++;
			x++;
		}
		y++;
	}
	return (total);
}

void	hide_collected_collectionable(t_game *game, int collected_x, int collected_y)
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

void	is_ollectionable(t_game *game, int new_x, int new_y)
{
	if (game -> map[new_y][new_x] == 'C')
	{
		game -> map[new_y][new_x] = '0';
		game->total_collecionables--;
		printf("Coleccionables restantes: %d\n", game->total_collecionables);
		hide_collected_collectionable(game, new_x, new_y);
	}
}

void	is_exit(t_game *game, int new_x, int new_y)
{
	if (game->map[new_y][new_x] == 'E')
	{
		if (game->total_collecionables == 0)
		{
			printf("Felicidades has escpado!!\n");
			mlx_close_window(game->mlx);
			return ;
		}
		else
		{
			printf("!Aun quedan coleccionables por recoger!\n");
			return ;
		}
	}
}

void	move(t_game *game, int new_x, int new_y)
{
	is_ollectionable(game, new_x, new_y);
	is_exit(game, new_x, new_y);
	if (game -> total_collecionables == 0 && game -> map[new_y][new_x] == 'E')
		return ;
	game->player_pos.x = new_x;
	game->player_pos.y = new_y;
	game->player_img->instances[0].x = game->player_pos.x * 64;
	game->player_img->instances[0].y = game->player_pos.y * 64;
	game->moves++;
	printf("Movimientos: %d\n", game->moves);
}

int	keys(mlx_key_data_t keydata, t_game *game, t_point *target_pos_out)
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
			if (temp_player_pos.x != game->player_pos.x || temp_player_pos.y != game->player_pos.y)
			{
				move(game, temp_player_pos.x, temp_player_pos.y);
			}
		}
	}
}

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
	game -> map = fill(map_path, game -> map_height, game);
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
