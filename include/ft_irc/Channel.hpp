/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 02:56:42 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/18 07:52:38 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

// C Header
#include <ctime>

// Cpp Header
#include <string>
#include <map>
#include <set>

enum e_modes
{
	MODE_INVITEONLY = 'i',
	MODE_PROTECTTOPIC = 't',
	MODE_CHANNELKEY = 'k',
	MODE_OPERATOR = 'o',
	MODE_USERLIMIT = 'l',
	MODE_RESERVE_ERROR = '!',
};

// Forward Decalration to avoid CIRCULAR include
class Client;

/**
 * @class Channel
 * @brief A Class For Encapsulate IRC Channel
 * 
 * This Class is Use for Encapsulate IRC Channel and Its assosiated properties.
 */
class Channel
{
	public:
		typedef std::map<Client*, std::set<char> >	t_userMap;

	private:
		/**
		 * @brief Channel's Name
		 * 
		 * @note It must be set at Constructor, and mustn't change
		 */
		const std::string	_name;

		/**
		 * @brief Channel's Password
		 * 
		 * @note It's ignorable, when password mode is unset
		 */
		std::string	_password;

		/**
		 * @brief Time when This Channel is created
		 * 
		 * @note It must be set at Constructor, and mustn't change
		 */
		std::time_t	const	_timeCreated;

		/**
		 * @brief Map that contain users and its membership mode
		 * 
		 * @note Only insert/erase Client in specific methods
		 */
		t_userMap	_userMap;

		/**
		 * @brief Maximum User in This Channel. Ignore if userlimit mode is unset
		 */
		size_t	_userCountLimit;

		/**
		 * @brief Channel's Modes
		 */
		std::set<char>	_modes;

		/**
		 * @brief Channel's Topic
		 */
		std::string	_topic;

		/**
		 * @brief The most recent time when topic is setted
		 */
		std::time_t	_timeTopicSet;

		/**
		 * @brief The Nickname of most recent user that set topic
		 */
		std::string	_topicSetter;

		/**
		 * @brief Invite Set that conatain the "session" (fd)
		 * 
		 * @warning Erase its member out everytime when This Client is joined
		 * 
		 * @note This Field is ignored if invite mode is unset
		 */
		std::set<int>	_inviteSet;

	public:
		/**
		 * @brief Construct an Channel object
		 * 
		 * @param name Channel's Name
		 * @param creator Client that requested to create channel
		 */
		Channel(std::string const& name, Client * const creator);

		/**
		 * @brief Default Destructor
		 * 
		 * @note 
		 * Should pop out this channel from client's channel set.
		 * Despite the fact that, it must have 0 user to be deleted
		 */
		~Channel();

		/**
		 * @brief Get Channel's Name
		 * 
		 * @return Channel's Name in lvalue
		 */
		std::string const&	getName()	const;

		/**
		 * @brief Get Channel's Password
		 * 
		 * @return Channel's Password
		 * 
		 * @note This Field is ignored if password mode is unsetted
		 */
		std::string const&	getPassword()	const;

		/**
		 * @brief Get Time when this channel is created
		 * 
		 * @return Time when this channel is created
		 */
		std::time_t	getTimeCreated()	const;

		/**
		 * @brief Get User map that joined this Channel
		 * 
		 * @return Map of Client Pointer and Its Membership Mode as lvalue
		 */
		t_userMap const&	getUserMap()	const;

		/**
		 * @brief Get Amount of Client that joined this Channel
		 * 
		 * @return Amount of Client that stored in map
		 */
		size_t	getUserCount()	const;

		/**
		 * @brief Get Amount of User Limit
		 * 
		 * @return If userlimit flag is setted, return N, otherwise return 0
		 */
		size_t	getUserCountLimit()	const;

		/**
		 * @brief Get This Channel Topic's Name
		 * 
		 * @return Topic Name in lvalue
		 */
		std::string const&	getTopic()	const;

		/**
		 * @brief Get Most Recent Time that topic is setted
		 * 
		 * @return Most Recent Time that topic is setted
		 */
		std::time_t	getTimeTopicSet()	const;

		/**
		 * @brief Get Most Recent Topic Setter's Nickname
		 * 
		 * @return Nickname of Most Recent Topic Setter in lvalue
		 */
		std::string const&	getTopicSetter()	const;

		/**
		 * @brief Get Channel Modes 
		 * 
		 * @note not to be confused with client's membership modes
		 * 
		 * @return Channel Mode Set in lvalue
		 */
		std::set<char> const&	getModes()	const;

		/**
		 * @brief Check if This Channel contain certain Client
		 * 
		 * @param client Client That want to Check (no NULL)
		 * 
		 * @return True if This Client is member of This Channel, False otherwise
		 */
		bool	hasThisClient(Client * const client)	const;

		/**
		 * @brief Check if This Channel contain certain mode
		 * 
		 * @param c character mode
		 * 
		 * @return True if This Channel contain this mode, False otherwise
		 */
		bool	hasThisMode(char const c)	const;

		/**
		 * @brief Set Channel's Password
		 * 
		 * @param password New Channel's Password
		 * 
		 * @note This Field is ignored if password mode is unsetted
		 */
		void	setPassword(std::string const& password);

		/**
		 * @brief Set User Limit in This Channel
		 * 
		 * @param limit New maximum number of user
		 * 
		 * @note It would be completely ignored, if limit mode is unsetted
		 */
		void	setUserCountLimit(size_t const limit);

		/**
		 * @brief Set This Channel's Topic
		 * 
		 * @param topic Topic Name
		 * @param setter Client Pointer that want to set topic (no NULL)
		 * 
		 * @warning This Method isn't check validation of setter
		 */
		void	setTopic(std::string const& topic, Client const * const setter);

		/**
		 * @brief Add Channel Mode
		 * 
		 * @param c Character Mode
		 */
		void	addMode(char const c);

		/**
		 * @brief Add Channel Mode
		 * 
		 * @param s String of Charracter Mode
		 */
		void	addMode(std::string const& s);

		/**
		 * @brief Remove Channel Mode
		 * 
		 * @param c Character Mode
		 */
		void	removeMode(char const c);

		/**
		 * @brief Remove Channel Mode
		 * 
		 * @param s String of Charracter Mode
		 */
		void	removeMode(std::string const& s);

		/**
		 * @brief Add Client to This Channel
		 * 
		 * @param client Client that want to add
		 * @param mode_str Membership Modes that want to set to this Client
		 * 
		 * @return
		 * - 0: success
		 * 
		 * - 1: client is already joined
		 * 
		 * - 2: exceed user count limit (if limit mod is setted)
		 * 
		 * @note Should be used under JOIN
		 */
		int	addUserToChannel(Client * const client, std::string const& mode_str);

		/**
		 * @brief Delete Client from This Channel
		 * 
		 * @param client Client that want to remove
		 * 
		 * @return
		 * - 0: success
		 * 
		 * - 1: client doesn't existed here
		 * 
		 * @note Should be used under wrapper method of class that contain STL of channel
		 */
		int	deleteUserFromChannel(Client * const client);

		/**
		 * @brief Get certain Client's Membership Modes in This Channel
		 * 
		 * @param client Client that want to know about its membership
		 * 
		 * @return
		 * - Set of Character Membership Mode of This Client in rvalue
		 * 
		 * - In Error, return ERROR_MODE as Set
		 */
		std::set<char>	getThisClientMembershipModes(Client * const client)	const;

		/**
		 * @brief Check if certain Client has particular mode in This Channel
		 * 
		 * @param client Client that want to check
		 * @param c Character Mode that want to know if it existed
		 * 
		 * @return True if This Client contain mode in This Chanel, False otherwise
		 */
		bool	hasThisClientMembershipMode(Client * const client, char const c)	const;

		/**
		 * @brief Add certain Channel Membership Mode to This Client in This Channel
		 * 
		 * @param client Client that want to add membership mode
		 * @param c Character Membership Mode
		 * 
		 * @return
		 * - 0: success
		 * 
		 * - -1: This Client isn't existed in This Channel
		 */
		int	addThisClientMembershipMode(Client * const client, char const c);

		/**
		 * @brief Add certain Channel Membership Mode to This Client in This Channel
		 * 
		 * @param client Client that want to add membership mode
		 * @param s String of Character Membership Mode
		 * 
		 * @return
		 * - 0: success
		 * 
		 * - -1: This Client isn't existed in This Channel
		 */
		int	addThisClientMembershipMode(Client * const client, std::string const& s);

		/**
		 * @brief Remove certain Channel Membership Mode to This Client in This Channel
		 * 
		 * @param client Client that want to remove membership mode
		 * @param c Character Membership Mode
		 * 
		 * @return
		 * - 0: success
		 * 
		 * - -1: This Client isn't existed in This Channel
		 */
		int	removeThisClientMembershipMode(Client * const client, char const c);

		/**
		 * @brief Remove certain Channel Membership Mode to This Client in This Channel
		 * 
		 * @param client Client that want to remove membership mode
		 * @param s String of Character Membership Mode
		 * 
		 * @return
		 * - 0: success
		 * 
		 * - -1: This Client isn't existed in This Channel
		 */
		int	removeThisClientMembershipMode(Client * const client, std::string const& s);

		/**
		 * @brief Check if This Client is Invited
		 * 
		 * @param client Client Pointer 
		 * 
		 * @return True if This Client is Invited, False otherwise
		 * 
		 * @note Use when invite mode is setted
		 */
		bool	isClientInvited(Client * const client);

		/**
		 * @brief Add Client to Invite Set
		 * 
		 * @param client Client Pointer 
		 */
		void	addClientToInviteSet(Client * const client);

		/**
		 * @brief Remove Client from Invite Set
		 * 
		 * @param client Client Pointer 
		 */
		void	removeClientToInviteSet(Client * const client);
};

#endif