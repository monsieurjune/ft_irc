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
NETWORK_SRC		:= listen_init.cpp \
					irc_recv.cpp
NETWORK_SRCS	:= $(addprefix $(NETWORK_SRC_DIR), $(NETWORK_SRC))

# Exception Source
EXCEPT_SRC_DIR	:= $(SRC_DIR)/exception/
EXCEPT_SRC		:= CstdException.cpp \
					IrcDisconnectedException.cpp \
					IrcInvalidPacketException.cpp \
					IrcUnknownCmdException.cpp
EXCEPT_SRCS		:= $(addprefix $(EXCEPT_SRC_DIR), $(EXCEPT_SRC))

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
UTILS_SRC		:= log.cpp
UTILS_SRCS		:= $(addprefix $(UTILS_SRC_DIR), $(UTILS_SRC))

# Main Source
MAIN_SRC	:= main.cpp
MAIN_DIR	:= $(SRC_DIR)/
MAIN_SRCS	:= $(addprefix $(MAIN_DIR), $(MAIN_SRC))

# Objects & Dependencies File
SRCS	:= $(MAIN_SRCS) $(EXCEPT_SRCS) $(STD_SRCS) $(NETWORK_SRCS) $(UTILS_SRCS)
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