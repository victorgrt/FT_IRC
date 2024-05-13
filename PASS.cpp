#include "Server.hpp"

std::string	Server::getServerPassword() const
{
	return _password;
}

std::string Server::getUsernameFormNick(std::string to_parse)
{
	std::string tmp_name = to_parse;
	size_t pos = tmp_name.find("NICK ");
	if (pos != std::string::npos)
	{
		pos += 5;
		std::string name;
		while (pos < tmp_name.size() && tmp_name[pos] != '\r' && tmp_name[pos] != '\n')
		{
			name += tmp_name[pos];
			pos++;
		}
		return (name);
	}
	else
	{
		return ("Error");
	}
}

bool Server::passCmd(std::string str, std::string cmd, int socket)
{
	if (cmd != "PASS" && (cmd.substr(0, 5) != "PASS " || cmd.length() <= 5))
		return false;
	std::string server_pass = getServerPassword();
	int i = 0;
	if (getClient(socket)->getStatus() == 4)
	{
		replyClient(ALREADYREGISTERED(getClient(socket)->getNickName()), socket);
		return false;
	}
	while (cmd[i] != '\0' && cmd[i] != ' ')
		i++;
	i++;
	if (cmd == "PASS" || cmd == "PASS ")
	{
		std::string dn = "Error";
		replyClient(ERR_NEEDMOREPARAMS(dn, "PASS"), socket);
		return false;
	}
	int start = i;
	std::string from_client = &cmd[start];
	if (from_client.compare(server_pass) != 0)
	{
		std::string username = getUsernameFormNick(str);
		_clients[socket]->dont_set_user(true);
		_clients[socket]->setSkip(true);
		replyClient(ERROR_INVPASS(username), socket);
		return false;
	}
	_clients[socket]->dont_set_user(false);
	if (_clients[socket]->getStatus() == 1)
		getClient(socket)->updateStatus(2);
	return (true);
}
