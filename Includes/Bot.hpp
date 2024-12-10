#ifndef		BOT_HPP
# define	BOT_HPP

#include "Client.hpp"
#include "Channel.hpp"

class Bot
{
	protected:
		std::string	_name;
		Channel		&_channel;
	public:
		Bot(std::string	name, Channel &channel);
		~Bot();
};

#endif	//	BOT_HPP
