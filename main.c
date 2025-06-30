/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:28:52 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/30 18:34:17 by aldiaz-u         ###   ########.fr       */
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
	int i;
	
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
		while(i < saltos)
			free(visited_map[i++]);
		free(visited_map);
	}
}

void    cleanup_mlx_resources(t_game *game)
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
	while ((line = get_next_line(map_fd)))
	{
		saltos++;
		free(line);
	}
	close(map_fd);
	return (saltos);
}

char **fill(char *map, int saltos, t_game *game)
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
	while ((line = get_next_line(map_fd)))
	{
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

void	quantity(t_game *game, int *index, int *k, int *c, int *p, int *e)
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
		if (game -> map[*index][*k] == 'C')
			(*c)++;
		if (game -> map[*index][*k] == 'P')
			(*p)++;
		if (game -> map[*index][*k] == 'E')
			(*e)++;
		if (*index > 0 && *index < game -> map_height - 1)
		{
			if (game -> map[*index][0] != '1' || game -> map[*index][line_len - 1] != '1')
				ft_error_and_exit(game);
		}
		if (game -> map[*index][*k] != '1' && game -> map[*index][*k] != '0' && game -> map[*index][*k] != 'C' && game -> map[*index][*k] != 'P' && game -> map[*index][*k] != 'E')
			ft_error_and_exit(game);
		(*k)++;
	}
}

void	is_valid_map(t_game *game)
{
	int	k;
	int	c;
	int	p;
	int	e;
	int	index;

	k = 0;
	c = 0;
	p = 0;
	e = 0;
	index = 0;
	while (game -> map[index])
	{
		k = 0;
		if (ft_strlen(game -> map[index]) < 3 || game -> map_height < 3)
			ft_error_and_exit(game);
		quantity(game, &index, &k, &c, &p, &e);
		index++;
	}
	if (c <= 0 || e != 1 || p != 1)
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

int	get_player_pos(char **fill_lines, t_point *pos)
{
	int	x;
	int	y;

	y = 0;
	while (fill_lines[y])
	{
		x = 0;
		while (fill_lines[y][x])
		{
			if (fill_lines[y][x] == 'P')
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

int **visited_map(char **fill_lines, int saltos, t_game *game)
{
	int len;
	int y;
	int x;
	int **visited;

	len = ft_strlen(fill_lines[0]);
	visited = (int **)malloc(sizeof(int *) * saltos);
	if (!visited) // CAMBIO: Chequear malloc
		ft_error_and_exit(game);
	y = 0;
	while (y < saltos)
	{
		visited[y] = (int *)malloc(sizeof(int) * len);
		if (!visited[y]) // CAMBIO: Chequear malloc y limpiar si falla
		{
			while (y > 0)
				free(visited[--y]);
			free(visited);
			ft_error_and_exit(game);
		}
		x = 0;
		while (x < len)
		{
			visited[y][x] = 0;
			x++;
		}
		y++;
	}
	return (visited);
}

void	flood_fil(t_point *pos, int saltos, char **fill_lines, int **visited_map)
{
	int	len;
	t_point	next_pos;
	
	len = ft_strlen(fill_lines[0]);
	if (pos -> x < 0 || pos -> x >= len || pos -> y < 0 || pos -> y >= saltos || fill_lines[pos -> y][ pos -> x] == '1' || visited_map[pos -> y][pos -> x] == 1)
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
	next_pos.x = pos  -> x;
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
			if (game -> map[y][x] == 'P' && game -> visited_map[y + 1][x] == 0 && game -> visited_map[y - 1][x] && game -> visited_map[y][x + 1] && game -> visited_map[y][x - 1])
				ft_error_and_exit(game);
			if ((game -> map[y][x] == 'C' || game -> map[y][x] == 'E') && game -> visited_map[y][x] == 0)
				ft_error_and_exit(game);
			x++;
		}
		y++;
	}
}

mlx_texture_t *get_texture(char *path)
{
	mlx_texture_t *texture;
	texture = mlx_load_png(path);
	if (!texture)
		return (NULL);
	return (texture);
}

mlx_image_t *get_image(mlx_t *mlx, mlx_texture_t *texture)
{
	mlx_image_t *img;
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

void	resize_image(mlx_image_t *img, t_game *game)
{
	if (mlx_resize_image(img, 64, 64) == false)
		ft_error_and_exit(game);
}

void    draw_walls(t_game *game)
{
	int x;
	int y;
	mlx_texture_t *texture;

	texture = get_texture("./temp/pared.png");
	if (!texture)
		ft_error_and_exit(game);
	game -> wall_img  = get_image(game->mlx, texture);
	if (!game->wall_img)
		ft_error_and_exit(game);
	resize_image(game -> wall_img, game);
	y = 0;
	while (game -> map[y])
	{
		x = 0;
		while (game -> map[y][x])
		{
			if (game -> map[y][x] == '1' && mlx_image_to_window(game -> mlx, game -> wall_img, (x * 64), (y * 64))  < 0)
			{
				ft_error_and_exit(game);
			}
			x++;
		}
		y++;
	}
}

mlx_image_t *draw_player(mlx_t *mlx, t_point player_pos, t_game *game)
{
	mlx_texture_t   *texture;
	mlx_image_t     *img;

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
	resize_image(img, game);
	if (mlx_image_to_window(mlx, img, (player_pos.x * 64), (player_pos.y * 64)) < 0)
	{
		ft_error_and_exit(game);
		return (NULL);
	}
	return (img);
}

void    draw_exit(t_game *game)
{
	int x;
	int y;
	mlx_texture_t *texture;

	texture = get_texture("./temp/exit.png");
	if (!texture)
		ft_error_and_exit(game);
	game -> exit_img  = get_image(game->mlx, texture);
	if (!game->exit_img)
		ft_error_and_exit(game);
	resize_image(game -> exit_img, game);
	y = 0;
	while (game -> map[y])
	{
		x = 0;
		while (game -> map[y][x])
		{
			if (game -> map[y][x] == 'E' && mlx_image_to_window(game -> mlx, game -> exit_img, (x * 64), (y * 64))  < 0)
			{
				ft_error_and_exit(game);
			}
			x++;
		}
		y++;
	}
}

void    draw_collectionables(t_game *game)
{
	int x;
	int y;
	mlx_texture_t *texture;

	texture = get_texture("./temp/queso.png");
	if (!texture)
		ft_error_and_exit(game);
	game -> collectionable_img = get_image(game->mlx, texture);
	if (!game->collectionable_img)
		ft_error_and_exit(game);
	resize_image(game -> collectionable_img, game);
	y = 0;
	while (game -> map[y])
	{
		x = 0;
		while (game -> map[y][x])
		{
			if (game -> map[y][x] == 'C' && mlx_image_to_window(game -> mlx, game -> collectionable_img, (x * 64), (y * 64))  < 0)
			{
				ft_error_and_exit(game);
			}
			x++;
		}
		y++;
	}
}


void	draw_start_map(t_game *game)
{
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

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game *game;
	int		new_x;
	int		new_y;
	int		moved;

	moved = 0;
	game = (t_game *) param;
	new_x = game -> player_pos.x;
	new_y = game -> player_pos.y;
	if (keydata.action == MLX_PRESS)
	{
		if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_UP)
		{
			new_y--;
			moved = 1;
		}
		else if (keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_DOWN)
		{
			moved = 1;
			new_y++;
		}
		else if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_RIGHT)
		{
			moved = 1;
			new_x++;
		}
		else if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_LEFT)
		{
			moved = 1;
			new_x--;
		}
		else if (keydata.key == MLX_KEY_ESCAPE)
			mlx_close_window(game -> mlx);
	}
	if (moved)
	{
		if (new_x >= 0 && new_x < game -> map_width
			&& new_y >= 0 && new_y < game -> map_height
			&& game -> map[new_y][new_x] != '1')
		{
			if (game -> map[new_y][new_x]  == 'C')
			{
				game -> map[new_y][new_x] = '0';
				game->total_collecionables--;
				printf("Coleccionables restantes: %d\n", game->total_collecionables);
			} 
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
			else
			{
				game->player_pos.x = new_x;
				game->player_pos.y = new_y;
				game->player_img->instances[0].x = game->player_pos.x * 64;
				game->player_img->instances[0].y = game->player_pos.y * 64;
				game->moves++;
			}
			printf("Movimientos: %d\n", game->moves);
		}
	}
}

int main(int argc, char *argv[])
{
	t_game  game;

	game.mlx = NULL;
	game.map = NULL;
	game.visited_map = NULL;
	game.wall_img = NULL;
	game.exit_img = NULL;
	game.collectionable_img = NULL;
	game.player_img = NULL;
	game.moves = 0;
	game.map_height = 0;
	game.map_width = 0;
	game.total_collecionables = 0;
	game.player_pos.x = -1;
	game.player_pos.y = -1;
	if (argc != 2 || ft_strncmp(&argv[1][ft_strlen(argv[1]) - 4], ".ber", 4) != 0)
	{
		ft_error_and_exit(&game);
	}
	game.map_height = height(argv[1], &game);
	game.map = fill(argv[1], game.map_height, &game);
	game.map_width = ft_strlen(game.map[0]);
	ft_map_form(&game);
	is_valid_map(&game);
	get_player_pos(game.map, &game.player_pos);
	game.visited_map = visited_map(game.map, game.map_height, &game);
	flood_fil(&game.player_pos, game.map_height, game.map, game.visited_map);
	check_acces(&game);
	game.mlx = mlx_init(game.map_width * 64, game.map_height * 64, "so_long", false);
	if (!game.mlx)
		ft_error_and_exit(&game);
	game.total_collecionables = total_collectionables(game.map);
	draw_start_map(&game);
	game.player_img = draw_player(game.mlx, game.player_pos, &game);
	mlx_key_hook(game.mlx, &key_hook, &game);
	mlx_loop(game.mlx);
	free_memory(game.map, game.visited_map, game.map_height);
	cleanup_mlx_resources(&game);
	return (0);
}

