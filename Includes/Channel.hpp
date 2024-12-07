#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <cstddef> // size_t
#include "Client.hpp"

typedef std::vector<Client>::iterator it;

#define RPL_PRIVMSG(source, target, message)            ":" + source + " PRIVMSG " + target + " :" + message
#define RPL_NAMREPLY(source, channel, users)            ": 353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)                 ": 366 " + source + " " + channel + " :End of /NAMES list."
#define RPL_JOIN(nick, channel)                         ":" + nick + " JOIN " + channel
#define RPL_MODE(source, channel, modes, args)          ":" + source + " MODE " + channel + " " + modes + " " + args
#define RPL_NOTOPIC(source, channel)                 	"331 " + source + " " + channel + " :No topic is set"
#define RPL_TOPIC(client, channel, topic) 				": 332 " + client + " " + channel + " " + topic
#define RPL_MODEONE(source, channel, modes) 			":" + source + " MODE " + channel + " " + modes
#define RPL_INVITING(client, invited, channel) 			" 341 " + client + " " + invited + " " + channel
#define RPL_PART(source, channel)                       ":" + source + " PART :" + channel


// Error
#define ERR_NOSUCHCHANNEL(source, channel)              ": 403 " + source + " " + channel + " :No such channel"
#define ERR_CHANOPRIVSNEEDED(client, channel)			": 482 " + client + " " + channel + " :You're not channel operator"
#define ERR_NEEDMOREPARAMS(client, command)				": 461 " + client + " " + command + " :Not enough parameters"
#define ERR_CHANNELISFULL(client, channel)				": 471 " + client + " " + channel + " :Channel is full"
#define ERR_INVITEONLYCHAN(client, channel)					": 473 " + client + " " + channel + " :Cannot join channel (+i)"
#define ERR_NOSUCHNICK(client, invited)					": 401" + client + " " + invited + " :No such nick"
#define ERR_NOTONCHANNEL(client, channel)				": 442 " + client + " " + channel + " :The user is not on this channel."

class Channel
{
	private:
		std::string 			_name;
		std::string 			_topic;
		std::map<char, bool>	_flags;
		std::vector<Client>		_clients;
		std::vector<Client> 	_operator;
		size_t					_maxLimit;
		std::vector<std::string>	_invites;

	public:
		Channel();
		Channel(const std::string& name);

		//get
		std::string			getName();
		std::vector<Client>	getClients();
		std::vector<Client>	getOperator();
		std::string 		getUsersNames();
		bool				getLimit() const;
		std::string			getTopic() const;
		Client&				getClient(std::string target);
		std::string			getSizeClient() const;
		bool 				IsFlags(char c);
		bool				IsOperator(Client &client);
		bool				IsClient(Client &client);
		bool				IsInvites(const std::string &invited);

		//set
		void	setInvite(std::string &invited);
		void	removeInvite(std::string &invited);
		void	setLimit(size_t Limit);
		void	setFlags(char c, bool status);
		void	ClientAdd(Client &newClient);
		void	ClientRemove(Client &removeClient);
		void	OperatorRemove(Client &removeClient);
		void	OperatorAdd(Client &newOperator);
		void	TopicAdd(std::string &topic);
		void	Brodcast(std::string &message);
};

#endif
