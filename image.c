/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 10:49:27 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/07/02 11:30:09 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
		ft_printf("error");
		return (NULL);
	}
	return (img);
}

void	resize_image(mlx_image_t *img, t_game *game, int width, int height)
{
	if (mlx_resize_image(img, width, height) == false)
		ft_error_and_exit(game);
}
