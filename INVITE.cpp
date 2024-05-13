#include "Server.hpp"

bool	Server::fillinBuffer(std::string locate, std::string& channelname, std::string& invited, std::string& nickname, int socket)
{
	int config = 0;
	Client *sender = getClient(socket);
	if (sender == NULL || !sender)
	{
		return (false);
	}
	nickname = sender->getNickName();
	if (nickname.empty())
	{
		return (false);
	}
    channelname.clear();
    invited.clear();
	size_t channel_end;
    size_t channel_start = locate.find("#");
    if (channel_start != std::string::npos)
    {
       	channel_end = locate.find(" ", channel_start);
        if (channel_end != std::string::npos)
        {
			channelname = locate.substr(channel_start, channel_end - channel_start);
		}
        else
        {
            channelname = locate.substr(channel_start);
			config = 1;
		}
    }
	channel_end = locate.find(" ", channel_start);
	if (static_cast<int>(channel_end) == -1)
	{
		size_t space_pos = locate.find(" ");
		if (space_pos != std::string::npos)
		{
			invited = locate.substr(space_pos + 1);

			size_t end_pos = invited.find(" ");
			if (end_pos != std::string::npos)
			{
				invited = invited.substr(0, end_pos);
			}
		}
	}
	else if (static_cast<int>(locate[channel_end]) == 32)
	{
		size_t start = channel_end + 1;
		size_t end = locate.find("\r");
		invited = locate.substr(start, end);
	}
	if (channelname.empty())
		return (false);
	if (invited.empty())
		return (false);
	if (nickname.empty())
		return (false);
    return true;
}

void	Server::inviteCmd(std::string locate, int socket)
{
	std::string invited_nickname;
	std::string channel_name;
	std::string nickname;
	Channel *channel_class;

	if (fillinBuffer(locate, channel_name, invited_nickname, nickname, socket) == true)
	{
		//??
	}
	else
	{
		return ;
	}

	Client *sender = getClient(socket);
	int socket_to_send_to = getSocketFromUser(invited_nickname);
	
	if (socket_to_send_to == -666)
	{
		std::string err_msg = ERR_NOSUCHNICK(_clients[socket]->getNickName(), invited_nickname);
		return ;
	}
	sender->printChannels();

	std::map<std::string, Channel*>::iterator it1 = _channelLst.begin();
	std::map<std::string, Channel*>::iterator ite1 = _channelLst.end();
	bool	channel_exist = false;
	bool	sender_in_channel = false;
	bool	already_on = false;
	std::string tmp_username_invited;
	while (it1 != ite1)
	{
		std::string tmp_name = "#" + it1->first;
		if (channel_name == tmp_name)
		{
			channel_exist = true;
			channel_class = it1->second;
			std::map<int, Client*> client_list = it1->second->getClientlst();
			std::map<int, Client *>::iterator first = client_list.begin();
			std::map<int, Client *>::iterator end = client_list.end();
			while (first != end)
			{
				std::cout << "comapring : '" << first->second->getNickName() << " and '" << invited_nickname << "'" << std::endl;
				if (first->second->getNickName() == invited_nickname)
				{
					std::cout << invited_nickname << " is already on !" << std::endl;
					already_on = true;
					tmp_username_invited = first->second->getUserName();
				}
				else if (first->second->getNickName() == _clients[socket]->getNickName())
				{
					sender_in_channel = true;
				}
				++first;
			}
			break ;
		}
		++it1;
	}
	if (channel_exist == false)
	{
		std::string doesnt_exist = ERR_NOSUCHCHANNEL(_clients[socket]->getNickName(), channel_name);
		replyClient(doesnt_exist, socket);
		return ;
	}
	if (already_on == true)
	{
		std::cout << "ici" << std::endl;
		std::string already_on = ERR_USERONCHANNEL(nickname, tmp_username_invited, channel_name);
		replyClient(already_on, socket);
		return ;
	}
	if (sender_in_channel == false)
	{
		std::string error_msg = ERR_NOTONCHANNEL(nickname, channel_name);
		replyClient(error_msg, socket);
		return ;
	}
	if (isClientOp(channel_class->getOperatorList(), socket) == false && channel_class->getInvitOnly() == true)
	{
		std::string chan_name = "\x1b[1m\x1b[36m" + channel_name + "\x1b[0m";
		std::cout << "On est ici normalement" << std::endl;
		std::string error_msg = ERR_CHANOPRIVSNEEDED(nickname, it1->first);
		// replyClient(error_msg, socket);
		std::string total = SENDINCHANNEL(chan_name, chan_name, error_msg, channel_class->getName());
		std::cout << "Sending : " << total << std::endl;
		replyClient(total, socket);

		return ;
	}

	std::string username = sender->getUserName();
	std::string userID = ":" + nickname + "!" + username + "@localhost";
	std::string msg = RPL_INVITING(userID,  nickname, invited_nickname, channel_name);
	Client *invited = getClient(socket_to_send_to);
	channel_class->addClient(socket_to_send_to, invited);

	replyClient(msg, socket_to_send_to);
	
	return ;
}