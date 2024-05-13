#include "Server.hpp"

bool	Server::checkNickName(std::string to_check, int socket)
{
	std::string tmp_name;
	std::map<int, Client*>::iterator it = _clients.begin();
	int i = 0;
	while (it != _clients.end())
	{
		tmp_name = it->second->getNickName();
		if (to_check == tmp_name && socket != it->first)
		{
			return (false);
		}
		++it;
		i++;
	}
	return (true);
}

std::string intToString(int value)
{
    std::string result;
    bool negative = false;  
    if (value < 0) {
        negative = true;
        value = -value;
    }
    do
	{
        result = char('0' + value % 10) + result;
        value /= 10;
    } while (value != 0);

    if (negative) {
        result = "-" + result;
    }
    return result;
}

void	Server::nickCmd(std::string str, int socket)
{
	size_t find = str.find(' ');
	if (str != "NICK" && (str.substr(0, 5) != "NICK " || str.length() <= 5))
	{
		if (_clients[socket]->getConnectedStatus() == false)
			getClient(socket)->updateStatus(0);
		return;
	}
	std::string cmd = str.substr(find + 1);	
	if ((cmd[0] == '\0' || find == std::string::npos) && _clients[socket]->getConnectedStatus() == false)
	{
		std::string error = "Error";
		replyClient(ERR_NONICKNAMEGIVE(error), socket);
		return;
	}
	if (_clients[socket]->do_we_set_or_not() == true)
	{
		return ;
	}
	for (int i = 0; cmd[i]; i++)
	{
		if (!(((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9') || cmd[i] == '_')) || (cmd.length() > 9))
		{
			std::string username;
			if (_clients[socket]->getUserName().empty())
				username = "Error";
			else
				username = _clients[socket]->getUserName();
			std::string msg = ERR_ERRONEUSNICKNAME(username, cmd);
			replyClient(msg, socket);
			return;
		}
	}
	std::map<int, Client*>::iterator it = _clients.find(socket);
	if (it != _clients.end())
	{
		std::string nickname = cmd;
		if (checkNickName(nickname, socket) == false)
		{
			_clients[socket]->setSkip(true);
			replyClient(NICKNAMEINUSE_ERR(nickname), socket);
			return ;
		}
		else
		{
			if (_clients[socket]->getConnectedStatus() == false)
			{
				it->second->setNickName(cmd);
				if (_clients[socket]->getStatus() == 2 || _clients[socket]->getStatus() == 3)
				{
					if (_clients[socket]->getNickFlag() == false)
					{
						if (_clients[socket]->getStatus() == 3)
							it->second->updateStatus(4);
						else
							it->second->updateStatus(3);
						_clients[socket]->setNickFlag(true);
					}
				}
			}
			else
			{
				if (cmd == "NICK")
				{
					std::string old = _clients[socket]->getNickName();
					std::string msg = ":" + old + " NICK " + old + "\r\n";
					replyClient(msg, socket);
					_clients[socket]->setTempBuffer("", 1);
					return ;
				}
				else
				{
					std::string old = _clients[socket]->getNickName();
					std::string msg = ":" + old + " NICK " + nickname + "\r\n";
					replyClient(msg, socket);
					it->second->setNickName(cmd);
					return ;
				}
			}
		}
		if (it->second->getStatus() >= 4)
		{
			it->second->setNickName(cmd);
			std::string server_name = "localhost";
			std::string username = it->second->getUserName();
			std::string nickname = it->second->getNickName();
			if (_clients[socket]->getConnectedStatus() == false)
			{
				replyClient(WELCOME_MSG(server_name, nickname, username), socket);
				_clients[socket]->change_connected();
				startingMsg(socket);
			}
		}
	}
	else
		std::cerr << RED << "Client not found for socket: " << socket << std::endl;
}