NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -g

DEPS = ${OBJS:.o=.d}
INC = Server.hpp Client.hpp Communication.hpp Channel.hpp
SRCS = main.cpp Channel.cpp  Server.cpp Client.cpp Communication.cpp \
	PRIVMSG.cpp PASS.cpp NICK.cpp PING.cpp CAPLS.cpp JOIN.cpp USER.cpp \
	QUIT.cpp INVITE.cpp KICK.cpp TOPIC.cpp MODE.cpp \
	BOT.cpp modeOptions.cpp PART.cpp\
		
OBJS_PATH = ./objs
OBJS_NAMES = $(SRCS:.cpp=.o)
OBJS = $(addprefix $(OBJS_PATH)/, $(OBJS_NAMES))

TOTAL_FILES := $(words $(SRCS))
CURRENT_FILE = 0
PERCENTAGE = 0

GREEN = \033[32m
RED = \033[31m
YELLOW = \033[33m
BOLD = \e[1m
UNBOLD = \e[0m
RESET = \033[0m

define update_progress
	$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))
	$(eval PERCENTAGE=$(shell echo $$(($(CURRENT_FILE)*100/$(TOTAL_FILES)))))
	@printf "$(GREEN)\rCompiling [%-20s] %d%% $(RESET)" "####################" "$(PERCENTAGE)"
endef

all : $(NAME)

$(NAME): $(OBJS) $(INC)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "\n $(BOLD)$(NAME)$(UNBOLD) is built  🪄$(RESET)"

$(OBJS_PATH)/%.o: %.cpp
	@mkdir -p $(OBJS_PATH)
	$(call update_progress)
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean :
	@rm -rf $(OBJS_PATH)
	@rm -rf $(DEPS)
	@echo "$(YELLOW)cleaning  🧽$(RESET)"

fclean : clean
	@rm -rf $(NAME)
	@echo "$(RED)full clean  🧹$(RESET)"

re: fclean all

-include $(DEPS)

.PHONY:	all clean fclean re bonus 