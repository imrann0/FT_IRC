• Several IRC clients exist. You have to choose one of them as a reference.  biz hexchat'i seçtik.


LOGIN
QUIT
INVITE
TOPIC
MODE


# bdemirbu
- ":" operatorünü araştır buna göre parse yapılacak
- hata durumlarında throw fırlat
- if/else ağacından kurtul



std::string mes = "001" + nickname + " :Welcome to the Internet Relay Network " + nickname + "!" +username + "@" + hostname + "\r\n";
send(client->getClientFd(), mes.c_str(), mes.length(), 0);

# operatörler:
∗ KICK - Bir istemciyi kanaldan çıkarır
∗ INVITE - Bir istemciyi bir kanala davet etme
∗ KONU - Kanal konusunu değiştirin veya görüntüleyin
∗ MODE - Kanalın modunu değiştirin:
- i: Yalnızca davet edilen kanalı ayarla/kaldır
- t: TOPIC komutunun kısıtlamalarını kanala ayarlayın/kaldırın
- k: Kanal anahtarını (şifre) ayarlama/kaldırma // burak
- o: Kanal operatörü ayrıcalığı verme/alma // burak
- l: Kanal için kullanıcı limitini ayarlayın/kaldırın
- Elbette sizden temiz bir kod yazmanız bekleniyor.

# Channel
- /list

# o - Operatör Yetkisi Verme/Alma (give/take channel operator privileges)

Bu mod, bir kullanıcıya kanal operatörü yetkisi verme veya yetkisini alma işlemi için kullanılır.
Kanal operatörleri, kanal kurallarını değiştirebilir, kullanıcıları ekleyebilir veya çıkarabilir.
Örnek:

MODE #channel +o user → Kullanıcıya operatör yetkisi verir.
MODE #channel -o user → Kullanıcının operatör yetkisini alır.

# p - Özel Kanal (private channel flag)

Kanal, özel bir kanal olarak ayarlanır. Özel bir kanal, kanal listelerinde (/list) görüntülenmez.
Kanal adı yalnızca katılan kişiler tarafından görülebilir.
Örnek:

MODE #channel +p → Kanalı özel olarak ayarlar.
MODE #channel -p → Kanalı herkese açık hale getirir.


# s - Gizli Kanal (secret channel flag)

Gizli bir kanal, yalnızca kullanıcı kanala katılmışsa veya birisi tarafından doğrudan bilgilendirilmişse görülebilir.
/who veya /list komutlarında kanal adı görüntülenmez.
Örnek:

MODE #channel +s → Kanalı gizli hale getirir.
MODE #channel -s → Kanalı gizlilikten çıkarır.

# i - Sadece Davetle Katılım (invite-only channel flag) ---

Kanala yalnızca davet edilen kullanıcılar katılabilir.
Operatörler kullanıcıları davet edebilir (/invite komutu ile).
Örnek:

MODE #channel +i → Kanalı sadece davetle katılım yapılabilir hale getirir.
MODE #channel -i → Davetsiz katılımı mümkün kılar.

# t - Konu Sadece Operatörler Tarafından Değiştirilebilir (topic settable by channel operator only flag) --

Kanalın konusu (topic) yalnızca kanal operatörleri tarafından değiştirilebilir.
Normal kullanıcılar konuyu değiştiremez.
Örnek:

MODE #channel +t → Kanal konusunu yalnızca operatörler değiştirebilir.
MODE #channel -t → Herkes kanal konusunu değiştirebilir.
# n - Dışarıdan Mesajlara Kapalı Kanal (no messages to channel from clients on the outside)

Kanala yalnızca o kanalda bulunan kullanıcılar mesaj gönderebilir.
Dışarıdaki (kanalda olmayan) kullanıcıların mesaj göndermesi engellenir.
Örnek:

MODE #channel +n → Kanal dışındaki kullanıcıların mesajlarını engeller.
MODE #channel -n → Herkese mesaj gönderme izni verir.

# m - Moderasyonlu Kanal (moderated channel)

Kanalda yalnızca operatörler ve belirli yetkilere sahip kullanıcılar mesaj gönderebilir.
Normal kullanıcılar yalnızca dinleyici konumundadır.
Örnek:

MODE #channel +m → Kanalı moderasyonlu hale getirir.
MODE #channel -m → Moderasyonu kaldırır.

# l - Kullanıcı Limiti Ayarlama (set the user limit to channel)

Kanala katılabilecek maksimum kullanıcı sayısını belirler.
Limit dolduğunda, diğer kullanıcılar kanala katılamaz.
Örnek:

MODE #channel +l 50 → Kanalın kullanıcı limitini 50 olarak ayarlar.
MODE #channel -l → Kullanıcı limitini kaldırır.

# b - Yasaklı Kullanıcılar (ban list)

Belirli kullanıcıların veya kullanıcı gruplarının kanala erişimi engellenir.
Ban maskesi kullanılarak geniş kullanıcı grupları engellenebilir.
Örnek:

MODE #channel +b user!*@* → Kullanıcıyı yasaklar.
MODE #channel -b user!*@* → Kullanıcının yasağını kaldırır.

# k - Kanal Şifresi Ayarlama (set a channel key/password)

Kanalı bir şifre ile koruma altına alır.
Kanala katılmak isteyen kullanıcılar, belirtilen şifreyi girmeden kanala katılamaz.
Şifre yalnızca kanal operatörleri tarafından ayarlanabilir veya kaldırılabilir.

Kullanımı:

MODE #channel +k <şifre> → Kanala bir şifre ekler.
MODE #channel -k → Kanal şifresini kaldırır.





//! LOGİNDE QUIT ATMAM LAZIM _clients ve _pollfd BANA VER !!!!
//! verileri vector<std::string> olarak vereceğim :) split yazdım YİNEEEEEE


Kanal ve Kullanıcı Yönetimi Hataları
ERR_NEEDMOREPARAMS (461)

Açıklama: Komut için eksik parametreler verilmiş.
Mesaj Örneği: :server 461 nickname command :Not enough parameters
RPL_CHANNELMODEIS (324)

Açıklama: Bir kanalın mevcut modlarını bildirir.
Mesaj Örneği: :server 324 nickname #channel +nt
ERR_CHANOPRIVSNEEDED (482)

Açıklama: Kanalın yönetici (operatör) yetkisine ihtiyaç duyulduğunu belirtir.
Mesaj Örneği: :server 482 nickname #channel :You're not channel operator
ERR_NOSUCHNICK (401)

Açıklama: Belirtilen kullanıcı bulunamadı.
Mesaj Örneği: :server 401 nickname target :No such nick/channel
ERR_NOTONCHANNEL (442)

Açıklama: Kullanıcı, belirtilen kanalda değil.
Mesaj Örneği: :server 442 nickname #channel :You're not on that channel
ERR_KEYSET (467)

Açıklama: Kanalın bir anahtarı (şifresi) zaten ayarlanmış.
Mesaj Örneği: :server 467 nickname #channel :Channel key already set
Ban Listesi ile İlgili Yanıtlar
RPL_BANLIST (367)

Açıklama: Bir kanalın yasaklılar listesindeki (ban list) bir giriş.
Mesaj Örneği: :server 367 nickname #channel bannedUser!ident@host
RPL_ENDOFBANLIST (368)

Açıklama: Ban listesinin sonu.
Mesaj Örneği: :server 368 nickname #channel :End of channel ban list
Modlar ve Bilinmeyen Komutlarla İlgili Yanıtlar
ERR_UNKNOWNMODE (472)

Açıklama: Bilinmeyen bir mod kullanıldığını belirtir.
Mesaj Örneği: :server 472 nickname unknownMode :is unknown mode char to me
ERR_NOSUCHCHANNEL (403)

Açıklama: Belirtilen kanal mevcut değil.
Mesaj Örneği: :server 403 nickname #channel :No such channel
Kullanıcı Modları ile İlgili Yanıtlar
ERR_USERSDONTMATCH (502)

Açıklama: Kullanıcı, kendi modunu değiştirmeye çalışmıyor.
Mesaj Örneği: :server 502 nickname :Cannot change mode for other users
RPL_UMODEIS (221)

Açıklama: Kullanıcının aktif modlarını gösterir.
Mesaj Örneği: :server 221 nickname +i
ERR_UMODEUNKNOWNFLAG (501)

Açıklama: Bilinmeyen bir kullanıcı modu belirtilmiş.
Mesaj Örneği: :server 501 nickname :Unknown MODE flag
