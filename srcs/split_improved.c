/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_improved.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:24:59 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/21 18:42:11 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*custom_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	if (!s)
		return (NULL);
	sub = (char *)malloc(sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

int	count_words_and_validate(const char *s)
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
			{
				if (in_quote == 0)
					in_quote = 1;
				else
					in_quote = 0;
			}
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
	{
		result[i] = NULL;
		i++;
	}
	i = 0;
	while (word_index < word_count)
	{
		while (s[i] == ' ')
			i++;
		int start = i;
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
		result[word_index] = custom_substr(s, start, i - start);
		if (!result[word_index])
		{
			free_2d_array(result);
			return (NULL);
		}
		word_index++;
	}
	return (result);
}
