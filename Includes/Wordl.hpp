#ifndef		WORDL_HPP
# define	WORDL_HPP

# include	"Bot.hpp"
# include	<vector>

class Wordl : public Bot
{
	private:
		static std::vector<std::string>	_wordDB;
		std::string						_word;
		bool							_isPlay;

		// member function
		void			play(Client &client, std::vector<std::string> &cmd);
		void			stop(Client &client, std::vector<std::string> &cmd);
		void			join(Client &client, std::vector<std::string> &cmd);
		void			restart(Client &client, std::vector<std::string> &cmd);

	public:
		// constructor
		Wordl(std::string name, Channel &channel);

		// desstructor
		~Wordl();

		// get functions

		// set functions

		// member function
		static size_t	utf8CharacterCount(const std::string& str);
		static void		worldl(std::map<std::string, Channel>&channels, Client & client, std::vector<std::string> &cmd);

};

#endif	//	WORDL_HPP
