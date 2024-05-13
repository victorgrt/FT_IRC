#include "Server.hpp"

bool 	Server::userCheckArgs(std::string str)
{
	size_t i = 0;
	int	found = 0;
	size_t len = str.length();
	while (str[i] != '\r' && i < len)
	{
		if (str[i] == ' ')
		{
			if (found == 0 && str[i + 1] != ' ')
				found++;
			else if (found == 1 && str[i + 1] != ' ')
				found++;
			else if (found == 2 && str[i + 1] != ' ')
				found++;
		}
		i++;
	}
	if (found == 3)
		return (true);
	return (false);
}

void	Server::userCmd(std::string str, int socket)
{
	if (str != "USER" && (str.substr(0, 5) != "USER " || str.length() <= 5))
		return;
	size_t find = str.find(' ');
	if (userCheckArgs(str) == false || find == std::string::npos)
	{
		replyClient(ERR_NEEDMOREPARAMS(_clients[socket]->getNickName(), "USER"), socket);
		return;
	}
	std::string cmd = str.substr(str.find(' ') + 1);
	cmd = cmd.substr(0, cmd.find(' '));
	for (int i = 0; cmd[i]; i++)
		if (!((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9')))
			return;
	if (_clients[socket])
	{
		_clients[socket]->setUserName(cmd);
		if (_clients[socket]->getStatus() == 3 || _clients[socket]->getStatus() == 2)
		{
			if (_clients[socket]->getStatus() == 3 && _clients[socket]->getUserFlag() == false)
			{
				_clients[socket]->setUserFlag(true);
				_clients[socket]->updateStatus(4);
			}
			else if (_clients[socket]->getStatus() == 2 && _clients[socket]->getUserFlag() == false)
			{
				_clients[socket]->setUserFlag(true);
				_clients[socket]->updateStatus(3);
			}
		}
		if (_clients[socket]->getStatus() == 4)
		{
			std::string server_name = "localhost";
			std::string username = _clients[socket]->getUserName();
			std::string nickname = _clients[socket]->getNickName();
			if (checkNickName(nickname, socket) == false)
			{
				std::string tmp_username = _clients[socket]->getUserName();
				replyClient(NICKNAMEINUSE_ERR(tmp_username), socket);
			}
			if (_clients[socket]->getConnectedStatus() == false)
			{
				replyClient(WELCOME_MSG(server_name, nickname, username), socket);
				_clients[socket]->change_connected();
				startingMsg(socket);
			}
		}
		else
		{
			return ;
		}
	}
	else
		std::cerr << "Client not found for socket: " << socket << std::endl;
}