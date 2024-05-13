#pragma once
#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::string _key;
        int         _limit;
        bool        _inviteonly;
        int         _topicStatus;
        std::map<int, Client*>  _clientslst;
        std::map<int, Client*>  _operators;
    public:
        Channel(std::string str);
        ~Channel();
        void    infoChannel();
        void    setName(std::string str);
        void    addClient(int socket, Client *client);
        void    changeOperator(char sign, std::vector<std::string>& args, int socket);

        void    newOperator(int socket, Client *client);
        void    changeInvit(char sign);
        void    changeTopic(char sign);
        void    changeKey(char sign, std::vector<std::string>& args, int socket);
        void    changeLimit(char sign, std::vector<std::string>& args, int socket);

       	void    defineMode(char sign, char option, std::vector<std::string>& args, int socket);
		void    execMode(std::string options, std::vector<std::string>& args, int socket);

        void setTopic(std::string str);
        int getTopicStatus();
        std::string getTopic();
        void    removeClientFromLst(std::string clientName);    
        std::map<int, Client*> getClientlst();
        std::string getName();

        int getLimit() const;
        bool getInvitOnly() const;
        std::string getKey() const;
        std::map<int, Client*>  getOperatorList() const;

        void    printChannelUsers();
};