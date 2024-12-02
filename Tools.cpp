#include "Tools.hpp"
#include <iostream>

void chatRegisterClient(const std::string& message, Client client)
{
	std::string str = message;
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	size_t pos = str.find('\n');
	std::string nickname = str.substr(5, pos - 5);
	str = str.substr(pos + 1);

	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	std::string userInfo = str.substr(5); // "USER" komutundan sonraki kısmı alıyoruz
	size_t firstSpace = userInfo.find(" "); // ilk boşluğu bul
	size_t secondSpace = userInfo.find(" ", firstSpace + 1); // ikinci boşluğu bul
	size_t thirdSpace = userInfo.find(" ", secondSpace + 1); // üçüncü boşluğu bul

	// Parçaları ayıralım
	std::string username = userInfo.substr(0, firstSpace); // username
	std::string hostname = userInfo.substr(firstSpace + 1, secondSpace - firstSpace - 1); // hostname
	std::string servername = userInfo.substr(secondSpace + 1, thirdSpace - secondSpace - 1); // servername
	std::string realname = userInfo.substr(thirdSpace + 2); // realname kısmını almak için ':'

	if (realname[0] == ':') {
		realname = realname.substr(1); // ':' karakterini çıkar
	}

	// Client bilgilerini güncelle
	client.setNickname(nickname);
	client.setUsername(username);
	client.setRealName(realname);
	client.setHostName(hostname);
	client.registerClient();

	std::cout << client.getClientFd() << " Başarılı Bir Şekilde Kayıt Gerçekleşti" << std::endl;
}
