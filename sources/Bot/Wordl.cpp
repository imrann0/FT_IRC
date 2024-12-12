#include "Wordl.hpp"
#include "Channel.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>

const std::vector<std::string>	Wordl::_wordDB = wordDB();

Wordl::Wordl(Channel *channel) : _channel(channel), _isPlay(false)
{
	std::cout << "wordlda tutulan adres: " << _channel << std::endl;
	_try = 0;
}

Wordl::~Wordl()
{

}

void	Wordl::setChannel(Channel &channel) {_channel = &channel; }

size_t	utf8ByteLength(char c)
{
	size_t charLength = 1;
	if ((c & 0xF8) == 0xF0)
		charLength = 4;
	else if ((c & 0xF0) == 0xE0)
		charLength = 3;
	else if ((c & 0xE0) == 0xC0)
		charLength = 2;
	return (charLength);
}

std::string	ft_itos(int value)
{
	std::stringstream	ret;

	ret << value;
	return (ret.str());
}

//
void	Wordl::show()
{
	std::string firstMes = "\x03,11--->WORDL<---";
	std::string a;
	std::string b;
	this->senChannelMsg(true, firstMes);
	for (int i = 0; i < _try; i++)
	{
		std::string word = ft_itos(i + 1) + ". ";
		for (int j = 0; j < 5; j++)
		{
			a = _triedWord[i].substr(j, utf8ByteLength(_triedWord[i][j]));
			b = _word.substr(j, utf8ByteLength(_word[j]));
			std::cout << "tried : |"<< a << "|"<<std::endl << "word : |" << b << "|"<< std::endl;
			if (a == b)
            	word.append(a);
			else
            	word.append("_");
            word.append(" ");
		}
		this->senChannelMsg(false, word);
	}
	this->senChannelMsg(false, "");
}

void	Wordl::triedClear()
{
	for (int i = 0; i < hak; ++i)
        _triedWord[i] = "";
}

size_t Wordl::utf8CharacterCount(const std::string& str)
{
	size_t length = 0;
	for (size_t i = 0; i < str.size(); ++i)
		if ((str[i] & 0xC0) != 0x80)
			++length;
	return length;
}

void	Wordl::worldl(std::map<std::string, Channel>&channels, Client &client, std::vector<std::string> &cmd)
{
	if (cmd.size() != 3)
		throw ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]);
	if (channels.find(cmd[1]) == channels.end())
		throw ERR_NOSUCHCHANNEL(client.getNickname(), cmd[0]);
	if (channels[cmd[1]].IsClient(client) == false)
		throw ERR_NOTONCHANNEL(client.getNickname(), channels[cmd[1]].getName());
	Wordl &bot = channels[cmd[1]].getBot();

	if (cmd[2] == "PLAY")
		bot.play();
	else if (cmd[2] == "RESTART")
		bot.reStart();
	else
		bot.guess(cmd);
}

void	Wordl::guess(std::vector<std::string> &cmd)
{
	if (this->_isPlay == false)
		throw std::string("WORDL <channel name> PLAY");
	if (utf8CharacterCount(cmd[2]) != 5)
		throw std::string("harf sayisi hatali");
	_triedWord[_try] = cmd[2];
	_try++;
	show();
	if (this->_try == 10 || this->_triedWord[this->_try - 1] == this->_word)
	{
		std::string message;
		if (this->_try == 10)
			message = "\x03,4Üzgünüm bilemedin doğru kelimemiz \""+ this->_word +"\" Asla pes etme 💪🏼";
		else
			message = "\x03,10Sen bir harikasın doğru kelimeyi buldun 🥳";
		this->triedClear();
		this->_isPlay = false;
		this->_try = 0;
		this->senChannelMsg(true, message);
	}
	else
		this->senChannelMsg(false, "\x03,7Maalesefki kelimemiz \"" + cmd[2] + "\" değil!");
}

void	Wordl::play()
{

	if (this->_isPlay == true)
	{
		this->show();
		throw	std::string("hali hazirda oynaniyor");
	}

	srand(static_cast<unsigned int>(time(0)));
    this->_word = _wordDB[rand() % (_wordDB.size() -1)];
	std::cout << "123" << std::endl;
	this->triedClear();

	std::cout << this->_word << std::endl;
	this->senChannelMsg(true, "\x03,8Ben aklımdan 5 karakterli bir kelime tuttum hadi bakalım bilebilecek misin? (kullanımı : WORDL <channel name> kelime)");
	std::cout << "123" << std::endl;
	this->_isPlay = true;
}

void	Wordl::reStart()
{
	this->_isPlay = false;
	this->triedClear();
	this->_try = 0;
	play();
}

void	Wordl::senChannelMsg(bool prefix, std::string message)
{
	std::string source = prefix == true ? "WORDL!WORDL@WORDL" : "";
	_channel->Brodcast(RPL_PRIVMSG(source, _channel->getName(), message));
}
