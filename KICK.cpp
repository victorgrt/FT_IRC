#include "Server.hpp"

void Server::kickCmd(std::string locate, int socket)
{
    if (locate != "KICK" && (locate.substr(0, 5) != "KICK " || locate.length() <= 5))
        return;
    size_t d_start = locate.find(":") - 2;
    size_t d_end = locate.rfind(" ", d_start) + 1;
    size_t start = locate.find("#");
    std::string channelName;
    size_t end;
    std::string end_bis;
    if (start != std::string::npos)
    {
        end = locate.find_first_of(" ", start);
        if (end != std::string::npos)
        {
            channelName = locate.substr(start + 1, end - start - 1);
        }
    }
    std::string userName2 = locate.substr(d_end, (d_start - d_end) + 1);
    size_t c_start = locate.find(":");
    std::string comment;
    if (c_start != std::string::npos)
    {
        comment = "\x1b[31m\x1b[1m" + locate.substr(c_start + 1) + "\x1b[0m";
    }
    if(channelName.empty() || userName2.empty())
    {
        replyClient(ERR_NEEDMOREPARAMS(getClient(socket)->getNickName(), "KICK"), socket);
        return;
    }
    if (findChannel(channelName) == _channelLst.end())
    {
        replyClient(ERR_NOSUCHCHANNEL(_clients[socket]->getNickName(), channelName), socket);
        return;
    }
    if (is_in_channel(userName2, channelName) == 1)
    {
        std::cout << "CHANNEL NAME : " << channelName << std::endl;
        std::string chan_name = "\x1b[1m\x1b[36m#" + channelName + "\x1b[0m";
		std::string error_msg = ERR_NOTINCHANNEL(_clients[socket]->getNickName(), userName2, channelName);
		std::string total = SENDINCHANNEL(chan_name, chan_name, error_msg, channelName);
		std::cout << "Sending : " << total << std::endl;
        // replyClient(ERR_NOTINCHANNEL(getClient(socket)->getNickName(), userName2, channelName), socket);
        replyClient(total, socket);
        return;
    }
    if (is_in_channel(getClient(socket)->getNickName(), channelName) == 1)
    {
        replyClient(ERR_NOTONCHANNEL(getClient(socket)->getNickName(), channelName), socket);
        return;
    }
    Channel *channel = getChannelFromName(channelName);
    if (channel == NULL)
    {
        std::string err_msg = ERR_NOSUCHCHANNEL(_clients[socket]->getNickName(), channelName);
        replyClient(err_msg, socket);
        return ;
    }
    else if (isClientOp(channel->getOperatorList(), socket) == false)
    {
		std::string chan_name = "\x1b[1m\x1b[36m#" + channel->getName() + "\x1b[0m";
        std::string err_msg = ERR_CHANOPRIVSNEEDED(_clients[socket]->getNickName(), channelName);
        std::string total = SENDINCHANNEL(chan_name, chan_name, err_msg, channel->getName());
        std::cout << "Sending : " << total << std::endl;
        replyClient(total, socket);
        return ;
    }
    if (comment.length() == 13)
    {
        comment.clear();
        std::string color = "\x1b[1m\x1b[31m";
        comment = color + "BE GONE FOOL\x1b[0m";
    }
    std::string msg = KICK(userID(getClient(socket)->getNickName(), getClient(socket)->getUserName()), channelName, userName2, comment);
    replyClient(msg, getSocketFromUser(userName2));
    replyClient(msg, socket);
    findChannel(channelName)->second->removeClientFromLst(userName2);
    std::string kicked_msg = "You were kicked by " + getClient(socket)->getNickName() + "! You need to \x1b[4m\x1b[1mbehave\x1b[0m.\nReason : \"" + comment + "\"\r\n";
    replyClient(kicked_msg, getSocketFromUser(userName2));
}

int Server::is_in_channel(std::string str, std::string channel)
{
    for(std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        if(it->first == channel)
        {
            std::map<int, Client*> clientList = it->second->getClientlst(); // Stocker la liste des clients dans une variable
            for (std::map<int, Client*>::iterator it_clt = clientList.begin(); it_clt != clientList.end(); ++it_clt)
            {
                if(it_clt->second->getNickName() == str)
                    return 0;
            }
        }
    }
    return 1;
}