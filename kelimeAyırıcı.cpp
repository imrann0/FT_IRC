#include <fstream>
#include <iostream>
#include <locale>

size_t utf8_character_count(const std::string& str)
{
    size_t length = 0;
    for (size_t i = 0; i < str.size(); ++i)
        if ((str[i] & 0xC0) != 0x80)
            ++length;
    return length;
}

int main(int argc, char const *argv[])
{
    std::ifstream pool("top-20K-words.txt");
    std::ifstream pool2("10000_frequent_turkish_word.txt");
    std::ofstream five("5_karakterli_liste2.txt");
    std::string str;

    if (!pool.is_open() || !five.is_open() || !pool2.is_open()){
        std::cerr << "Dosya açılamadı!" << std::endl;
        return 1;
    }

    while (std::getline(pool, str))
    {
        if (utf8_character_count(str) == 5)
            five <<"_wordDB.push_back(\""<< str <<"\");" << std::endl;
    }
	while (std::getline(pool2, str))
    {
        if (utf8_character_count(str) == 5)
            five <<"_wordDB.push_back(\""<< str <<"\");" << std::endl;
    }

    return 0;
}
