#ifndef		WORDL_HPP
# define	WORDL_HPP

# include	"Bot.hpp"
# include	<vector>

class Wordl : public Bot
{

	private:
		std::vector<std::string>	_wordDB;
		std::string					_word;
		bool						_isPlay;
	public:
		Wordl(std::string name, Channel &channel);
		~Wordl();
};

#endif	//	WORDL_HPP
