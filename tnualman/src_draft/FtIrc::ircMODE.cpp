/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc::ircMODE.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:04:23 by tnualman          #+#    #+#             */
/*   Updated: 2025/02/01 16:04:57 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h> 

#include "FtIrc.hpp"

bool	isnumber(const char *num)
{
	size_t	i = 0;

	while (num[i] != '\0')
	{
		if (num[i] < '0' || num[i] > '9')
		{
			return false;
		}
		i++;
	}
	return true;
}

ssize_t	stoi(std::string str)
{
	std::stringstream	ss;
	ssize_t				num = 0;

	// conver string to int
	ss << str;
	ss >> num;

	return num;
}

std::string	ltoa(long num)
{
	std::ostringstream oss;

	oss << num;
	return oss.str();
}

std::string const & set_of_char_to_string(std::set<char> const & set)
{
	std::string str = "";
	for (std::set<char>::iterator it = set.begin(); it != set.end(); it++)
	{
		str.push_back(*it);
	}
	return (str);
}

FtIrc::t_replyBatch FtIrc::ircMODE(FtIrc * const obj, Message const & message, Client * const sender)
{
	std::vector<std::string> const &	params = message.getParams();

	if (params.size() == 0 || params.at(0).empty())
	{
		return (obj->err_NeedMoreParams(message, sender));
	}

	if (params.at(0).at(0) == '#')
	{
		return (obj->ircMODE_channel(message, sender));
	}
	else
	{
		return (obj->ircMODE_user(message, sender));
	}
}

FtIrc::t_replyBatch FtIrc::ircMODE_channel(Message const & message, Client * const sender)
{
	std::vector<std::string> const &	params = message.getParams();
	std::string const 					channel_name = params.at(0).substr(1, std::string::npos);
	Channel * const 					channel = getChannelByName(channel_name);
	
	Message								reply_msg;
	t_reply								reply_sender;
	t_reply								reply_members;
	t_replyBatch						batch;

	reply_msg.setSource(_serverName);
	reply_sender.first = sender;
	
	if (channel == NULL)
	{
		reply_msg.setCommand(ERR_NOSUCHCHANNEL);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam("#" + channel_name);
		reply_msg.pushParam("No such channel");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
	}

	// Reply back with the channel's mode status. Need to test this more.
	if (params.size() == 1)
	{	
		std::string	modestr = set_of_char_to_string(channel->getModes());
		
		reply_msg.setCommand(RPL_CHANNELMODEIS);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam("#" + channel_name);
		reply_msg.pushParam(modestr);
		// <mode arguments> // Only deal with MODE_USERLIMET ('l'); MODE_CHANNELKEY ('k')'s argument (the password) is secret.
		if (modestr.find(MODE_USERLIMIT) != std::string::npos)
		{
			reply_msg.pushParam(ltoa(channel->getUserCountLimit()));
		}
		reply_sender.second.push(reply_msg);

		reply_msg.resetParams();
		reply_msg.setCommand(RPL_CREATIONTIME);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam("#" + channel_name);
		reply_msg.pushParam(ltoa(channel->getTimeCreated()));
		reply_sender.second.push(reply_msg);

		batch.push_back(reply_sender);
		return (batch);
	}

	// ERR_NOTONCHANNEL
	if (!(channel->hasThisClient(sender)))
	{
		reply_msg.setCommand(ERR_NOTONCHANNEL);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam("#" + channel_name);
		reply_msg.pushParam("You're not on that channel");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
	}
	
	// ERR_CHANOPRIVSNEEDED
	if (!(channel->hasThisClientMembershipMode(sender, MODE_OPERATOR)))
	{
		reply_msg.setCommand(ERR_CHANOPRIVSNEEDED);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam("#" + channel_name);
		reply_msg.pushParam("You're not channel operator");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
	}
	
	std::string	modestr = params.at(1);
	
	// ERR_UNKNOWNERROR (bad modestring)
	if (modestr.size() < 2 || (modestr.at(0) != '-' && modestr.at(0) != '+'))
	{
		reply_msg.setCommand(ERR_UNKNOWNERROR);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam("MODE");
		reply_msg.pushParam("modestring is invalid");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
	}

	char							sign = modestr.at(0);
	Channel::t_userMap 				userMap = channel->getUserMap();
	Channel::t_userMap::iterator	it_umap;
	Channel::t_userMap::iterator	it_umap_end;

	int								mode_arg_idx = 1;

	for (std::string::iterator it = modestr.begin() + 1; it != modestr.end(); it++)
	{
		if (*it == '-' || *it == '+')
		{
			sign = *it;
			continue ;
		}

		reply_msg.setCommand("MODE");
		
		it_umap = userMap.begin();
		it_umap_end = userMap.end();

		switch (*it)
		{
			case (MODE_INVITEONLY):
			{
				if (channel->hasThisMode(MODE_INVITEONLY) && sign == '-')
				{
					channel->removeMode(MODE_INVITEONLY);
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("-i");
					reply_members.second.push(reply_msg);
					for (it_umap; it_umap != it_umap_end; it_umap++)
					{
						reply_members.first = it_umap->first;
						batch.push_back(reply_members);
					}
					continue ;
				}
				
				if (!(channel->hasThisMode(MODE_INVITEONLY)) && sign == '+')
				{
					channel->addMode(MODE_INVITEONLY);
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("+i");
					reply_members.second.push(reply_msg);
					for (it_umap; it_umap != it_umap_end; it_umap++)
					{
						reply_members.first = it_umap->first;
						batch.push_back(reply_members);
					}
					continue ;
				}

			} break ;

			case (MODE_CHANNELKEY):
			{
				if (channel->hasThisMode(MODE_CHANNELKEY) && sign == '-')
				{
					channel->setPassword("");
					channel->removeMode(MODE_CHANNELKEY);
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("-k");
					reply_members.second.push(reply_msg);
					for (it_umap; it_umap != it_umap_end; it_umap++)
					{
						reply_members.first = it_umap->first;
						batch.push_back(reply_members);
					}
					continue ;
				}

				if (sign == '+')
				{
					++mode_arg_idx;
					
					// No parameter
					if (params.size() <= mode_arg_idx || params.at(mode_arg_idx).empty())
					{
						reply_msg.setCommand(ERR_INVALIDMODEPARAM);
						reply_msg.pushParam(sender->getNickname());
						reply_msg.pushParam("#" + channel_name);
						reply_msg.pushParam("k");
						reply_msg.pushParam("*");
						reply_msg.pushParam("You must specify a parameter for the key mode.");
						reply_sender.second.push(reply_msg);
						continue ;
					}
					
					channel->addMode(MODE_CHANNELKEY);
					channel->setPassword(params.at(2));
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("+k");
					reply_members.second.push(reply_msg);
					for (it_umap; it_umap != it_umap_end; it_umap++)
					{
						reply_members.first = it_umap->first;
						batch.push_back(reply_members);
					}
					continue ;
				}
			
			} break ;

			case (MODE_USERLIMIT):
			{
				if (channel->hasThisMode(MODE_USERLIMIT) && sign == '-')
				{
					channel->removeMode(MODE_USERLIMIT);
					channel->setUserCountLimit(0);
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("-l");
					reply_members.second.push(reply_msg);
					for (it_umap; it_umap != it_umap_end; it_umap++)
					{
						reply_members.first = it_umap->first;
						batch.push_back(reply_members);
					}
					continue ;
				}

				if (!(channel->hasThisMode(MODE_USERLIMIT)) && sign == '+')
				{
					++mode_arg_idx;
					
					// No parameter.
					if (params.size() <= mode_arg_idx || params.at(mode_arg_idx).empty())
					{
						reply_msg.setCommand(ERR_INVALIDMODEPARAM);
						reply_msg.pushParam(sender->getNickname());
						reply_msg.pushParam("#" + channel_name);
						reply_msg.pushParam("l");
						reply_msg.pushParam("*");
						reply_msg.pushParam("You must specify a parameter for the limit mode.");
						reply_sender.second.push(reply_msg);
						continue ;
					}

					std::string limit_arg = params.at(mode_arg_idx);

					// Invalid limit mode parameter.
					if (!isnumber(limit_arg.c_str()) || stoi(limit_arg) < 1)
					{
						reply_msg.setCommand(ERR_INVALIDMODEPARAM);
						reply_msg.pushParam(sender->getNickname());
						reply_msg.pushParam("#" + channel_name);
						reply_msg.pushParam("l");
						reply_msg.pushParam(limit_arg);
						reply_msg.pushParam("Invalid limit mode parameter.");
						reply_sender.second.push(reply_msg);
						continue ;
					}

					channel->setUserCountLimit(stoi(limit_arg));
					channel->addMode(MODE_USERLIMIT);
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("+l");
					reply_msg.pushParam(limit_arg);
					reply_members.second.push(reply_msg);
					for (it_umap; it_umap != it_umap_end; it_umap++)
					{
						reply_members.first = it_umap->first;
						batch.push_back(reply_members);
					}
					continue ;
				}

			} break ;

			case (MODE_OPERATOR):
			{
				++mode_arg_idx;
					
				// No parameter.
				if (params.size() <= mode_arg_idx || params.at(mode_arg_idx).empty())
				{
					reply_msg.setCommand(ERR_INVALIDMODEPARAM);
					reply_msg.pushParam(sender->getNickname());
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("o");
					reply_msg.pushParam("*");
					reply_msg.pushParam("You must specify a parameter for the op mode.");
					reply_sender.second.push(reply_msg);
					continue ;
				}
				
				std::string	target_str = params.at(mode_arg_idx);
				Client *	target = getClientByNickname(params.at(mode_arg_idx));

				// ERR_NOSUCHNICK
				if (!target)
				{
					reply_msg.setCommand(ERR_NOSUCHNICK);
					reply_msg.pushParam(sender->getNickname());
					reply_msg.pushParam(target_str);
					reply_msg.pushParam("No such nickname.");
					reply_sender.second.push(reply_msg);
					continue ;
				}

				// ERR_USERNOTINCHANNEL
				if (!(channel->hasThisClient(target)))
				{
					reply_msg.setCommand(ERR_USERNOTINCHANNEL);
					reply_msg.pushParam(sender->getNickname());
					reply_msg.pushParam(target_str);
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("They aren't on that channel.");
					reply_sender.second.push(reply_msg);
					continue ;
				}

				if (channel->hasThisClientMembershipMode(target, MODE_OPERATOR) && sign == '-')
				{
					channel->removeThisClientMembershipMode(target, MODE_OPERATOR);
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("-o");
					reply_msg.pushParam(target_str);
					reply_members.second.push(reply_msg);
					for (it_umap; it_umap != it_umap_end; it_umap++)
					{
						reply_members.first = it_umap->first;
						batch.push_back(reply_members);
					}
					continue ;
				}

				if (!(channel->hasThisClientMembershipMode(target, MODE_OPERATOR)) && sign == '+')
				{
					channel->addThisClientMembershipMode(target, MODE_OPERATOR);
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("+o");
					reply_msg.pushParam(target_str);
					reply_members.second.push(reply_msg);
					for (it_umap; it_umap != it_umap_end; it_umap++)
					{
						reply_members.first = it_umap->first;
						batch.push_back(reply_members);
					}
					continue ;
				}

			} break ;
			
			case (MODE_PROTECTTOPIC):
			{
				if (channel->hasThisMode(MODE_PROTECTTOPIC) && sign == '-')
				{
					channel->removeMode(MODE_PROTECTTOPIC);
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("-t");
					reply_members.second.push(reply_msg);
					for (it_umap; it_umap != it_umap_end; it_umap++)
					{
						reply_members.first = it_umap->first;
						batch.push_back(reply_members);
					}
					continue ;
				}
				
				if (!(channel->hasThisMode(MODE_PROTECTTOPIC)) && sign == '+')
				{
					channel->addMode(MODE_PROTECTTOPIC);
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("+t");
					reply_members.second.push(reply_msg);
					for (it_umap; it_umap != it_umap_end; it_umap++)
					{
						reply_members.first = it_umap->first;
						batch.push_back(reply_members);
					}
					continue ;
				}

			} break ;

			default:
			{
				reply_msg.setCommand(ERR_UNKNOWNMODE);
				reply_msg.pushParam(sender->getNickname());
				reply_msg.pushParam(std::string(1, *it));
				reply_msg.pushParam("is not a recognized channel mode character");
				reply_sender.second.push(reply_msg);
			}
		}
	}
	
	batch.push_back(reply_sender);
	return (batch);
}

FtIrc::t_replyBatch	FtIrc::ircMODE_user(Message const & message, Client * const sender)
{
	std::vector<std::string> const &	params = message.getParams();
	std::string const 					target_name = params.at(0);
	Client * const 						target = getClientByNickname(target_name);
	
	Message								reply_msg;
	t_reply								reply_sender;
	t_reply								reply_members;
	t_replyBatch						batch;

	if (!target)
	{
		reply_msg.setCommand(ERR_NOSUCHNICK);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam(target_name);
		reply_msg.pushParam("No such nickname.");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
	}

	std::string							modestr = params.at(1);
	char								sign = modestr.at(0);
	std::map<int, Client*>::iterator	it_cmap;
	std::map<int, Client*>::iterator	it_cmap_end;

	for (std::string::iterator it = modestr.begin() + 1; it != modestr.end(); it++)
	{
		if (*it == '-' || *it == '+')
		{
			sign = *it;
			continue ;
		}

		reply_msg.setCommand("MODE");
		
		it_cmap = _clientMapByFd.begin();
		it_cmap_end = _clientMapByFd.end();

		switch (*it)
		{
			case (MODE_OPERATOR):
			{
				if (!sender->hasMode(MODE_OPERATOR))
				{		
					reply_msg.setCommand(ERR_NOPRIVILEGES);
					reply_msg.pushParam(sender->getNickname());
					reply_msg.pushParam("You're not an IRC operator.");
					reply_sender.second.push(reply_msg);
					continue ;
				}

				if (target->hasMode(MODE_OPERATOR) && sign == '-')
				{
					target->removeMode(MODE_OPERATOR);
					reply_msg.pushParam(target_name);
					reply_msg.pushParam("-o");
					reply_members.second.push(reply_msg);
					for (it_cmap; it_cmap != it_cmap_end; it_cmap++)
					{
						reply_members.first = it_cmap->second;
						batch.push_back(reply_members);
					}
					continue ;
				}

				if (!(target->hasMode(MODE_OPERATOR)) && sign == '+')
				{
					target->addMode(MODE_OPERATOR);
					reply_msg.pushParam(target_name);
					reply_msg.pushParam("+o");
					reply_members.second.push(reply_msg);
					for (it_cmap; it_cmap != it_cmap_end; it_cmap++)
					{
						reply_members.first = it_cmap->second;
						batch.push_back(reply_members);
					}
					continue ;
				}

			} break ;

			default:
			{
				reply_msg.setCommand(ERR_UNKNOWNMODE);
				reply_msg.pushParam(sender->getNickname());
				reply_msg.pushParam(std::string(1, *it));
				reply_msg.pushParam("is not a recognized user mode character.");
				reply_sender.second.push(reply_msg);
			}
		}
	}
	
	batch.push_back(reply_sender);
	return (batch);
}
