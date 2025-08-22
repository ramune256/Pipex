/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_improved.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:24:59 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/22 14:01:14 by shunwata         ###   ########.fr       */
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

char	**split_improved(const char *s)
{
	char	**result;
	int		word_count;
	int		i;
	int		word_index;
	int		in_quote;
	int		start;

	i = 0;
	word_index = 0;
	in_quote = 0;
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
	i = 0;
	while (word_index < word_count)
	{
		while (s[i] == ' ')
			i++;
		start = i;
		in_quote = 0;
		while (s[i])
		{
			if (s[i] == '\'')
			{
				if (in_quote == 0)
					in_quote = 1;
				else
					in_quote = 0;
			}
			if (s[i] == ' ' && in_quote == 0)
				break ;
			i++;
		}
		result[word_index] = ft_substr(s, start, i - start);
		if (!result[word_index])
			return (free_2d_array(result), NULL);
		word_index++;
	}
	return (result);
}
