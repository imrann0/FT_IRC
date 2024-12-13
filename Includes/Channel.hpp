#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <cstddef> // size_t
#include "Client.hpp"
#include "Wordl.hpp"

typedef std::vector<Client *>::iterator it;

class Channel
{
	private:
		std::string 				_name;
		std::string 				_topic;
		std::string 				_password;
		std::map<char, bool>		_flags;
		std::vector<Client *>		_clients;
		std::vector<Client *>		_operator;
		Wordl						_bot;
		size_t						_maxLimit;
		std::vector<std::string>	_invites;

	public:
		Channel();
		Channel(const std::string& name, const std::string &password);

		//get
		std::string				getName();
		std::vector<Client *>	&getClients();
		std::vector<Client *>	&getOperator();
		std::string 			getUsersNames();
		bool					getLimit() const;
		std::string				getTopic() const;
		Client					&getClient(std::string target);
		std::string				getSizeClient() const;
		std::string				getPassword() const;
		Wordl   				&getBot();

		// is
		bool	IsFlags(char c);
		bool	IsOperator(Client &client);
		bool 	IsOperator(std::string &op);
		bool	IsClient(Client &client);
		bool	IsClient(std::string &client);
		bool	IsInvites(const std::string &invited);

		//set
		void	setPassword(std::string password);
		void	setInvite(std::string &invited);
		void	setLimit(size_t Limit);
		void	setFlags(char c, bool status);

		// member function
		void	removeInvite(std::string &invited);
		void	ClientAdd(Client &newClient);
		void	ClientRemove(Client &removeClient);
		void	OperatorRemove(Client &removeClient);
		void	OperatorAdd(Client &newOperator);
		void	TopicAdd(std::string &topic);
		void	Brodcast(std::string message);
		void	Brodcast(std::string message, Client &client);
		std::string getFlags();
};

#endif
