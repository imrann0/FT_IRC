#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

class Client
{
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
		std::string			getPrefixName();

		// is?
		bool				isRegistered() const;

		// Setters
		void	setOldNickName(const std::string& oldNickName);
		void	setUsername(const std::string& username);
		void	setRealName(const std::string& realname);
		void	setHostName(const std::string& hostname);
		void	setNickname(const std::string& nickname);
		void	setBuffer(const std::string& Buffer); // fonksiyon kullanılmıyor
		void	setClientFd(int fd); // fonksiyon kullanılmıyor
		void	setPass(bool pass);
		void	registerClient(); // ismini set olarak düzenle

		void	MsgToClient(const std::string &message);
		void	appendBuffer(const std::string& appendBuffer);
		bool	operator==(const Client& other) const;

	protected:
		int _clientFd;
		std::string _oldNickName;
		std::string _nickname;		// İstemcinin takma adı
		std::string _username;		// İstemcinin kullanıcı adı
		std::string _realname;		// İstemcinin gerçek adı
		std::string _hostname;
		std::string _buffer;
		bool		_pass;
		bool		_registered;	// İstemci kayıt oldu mu? (NICK ve USER komutlarını tamamladıktan sonra)
};

#endif
