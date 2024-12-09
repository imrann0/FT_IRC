#include "Client.hpp"
#include "Tools.hpp"
#include <iostream>
#include <cstring>	 	//  strerror
#include <sys/socket.h> // send
#include "Channel.hpp" // RPL and ERR
// :!bdemirbu@0 NICK bdemirbu_


bool isValidNickname(const std::string& nickname) {
    // 1. Minimum ve maksimum uzunluk kontrolü
    if (nickname.length() < 1 || nickname.length() > 9) {
        std::cout << "Hata: Takma adin uzunlugu 1 ile 9 karakter arasinda olmali." << std::endl;
        return false;
    }

    // 2. İlk karakter harf olmalı
    if (!isalpha(nickname[0])) {
        std::cout << "Hata: Takma ad ilk karakteri bir harf olmalidir." << std::endl;
        return false;
    }

    // 3. Takma adda sadece izin verilen karakterler olmalı
    // Harfler (a-zA-Z), rakamlar (0-9), ve belirli özel karakterler
    for (size_t i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (!isalnum(c) && c != '-' && c != '[' && c != ']' && c != '\\' && c != '^' && c != '{' && c != '}' && c != '_') {
            std::cout << "Hata: Takma adda geçersiz karakter var: " << c << std::endl;
            return false;
        }
    }

    // 4. Boşluk karakteri içeremez
    if (nickname.find(' ') != std::string::npos) {
        std::cout << "Hata: Takma adda bosluk karakteri bulunamaz." << std::endl;
        return false;
    }

    // 5. Takma adda kontrol karakterleri (ASCII 0-31) bulunmamalıdır
    for (size_t i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (c < 32) {  // ASCII kontrol karakterleri (0-31)
            std::cout << "Hata: Takma adda kontrol karakteri bulunamaz." << std::endl;
            return false;
        }
    }

    // 6. Takma ad geçerli
    return true;
}

void Nick(std::map<int, Client>& clients, Client &client, std::vector<std::string> cmd)
{
	std::cout << cmd.size() << std::endl;
	if (cmd.size() == 1)
		throw ERR_NONICKNAMEGIVEN(client.getNickname());
	else if (cmd.size() != 2)
		throw ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]);
	else if (IsClient(clients, cmd[1]) == true)
		throw ERR_NICKNAMEINUSE(cmd[1]);
	else if (isValidNickname(cmd[1]) == false)
		throw ERR_NONICKNAMEGIVEN(cmd[1]);
	else if (client.isRegistered() == true)
	{
		std::string a = RPL_NICK(client.getPrefixName(), cmd[1]);
		client.MsgToClient(a);
	}
	client.setNickname(cmd[1]);
}
