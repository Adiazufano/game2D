/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 09:38:01 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/07/02 11:31:16 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H
# include "MLX42/include/MLX42/MLX42.h"
# include "get_next_line.h"
# include "printf/ft_printf.h"

typedef struct s_point
{
	int	x;
	int	y;
}				t_point;

typedef struct s_game
{
	mlx_t		*mlx;
	t_point		player_pos;
	char		**map;
	int			moves;
	int			map_width;
	int			map_height;
	int			total_collecionables;
	int			total_player;
	int			total_exits;
	int			initial_collectibles_drawn;
	mlx_image_t	*player_img;
	mlx_image_t	*wall_img;
	mlx_image_t	*exit_img;
	mlx_image_t	*collectionable_img;
	mlx_image_t	*background;
	int			**visited_map;
}				t_game;
void			draw_collectionables(t_game *game);
void			is_collectionable(t_game *game, int new_x, int new_y);
mlx_image_t		*draw_player(mlx_t *mlx, t_point player_pos, t_game *game);
void			draw_start_map(t_game *game);
void			draw_exit(t_game *game);
void			is_exit(t_game *game, int new_x, int new_y);
void			ft_error_and_exit(t_game *game);
void			map_errors(int map_fd, t_game *game);
void			check_acces(t_game *game);
mlx_image_t		*get_image(mlx_t *mlx, mlx_texture_t *texture);
void			resize_image(mlx_image_t *img,
					t_game *game, int width, int height);
mlx_texture_t	*get_texture(char *path);
char			**get_map(char *map_path, int saltos, t_game *game);
void			is_valid_map(t_game *game);
void			ft_map_form(t_game *game);
void			key_hook(mlx_key_data_t keydata, void *param);
void			free_memory(char **map, int **visited_map, int saltos);
void			cleanup_mlx_resources(t_game *game);
int				height(char *map, t_game *game);
int				get_player_pos(char **map, t_point *pos);
int				**visited_map(t_game *game);
void			flood_fil(t_point *pos, int saltos,
					char **map, int **visited_map);
#endif