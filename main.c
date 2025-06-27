/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:28:52 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/27 11:45:44 by aldiaz-u         ###   ########.fr       */
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
void	ft_error_and_exit(char **fill_lines, int **visited_map, int saltos)
{
	if (fill_lines)
		free_memory(fill_lines, NULL, saltos);
	if (visited_map)
		free_memory(NULL, visited_map, saltos);
	get_next_line(-1);
	printf("Error\n");
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

void	map_errors(int map_fd)
{
	if (map_fd < 0)
	{
		printf("Error\n");
		exit(1);
	}
}

int	height(char *map)
{
	int		map_fd;
	char	*line;
	int		saltos;

	saltos = 0;
	map_fd = open(map, O_RDONLY);
	map_errors(map_fd);
	while ((line = get_next_line(map_fd)))
	{
		saltos++;
		free(line);
	}
	close(map_fd);
	return (saltos);
}

char **fill(char *map, int saltos)
{
	int		map_fd;
	char	*line;
	char	**fill_lines;
	int		i;
	int		len;

	i = 0;
	fill_lines = malloc(sizeof(char *) * (saltos + 1));
	map_fd = open(map, O_RDONLY);
	map_errors(map_fd);
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

void	quantity(char **fill_lines, int *index, int *k, int saltos, int *c, int *p, int *e)
{
	int	line_len;

	line_len = (int) ft_strlen(fill_lines[*index]);
	while (fill_lines[*index][*k])
	{
		if (*index == 0 || *index == saltos - 1)
		{
			if (fill_lines[*index][*k] != '1')
				ft_error_and_exit(fill_lines, NULL, saltos);
		}
		if (fill_lines[*index][*k] == 'C')
			(*c)++;
		if (fill_lines[*index][*k] == 'P')
			(*p)++;
		if (fill_lines[*index][*k] == 'E')
			(*e)++;
		if (*index > 0 && *index < saltos - 1)
		{
			if (fill_lines[*index][0] != '1' || fill_lines[*index][line_len - 1] != '1')
				ft_error_and_exit(fill_lines, NULL, saltos);
		}
		if (fill_lines[*index][*k] != '1' && fill_lines[*index][*k] != '0' && fill_lines[*index][*k] != 'C' && fill_lines[*index][*k] != 'P' && fill_lines[*index][*k] != 'E')
			ft_error_and_exit(fill_lines, NULL, saltos);
		(*k)++;
	}
}

void	is_valid_map(char **fill_lines, int saltos)
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
	while (fill_lines[index])
	{
		k = 0;
		if (ft_strlen(fill_lines[index]) < 5 || saltos < 3)
			ft_error_and_exit(fill_lines, NULL, saltos);
		quantity(fill_lines, &index, &k, saltos, &c, &p, &e);
		index++;
	}
	if (c <= 0 || e != 1 || p != 1)
		ft_error_and_exit(fill_lines, NULL, saltos);
}

void	ft_map_form(char **fill_lines, int saltos)
{
	int		i;
	size_t	width;

	i = 0;
	width = ft_strlen(fill_lines[0]);
	if (saltos == (int) width)
		ft_error_and_exit(fill_lines, NULL, saltos);
	while (fill_lines[i])
	{
		if (ft_strlen(fill_lines[i]) != width)
			ft_error_and_exit(fill_lines, NULL, saltos);
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
			}
			x++;
		}
		y++;
	}
	return (0);
}

int	**visited_map(char **fill_lines,int saltos)
{
	int	len;
	int	y;
	int	x;
	int	**visited;

	len = ft_strlen(fill_lines[0]);
	visited = (int **)malloc(sizeof(int *) * saltos);
	y = 0;
	while (y < saltos)
	{
		x = 0;
		visited[y] = (int *)malloc(sizeof(int) * len);
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

void	check_acces(int **visited_map, char **fill_lines, int saltos)
{
	int	len;
	int	y;
	int	x;

	len = ft_strlen(fill_lines[0]);
	y = 0;
	while (y < saltos)
	{
		x = 0;
		while (x < len)
		{
			if (fill_lines[y][x] == 'P' && visited_map[y + 1][x] == 0 && visited_map[y - 1][x] && visited_map[y][x + 1] && visited_map[y][x - 1])
				ft_error_and_exit(fill_lines, visited_map, saltos);
			if ((fill_lines[y][x] == 'C' || fill_lines[y][x] == 'E') && visited_map[y][x] == 0)
				ft_error_and_exit(fill_lines, visited_map, saltos);
			x++;
		}
		y++;
	}
}

int	main(int argc, char *argv[])
{
	int		saltos;
	char	**fill_lines;
	t_point	pos;
	int	len;
	int	**visited;


	if (argc != 2 || ft_strncmp(&argv[1][ft_strlen(argv[1]) - 4], ".ber", 4) != 0)
	{
		printf("Error\n");
		exit(1);
	}
	pos.x = -1;
	pos.y = -1;
	saltos = height(argv[1]);
	fill_lines = fill(argv[1], saltos);
	ft_map_form(fill_lines, saltos);
	is_valid_map(fill_lines, saltos);
	get_player_pos(fill_lines, &pos);
	len = ft_strlen(fill_lines[0]);
	visited = visited_map(fill_lines, saltos);
	flood_fil(&pos, saltos, fill_lines, visited);
	check_acces(visited, fill_lines, saltos);
	free_memory(fill_lines, visited, saltos);
	return (0);
}
