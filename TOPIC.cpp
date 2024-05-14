#include "Server.hpp"

#include <iostream>
#include <string>
#include <cstring>

bool    Server::isClientOp(std::map<int, Client*> op_list, int socket)
{
    for (std::map<int, Client *>::iterator it = op_list.begin(); it != op_list.end(); ++it)
    {
        if (_clients[socket]->getNickName() == it->second->getNickName())
        {
            return (true);
        }
    }
    return (false);
}

void Server::topicCmd(std::string locate, int socket)
{
    if (locate != "TOPIC" && (locate.substr(0, 6) != "TOPIC " || locate.length() <= 6))
        return;
    std::string channelName;
    std::string topicName;

    size_t start = locate.find('#');
    if (start != std::string::npos)
    {
        size_t end = locate.find(' ', start);
        if (end != std::string::npos)
        {
            channelName = locate.substr(start + 1, end - start - 1);
        }
        else
        {
            channelName = locate.substr(start + 1);
        }
    }
    start = locate.rfind(':');
    if (start != std::string::npos)
    {
        topicName = locate.substr(start + 1);
    }
    else
    {
        Channel *found = getChannelFromName(channelName);
        if (found == NULL)
        {
            std::string err = ERR_NOSUCHCHANNEL(_clients[socket]->getNickName(), channelName);
            replyClient(err, socket);
            return;
        }
        else
        {
            if (found->getTopic() == "")
                replyClient(TOPIC(_clients[socket]->getNickName(),_clients[socket]->getUserName(), channelName, "No Topic"), socket);
            else
                replyClient(TOPIC(_clients[socket]->getNickName(), _clients[socket]->getUserName(), channelName, found->getTopic()), socket);
        }
        return ;
    }
    for (std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        if (it->first == channelName)
        {
            if (it->second->getTopicStatus() == 1 && isClientOp(it->second->getOperatorList(), socket) == false)
            {
                std::string chan_name = "\x1b[1m\x1b[36m#" + it->second->getName() + "\x1b[0m";
		        std::cout << "On est ici normalement" << std::endl;
		        std::string error_msg = ERR_CHANOPRIVSNEEDED(_clients[socket]->getNickName(), it->second->getName());
		        std::string total = SENDINCHANNEL(chan_name, chan_name, error_msg, it->second->getName());
		        std::cout << "Sending : " << total << std::endl;
		        replyClient(total, socket);
                return ;
            }
            else
            {
                it->second->setTopic(topicName);
                break;
            }
        }
    }
    send_in_channel(channelName, _clients[socket]->getNickName(), topicName, socket, "topic");
    (void)socket;
}