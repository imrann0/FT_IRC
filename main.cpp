#include "Server.hpp"

int main(int ar, char **av)
{
	if (ar != 3)
	{
		std::cerr << "Kullanım: ./program <port> <password>" << std::endl;
		return 1;
	}

	try
	{
		(void)ar;
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
