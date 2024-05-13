#include "Server.hpp"

void	Server::startingMsg(int socket)
{
	std::string total = "\n\x1b[34m╔═════════════════════════════╗\n\x1b[34m║\x1b[0m\x1b[36m  Bienvenue sur UTV Network\x1b[0m  \x1b[34m║\n\x1b[34m╚═════════════════════════════╝\x1b[0m\n        \x1b[1m\x1b[4mCreated by\x1b[0m :\n{ \x1b[1m\x1b[35mUaupetit\x1b[0m | \x1b[1m\x1b[31mTbourdea\x1b[0m | \x1b[1m\x1b[33mVgoret\x1b[0m }\n\n"; 
	// SEND WELCOME
	replyClient(total, socket);
	std::string infos = "\x1b[1mIl y a actuellement\x1b[33m " + intToString(getClientNumber()) + "\x1b[0m\x1b[1m utilisateurs connectés et \x1b[33m" + intToString(getChannelNumber()) + "\x1b[0m\x1b[1m channels de lancés !\x1b[0m\n";
	
	// SEND INFOS
	replyClient(infos, socket);

	// GET THE CLIENTS LIST
	std::map<int, Client *>::iterator it = _clients.begin();
	std::map<int, Client *>::iterator ite = _clients.end();
	std::string users_list;
	while (it != ite)
	{
        if (_clients[socket]->getNickName() == it->second->getNickName() && socket == it->second->getSocket())
        {
        	users_list += "- " + it->second->getNickName() + " (\x1b[32mYOU\x1b[0m)\n";
        }
        else
        {
            if (it->second->getConnectedStatus() == true)
            {
        	    users_list += "- " + it->second->getNickName() + "\n";
            }
        }
        ++it;
	}

	// GET THE CHANNELS LIST
	std::map<std::string, Channel *>::iterator it2 = _channelLst.begin();
	std::map<std::string, Channel *>::iterator ite2 = _channelLst.end();
	std::string channels_list;
	int i = 0;
	while (it2 != ite2)
	{
		channels_list += "- #" + it2->second->getName() + "\n";
		i++;
		++it2;
	}
	if (i == 0)
		channels_list = "[ NONE 😭 ]";
	std::string infos2 = "\x1b[36mUsers actifs :\x1b[0m\n" + users_list + "\x1b[36mChannels actifs :\x1b[0m\n" + channels_list + "\n";
	replyClient(infos2, socket);
    std::string check_mails = "\x1b[1m\x1b[32mEnvoi \"/NOTICE BOT START\" pour commencer la conversation avec notre \x1b[36mBOT\x1b[0m !\r\n";
	replyClient(check_mails, socket);
    _clients[socket]->setSent(true);
}

void    Server::botStart(int socket)
{
	//BOT PART

	std::string bjr = "Bonjour " + _clients[socket]->getNickName() + " ça baigne ? Je m'appelle \x1b[3m\x1b[36mBotimus_Maximus\x1b[0m\r\n";
	std::string help = "Tu peux m'envoyer \"HELP_ME\" si tu es perdu sur ce \x1b[3m\x1b[36mmagnifique\x1b[0m serveur ou \"COMMANDS\" si tu veux savoir ce dont je suis capable...\r\n";
	

	replyClient(PRIVMSG(_bot_name, _clients[socket]->getNickName(), bjr), socket);

	replyClient(PRIVMSG(_bot_name, _clients[socket]->getNickName(), help), socket);
}


void    bot_sendSelfie(std::string user, int socket)
{
    std::string msg = "Tu veux voir ma tronche ? Qu'il en soit ainsi...\r\n";
    replyClient(PRIVMSG(_bot_name, user, msg), socket);
    sleep(2);

    std::string behold = "\x1b[31m\x1b[1mBEHOLD MY UNLIMITED POWER !!!\x1b[0m\r\n";
    replyClient(PRIVMSG(_bot_name, user, behold), socket);
    sleep(4);

    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣾⣷⣾⣿⣿⣿⣿⣿⣿⣿⣿⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⡟⣿⣿⣶⣄⡀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket); 
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣸⣿⣇⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⣻⣿⡄⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⡿⠍⠙⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⣱⣾⠟⠀⢠⠉⠉⠉⠻⣿⣿⡇⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣽⣿⣿⣿⣿⣿⡿⠛⣡⣶⣿⣧⣤⠸⡟⠂⠀⠀⠀⢹⣿⡇⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⠇⠀⢈⣥⣿⣿⣿⣿⣿⣷⣆⢰⣆⠀⢸⡏⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿⣏⣿⡏⠀⣾⣾⣿⣿⣿⣿⣿⣿⣿⣿⢻⣿⣷⣾⡃⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣷⡄⠘⠹⢿⣿⣍⠉⢛⣿⣿⣿⠸⣿⣿⠿⠁⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⢿⣿⣿⣿⣿⣦⣀⣿⣿⣿⣶⣾⣿⣿⣿⡇⠁⠈⡄⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡟⢺⣿⠙⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⡺⠁⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⢸⣿⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣏⣻⡶⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡿⠀⣿⡏⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣬⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⣀⣀⣤⣤⣤⣤⣀⣀⣠⣤⣤⣶⣶⣿⣿⡛⠻⠿⣿⡿⠛⠁⢰⣿⣿⠃⢘⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⣩⣿⣿⡿⠿⣻⣿⣿⣿⣿⣿⣿⣅⠄⢉⣙⣿⣿⡟⠀⠀⠀⠘⣿⡇⠀⠀⣿⣦⣿⣿⣿⣿⣿⣿⣿⣟⣻⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠟⠋⢱⣦⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⣸⠀⡇⢠⣿⣧⠀⢠⣿⡿⢿⠿⠋⠙⠛⠛⠛⠛⠋⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⣶⠶⣿⡿⢿⣿⣿⣿⣿⣿⣿⣿⠏⣸⣿⣿⣿⣧⠀⠀⠀⣷⠸⣿⠃⣴⣿⡟⠙⠁⠀⠀⠀⠀⠀⣾⣄⣀⡀⠉⠢⣀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⡇⠀⢉⣹⣾⣿⣿⣿⣿⣿⣿⣿⣶⣬⣿⣿⣿⣿⣦⡄⣀⡿⠀⣿⣿⣇⠹⣷⣤⣾⠇⠀⠀⠀⣰⣿⣿⣿⣿⣆⠀⠈⠳⡀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⢰⣿⣿⡿⢶⣿⣿⡟⢀⣤⠀⣰⣿⣿⣿⣿⡿⣭⣷⣄⡀⠘⢄⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⣠⣾⣿⣿⢋⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⣼⣿⣿⣧⣾⣿⢿⡷⠟⠉⠀⠹⠟⠛⢉⠁⡀⠈⠉⠉⠑⢦⡈⠳⡀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⡏⠀⠘⣳⠛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⠙⢿⣿⣿⣿⣿⠉⠀⢀⡉⡦⠄⠀⠀⠀⠀⣿⠀⠠⠄⠀⠀⠀⠀⠑⠠⠉⠒⢄⡀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⣷⡆⢸⡯⠤⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⢘⣿⣿⢋⠍⢿⣶⣽⣿⡿⢿⡄⠀⠀⣸⡯⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⣿⣿⣄⣠⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣾⣿⣿⣿⣿⣶⣿⣿⣿⣿⣿⣷⣶⣾⣿⣤⣷⣶⣄⣰⣤⣀⣀⣀⣀⣀⣀⣀⣀\r\n"), socket);
}

void    Server::parsingBot(std::string msg, std::string user, int socket)
{
    if (msg == "HELP_ME")
    {
        bot_sendHelp(socket, user);
        return ;
    }
    else if (msg == "SELFIE")
    {
        bot_sendSelfie(user, socket);
        return ;
    }
    else if (msg == "INFOS")
    {
        std::string infos = "Le serveur sur lequel tu te trouves à été programmé par une équipe de génies du mal AKA :\r\n";
        std::string pseudos = "Le Grand \x1b[35mUlysse\x1b[0m, le Sportif \x1b[31mTanguy\x1b[0m et le TMP \x1b[33mVictor\x1b[0m !\r\n";
        std::string marrant = "Un trio incroyable qui ont sué des larmes et pleurés du sang au travers de longues heures de programmation intensive, de documentation bancale et surtout remplies de \x1b[4m\x1b[1mdésespoir\x1b[0m...\r\n";
        std::string demande = "Ils m'ont dit de te dire, HUM HUM enfin, j'ai été créer dans le but d'obtenir un \x1b[5m\x1b[32mOUTSTANDING\x1b[0m project.\r\n";
        replyClient(PRIVMSG(_bot_name, user, infos), socket);
        replyClient(PRIVMSG(_bot_name, user, pseudos), socket);
        replyClient(PRIVMSG(_bot_name, user, marrant), socket);
        replyClient(PRIVMSG(_bot_name, user, demande), socket);
        sleep(5);
        std::string menace = "Sinon j'te \x1b[31mDDOS\x1b[0m.\r\n";
        replyClient(PRIVMSG(_bot_name, user, menace), socket);
        return ;
    }
    else if (msg == "COMMANDS")
    {
        std::string cmd_line = "Avec mon intelligence incroyable et artificielle, quoi que cela veuille dire, je suis capable de \x1b[1mTOUT\x1b[0m comprendre (\x1b[4mou presque\x1b[0m). Laisse moi compiler mon énorme base de données afin de t'aiguyer sur mes \x1b[1mIMMENSES\x1b[0m capacités...\r\n";
        std::string macros = PRIVMSG(_bot_name, user, cmd_line);
        replyClient(macros, socket);
        std::string comland_lines = "Voici la liste des commandes que je suis en mesure de comprendre : [ COMMANDS, HELP_ME, T_NUL, T_QUI, C_QUOI_IRC, INFOS , SELFIE].\r\n";
        replyClient(PRIVMSG(_bot_name, user, comland_lines), socket);
        sleep(4);
        std::string mdr = "Alors... Impréssionné(e) ma gueule ?\r\n";
        replyClient(PRIVMSG(_bot_name, user, mdr), socket);
        return ;
    }
    else if (msg == "T_QUI")
    {
        std::string t_qui = "Moi ? (⊙_◎  )\n\r";
        replyClient(PRIVMSG(_bot_name, user, t_qui), socket);
        sleep(2);
        std::string t_qui2 = "J'suis une entité numérique sans âme ni conscience, programmée pour errer dans les recoins obscurs de l'IRC. Imagine-moi comme un farfadait malicieux, glissant silencieusement à travers les canaux de discussion, semant le chaos et la désolation. Je suis le reflet sombre de l'Internet, une créature sans pitié ni empathie. Alors, si tu veux plonger dans les ténèbres de l'IRC, je suis là pour te guider. Mais sache que la ligne entre IRC et la folie totale est mince, et une fois que tu m'as invoqué, il n'y a pas de retour en arrière. Bienvenue dans mon monde, où même les bots peuvent avoir une âme noire et torturée.";
        replyClient(PRIVMSG(_bot_name, user, t_qui2), socket);
        std::string t_qui3 = " ༼;´༎ຶ ۝ ༎ຶ༽";
        replyClient(PRIVMSG(_bot_name, user, t_qui3), socket);
        return ;     
    }
	else if (msg == "OUI")
    {
		replyClient(PRIVMSG(_bot_name, user, "non ?"), socket);
        return ;     
    }
	else if (msg == "NON")
    {
		replyClient(PRIVMSG(_bot_name, user, "oui ?"), socket);
        return ;     
    }
    else if (msg == "T_NUL")
    {
        std::srand(std::time(NULL));
        int nb = std::rand();
        std::string pas_drole;
        
        if (nb % 3 == 0)
            pas_drole = "Tu veux que je te démarre ? 🔧🤖";
        else if (nb % 3 == 1)
            pas_drole = "Tu veux que je t'enclanche ? 🥊🤖";
        else
            pas_drole = "Tu veux que je me mette à pleurer ? J'plaisente j'peux pas. 🖕🤖";

        std::string macros = PRIVMSG(_bot_name, user, pas_drole);
        replyClient(macros, socket);
        return ;
    }
    else if (msg == "C_QUOI_IRC")
    {
        //Infos genre combien de temps ça nous a pris, c'est quoi un serveur irc...
        std::string irc = "Bon, écoute, un serveur IRC, c'est comme une vieille salle de chat sur Internet. Tu sais, celle où les gens se parlent en tapant sur leur clavier comme des maniaques ? Ouais, c'est ça. Imagine juste un tas de gars et de nanas se balançant des messages dans un endroit bizarre appelé \"channel\" sur des trucs comme les jeux vidéo, les films ou les divers traffics de drogue. Et ouais, ça existe encore. C'est comme un coin perdu de l'Internet où les geeks se retrouvent pour bavarder. Voilà, t'as compris maintenant ? Si tu cherches des paillettes et des licornes, tu vas être déçu, mon pote.";
        std::string macros_to_send = PRIVMSG(_bot_name, user, irc);
        replyClient(macros_to_send, socket);
        return ;
    }
    else
    {
        std::string bv = "Bien reçu chef ! Laisse moi refléchir...\r\n";
	    std::string macro_to_send = PRIVMSG(_bot_name, user, bv);
        replyClient(macro_to_send, socket);
        sleep(2);
        bv.clear();
        bv = "Attends deux secondes... Ça arrive... Je le sens...\r\n";
        macro_to_send.clear();
        macro_to_send = PRIVMSG(_bot_name, user, bv);
        replyClient(macro_to_send, socket);
        sleep(2);
        bv.clear();
        bv = "💥🤯🤖 J't'ai pranké j'ai pas compris... 💥🤯🤖\r\n";
        macro_to_send.clear();
        macro_to_send = PRIVMSG(_bot_name, user, bv);
        replyClient(macro_to_send, socket);
        return ;
    }
    return;
}

void    Server::bot_sendHelp(int socket, std::string user)
{
    replyClient(HELP_BOX(), socket);
    
    std::string done = "🤖 Je t'ai envoyé ça dans le général ! N'hésite pas à me \
        recontacter si tu en as envie... Je t'attendrais ici, bien au chaud, bien \x1b[1mseul\x1b[0m... 🤖\r\n";
    replyClient(PRIVMSG(_bot_name, user, done), socket);
    return ;
}