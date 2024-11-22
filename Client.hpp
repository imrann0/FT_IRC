#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

#include <string>

class Client {
public:
    // Constructors
    Client();
    Client(const std::string& nickname, const std::string& username);

    // Getters
    const std::string& getNickname() const;
    const std::string& getUsername() const;
    const std::string& getRealName() const;
    bool isRegistered() const;

    // Setters
    void setSocketFD(int fd);
    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);
    void setRealName(const std::string& realname);
    void registerClient();

private:
    std::string _nickname;        // İstemcinin takma adı
    std::string _username;        // İstemcinin kullanıcı adı
    std::string _realname;        // İstemcinin gerçek adı
    bool _registered;             // İstemci kayıt oldu mu? (NICK ve USER komutlarını tamamladıktan sonra)
};

#endif
