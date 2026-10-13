/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefimov <nefimov@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 15:00:05 by nefimov           #+#    #+#             */
/*   Updated: 2026/10/13 15:23:09 by nefimov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Identify.hpp"

#include <iostream>

int main()
{
	int i;

	for (i = 0; i < 3; ++i)
	{
		Base *object = generate();

		std::cout << "Identify pointer: ";
		identify(object);
		std::cout << "Identify reference: ";
		identify(*object);
		delete object;
		std::cout << std::endl;
	}
	std::cout << "Null pointer: ";
	identify(static_cast<Base *>(NULL));
	return 0;
}
