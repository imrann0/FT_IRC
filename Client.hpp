#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

class Client {
public:
	// Constructors
	Client();
	Client(int fd);
	Client(const std::string& nickname, const std::string& username, int fd);

	// Getters
	int					getClientFd() const;
	const std::string&	getNickname() const;
	const std::string&	getUsername() const;
	const std::string&	getRealName() const;
	const std::string&	getBuffer() const;
	const std::string&	getHostName() const;
	bool				getBufferLine(std::string &str);
	bool				isRegistered() const;

	// Setters
	void	setClientFd(int fd);
	void	setNickname(const std::string& nickname);
	void	setBuffer(const std::string& Buffer);
	void	setUsername(const std::string& username);
	void	setRealName(const std::string& realname);
	void	setHostName(const std::string& hostname);
	void	registerClient();

	std::string getPrefixName();
	void		appendBuffer(const std::string& appendBuffer);
	bool		operator==(const Client& other) const;

private:
	int _clientFd;
	std::string _nickname;		// İstemcinin takma adı
	std::string _username;		// İstemcinin kullanıcı adı
	std::string _realname;		// İstemcinin gerçek adı
	std::string _hostname;
	std::string _buffer;
	bool _registered;			// İstemci kayıt oldu mu? (NICK ve USER komutlarını tamamladıktan sonra)
};

#endif
