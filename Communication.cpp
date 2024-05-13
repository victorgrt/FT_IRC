#include "Communication.hpp"

size_t  replyClient(std::string Macros, int socket)
{
	ssize_t bytes_sent = 0;
	bytes_sent = send(socket, Macros.c_str(), Macros.size(), 0);
	if (bytes_sent == static_cast<ssize_t>(-1))
	{
		return (bytes_sent);
	}
	return (bytes_sent);
}