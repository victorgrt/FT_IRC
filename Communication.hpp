#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

#include "Server.hpp"
#include "Client.hpp"

// COMMANDS //

#define WELCOME_MSG(servername, username, nickname) (":" + servername + " 001 " + username + \
	" :Welcome to the Internet Relay Network " + username + "!" + nickname + "\r\n")
#define CAP_LS() ("CAP_ACK LS\r\n")
#define CREATECHANNEL(nickname, username, name) (":" + nickname + "!" + username + "@localhost JOIN " + name + "\r\n") //CREATE CHANNEL
#define SENDINCHANNEL(sender, username, msg, channelname) (":" + sender + "!" + username + "@localhost PRIVMSG #" + channelname + " :" + msg + "\r\n") //PRIVMSG FOR CHANNEL
#define SENDINCHANNEL_OP(sender, username, msg, channelname) (":\x1b[31m\x1b[1m@" + sender + "\x1b[0m!" + username + "@localhost PRIVMSG #" + channelname + " :\x1b[31m\x1b[1m" + msg + "\x1b[0m\r\n") //PRIVMSG FOR CHANNEL OPERATORS
#define PRIVMSG(nickname, envoyeur, msg) (":" + std::string(nickname) + "!" + "localhost" + " PRIVMSG " + envoyeur + " :" + msg + "\r\n") //PRIVMSG CMD
#define DEFAULTPONG(servername) ("PONG " + servername + "\r\n") //PONG CMD
#define RPL_INVITING(userID, client_nickname, invited_nickname, channel_name) (userID + " INVITE " + invited_nickname + " :" + channel_name + "\r\n") //INVITE CMD
#define PART(userid, channel, reason) (userid + " PART " + channel + " :" + reason + "\r\n");

// NICK ERRORS //
#define NICKNAMEINUSE_ERR(username) (":localhost 433 * " + username + " :Nickname is already in use." + "\r\n")
#define ERR_ERRONEUSNICKNAME(client_username, nick) (client_username + " " + nick + " :Erroneus nickname" + "\r\n") //THERE IS A FORDBIDEN CHAR IN THE NICKNAME
#define TOPIC(nickname, username, channelName, topic) (":" + nickname + "!" + username + "@localhost" + " TOPIC #" + channelName + " :" + topic + "\r\n") 
#define KICKUSER(channel_name, nickname, reason) ("KICK " + channel_name + " " + nickname + " :" + reason + "\r\n")
#define KICK(userid, channel_name, kicked_nickname, reason) (userid + " KICK #" + channel_name + " " + kicked_nickname + " :" + reason + "\r\n")
// PRIVMSG ERRORS

// PASS ERRORS //
#define ERROR_INVPASS(nickname) (nickname + " :Password incorrect\r\n")
#define ERR_NEEDMOREPARAMS(nickname, cmd) (nickname + " " + cmd + " :Not enough parameters\r\n")
#define ALREADYREGISTERED(nickname) (nickname + " :You may not reregister\r\n")
#define ALREADYTOPIC(nickname, channelName, topicName) (":localhost 332 " + nickname + " #" + channelName + " :" + topicName + "\r\n")
#define ERR_NOSUCHNICK(client, dest) (client + " " + dest + " :No such nick/channel\r\n") 
#define ERR_NONICKNAMEGIVE(nickname) (nickname + " :No nickname given\r\n")
#define ERRONEUSNICKNAME_ERR(nickname) (":localhost 432 " + nickname + " :Erroneous nickname" + "\r\n")

// INVITE ERRORS //
#define ERR_NOTINCHANNEL(client_nickname, user_nick, channelname) (client_nickname + " " + user_nick + " " + channelname + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(client_nickname, channelname) (client_nickname + " " + channelname + " :You're not on that channel" + "\r\n") //TRYING TO INVITE SOMEONE INTO A CHANNEL YOUR ARE NOT PART OF
#define ERR_NOSUCHCHANNEL(nickname, channelName) (":localhost 403 " + nickname + " " + channelName + " :No such channel" + "\r\n") //NEEDS A # IN THE CHANNEL NAME !!
#define ERR_USERONCHANNEL(client, nickname , channelname) (":localhost 403 " + client + " " + nickname + " " \
+ channelname + " :is already on channel" + "\r\t") //TRYING TO INVITE SOMEONE THAT IS ALREADY IN THE CHANNEL
#define ERR_CHANOPRIVSNEEDED(client_nickname, channel_name) (client_nickname + " #" + channel_name + " :You're not channel operator" + "\r\n") //OPERTORS CAN INVITE ONLY

// JOIN ERRORS //
#define ERR_INVITEONLYCHAN(nickname, channel_name) (":localhost 473 " + nickname + " " + channel_name + " :Cannot join channel (+i)" + "\r\n")
#define ERR_BADCHANNELKEY(client_nickname, channel_name) (":localhost 475 " + client_nickname + " " + channel_name + " :Cannot join channel (+k)" + "\r\n") //TRYING TO JOIN A CHANNEL WITH BAD KEY
#define ERR_CHANNELISFULL(client_nickname, channel_name) (":localhost 471 " + client_nickname + " " + channel_name + " :Cannot join channel (+l)" + "\r\n") //TRYING TO JOIN A CHANNEL THAT IS FULL
#define NOT_EXISTING_CHANNEL(channelName) ("#" + channelName + " :No such channel\r\n") //TRYING TO JOIN A CHANNEL THAT DOESNT EXIST
#define NOTOPIC(nickname, channelName) (":localhost 331 " + nickname + " #" + channelName + " :No topic is set\r\n")
#define LISTUSERS(nickname, channelName, userslst) (":localhost 353 " + nickname + " = #" + channelName+ " :@" + userslst + "\r\n")
#define NAMELIST(user, channel) (":localhost 366 " + user + " " + channel + " :End of /NAMES list\r\n")

// MODE ERRORS //
#define ERR_INVALIDMODEPARAM(client_nickname, mode_sign, mode_option , description) (":localhost 696 " + client_nickname + " " + \
		mode_sign + mode_option + " :" + description + "\r\n")
#define ERR_INVALIDKEY(client_nickname, channel_nickname) (":localhost 525 " + client_nickname + " " + channel_name + " :Key is not well-formed" + "\r\n")

// OTHER
#define PINGMACRO(sender_nick, dest_nick, token) (sender_nick + "!localhost PRIVMSG " + dest_nick + " : PING " + token + " \r\n")
#define PONGMACRO(receiver_nick, sender_nick, token) (receiver_nick + "!localhost NOTICE " + sender_nick + " : PING " + token + "\r\n")

#define userID(nickname, username) (":" + nickname + "!" + username + "@localhost")

#define infos(nb_client, nb_channel) ("There is " + nb_client + " clients and " + nb_channel + " launched" + "\n")

// BOT
#define _bot_name "Botimus_Maximus"

#define HELP_BOX() (\
"       Voici quelques commandes utiles :        \n" \
"┌──────────────────────────────────────────────┐\n" \
"│   \x1b[33mNICK\x1b[0m <new_nickname> : permet de changer de │\n" \
"│ changer de blaze si tu veux en avoir un      │\n" \
"│ aussi cool que Botimus_Maximus.              │\n" \
"└──────────────────────────────────────────────┘\n" \
"┌──────────────────────────────────────────────┐\n" \
"│   \x1b[34mJOIN\x1b[0m <#channel_name> : permet de joindre   │\n" \
"│ un channel et parler avec des petits geeks.  │\n" \
"└──────────────────────────────────────────────┘\n" \
"┌──────────────────────────────────────────────┐\n" \
"│   \x1b[35mPRIVMSG\x1b[0m <receiver_name> <msg> : envoi un   │\n" \
"│ message privé a ton amoureux.                │\n" \
"└──────────────────────────────────────────────┘\n" \
"┌──────────────────────────────────────────────┐\n" \
"│   \x1b[31mQUIT\x1b[0m : te rends raciste, leak tes données  │\n" \
"│ bancaire, me fait pleurer et quitte le       │\n" \
"│ serveur.                                     │\n" \
"└──────────────────────────────────────────────┘\r\n")


//FUNCTIONS
size_t	replyClient(std::string Macros, int socket);

#endif