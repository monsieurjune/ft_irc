/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircMODE_channel.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:57:10 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/16 16:43:50 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"
#include "std/ft_cstd.hpp"

static std::string const set_of_char_to_string(std::set<char> const & set)
{
	std::string str = std::string();

	for (std::set<char>::iterator it = set.begin(); it != set.end(); it++)
	{
		str.push_back(*it);
	}
	return (str);
}

FtIrc::t_replyBatch FtIrc::ircMODE_channel(Message const & message, Client * const sender)
{
	std::vector<std::string> const &	params = message.getParams();
	std::string const 					channel_name = params.at(0);
	Channel * const 					channel = getChannelByName(channel_name);
	Message								reply_msg;
	t_reply								reply_sender;
	t_replyBatch						batch;

	reply_msg.setSource(_serverName);
	reply_sender.first = sender;

	if (channel == NULL)
	{
		reply_msg.setCommand(ERR_NOSUCHCHANNEL);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam(channel_name);
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
		reply_msg.pushParam(channel_name);
		reply_msg.pushParam(modestr);
		// <mode arguments> // Only deal with MODE_USERLIMET ('l'); MODE_CHANNELKEY ('k')'s argument (the password) is secret.
		if (modestr.find(MODE_USERLIMIT) != std::string::npos)
		{
			reply_msg.pushParam(ft_std::itoa(channel->getUserCountLimit()));
		}
		reply_sender.second.push(reply_msg);

		reply_msg.resetParams();
		reply_msg.setCommand(RPL_CREATIONTIME);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam(channel_name);
		reply_msg.pushParam(ft_std::itoa(channel->getTimeCreated()));
		reply_sender.second.push(reply_msg);

		batch.push_back(reply_sender);
		return (batch);
	}

	// ERR_NOTONCHANNEL
	if (!(channel->hasThisClient(sender)))
	{
		reply_msg.setCommand(ERR_NOTONCHANNEL);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam(channel_name);
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
		reply_msg.pushParam(channel_name);
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
	unsigned int					mode_arg_idx = 1;

	for (std::string::iterator it = modestr.begin() + 1; it != modestr.end(); it++)
	{
		if (*it == '-' || *it == '+')
		{
			sign = *it;
			continue ;
		}

		reply_msg.resetParams();
		reply_msg.setSource(sender->constructSource());
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
					reply_msg.pushParam(channel_name);
					reply_msg.pushParam("-i");
					pushChannelReplyAll(reply_msg, channel, batch);
					continue ;
				}
				
				if (!(channel->hasThisMode(MODE_INVITEONLY)) && sign == '+')
				{
					channel->addMode(MODE_INVITEONLY);
					reply_msg.pushParam(channel_name);
					reply_msg.pushParam("+i");
					pushChannelReplyAll(reply_msg, channel, batch);
					continue ;
				}

			} break ;

			case (MODE_CHANNELKEY):
			{
				if (channel->hasThisMode(MODE_CHANNELKEY) && sign == '-')
				{
					channel->setPassword("");
					channel->removeMode(MODE_CHANNELKEY);
					reply_msg.pushParam(channel_name);
					reply_msg.pushParam("-k");
					pushChannelReplyAll(reply_msg, channel, batch);
					continue ;
				}

				if (sign == '+')
				{
					++mode_arg_idx;

					// No parameter
					if (params.size() <= mode_arg_idx || params.at(mode_arg_idx).empty())
					{
						reply_msg.setSource(_serverName);
						reply_msg.setCommand(ERR_INVALIDMODEPARAM);
						reply_msg.pushParam(sender->getNickname());
						reply_msg.pushParam(channel_name);
						reply_msg.pushParam("k");
						reply_msg.pushParam("*");
						reply_msg.pushParam("You must specify a parameter for the key mode.");
						reply_sender.second.push(reply_msg);
						continue ;
					}
					
					channel->addMode(MODE_CHANNELKEY);
					channel->setPassword(params.at(2));
					reply_msg.pushParam(channel_name);
					reply_msg.pushParam("+k");
					pushChannelReplyAll(reply_msg, channel, batch);
					continue ;
				}
			
			} break ;

			case (MODE_USERLIMIT):
			{
				if (channel->hasThisMode(MODE_USERLIMIT) && sign == '-')
				{
					channel->removeMode(MODE_USERLIMIT);
					channel->setUserCountLimit(0);
					reply_msg.pushParam(channel_name);
					reply_msg.pushParam("-l");
					pushChannelReplyAll(reply_msg, channel, batch);
					continue ;
				}

				if (sign == '+')
				{
					++mode_arg_idx;
					
					// No parameter.
					if (params.size() <= mode_arg_idx || params.at(mode_arg_idx).empty())
					{
						reply_msg.setSource(_serverName);
						reply_msg.setCommand(ERR_INVALIDMODEPARAM);
						reply_msg.pushParam(sender->getNickname());
						reply_msg.pushParam(channel_name);
						reply_msg.pushParam("l");
						reply_msg.pushParam("*");
						reply_msg.pushParam("You must specify a parameter for the limit mode.");
						reply_sender.second.push(reply_msg);
						continue ;
					}

					std::string limit_arg = params.at(mode_arg_idx);

					// Invalid limit mode parameter.
					if (!ft_std::isnumber(limit_arg.c_str()) || ft_std::stoi(limit_arg) < 1)
					{
						reply_msg.setSource(_serverName);
						reply_msg.setCommand(ERR_INVALIDMODEPARAM);
						reply_msg.pushParam(sender->getNickname());
						reply_msg.pushParam(channel_name);
						reply_msg.pushParam("l");
						reply_msg.pushParam(limit_arg);
						reply_msg.pushParam("Invalid limit mode parameter.");
						reply_sender.second.push(reply_msg);
						continue ;
					}

					channel->setUserCountLimit(ft_std::stoi(limit_arg));
					channel->addMode(MODE_USERLIMIT);
					reply_msg.pushParam(channel_name);
					reply_msg.pushParam("+l");
					reply_msg.pushParam(limit_arg);
					pushChannelReplyAll(reply_msg, channel, batch);
					continue ;
				}

			} break ;

			case (MODE_OPERATOR):
			{
				++mode_arg_idx;
					
				// No parameter.
				if (params.size() <= mode_arg_idx || params.at(mode_arg_idx).empty())
				{
					reply_msg.setSource(_serverName);
					reply_msg.setCommand(ERR_INVALIDMODEPARAM);
					reply_msg.pushParam(sender->getNickname());
					reply_msg.pushParam(channel_name);
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
					reply_msg.setSource(_serverName);
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
					reply_msg.setSource(_serverName);
					reply_msg.setCommand(ERR_USERNOTINCHANNEL);
					reply_msg.pushParam(sender->getNickname());
					reply_msg.pushParam(target_str);
					reply_msg.pushParam(channel_name);
					reply_msg.pushParam("They aren't on that channel.");
					reply_sender.second.push(reply_msg);
					continue ;
				}

				if (channel->hasThisClientMembershipMode(target, MODE_OPERATOR) && sign == '-')
				{
					channel->removeThisClientMembershipMode(target, MODE_OPERATOR);
					reply_msg.pushParam(channel_name);
					reply_msg.pushParam("-o");
					reply_msg.pushParam(target_str);
					pushChannelReplyAll(reply_msg, channel, batch);
					continue ;
				}

				if (!(channel->hasThisClientMembershipMode(target, MODE_OPERATOR)) && sign == '+')
				{
					channel->addThisClientMembershipMode(target, MODE_OPERATOR);
					reply_msg.pushParam(channel_name);
					reply_msg.pushParam("+o");
					reply_msg.pushParam(target_str);
					pushChannelReplyAll(reply_msg, channel, batch);
					continue ;
				}

			} break ;

			case (MODE_PROTECTTOPIC):
			{
				if (channel->hasThisMode(MODE_PROTECTTOPIC) && sign == '-')
				{
					channel->removeMode(MODE_PROTECTTOPIC);
					reply_msg.pushParam(channel_name);
					reply_msg.pushParam("-t");
					pushChannelReplyAll(reply_msg, channel, batch);
				}
				
				if (!(channel->hasThisMode(MODE_PROTECTTOPIC)) && sign == '+')
				{
					channel->addMode(MODE_PROTECTTOPIC);
					reply_msg.pushParam(channel_name);
					reply_msg.pushParam("+t");
					pushChannelReplyAll(reply_msg, channel, batch);
					continue ;
				}

			} break ;

			default:
			{
				reply_msg.setSource(_serverName);
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