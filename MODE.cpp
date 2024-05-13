#include "Channel.hpp"

int	isOneOf(char c, std::string chars)
{
	for(int i = 0; chars[i]; i++)
		if (c == chars[i])
			return (1);
	return (0);
}

std::string	Server::initOptions(std::string cmd)
{
	std::string	ret = "";
	size_t		i = 0;

	if (cmd == "")
		return ("");
	while (cmd[i] && cmd[i] != '+' && cmd[i] != '-')
		i++;
	if (cmd[i] != '+' && cmd[i] != '-')
		return (""); // 501 ERR_UMODEUNKNOWNFLAG
	while (cmd[i] && cmd[i] != ' ')
	{
		if (!isOneOf(cmd[i], "+-itkol"))
			return (""); // 501 ERR_UMODEUNKNOWNFLAG
		ret += cmd[i++];
	}
	return (ret);
}

std::string	Server::initArgs(std::string cmd)
{
	std::string					node = "";
	size_t						stop = cmd.find(' ');
	if (cmd == "")
		return ("");
	if (stop == std::string::npos)
		stop = cmd.size();
	for(size_t i = 0; i != stop; i++)
		node += cmd[i];
	return (node);
}

void	Channel::defineMode(char sign, char option, std::vector<std::string>& args, int socket)
{
	if (option == 'i')
		return (changeInvit(sign));
	if (option == 't')
		return (changeTopic(sign));
	if (option == 'k')
		return (changeKey(sign, args, socket));
	if (option == 'o')
		return (changeOperator(sign, args, socket));
	if (option == 'l')
		return (changeLimit(sign, args, socket));
}

void	Channel::execMode(std::string options, std::vector<std::string>& args, int socket)
{
	if (options.empty())
		return ;

	char sign = options[0];

	for(size_t i = 0; options[i]; i++)
	{
		if (options[i] == '+' || options[i] == '-')
			sign = options[i];
		else
			defineMode(sign, options[i], args, socket);
	}
}

void	Channel::infoChannel()
{
	std::cout << MAGENTA << "ABOUT " << getName() << ":" << std::endl;
	std::cout << "Topic : [" << _topic << "]" << std::endl;
	std::cout << "Key : [" << _key << "]" << std::endl;
	std::cout << "Limit : [" << _limit << "]" << std::endl;
	std::cout << "Inviteonly : [" << _inviteonly << "]" << std::endl;
	std::cout << "Clients :" << std::endl;
	for (std::map<int, Client*>::iterator it = _clientslst.begin(); it != _clientslst.end(); it++)
		std::cout << "[" << it->second->getNickName() << "]" << std::endl;
	std::cout << "Operators :" << std::endl;
	for (std::map<int, Client*>::iterator it = _operators.begin(); it != _operators.end(); it++)
		std::cout << "[" << it->second->getNickName() << "]" << std::endl;
}

size_t	optionNB(std::string options)
{
	size_t  count = 0;

	for(size_t i = 0; i < options.size(); i++)
	{
		if (options[i] == '+' || options[i] == '-')
			continue ;
		count++;
	}
	return (count);
}

int	Server::modeCmd(std::string locate, int socket)
{
	if (locate != "MODE" && (locate.substr(0, 5) != "MODE " || locate.length() <= 5))
		return 1;
	std::string	cmd = locate.substr(locate.find(' ') + 1);
	std::string	channelName = "";
	size_t		end = cmd.find(' ');

	if (end == std::string::npos)
		end = cmd.size();
	channelName.append(cmd, 0, end);
	for (std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); it++)
	{
		if (cmd.find(it->first) == 1 && cmd[0] == '#' && "#" + it->first == channelName)
		{
			if (locate == "MODE " + channelName)
				return 0;
			if (isClientOp(it->second->getOperatorList(), socket) == false)
            {
                std::string tmp = channelName;
                if (tmp[0] == '#')
                    tmp.erase(0, 1);
				std::string chan_name = "\x1b[1m\x1b[36m#" + it->second->getName() + "\x1b[0m";
				std::cout << "On est ici normalement" << std::endl;
				std::string error_msg = ERR_CHANOPRIVSNEEDED(_clients[socket]->getNickName(), it->second->getName());
				// replyClient(error_msg, socket);
				std::string total = SENDINCHANNEL(chan_name, chan_name, error_msg, it->second->getName());
				std::cout << "Sending : " << total << std::endl;
				replyClient(total, socket);
				return 1;
			}
            size_t	nextArg = cmd.find(' ');
			if (nextArg == std::string::npos)
				nextArg = cmd.size() - 1;
			cmd = cmd.substr(nextArg + 1);
			nextArg = cmd.find(' ');
			if (nextArg == std::string::npos)
				nextArg = cmd.size() - 1;
			while (!cmd.empty())
			{
				// AJOUTER UNE CONDITION DE VERIFICATION SI SOCKET EST OPERATOR ICI
				std::string					options = "";
				std::vector<std::string>	args;

				options = initOptions(cmd);
				if (!options.empty())
					cmd = cmd.substr(nextArg + 1);
				else
					break ;
				for(size_t i = 0; i != optionNB(options) && !cmd.empty(); i++)
				{
					args.push_back(initArgs(cmd));
					nextArg = cmd.find(' ');
					if (nextArg == std::string::npos)
						nextArg = cmd.size() - 1;
					cmd = cmd.substr(nextArg + 1);
				}
				it->second->execMode(options, args, socket);
				// it->second->infoChannel();
			}
			return (0);
		}
	}
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (cmd.find(it->second->getNickName()) == 0)
			return (0);
	return (replyClient(ERR_NOSUCHCHANNEL(_clients[socket]->getNickName(), channelName), socket));
}