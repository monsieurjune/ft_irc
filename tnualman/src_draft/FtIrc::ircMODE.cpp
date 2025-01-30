/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc::ircMODE.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:04:23 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/30 19:48:14 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h> 

#include "FtIrc.hpp"

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

	// ERR_CHANOPRIVSNEEDED // Assuming that all channel mode changing require operator privilege.
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

	char sign = modestr.at(0);
	std::string working_modes = "iklot";

	Channel::t_userMap 				userMap = channel->getUserMap();
	Channel::t_userMap::iterator	it_umap;
	Channel::t_userMap::iterator	it_umap_end;

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
				}
				if (sign == '+')
				{
					// If new password not given in params.at(2), ignore this 'k' mode!  
					if (params.size() < 3 || params.at(2).empty())
					{
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
				}
				if (!(channel->hasThisMode(MODE_USERLIMIT)) && sign == '+')
				{
					int	limit;
					
					// If new user limit not given in params.at(2) or params.at(3), ignore this 'l' mode!  
					if (params.size() < 3 || (params.size() == 3 && modestr.find(MODE_CHANNELKEY) != std::string::npos) 
						|| (modestr.find(MODE_CHANNELKEY) == std::string::npos && params.at(2).empty())
						|| (params.size() > 3 && modestr.find(MODE_CHANNELKEY) != std::string::npos && params.at(3).empty())) 
					{
						continue ;
					}
					
					if (modestr.find(MODE_CHANNELKEY) == std::string::npos)
					{
						limit = stoi(params.at(2));
					}
					else
					{
						limit = stoi(params.at(3));
					}

					// If given user count limit is not valid (non-positive).
					if (limit < 1)
					{
						reply_msg.setCommand(ERR_INVALIDMODEPARAM);
						reply_msg.pushParam(sender->getNickname());
						reply_msg.pushParam(channel_name);
						reply_msg.pushParam(std::string(1, MODE_USERLIMIT));
						reply_msg.pushParam(ltoa(limit));
						reply_msg.pushParam("Invalid user count limit (non-positive)");
						reply_sender.second.push(reply_msg);
						batch.push_back(reply_sender);
						continue;
					}

					channel->setUserCountLimit(limit);
					channel->addMode(MODE_USERLIMIT);
					reply_msg.pushParam("#" + channel_name);
					reply_msg.pushParam("+l");
					reply_msg.pushParam(ltoa(limit));
					reply_members.second.push(reply_msg);
					for (it_umap; it_umap != it_umap_end; it_umap++)
					{
						reply_members.first = it_umap->first;
						batch.push_back(reply_members);
					}
				}
			} break ;
			case (MODE_OPERATOR):
				// Check for user not in case in _userMap, then check if user is already an op, then change _userMembershipMode
				break ;
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
