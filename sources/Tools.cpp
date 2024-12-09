#include "Tools.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

std::vector<std::string>	split(std::string str, char	c)
{
	std::vector<std::string>	ret;
	unsigned long				end;
	unsigned long				start;
	std::string					findStr;

	if (str.find_first_not_of(c) == std::string::npos)
		return ret;
	end = str.find(c);
	start = 0;
	while (end != std::string::npos)
	{
		findStr = str.substr(start, end - start);
		if (findStr[0] == ':') // "," ile ayırmada sıkıntı
			break ;
		if (!findStr.empty())
			ret.push_back(findStr);
		start = end + 1;
		end = str.find(c, start);
	}
	findStr = str.substr(start);
	if (findStr[0] == ':') // "," ile ayırmada sıkıntı
		findStr = str.substr(start + 1);
	if (!findStr.empty())
		ret.push_back(findStr);
	if (findStr.find(" :") != std::string::npos)
	{
		// birden fazla " :" kullanımı hatası
		return ret;
	}

	return (ret);
}

void chatRegisterClient(const std::string& message, Client *client)
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
	client->setNickname(nickname);
	client->setUsername(username);
	client->setRealName(realname);
	client->setHostName(hostname);

	std::cout << client->getClientFd() << " Başaril Bir Şekilde Kayıt Gerçekleşti" << std::endl;
}

const Client getClientNameFd(const std::map<int, Client>& clients, const std::string& target)
{
    for (std::map<int, Client>::const_iterator user = clients.begin(); user != clients.end(); ++user)
    {
        if (user->second.getNickname() == target)
            return user->second;
    }
    throw std::runtime_error("Client not found");
}

bool IsClient(const std::map<int, Client>& clients, const std::string& client)
{
    try
    {
        getClientNameFd(clients, client);
        return true;
    }
    catch (const std::exception& e)
    {
        return false;
    }
}
