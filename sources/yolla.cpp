/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yolla.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemirbu <bdemirbu@student.42kocaeli.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:14:30 by bdemirbu          #+#    #+#             */
/*   Updated: 2024/12/04 12:21:37 by bdemirbu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h> // socket, bind
#include <string>

int	yolla(int fd, std::string str)
{
	return (send(fd, str.c_str(), str.length(), 0));
}
