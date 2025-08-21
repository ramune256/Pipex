/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:25:43 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/18 15:50:53 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*free_and_null(char *text, char *buffer)
{
	if (text)
		free(text);
	if (buffer)
	{
		free(buffer);
		buffer = NULL;
	}
	return (NULL);
}

static char	*extract_line(char *text)
{
	char	*result;
	int		len;
	int		i;

	len = 0;
	i = 0;
	if (!text)
		return (NULL);
	if (*text == '\0')
		return (NULL);
	while (text[len] != '\0' && text[len] != '\n')
		len++;
	if (text[len] == '\n')
		len++;
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	while (i < len)
		result[i++] = *text++;
	result[i] = '\0';
	return (result);
}

static char	*update_saved(char *text)
{
	char	*new_saved;
	char	*text_ptr;
	int		i;

	if (!text)
		return (NULL);
	text_ptr = text;
	while (*text != '\0' && *text != '\n')
		text++;
	if (*text == '\0')
		return (free_and_null(text_ptr, NULL));
	text++;
	new_saved = malloc(sizeof(char) * (ft_strlen(text) + 1));
	if (!new_saved)
		return (free_and_null(text_ptr, NULL));
	i = 0;
	while (*text)
		new_saved[i++] = *text++;
	new_saved[i] = '\0';
	free(text_ptr);
	return (new_saved);
}

static char	*add_text(int fd, char *saved_text)
{
	char	*buffer;
	char	*temp;
	int		bytes_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free_and_null(saved_text, NULL));
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(saved_text, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free_and_null(saved_text, buffer));
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(saved_text, buffer);
		free(saved_text);
		if (!temp)
			return (free_and_null(NULL, buffer));
		saved_text = temp;
	}
	free(buffer);
	return (saved_text);
}

char	*get_next_line(int fd)
{
	static char	*saved_text;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (saved_text == NULL)
	{
		saved_text = malloc(sizeof(char));
		if (!saved_text)
			return (NULL);
		saved_text[0] = '\0';
	}
	saved_text = add_text(fd, saved_text);
	if (!saved_text)
		return (NULL);
	line = extract_line(saved_text);
	if (!line)
		return (saved_text = free_and_null(saved_text, NULL));
	saved_text = update_saved(saved_text);
	return (line);
}
