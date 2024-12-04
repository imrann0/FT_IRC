/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemirbu <bdemirbu@student.42kocaeli.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 19:49:45 by bdemirbu          #+#    #+#             */
/*   Updated: 2024/12/04 20:14:26 by bdemirbu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>

std::vector<std::string>	split(std::string str, char	c)
{
	std::vector<std::string>	ret;
	unsigned long				end;
	unsigned long				start;
	std::string					findStr;

	end = str.find(c);
	start = 0;
	while (end != std::string::npos)
	{
		findStr = str.substr(start, end - start);
		if (!findStr.empty())
			ret.push_back(findStr);
		start = end + 1;
		end = str.find(c, start);
	}
	findStr = str.substr(start, end);
	if (!findStr.empty())
		ret.push_back(findStr);
	return (ret);
}
