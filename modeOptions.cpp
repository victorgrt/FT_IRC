#include "Channel.hpp"

void	Channel::changeOperator(char sign, std::vector<std::string>& args, int socket)
{
	int									targSocket = -1;
	bool								opeStatus = false;
	std::map<int, Client*>::iterator	it = _clientslst.begin();
	if (!args.size())
	{
		std::string	nick = _clientslst[socket]->getNickName();
		replyClient(ERR_NEEDMOREPARAMS(nick, sign + "o"), socket);
		return ;
	}
	for (;it != _clientslst.end(); it++)
	{
		if (it->second->getNickName() == args[0])
		{
			targSocket = it->first;
			break;
		}
	}
	if (targSocket == -1)
	{
		return ; // 401 ERR_NOSUCHNICK && 441 ERR_USERNOTINCHANNEL
	}
	for (std::map<int, Client*>::iterator it = _operators.begin();it != _operators.end(); it++)
			if (it->second->getNickName() == args[0])
				opeStatus = true;
	if (sign == '+' && opeStatus == false)
		newOperator(targSocket, it->second);
	else if (sign == '-' && opeStatus == true)
		_operators.erase(targSocket);
	args.erase(args.begin());
}


void	Channel::changeInvit(char sign)
{
	if (sign == '+')
		_inviteonly = true;
	else
		_inviteonly = false;
}

void	Channel::changeTopic(char sign)
{
	if (sign == '+')
		_topicStatus = 1;
	else
		_topicStatus = 0;
}


void	Channel::changeLimit(char sign, std::vector<std::string>& args, int socket)
{
	if (sign == '+')
	{
		if (args.size() == 0)
		{
			std::string nick = _clientslst[socket]->getNickName();
			replyClient(ERR_NEEDMOREPARAMS(nick, sign + "l"), socket);
			return ;
		}
		if (atoi(args[0].c_str()) < 1 || strtod(args[0].c_str(), NULL) > 2147483647)
			return ;
		_limit = atoi(args[0].c_str());
		args.erase(args.begin());
	}
	else
		_limit = 0;
}

void	Channel::changeKey(char sign, std::vector<std::string>& args, int socket)
{
	if (sign == '+')
	{
		if (!args.size())
		{
			std::string nick = _clientslst[socket]->getNickName();
			replyClient(ERR_NEEDMOREPARAMS(nick, sign + "k"), socket);
			return ;
		}
		_key = args[0];
		args.erase(args.begin());
	}
	else
	{
		_key = "";
		if (args.size())
			args.erase(args.begin());
	}
}