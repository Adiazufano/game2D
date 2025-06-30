/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 09:38:01 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/30 17:49:13 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
#define SO_LONG_H
#include "MLX42/include/MLX42/MLX42.h"
typedef struct	s_point
{
	int	x;
	int	y;
}				t_point;

typedef	struct s_game
{
	mlx_t	*mlx;
	t_point	player_pos;
	char	**map;
	int			moves;
	int			map_width;
	int			map_height;
	int			total_collecionables;
	mlx_image_t	*player_img;
	mlx_image_t	*wall_img;
	mlx_image_t	*exit_img;
	mlx_image_t	*collectionable_img;
	int			**visited_map;
}				t_game;
#endif