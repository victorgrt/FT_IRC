#include "Server.hpp"

void Server::partCmd(std::string locate, int socket)
{
    if (locate != "PART" && (locate.substr(0, 5) != "PART " || locate.length() <= 5))
        return;
    size_t len = locate.find(" ");
    if (len == std::string::npos)
    {
        std::string needmoreparams = ERR_NEEDMOREPARAMS(_clients[socket]->getNickName(), "PART");
        replyClient(needmoreparams, socket);
        return;
    }

    size_t secure = locate.find(" ", len + 1);
    if (secure == std::string::npos)
    {
        std::string needmoreparams = ERR_NEEDMOREPARAMS(_clients[socket]->getNickName(), "PART");
        replyClient(needmoreparams, socket);
        return;
    }

    std::string channel_name = locate.substr(len + 1, secure - len - 1);

    size_t len2 = locate.find(":");
    if (len2 == std::string::npos)
    {
        std::string needmoreparams = ERR_NEEDMOREPARAMS(_clients[socket]->getNickName(), "PART");
        replyClient(needmoreparams, socket);
        return;
    }
    std::string reason = locate.substr(len2 + 1);

    // CHECK THAT THE CHANNEL EXISTS
    std::string tmp;
    if (channel_name[0] == '#')
        tmp = channel_name.erase(0, 1);
    else
        tmp = channel_name;
    Channel *partingChannel = getChannelFromName(tmp);
    if (partingChannel == NULL)
    {
        std::string notfound = ERR_NOSUCHCHANNEL(_clients[socket]->getNickName(), channel_name);
        replyClient(notfound, socket);
        return;
    }
    // CHECK THAT THE USER IS ON THE CHANNEL
    std::map<int, Client *> liste = partingChannel->getClientlst();
    std::map<int, Client *>::iterator it = liste.begin();
    std::map<int, Client *>::iterator ite = liste.end();
    bool found = false;
    while (it != ite)
    {
        if (it->second->getNickName() == _clients[socket]->getNickName())
        {
            found = true;
            break;
        }
        ++it;
    }
    if (found == false)
    {
        std::string noton = ERR_NOTONCHANNEL(_clients[socket]->getNickName(), channel_name);
        replyClient(noton, socket);
        return;
    }
    std::string parting_channel_name = tmp;
    if (channel_name[0] != '#')
    {
        std::string tmp2 = channel_name;
        channel_name.clear();
        channel_name += "#" + tmp;
    }
    std::string user = userID(_clients[socket]->getNickName(), _clients[socket]->getUserName());
    std::string part = PART(user, channel_name, reason);
    std::map<int, Client *>::iterator it2 = liste.begin();
    std::map<int, Client *>::iterator it2e = liste.end();
    while (it2 != it2e)
    {
        replyClient(part, it2->first);
        ++it2;
    }
    findChannel(tmp)->second->removeClientFromLst(_clients[socket]->getNickName());
    
    // NEEDED TO CLOSE THE IRSSI WINDOW
    std::string notfound = ERR_NOSUCHCHANNEL(_clients[socket]->getNickName(), parting_channel_name); // Utilise parting_channel_name pour générer le message d'erreur
	replyClient(notfound, socket);

    // SEND INFO
	std::string info = "\x1b[33mYou left \x1b[36m#" + parting_channel_name + "\x1b[33m ! Reason : " + reason + "\x1b[0m\r\n";
	replyClient(info, socket);
    _clients[socket]->setTempBuffer("", 1);
}
