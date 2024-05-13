#include "Server.hpp"

bool	interrupted = false;

void handleSignal(int signal_recu)
{
	if (signal_recu == SIGINT)
	{
		interrupted = true;
		return ;
	}
	return ;
}

void	Server::loop()
{
	struct timeval timer;
	signal(SIGINT, handleSignal);
	while (!interrupted)
	{
		_readFds = _allSockets;
		timer.tv_sec = 2;
		timer.tv_usec = 0;
		int result;
		result = select(_fdMax + 1, &_readFds, NULL, NULL, &timer);
		if (result == -1 || interrupted == true)
		{
			std::cout << "Closing Server" << std::endl;
			return;
		}
		for (int i = 0; i <= _fdMax; i++)
		{
			if (FD_ISSET(i, &_readFds) != 1)
			{
				continue ;
			}
			if (i == _socket)
				accept_new_connection();
			else
			{
				read_data_from_socket(i);
			}
		}
	}
}

int main(int ac, char **av)
{

	if (ac == 3)
	{
		for (size_t i = 0; av[1][i]; i++)
		{
			if (!(av[1][i] >= '0' && av[1][i] <= '9') || strlen(av[1]) != 4)
			{
				std::cout << "Invalid Port Number !" << std::endl;
				return (1);
			}
		}
		int status = 0;
		Server server(av);
		status = listen(server.getSocket(), 10);
		if (status < 0)
		{
			std::cout << "Socket error from listen()" << std::endl;
			return (1);
		}
		FD_ZERO(&server.getallSockets());
		FD_ZERO(&server.getreadFds());
		FD_SET(server.getSocket(), &server.getallSockets());
		server.setfdMax(server.getSocket());
		server.loop();
	}
	else
		std::cout << "Usage : ./ircserv *port* *mdp*" << std::endl;
	return (0);
}