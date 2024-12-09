#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <cstddef> // size_t
#include "Client.hpp"

typedef std::vector<Client *>::iterator it;

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
#define RPL_NICK(source,nickname)                       ":" + source + " NICK " + nickname

// Error
#define ERR_NOSUCHCHANNEL(source, channel)              ": 403 " + source + " " + channel + " :No such channel"
#define ERR_CHANOPRIVSNEEDED(client, channel)			": 482 " + client + " " + channel + " :You're not channel operator"
#define ERR_NEEDMOREPARAMS(client, command)				": 461 " + client + " " + command + " :Not enough parameters"
#define ERR_CHANNELISFULL(client, channel)				": 471 " + client + " " + channel + " :Channel is full"
#define ERR_INVITEONLYCHAN(client, channel)				": 473 " + client + " " + channel + " :Cannot join channel (+i)"
#define ERR_NOSUCHNICK(client, invited)					": 401 " + client + " " + invited + " :No such nick"
#define ERR_NOTONCHANNEL(client, channel)				": 442 " + client + " " + channel + " :The user is not on this channel."
#define ERR_NICKNAMEINUSE(client)						": 433 " + client + " :Nickname is already in use"
#define ERR_ERRONEUSNICKNAME(client)					": 432 " + client + " :Erroneous nickname"
// -NICK ERRORS-
#define ERR_ALREADYREGISTRED(client)							": 462 " + client + " :You may not register"
#define ERR_NONICKNAMEGIVEN(client)							": 431 " + client + " :No nickname given" //?
#define ERR_NICKCOLLISION(client)							": 436 " + client + " :Nickname collision KILL"

// -USER ERRORS-

// -JOIN ERRORS-
#define ERR_BADCHANMASK(client, channel)				": 476 " + client + " " + channel + " :Bad Channel Mask"
#define ERR_USERONCHANNEL(client, invited, channel)		": 443 " + client + " " + invited + " " + channel + " :is already on channel"
#define ERR_NOTREGISTERED(client)						": 451 " + client + " :You have not registered"

// -PART ERRORS-

// -MODE ERRORS-
#define ERR_UMODEUNKNOWNFLAG(client)					": 501 " + client + " :Unknown MODE flag"
#define ERR_USERSDONTMATCH(client)						": 502 " + client + " :Cannot change mode for other users"

// -KICK ERRORS-
#define ERR_USERNOTINCHANNEL(client, invited, channel)	": 441 " + client + " " + invited + " " + channel + " :They aren't on that channel"

// -INVITE ERRORS-
#define ERR_BANNEDFROMCHAN(client, channel)				": 474 " + client + " " + channel + " :Cannot join channel (+b)"
#define ERR_BADCHANNELKEY(client, channel)				": 475 " + client + " " + channel + " :Cannot join channel (+k)"
#define ERR_NOPRIVILEGES(client)						": 481 " + client + " :Permission Denied- You're not an IRC operator"
#define ERR_RESTRICTED(client)							": 484 " + client + " :Your connection is restricted!"
#define ERR_SUMMONDISABLED(client)						": 445 " + client + " :SUMMON has been disabled"
#define ERR_USERSDISABLED(client)						": 446 " + client + " :USERS has been disabled"
#define ERR_PASSWDMISMATCH(client)						": 464 " + client + " :Password incorrect"
#define ERR_YOUREBANNEDCREEP(client)					": 465 " + client + " :You are banned from this server"

class Channel
{
	private:
		std::string 				_name;
		std::string 				_topic;
		std::string 				_password;
		std::map<char, bool>		_flags;
		std::vector<Client *>		_clients;
		std::vector<Client *>		_operator;
		size_t						_maxLimit;
		std::vector<std::string>	_invites;

	public:
		Channel();
		Channel(const std::string& name);

		//get
		std::string				getName();
		std::vector<Client *>	&getClients();
		std::vector<Client *>	&getOperator();
		std::string 			getUsersNames();
		bool					getLimit() const;
		std::string				getTopic() const;
		Client					&getClient(std::string target);
		std::string				getSizeClient() const;

		// is
		bool	IsFlags(char c);
		bool	IsOperator(Client &client);
		bool	IsClient(Client &client);
		bool	IsClient(std::string &client);
		bool	IsInvites(const std::string &invited);

		//set
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
		void	Brodcast(std::string &message);
};

#endif
