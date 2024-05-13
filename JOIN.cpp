#include "Server.hpp"

bool    Server::isInsideChannel(Channel *channel, int socket)
{
    std::map<int, Client*>client_list = channel->getClientlst();
    std::map<int, Client *>::iterator it = client_list.begin();
    std::map<int, Client *>::iterator ite = client_list.end();
    
    while (it != ite)
    {
        if (it->first == socket)
        {
            return (true);
        }
        ++it;
    }

    return (false);
}

void extractStringAndFill(std::string& input, std::string& afterSpace)
{
    size_t pos = input.find(' ');
    if (pos != std::string::npos && pos + 1 < input.size())
    {
        afterSpace = input.substr(pos + 1);
        input.resize(pos);
    }
}


void Server::joinCmd(std::string locate, int socket)
{
        if (locate != "JOIN" && (locate.substr(0, 5) != "JOIN " || locate.length() <= 5))
        {
            return;
        }
        int flag = 0;
        size_t start = 0;
        start = locate.find("#");
        std::string channelName = locate.substr(start + 1);
        std::string key;
        if (channelName.find(" ") != std::string::npos)
        {
            extractStringAndFill(channelName, key);
        }
        if (start == std::string::npos || channelName.empty())
        {
            replyClient(ERR_NEEDMOREPARAMS(getClient(socket)->getNickName(), "JOIN"), socket);
            return;
        }
        for (std::map<std::string,
		Channel *>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
	    {
		    std::string tmp = it->first;
		    Channel *channel = it->second;
		    if (tmp == channelName)
            {
                if (channel->getInvitOnly() == true && isInsideChannel(channel, socket) == false)
                {
                    std::string err_msg = ERR_INVITEONLYCHAN(_clients[socket]->getNickName(), channelName);
                    replyClient(err_msg, socket);
                    return ;    
                }
                if (channel->getKey() != "")
                {
                    std::string mdp = channel->getKey();
                    if (mdp != key)
                    {
                        std::string err_msg = ERR_BADCHANNELKEY(_clients[socket]->getNickName(), channelName);
                        replyClient(err_msg, socket);
                        return ;
                    }
                }
                if (channel->getLimit() != 0 && (static_cast<size_t>(channel->getLimit())) < (channel->getClientlst().size() + 1))
                {
                    std::string err_msg = ERR_CHANNELISFULL(_clients[socket]->getNickName(), channelName);
                    replyClient(err_msg, socket);
                    return ;
                }
                flag = 1;
                channel->addClient(socket, getClient(socket));
                if (findChannel(channelName)->second->getTopicStatus() == 1)
                {
                    std::string tn = findChannel(channelName)->second->getTopic();
                    replyClient(ALREADYTOPIC(getClient(socket)->getNickName(), channelName, tn), socket);
                }
                else
                    replyClient(NOTOPIC(getClient(socket)->getNickName(), channelName), socket);
                break; 
            }
        }
        if (flag == 1)
        {
            std::map<std::string, Channel*>::iterator channelIt = findChannel(channelName);
            if (channelIt != _channelLst.end())
            {
                std::map<int, Client*> clientMap = channelIt->second->getClientlst();
                std::string userlst;
                for (std::map<int, Client*>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)
                {
                    userlst += it->second->getNickName() + " ";
                }
                replyClient(LISTUSERS(getClient(socket)->getNickName(), channelName, userlst), socket);
                replyClient(NAMELIST(getClient(socket)->getNickName(), channelName), socket);
                WelcomeMsg(channelName, getClient(socket), socket);
            }
        }
        else if (flag == 0)
        {
            createChannel(channelName, socket);
            _nb_channels++;
        }
}

void Server::createChannel(std::string name, int socket)
{
    if (_channelLst.find(name) == _channelLst.end())
    {
        replyClient(NOT_EXISTING_CHANNEL(name), socket);
    }
    Channel* channel = new Channel(name);
    _channelLst.insert(std::make_pair(name, channel));
    channel->newOperator(socket, getClient(socket));
    channel->addClient(socket, getClient(socket));
    getClient(socket)->getChannel().push_back(name);
    replyClient(CREATECHANNEL(getClient(socket)->getName(), getClient(socket)->getUserName(), name), socket);
    if (findChannel(name)->second->getTopicStatus() == 0)
        replyClient(NOTOPIC(getClient(socket)->getNickName(), name), socket);
    replyClient(LISTUSERS(getClient(socket)->getNickName(), name, getClient(socket)->getNickName()), socket);
    replyClient(NAMELIST(getClient(socket)->getNickName(), name), socket);
    std::string color_name = "\x1b[1m\x1b[36m#" + name + "\x1b[0m";
    std::string info = "\x1b[32m\x1b[1mChannel \x1b[36m#" + name + "\x1b[32m was created successfully.\r\n";
    replyClient(SENDINCHANNEL(color_name, color_name, info, name), socket);
}

std::map<std::string, Channel*>::iterator Server::findChannel(std::string channelName)
{
    for(std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        if (it->second->getName() == channelName)
        {
            return it;
        }
    }
    return _channelLst.end();
}