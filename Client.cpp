#include "Client.hpp"

Client::Client()
{
	_nickcount = 0;
	_connection_status = 0;
	_dont_set_user = false;
	_is_connected = false;
	_sent = false;
	_skip = false;
	_userflag = false;
	_nickflag = false;
}

void	Client::setSent(bool to_set)
{
	_sent = to_set;
}

bool	Client::getSent() const
{
	return (_sent);
}

void	Client::setSkip(bool to_set)
{
	if (to_set == true)
		_skip = true;
	else
		_skip = false;
}

bool	Client::getSkip() const
{
	return (_sent);
}

Client::Client(struct sockaddr client_addr) : _clientAddr(client_addr)
{}

void	Client::change_connected()
{
	_is_connected = true;
}

bool	Client::getConnectedStatus() const
{
	return (_is_connected);
}

std::string	Client::getTempBuffer()
{
	return _tempBuffer;
}

void  Client::setTempBuffer(std::string str, int flag)
{
	if (flag == 0)
		_tempBuffer += str;
	else if (flag == 1)
		_tempBuffer = "";
}

void	Client::dont_set_user(bool value)
{
	if (value == true)
		_dont_set_user = true;
	else
		_dont_set_user = false;
}

bool	Client::do_we_set_or_not() const
{
	return _dont_set_user;
}

void	Client::printChannels()
{
	std::vector<std::string>::iterator it = _channel.begin();
	std::vector<std::string>::iterator itend = _channel.end();
	
	while (it != itend)
	{
		++it;
	}
}

void	Client::delChannel(std::string channelName)
{
	for (std::vector<std::string>::iterator it = _channel.begin(); it != _channel.end(); ++it)
	{
		if(*it == channelName)
		{
			it = _channel.erase(it);
		}
	}
}

std::string Client::getNickName() const
{
	return _nickname;
}

void    Client::updateStatus(int to_set)
{
	if (to_set == 0)
	{
		std::string end = "CAP_ACK END\r\n";
		replyClient(end, _socket);
		std::string msg = "\x1b[4m\x1b[1mCONNECTION FAILED ! TRY FULL PROCESSUS AGAIN FROM CAP LS !!\x1b[0m\r\n";
		replyClient(msg, _socket);
	}
	_connection_status = to_set;
}

void	Client::setNickFlag(bool to_set)
{
	_nickflag = to_set;
}

bool    Client::getNickFlag() const
{
	return _nickflag;
}

std::string Client::getUserName() const
{
	return _username;
}

std::string Client::getName() const
{
	return _name;
}

std::vector<std::string> Client::getChannel()
{
	return _channel;
}

struct sockaddr	Client::getClientAddr() const
{
	return (_clientAddr);
}

int Client::getSocket() const
{
	return _socket;
}

int     Client::getStatus()
{
    return _connection_status;
}

void    Client::addChannel(std::string channelname_to_add)
{
	_channel.push_back(channelname_to_add);
}

void Client::setSocket(int tmp)
{
	_socket = tmp;
}

void Client::setNickName(std::string str)
{
	_nickname = str;
}

void Client::setUserName(std::string str)
{
    _username = str;
}

void	Client::setConnectedStatus(bool to_set)
{
	_connection_status = to_set;
}

void	Client::setUserFlag(bool to_set)
{
	_userflag = to_set;
}

bool	Client::getUserFlag() const
{
	return (_userflag);
}

void	Client::ClearNick()
{
	_nickname.clear();
	_nickname.erase();
}

Client::~Client()
{}