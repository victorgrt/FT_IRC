#include "Server.hpp"

bool    Server::isPartOfChannel(int socket, std::map<int, Client*> client_list)
{
    std::map<int, Client*>::iterator it = client_list.begin();
    std::map<int, Client*>::iterator ite = client_list.end();

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

void Server::send_in_channel(std::string user, std::string s_nick, std::string msg_to_send, int socket, std::string macro)
{
    std::string final_user = user;
    bool    channel_found = false;
    for (std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        std::string channel_name = it->first;
        Channel *channel = it->second;
        if (channel_name == user)
        {
            channel_found = true;
            std::map<int, Client*> client_list = channel->getClientlst();
            if (macro == "send")
            {
                if (isPartOfChannel(socket, client_list) == false)
                {
                    std::string err_msg = ERR_NOTONCHANNEL(_clients[socket]->getNickName(), user);
                    replyClient(err_msg, socket);
                    return ; 
                }
                for (std::map<int, Client*>::iterator it_clt = client_list.begin(); it_clt != client_list.end(); ++it_clt)
                {
                    if (it_clt->first != socket)
                    {
                        if (isClientOp(it->second->getOperatorList(), socket) == true)
                        {
                            std::string username = _clients[socket]->getUserName();
                            std::string msg = SENDINCHANNEL_OP(s_nick, username, msg_to_send, user);
                            replyClient(msg, it_clt->first);
                        }
                        else
                        {
                            std::string username = _clients[socket]->getUserName();
                            std::string msg = SENDINCHANNEL(s_nick, username, msg_to_send, user);
                            replyClient(msg, it_clt->first);
                        }
                    }
                }
            }
            else if (macro == "topic")
            {
                for (std::map<int, Client*>::iterator it_clt = client_list.begin(); it_clt != client_list.end(); ++it_clt)
                {
                    std::string msg = TOPIC(it_clt->second->getNickName(), user, msg_to_send);
                    replyClient(msg, it_clt->first);
                }
            }
        }
    }
    if (channel_found == false)
    {
        std::string err_msg = ERR_NOSUCHCHANNEL(_clients[socket]->getNickName(), user);
        replyClient(err_msg, socket);
        return ;
    }
}

int Server::getSocketFromUser(std::string to_find)
{
    int socket = -666;

	std::map<int,Client*>::iterator it = _clients.begin();

    if (it != _clients.end())
    {
        while (it != _clients.end())
        {
            if (it->second->getNickName() == to_find)
            {
                socket = it->first;
                return (socket);
            }
            ++it;
        }
        if (socket == -666)
        {
            if (to_find == "Botimus_Maximus" || to_find == "BOT")
            {
                socket = -777;
                return (socket);
            }
        }
    }
    return (socket);
}

int     Server::isUser(std::string str)
{
    	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        {
            if (it->second->getNickName() == str)
            {
                return 0;
            }
        }
        return 1;
}

bool Server::msgCheckArgs(std::string locate)
{
    size_t found;
    found = locate.find(":", 0);
    if (found == std::string::npos)
        return (false);
    return (true);
}

void Server::msgCmd(std::string locate, int socket)
{
    if (msgCheckArgs(locate) == false)
    {
        replyClient(ERR_NEEDMOREPARAMS(_clients[socket]->getNickName(), "PRIVMSG"), socket);
        return ;
    }
	int is_channel = 0;
    std::string user;
    std::string msg;
	size_t start = locate.find("PRIVMSG ");
	if (start != std::string::npos)
	{
        start += 8;
		size_t end = locate.find(' ', start);
		if (end != std::string::npos)
        {
            size_t test = locate.find(':', start);
            user = '\0';
            msg = '\0';
            user = locate.substr(start, end - start);
            if (locate[test] == ':')
                msg = locate.substr(test + 1);
            else
                msg = locate.substr(test);
            if (user[0] == '#')
 			{
 				user = user.substr(1);
 				is_channel = 1;
 			}
        }
    }
    std::string s_user = _clients[socket]->getUserName();
    std::string s_nick = _clients[socket]->getNickName();
	if (is_channel == 0)
 	{
        int socket_to_send_to = getSocketFromUser(user);
        if (socket_to_send_to == -777)
        {
            if (user == "BOT")
                botStart(socket);
            else
                parsingBot(msg, user, socket);
            return ;
        }
        if (socket_to_send_to == -666)
        {
            std::string no_such_nick = ERR_NOSUCHNICK(s_nick, user);
            replyClient(no_such_nick, socket);
            return ;
        }
		std::string msg_to_send = PRIVMSG(s_nick, user, msg);
		if (replyClient(msg_to_send, socket_to_send_to) == static_cast<size_t>(-1))
            quitCmd(socket);
	}
	else
	{
		send_in_channel(user, s_nick, msg, socket, "send");
	}
}