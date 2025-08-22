/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_improved.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:24:59 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/22 19:26:10 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	count_words_and_validate(const char *s)
{
	int	count;
	int	in_quote;

	count = 0;
	in_quote = 0;
	while (*s)
	{
		while (*s == ' ')
			s++;
		if (*s)
			count++;
		while (*s)
		{
			if (*s == '\'')
				in_quote = !in_quote;
			if (*s == ' ' && in_quote == 0)
				break ;
			s++;
		}
	}
	if (in_quote)
		return (-1);
	return (count);
}

static void	find_next_word(const char *s, int *i, int *start)
{
	int	in_quote;

	while (s[*i] == ' ')
		(*i)++;
	*start = *i;
	in_quote = 0;
	while (s[*i])
	{
		if (s[*i] == '\'')
			in_quote = !in_quote;
		if (s[*i] == ' ' && in_quote == 0)
			break ;
		(*i)++;
	}
}

static char	*remove_single_quotes(const char *s)
{
	char	*dst;
	size_t	src_len;
	size_t	i;
	size_t	dst_i;

	if (!s)
		return (NULL);
	src_len = ft_strlen(s);
	dst = (char *)malloc(src_len + 1);
	if (!dst)
		return (NULL);
	i = 0;
	dst_i = 0;
	while (i < src_len)
	{
		if (s[i] != '\'')
			dst[dst_i++] = s[i];
		i++;
	}
	dst[dst_i] = '\0';
	return (dst);
}

static char	**fill(const char *s, char **result, int word_count)
{
	int		i;
	int		word_index;
	int		start;
	char	*str;
	char	*str_noquotes;

	i = 0;
	word_index = 0;
	while (word_index < word_count)
	{
		find_next_word(s, &i, &start);
		str = ft_substr(s, start, i - start);
		if (!str)
			return (free_2d_array(result), NULL);
		str_noquotes = remove_single_quotes(str);
		free(str);
		if (!str_noquotes)
			return (free_2d_array(result), NULL);
		result[word_index] = str_noquotes;
		word_index++;
	}
	return (result);
}

char	**split_improved(const char *s)
{
	char	**result;
	int		word_count;
	int		i;

	if (!s)
		return (NULL);
	word_count = count_words_and_validate(s);
	if (word_count == -1)
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < word_count + 1)
		result[i++] = NULL;
	return (fill(s, result, word_count));
}
