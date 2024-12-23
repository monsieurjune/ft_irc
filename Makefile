# Project Name & Properties
NAME	:= ./ircserv
SHELL	:= /usr/bin/env sh
RM		:= rm
RMDIR	:= rm -r
MKDIR	:= mkdir -p

# Complier Propeties
CXX			:= c++
CXX_WFLAG	:= -Wall -Wextra -std=c++98
CXX_MFLAG	:= -MP -MD
CXX_OFLAG	:= -O0
CXXFLAG		:= $(CXX_WFLAG) $(CXX_MFLAG) $(CXX_OFLAG)

# Directories
BUILD_DIR	:= ./build
SRC_DIR		:= ./src
INCLUDE_DIR	:= ./include

# Network Source
NETWORK_SRC_DIR	:= $(SRC_DIR)/network/
NETWORK_SRC		:= listen_init.cpp
NETWORK_SRCS	:= $(addprefix $(NETWORK_SRC_DIR), $(NETWORK_SRC))

# Utils Source
UTILS_SRC_DIR	:= $(SRC_DIR)/utils/
UTILS_SRC		:= stoi.cpp \
					log.cpp \
					is_number.cpp \
					memset.cpp
UTILS_SRCS		:= $(addprefix $(UTILS_SRC_DIR), $(UTILS_SRC))

# Main Source
MAIN_SRC	:= main.cpp
MAIN_DIR	:= $(SRC_DIR)/
MAIN_SRCS	:= $(addprefix $(MAIN_DIR), $(MAIN_SRC))

# Objects & Dependencies File
SRCS	:= $(MAIN_SRCS) $(NETWORK_SRCS) $(UTILS_SRCS)
OBJS	:= $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS	:= $(OBJS:.o=.d)

# Main Rule
.PHONY: all clean fclean re

all:    $(NAME)

$(NAME):	$(OBJS)
	$(CXX) $(CXXFLAG) $(OBJS) -o $(NAME)

$(BUILD_DIR)/%.o:	%.cpp
	$(MKDIR) $(@D)
	$(CXX) $(CXXFLAG) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RMDIR) $(BUILD_DIR)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

-include $(DEPS)