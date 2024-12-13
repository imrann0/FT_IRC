#include "Server.hpp"
#include <cstdlib> // std::atoi
#include <iostream>

int main(int ar, char **av)
{
	if (ar != 3)
	{
		std::cerr << "used: ./program <port> <password>" << std::endl;
		return 1;
	}
	try
	{
		Server server(std::atoi(av[1]), av[2]);
		server.Debug();

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}

	return (0);
}
