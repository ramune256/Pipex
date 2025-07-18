/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:50:26 by shunwata          #+#    #+#             */
/*   Updated: 2025/06/04 20:50:22 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static char	*get_word(char const *s, char c, size_t *i)
{
	size_t	len;
	char	*word;

	len = 0;
	while (s[*i + len] && s[*i + len] != c)
		len++;
	word = ft_substr(s, *i, len);
	if (!word)
		return (NULL);
	*i = *i + len;
	return (word);
}

static char	**fill(char const *s, char c, char **result, size_t word_count)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (j < word_count)
	{
		while (s[i] && s[i] == c)
			i++;
		result[j] = get_word(s, c, &i);
		if (!result[j])
		{
			while (j > 0)
			{
				j--;
				free(result[j]);
			}
			free(result);
			return (NULL);
		}
		j++;
	}
	result[j] = NULL;
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	word_count;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	return (fill(s, c, result, word_count));
}
