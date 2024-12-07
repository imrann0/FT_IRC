/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemirbu <bdemirbu@student.42kocaeli.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 19:49:45 by bdemirbu          #+#    #+#             */
/*   Updated: 2024/12/07 14:34:20 by bdemirbu         ###   ########.fr       */
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

	if (str.find_first_not_of(c) == std::string::npos)
		return ret;
	end = str.find(c);
	start = 0;
	while (end != std::string::npos)
	{
		findStr = str.substr(start, end - start);
		if (findStr[0] == ':') // "," ile ayırmada sıkıntı
		{
			start++;
			break ;
		}
		if (!findStr.empty())
			ret.push_back(findStr);
		start = end + 1;
		end = str.find(c, start);
	}
	findStr = str.substr(start);
	if (!findStr.empty())
		ret.push_back(findStr);
	if (findStr.find(" :") != std::string::npos)
	{
		// birden fazla " :" kullanımı hatası
		return ret;
	}

	return (ret);
}
