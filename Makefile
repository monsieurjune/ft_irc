# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/01 14:04:58 by tponutha          #+#    #+#              #
#    Updated: 2025/02/01 16:29:09 by tponutha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project Name & Properties
NAME	:= ./ircserv
SHELL	:= /usr/bin/env sh
RM		:= rm -f
RMDIR	:= rm -r
MKDIR	:= mkdir -p

# Complier Propeties
CXX			:= c++
CXX_WFLAG	:= -Wall -Wextra -std=c++98 -g
CXX_MFLAG	:= -MP -MD
CXX_OFLAG	:= -O0
CXXFLAG		:= $(CXX_WFLAG) $(CXX_MFLAG) $(CXX_OFLAG)

# Directories
BUILD_DIR	:= ./build
SRC_DIR		:= ./src
INCLUDE_DIR	:= ./include

# Network Source
NETWORK_SRC_DIR	:= $(SRC_DIR)/network/
NETWORK_SRC		:= listen_init.cpp \
					irc_recv.cpp \
					pollin.cpp \
					pollout.cpp
NETWORK_SRCS	:= $(addprefix $(NETWORK_SRC_DIR), $(NETWORK_SRC))

# Exception Source
EXCEPT_SRC_DIR	:= $(SRC_DIR)/exception/
EXCEPT_SRC		:= CstdException.cpp \
					IrcDisconnectedException.cpp \
					IrcInvalidPacketException.cpp \
					IrcUnknownCmdException.cpp \
					IrcListenBindingException.cpp \
					IrcTooLongMsgException.cpp \
					IrcContinueException.cpp
EXCEPT_SRCS		:= $(addprefix $(EXCEPT_SRC_DIR), $(EXCEPT_SRC))

# IRC Command Source
IRC_CMD_SRC_DIR	:= $(SRC_DIR)/irc_cmd/
IRC_CMD_SRC		:= ircCAP.cpp \
					ircINVITE.cpp \
					ircJOIN.cpp \
					ircKICK.cpp \
					ircMODE.cpp \
					ircNICK.cpp \
					ircPART.cpp \
					ircPASS.cpp \
					ircPING.cpp \
					ircPONG.cpp \
					ircPRIVMSG.cpp \
					ircQUIT.cpp \
					ircTOPIC.cpp \
					ircUSER.cpp
IRC_CMD_SRCS	:= $(addprefix $(IRC_CMD_SRC_DIR), $(IRC_CMD_SRC))

# IRC Reply Source
IRC_RPL_SRC_DIR	:= $(SRC_DIR)/irc_response/
IRC_RPL_SRC		:= errAlreadyRegistered.cpp \
					errInputTooLong.cpp \
					errNeedMoreParams.cpp \
					errNotRegistered.cpp \
					errUnknownCmd.cpp
IRC_RPL_SRCS	:= $(addprefix $(IRC_RPL_SRC_DIR), $(IRC_RPL_SRC))

# FtIRC Source
FTIRC_SRC_DIR	:= $(SRC_DIR)/ft_irc/
FTIRC_SRC		:= Channel.cpp \
					Client.cpp \
					FtIrc.cpp \
					Message.cpp
FTIRC_SRCS		:= $(addprefix $(FTIRC_SRC_DIR), $(FTIRC_SRC))

# STD Source
STD_SRC_DIR		:= $(SRC_DIR)/std

#  CSTD Source
CSTD_SRC_DIR	:= $(STD_SRC_DIR)/c/
CSTD_SRC		:= memset.cpp \
					isnumber.cpp \
					strcnlen.cpp \
					strnstr.cpp
CSTD_SRCS		:= $(addprefix $(CSTD_SRC_DIR), $(CSTD_SRC))

#  CPPSTD Source
CPPSTD_SRC_DIR	:= $(STD_SRC_DIR)/cpp/
CPPSTD_SRC		:= stoi.cpp \
					itoa.cpp
CPPSTD_SRCS		:= $(addprefix $(CPPSTD_SRC_DIR), $(CPPSTD_SRC))

STD_SRCS		:= $(CSTD_SRCS) $(CPPSTD_SRCS)

# Utils Source
UTILS_SRC_DIR	:= $(SRC_DIR)/utils/
UTILS_SRC		:= logger.cpp \
					signal_init.cpp
UTILS_SRCS		:= $(addprefix $(UTILS_SRC_DIR), $(UTILS_SRC))

# Main Source
MAIN_SRC	:= main.cpp
MAIN_DIR	:= $(SRC_DIR)/
MAIN_SRCS	:= $(addprefix $(MAIN_DIR), $(MAIN_SRC))

# Objects & Dependencies File
SRCS	:= $(MAIN_SRCS) \
			$(EXCEPT_SRCS) \
			$(FTIRC_SRCS) \
			$(STD_SRCS) \
			$(NETWORK_SRCS) \
			$(UTILS_SRCS) \
			$(IRC_CMD_SRCS) \
			$(IRC_RPL_SRCS)
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