#include "Server.hpp"

bool	quitting = false;

Server::Server(char **av)
{
	size_t i;
	_status = 0;
	_nb_channels = 0;
	_nb_clients = 0;
	for(i = 0; isdigit(av[1][i]) != 0; i++)
		continue ;
	_password = av[2];
	if (i == strlen(av[1]))
	{
		int port = atoi(av[1]);
		_socket = socket(AF_INET, SOCK_STREAM, 0);
		_serverAddress.sin_family = AF_INET;
		_serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
		_serverAddress.sin_port = htons(port);
		if (bind(_socket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress)) == -1)
		{
			std::cout << "failed to bind" << std::endl;
			return;
		}
	}
}

Channel* Server::getChannelFromName(std::string name)
{
	std::map<std::string, Channel*>::iterator it = _channelLst.begin();
	std::map<std::string, Channel*>::iterator ite = _channelLst.end();

	while (it != ite)
	{
		std::string tmp_channel_name = it->first;
		if (tmp_channel_name == name)
		{
			return (it->second);
		}
		++it;
	}
	return (NULL);
}

std::map<std::string, Channel*> Server::getChannelListe() const
{
	return _channelLst;
}


int	Server::getSocket() const
{
	return _socket;
}

int		Server::read_data_from_socket(int socket)
{
	char buffer[1024];
	int bytes_read;
	bytes_read = recv(socket, buffer, 1024, 0);
	buffer[bytes_read] = '\0';
	if (bytes_read <= 0 || quitting == true)
    {
		_clients[socket]->setConnectedStatus(false);
        quitCmd(socket);
        return 0; 
    }
    if (buffer[0] == '\0')
	{
		return 0;
	}
	std::string cmd1 = buffer;
	getClient(socket)->setTempBuffer(static_cast<char*>(buffer), 0);
	std::string cmd = getClient(socket)->getTempBuffer();
	if ((cmd[cmd.size() - 2] == '\r' && cmd[cmd.size() - 1] == '\n') || cmd == "")
	{
		parser(getClient(socket)->getTempBuffer(), socket);
		if (getClient(socket) && quitting == false)
			getClient(socket)->setTempBuffer("", 1);
	}
	buffer[0] = '\0';
	return (1);
}

struct sockaddr_in	Server::getServerAddress() const
{
	return _serverAddress;
}

int	Server::getClientSocket(int socket) const
{
	std::map<int, Client*>::const_iterator it = _clients.find(socket);
	if (it != _clients.end())
		return it->second->getSocket();
	else
		return -1;
}

Client*	Server::getClient(int socket) const
{
	std::map<int, Client*>::const_iterator it = _clients.find(socket);
	if (it != _clients.end())
		return it->second;
	else
		return NULL;
}

fd_set& Server::getallSockets()
{
	return _allSockets;
}

fd_set& Server::getreadFds()
{
	return _readFds;
}

int	Server::getChannelNumber() const
{
	return _nb_channels;
}

int	Server::getClientNumber() const
{
	return _nb_clients;
}

int Server::getfdMax()
{
	return _fdMax;
}

void	Server::setfdMax(int socket)
{
	_fdMax = socket;
}

void	Server::setClientSocket(int tmp)
{
	Client* client = new Client();
	client->setSocket(tmp);
	_clients.insert(std::make_pair(tmp, client));
}

void	Server::accept_new_connection()
{
	quitting = false;
	int client_fd;

	client_fd = accept(_socket, NULL, NULL);
	setClientSocket(client_fd);
	_nb_clients++;
	FD_SET(client_fd, &_allSockets);
	if (client_fd > _fdMax)
		_fdMax = client_fd;
}

void	Server::defineCmd(std::string str, int start, int it, int socket)
{
	std::string locate;
	std::string	options;
	std::string	args;
	std::string	cmd;
	locate.append(str, start, it - start);
	cmd.append(str, start, str.find(' '));
	if (_clients[socket]->getConnectedStatus() == false && str.find("PASS") == std::string::npos)
	{
		if (_clients[socket]->getSkip() == true)
		{
			getClient(socket)->updateStatus(0);

			_clients[socket]->setTempBuffer("", 1);
			if (_nb_clients > 0)
				_nb_clients--;
			_clients[socket]->setConnectedStatus(false);
			quitting = true;
			return;
		}
	}
	if (locate.find("NICK") == 0)
	{
		nickCmd (locate, socket);
		return ;
	}
	else if (locate.find("CAP LS") == 0 && _clients[socket]->getConnectedStatus() == false)
		caplsCmd(locate, socket);
	else if (locate.find("USER") == 0)
	{
        userCmd(locate, socket);
		if (_clients[socket]->getStatus() == 1)
		{
			getClient(socket)->updateStatus(0);
			_clients[socket]->setTempBuffer("", 1);
			_clients[socket]->setNickName("\0");
			_clients[socket]->ClearNick();
			if (_nb_clients > 0)
				_nb_clients--;
			_clients[socket]->setConnectedStatus(false);
			return ;
		}
	}
	else if (locate.find("MODE") == 0)
		modeCmd(locate, socket);
	else if (locate.find("PASS") == 0)
	{
		if (passCmd(cmd, locate, socket) == false && _clients[socket]->getConnectedStatus() == false)
		{
			getClient(socket)->updateStatus(0);
			_clients[socket]->setTempBuffer("", 1);
			if (_nb_clients > 0)
				_nb_clients--;
			_clients[socket]->setConnectedStatus(false);
			return ;
		}
	}
	else if (locate.find("JOIN") == 0)
	{
		joinCmd(locate, socket);
	}
	else if (locate.find("PING") == 0)
		pingCmd(locate, socket);
	else if (locate.find("PRIVMSG") == 0)
	{
		msgCmd(locate, socket);
	}
	else if (locate.find("INVITE") == 0)
	{
		inviteCmd(locate, socket);
	}
	else if (locate.find("TOPIC") == 0)
	{
		topicCmd(locate, socket);
	}
	else if (locate.find("KICK") == 0)
	{
		kickCmd(locate, socket);
	}
	else if (locate.find("NOTICE") == 0)
	{
		if ((locate.find("BOT") != std::string::npos && locate.find("START") != std::string::npos) || (locate.find("bot") != std::string::npos && locate.find("start") != std::string::npos))
			botStart(socket);
	}
	else if (locate.find("PART") == 0)
	{
		partCmd(locate, socket);
	}
	else
	{
		if (locate == "" || locate == "WHOIS")
			return ;
		std::string err = "Unknown Command : " + locate + "\r\n";
		replyClient(err, socket);
		if (_clients[socket]->getConnectedStatus() == false)
			_clients[socket]->updateStatus(0);
		return ;
	}
	_clients[socket]->setTempBuffer("", 1);
}

void	Server::parser(std::string cmd, int socket)
{
	if (cmd.empty())
		return ;
	int start = 0;
	for (int i = 0; cmd[i]; i++)
	{
		if (cmd[i] == '\r' && cmd[i + 1] == '\n')
		{
			defineCmd(cmd, start, i, socket);
			i ++;
			start = i + 1;
		}
	}
}

void	Server::WelcomeMsg(std::string channel_name, Client *client, int socket)
{
	Channel	*channel;
	for (std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        std::string tmp_name = it->first;
        channel = it->second;
        if (tmp_name == channel_name)
		{
            std::map<int, Client*> client_list = channel->getClientlst();
			for (std::map<int, Client*>::iterator it_clt = client_list.begin(); it_clt != client_list.end(); ++it_clt)
            {
            	if (it_clt->first != socket)
                {
		 			std::string nickname = client->getNickName();
					std::string msg_to_send = "\x1b[1m\x1b[32mSay Hello to our new member : " + nickname + " !\x1b[0m";
					std::string channel_msg = "\x1b[1m\x1b[36m#" + channel_name + "\x1b[0m";
					// std::string channel_without = "\x1b[1m\x1b[36m" + channel_name + "\x1b[0m";
					std::cout << "Sending : " << SENDINCHANNEL(channel_msg, channel_msg, msg_to_send, channel_name) << std::endl;
            		replyClient(SENDINCHANNEL(channel_msg, channel_msg, msg_to_send, channel_name), it_clt->first);
                }
				else
				{
					std::map<int, Client*> op_list = channel->getOperatorList();
					std::string first_op_name = op_list.begin()->second->getNickName();
					std::cout << "first op name : '" << first_op_name << "'" << std::endl;
		 			std::string nickname = client->getNickName();
					std::string welcome = "\x1b[1m\x1b[32mBienvenue dans le #" + channel_name + " channel !\x1b[0m";
					std::string cowboy;
					if (first_op_name != "")
						cowboy = "\x1b[31m\x1b[4mRègle :\x1b[31m Fait pas le cowboy et tout ira bien sinon " + first_op_name + " va te regler ton compte.\x1b[0m\n";
					else
						cowboy = "\x1b[31m\x1b[4mRègle :\x1b[31m Fait pas le cowboy et tout ira bien\x1b[0m\n";
					std::string channel_msg = "\x1b[1m\x1b[36m#" + channel_name + "\x1b[0m";
					std::string msg = SENDINCHANNEL(channel_msg, nickname, welcome, channel_name);
            		replyClient(msg, socket);
					msg.clear();
					msg = SENDINCHANNEL(channel_msg, nickname, cowboy, channel_name);
            		replyClient(msg, socket);
				}
            }
			
		}
	
	}
}

void	Server::closeSockets()
{
	for(std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		close(it->first);
}

Server::~Server()
{
    std::map<int, Client*>::iterator client_it;
    for (client_it = _clients.begin(); client_it != _clients.end(); ++client_it)
    {
		if (_socket != 0)
			close(client_it->first);
        delete client_it->second;
    }
    _clients.clear();
    std::map<std::string, Channel*>::iterator channel_it;
    for (channel_it = _channelLst.begin(); channel_it != _channelLst.end(); ++channel_it)
    {
        delete channel_it->second;
    }
    _channelLst.clear();

    FD_CLR(_socket, &_allSockets);
    if (_socket != 0)
		close(_socket);
}

