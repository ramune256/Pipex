/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_improved.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:24:59 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/22 17:34:57 by shunwata         ###   ########.fr       */
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
		*i++;
	*start = *i;
	in_quote = 0;
	while (s[*i])
	{
		if (s[*i] == '\'')
			in_quote = !in_quote;
		if (s[*i] == ' ' && in_quote == 0)
			break ;
		*i++;
	}
}

static char	**fill(const char *s, char **result, int word_count)
{
	int		i;
	int		word_index;
	int		start;

	i = 0;
	word_index = 0;
	while (word_index < word_count)
	{
		find_next_word(s, &i, &start);
		result[word_index] = ft_substr(s, start, i - start);
		if (!result[word_index])
			return (free_2d_array(result), NULL);
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
	while (i < word_count + 1)
		result[i++] = NULL;
	return (fill(s, result, word_count));
}
