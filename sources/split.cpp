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
