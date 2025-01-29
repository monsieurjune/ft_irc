/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc::ircMODE.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:04:23 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/29 22:34:43 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

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
	Message								reply_msg;
	t_reply								reply;
	t_replyBatch						batch;

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
	t_reply								reply_other;
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
			reply_msg.pushParam(ltoa(channel->getUserCountLimit()));
		}
		reply_sender.second.push(reply_msg);

		reply_msg.resetParams();
		reply_msg.setCommand(RPL_CREATIONTIME);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam(channel_name);
		reply_msg.pushParam(ltoa(channel->getTimeCreated()));
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
		reply_msg.pushParam("Modestring is invalid");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
	}

	char sign = modestr.at(0);
	std::string working_modes = "iklot";

	for (std::string::iterator it = modestr.begin() + 1; it != modestr.end(); it++)
	{
		if (*it == '-' || *it == '+')
		{
			sign = *it;
			continue ;
		}
		// ERR_UNKNOWNMODE
		if (working_modes.find(*it) == std::string::npos)
		{
			reply_msg.setCommand(ERR_UNKNOWNMODE);
			reply_msg.pushParam(sender->getNickname());
			reply_msg.pushParam(std::string() + *it); // A single character string hack :p
			reply_msg.pushParam("is not a recognized mode character");
			reply_sender.second.push(reply_msg);
			continue ;
		}
		// This block should be its own sub method for FtIrc class (how about ircMODE_channel_changeMode() ?).
		switch (*it)
		{
			case (MODE_INVITEONLY):
				//
				break ;
			case (MODE_CHANNELKEY):
				//
				break ;
			case (MODE_USERLIMIT):
				//
				break ;
			case (MODE_OPERATOR):
				// ERR_INVALIDMODEPARAM
				break ;
			case (MODE_PROTECTTOPIC):
				//
				break ;
		}
	}

	// ERR_INVALIDMODEPARAM
	
	batch.push_back(reply_sender);
	return (batch);
}
