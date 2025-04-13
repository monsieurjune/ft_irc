/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:42:52 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/13 22:29:36 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "std/ft_cppstd.hpp"

namespace ft_std
{

std::vector<std::string> split(std::string const & s, std::string const & delimiter)
{
    std::string tmp = s;
	std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = tmp.find(delimiter)) != std::string::npos)
	{
        token = tmp.substr(0, pos);
        tokens.push_back(token);
        tmp.erase(0, pos + delimiter.length());
    }
    tokens.push_back(tmp);
    return (tokens);
}

}