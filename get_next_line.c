/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:56:13 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/05/01 11:43:31 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*free_and_return(char *to_free1, char *to_free2)
{
	if (to_free1)
		free(to_free1);
	if (to_free2)
		free(to_free2);
	return (NULL);
}

char	*read_line(int fd, char *stack)
{
	char	*buffer;
	char	*temp;
	ssize_t	number_bytes;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free_and_return(stack, NULL));
	while (!ft_strchr(stack, '\n'))
	{
		number_bytes = read(fd, buffer, BUFFER_SIZE);
		if (number_bytes == 0)
			break ;
		if (number_bytes < 0)
			return (free_and_return(buffer, stack));
		buffer[number_bytes] = '\0';
		temp = stack;
		stack = ft_strjoin(stack, buffer);
		free(temp);
		if (!stack)
			return (free_and_return(buffer, NULL));
	}
	free(buffer);
	return (stack);
}

static char	*extract_line(char *s, char **r)
{
	char	*l;
	size_t	i;

	i = 0;
	if (!s || !s[0])
		return (free(s), NULL);
	while (s[i] && s[i] != '\n')
		i++;
	l = ft_substr(s, 0, i + (s[i] == '\n'));
	if (!l)
		return (free_and_return(s, NULL));
	*r = ft_strdup(s + i + (s[i] == '\n'));
	if (!*r && s[i])
	{
		free(l);
		free(s);
		return (NULL);
	}
	return (free(s), l);
}

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*stack;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(remainder);
		remainder = NULL;
		return (NULL);
	}
	if (remainder)
		stack = ft_strdup(remainder);
	else
		stack = ft_strdup("");
	free(remainder);
	remainder = NULL;
	if (!stack)
		return (NULL);
	stack = read_line(fd, stack);
	if (!stack)
		return (NULL);
	line = extract_line(stack, &remainder);
	return (line);
}
// int	main(void)
// {
// 	int fd = open("prueba.txt", O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror("Error al abrir el archivo");
// 		return (1);
// 	}

// 	char *line;
// 	int count = 0;

// 	// 🔁 Leer todas las líneas incluyendo los saltos de línea y NULL
// 	while ((line = get_next_line(fd)))
// 	{
// 		printf("Línea %d: [", count + 1);
// 		for (int i = 0; line[i] != '\0'; i++)
// 		{
// 			if (line[i] == '\n')
// 				printf("\\n"); // Mostrar explícitamente el salto de línea
// 			else
// 				putchar(line[i]); // Mostrar el resto de los caracteres
// 		}
// 		printf("]\n");
// 		free(line);
// 		count++;
// 	}

// 	// Mostrar NULL al final
// 	printf("Línea %d: [NULL]\n", count + 1);
// 	printf("BUFFER_SIZE: %i\n", BUFFER_SIZE);

// 	// Liberar recursos adicionales si es necesario
// 	get_next_line(-1); // Forzar liberación de la memoria estática en GNL
// 	close(fd);
// 	return (0);
// }
