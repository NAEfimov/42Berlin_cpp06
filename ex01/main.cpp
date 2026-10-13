/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefimov <nefimov@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 14:41:20 by nefimov           #+#    #+#             */
/*   Updated: 2026/10/13 14:52:53 by nefimov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serializer.hpp"

#include <iomanip>
#include <iostream>

int main()
{
	Data data;
	Data *original;
	Data *restored;
	uintptr_t raw;

	data.value = 42;
	data.marker = 'D';
	original = &data;
	std::cout << "Original data: " << original << std::endl;
	
	raw = Serializer::serialize(original);
	std::cout << "Serialized data: 0x" << std::hex << raw << std::dec << std::endl;
	
	restored = Serializer::deserialize(raw);
	std::cout << "Restored data: " << restored << std::endl;
	
	if (restored != original)
	{
		std::cout << "Serialization failed" << std::endl;
		return 1;
	}
	std::cout << "Serialization successful" << std::endl;
	return 0;
}
