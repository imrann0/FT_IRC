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
	const std::string&	getOldName() const;
	bool				getBufferLine(std::string &str);
	bool				getPass() const;
	bool				getNickStatus() const;
	// is?
	bool				isRegistered() const;

	// Setters
	void	setClientFd(int fd);
	void	setOldNickName(const std::string& oldNickName);
	void	setNickname(const std::string& nickname);
	void	setBuffer(const std::string& Buffer);
	void	setUsername(const std::string& username);
	void	setRealName(const std::string& realname);
	void	setHostName(const std::string& hostname);
	void	setPass(bool pass);
	void	registerClient();
	void	MsgToClient(const std::string &message);
	void	setNickStatus(bool status);

	std::string getPrefixName();
	void		appendBuffer(const std::string& appendBuffer);
	bool		operator==(const Client& other) const;

private:
	int _clientFd;
	std::string _oldNickName;
	std::string _nickname;		// İstemcinin takma adı
	std::string _username;		// İstemcinin kullanıcı adı
	std::string _realname;		// İstemcinin gerçek adı
	std::string _hostname;
	std::string _buffer;
	bool		_pass;
	bool		_registered;	// İstemci kayıt oldu mu? (NICK ve USER komutlarını tamamladıktan sonra)
	bool		_nickStatus;
};

#endif
