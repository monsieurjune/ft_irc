/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 19:47:15 by tnualman          #+#    #+#             */
/*   Updated: 2024/12/25 15:57:54 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        return (0);
    }
    std::ifstream file(argv[1]);
	if (!file.is_open())
	{
		std::cout << "Error: could not open file.\n";
		return (1);
	}
    
	std::string	line;
    int i = 0;

    while (std::getline(file, line))
    {
        Message msg(line.c_str());
        std::cout << "Message number " << i++ << ":\n";
        std::cout << "  Raw     : " << line << "\n";
        if (msg.isValid() == false)
        {
            std::cout << "  Message is invalid, parsing may be incomplete, and does not matter! :p\n";
        }
        std::cout << "  Source  : " << msg.getSource() << "\n";
        std::cout << "  Command : " << msg.getCommand() << "\n";
        std::vector<std::string> params = msg.getParams();
        for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
        {
            std::cout << "  Param   : " << *it << "\n";
        }
    }
    return (0);
}