• Several IRC clients exist. You have to choose one of them as a reference.  biz hexchat'i seçtik.


LOGIN
QUIT
INVITE
TOPIC
MODE


std::string mes = "001" + nickname + " :Welcome to the Internet Relay Network " + nickname + "!" +username + "@" + hostname + "\r\n";
send(client->getClientFd(), mes.c_str(), mes.length(), 0);


# bdemirbu
- send düzenle