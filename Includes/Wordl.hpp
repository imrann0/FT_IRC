#ifndef		WORDL_HPP
# define	WORDL_HPP

# include	<vector>
# include	<map>
# include	<string>

class Client;
class Channel;

#define hak 10

class Wordl
{
	private:
		static const  std::vector<std::string>	_wordDB;
		std::string								_triedWord[hak];
		std::string								_word;
		Channel									*_channel;
		bool									_isPlay;
		int										_try;


		// member function
		void	play();
		void	reStart();
		void	guess(std::vector<std::string> &cmd);
		void	show();
		void	triedClear();
		void	senChannelMsg(bool prefix, std::string message);

	public:
		void	setChannel(Channel &Channel);
		// constructor
		Wordl(Channel *channel);

		// desstructor
		~Wordl();

		// get functions

		// set functions

		// member function
		static size_t	utf8CharacterCount(const std::string& str);
		static void		worldl(std::map<std::string, Channel> &channels, Client & client, std::vector<std::string> &cmd);

};

std::vector<std::string>	wordDB();

#endif	//	WORDL_HPP
