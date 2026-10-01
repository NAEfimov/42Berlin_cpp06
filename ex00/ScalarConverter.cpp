/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefimov <nefimov@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 17:10:00 by nefimov           #+#    #+#             */
/*   Updated: 2026/10/01 19:20:49 by nefimov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScalarConverter.hpp"

#include <cerrno>
#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

namespace
{
	enum LiteralType
	{
		TYPE_INVALID,
		TYPE_CHAR,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_DOUBLE
	};

	bool isDigitString(const std::string &value, std::size_t start)
	{
		std::size_t i = start;

		if (i == value.size())
			return false;
		while (i < value.size())
		{
			if (!std::isdigit(static_cast<unsigned char>(value[i])))
				return false;
			++i;
		}
		return true;
	}

	bool isSignedInteger(const std::string &value)
	{
		std::size_t start = 0;

		if (value.empty())
			return false;
		if (value[0] == '+' || value[0] == '-')
			start = 1;
		return isDigitString(value, start);
	}

	bool isDecimal(const std::string &value, bool &hasFraction)
	{
		std::size_t i = 0;
		bool hasDigitsBefore = false;
		bool hasDigitsAfter = false;

		hasFraction = false;
		if (value.empty())
			return false;
		if (value[i] == '+' || value[i] == '-')
			++i;
		while (i < value.size() && std::isdigit(static_cast<unsigned char>(value[i])))
		{
			hasDigitsBefore = true;
			++i;
		}
		if (i < value.size() && value[i] == '.')
		{
			hasFraction = true;
			++i;
			while (i < value.size()
				&& std::isdigit(static_cast<unsigned char>(value[i])))
			{
				hasDigitsAfter = true;
				++i;
			}
		}
		return i == value.size() && (hasDigitsBefore || hasDigitsAfter);
	}

	bool isPseudoLiteral(const std::string &value)
	{
		return value == "nan" || value == "nanf"
			|| (value.size() >= 4 && (value[0] == '+' || value[0] == '-')
				&& value.compare(1, 3, "inf") == 0
				&& (value.size() == 4
					|| (value.size() == 5 && value[4] == 'f')));
	}

	LiteralType detectType(const std::string &value)
	{
		if (value.size() == 3 && value[0] == '\'' && value[2] == '\'')
			return TYPE_CHAR;
		if (isPseudoLiteral(value))
			return value == "nanf" || value == "+inff" || value == "-inff"
				? TYPE_FLOAT : TYPE_DOUBLE;
		if (isSignedInteger(value))
			return TYPE_INT;
		if (!value.empty() && value[value.size() - 1] == 'f')
		{
			bool hasFraction = false;

			if (isDecimal(value.substr(0, value.size() - 1), hasFraction)
				&& hasFraction)
				return TYPE_FLOAT;
		}
		{
			bool hasFraction = false;

			if (isDecimal(value, hasFraction) && hasFraction)
				return TYPE_DOUBLE;
		}
		return TYPE_INVALID;
	}

	bool parseNumber(const std::string &value, double &result)
	{
		char *end = NULL;

		errno = 0;
		result = std::strtod(value.c_str(), &end);
		return end != value.c_str() && *end == '\0'
			   && (errno != ERANGE || result != result
				  || result == std::numeric_limits<double>::infinity()
				  || result == -std::numeric_limits<double>::infinity());
	}

	bool isNan(double value)
	{
		return value != value;
	}

	bool isInfinite(double value)
	{
		return !isNan(value)
			&& (value == std::numeric_limits<double>::infinity()
				|| value == -std::numeric_limits<double>::infinity());
	}

	bool fitsInChar(double value)
	{
		return !isNan(value) && !isInfinite(value)
			&& value >= static_cast<double>(std::numeric_limits<char>::min())
			&& value <= static_cast<double>(std::numeric_limits<char>::max());
	}

	bool fitsInInt(double value)
	{
		return !isNan(value) && !isInfinite(value)
			&& value >= static_cast<double>(std::numeric_limits<int>::min())
			&& value <= static_cast<double>(std::numeric_limits<int>::max());
	}

	bool fitsInFloat(double value)
	{
		return isNan(value) || isInfinite(value)
			|| (value >= -static_cast<double>(std::numeric_limits<float>::max())
				&& value <= static_cast<double>(std::numeric_limits<float>::max()));
	}

	std::string formatFloating(double value, bool asFloat)
	{
		std::ostringstream output;

		if (isNan(value))
			return asFloat ? "nanf" : "nan";
		if (isInfinite(value))
			return value < 0 ? (asFloat ? "-inff" : "-inf")
				: (asFloat ? "+inff" : "+inf");
		output << std::fixed << std::setprecision(1) << value;
		if (asFloat)
			output << 'f';
		return output.str();
	}

	void printChar(double value)
	{
		if (!fitsInChar(value))
			std::cout << "char: impossible" << std::endl;
		else if (!std::isprint(static_cast<unsigned char>(static_cast<char>(value))))
			std::cout << "char: Non displayable" << std::endl;
		else
			std::cout << "char: '" << static_cast<char>(value) << "'" << std::endl;
	}

	void printInt(double value)
	{
		if (!fitsInInt(value))
			std::cout << "int: impossible" << std::endl;
		else
			std::cout << "int: " << static_cast<int>(value) << std::endl;
	}

	void printFloat(double value)
	{
		if (!fitsInFloat(value))
			std::cout << "float: impossible" << std::endl;
		else
			std::cout << "float: " << formatFloating(value, true) << std::endl;
	}

	void printDouble(double value)
	{
		std::cout << "double: " << formatFloating(value, false) << std::endl;
	}
}

ScalarConverter::ScalarConverter()
{
}

ScalarConverter::ScalarConverter(const ScalarConverter &other)
{
	(void)other;
}

ScalarConverter &ScalarConverter::operator=(const ScalarConverter &other)
{
	(void)other;
	return *this;
}

ScalarConverter::~ScalarConverter()
{
}

void ScalarConverter::convert(const std::string &literal)
{
	LiteralType type = detectType(literal);
	double value = 0.0;

	if (type == TYPE_INVALID)
	{
		std::cout << "char: impossible" << std::endl;
		std::cout << "int: impossible" << std::endl;
		std::cout << "float: impossible" << std::endl;
		std::cout << "double: impossible" << std::endl;
		return;
	}
	if (type == TYPE_CHAR)
		value = static_cast<unsigned char>(literal[1]);
	else
	{
		std::string number = literal;

		if (type == TYPE_FLOAT && number[number.size() - 1] == 'f')
			number.erase(number.size() - 1);
		if (number == "nan")
			value = std::numeric_limits<double>::quiet_NaN();
		else if (number.size() == 4 && number[1] == 'i'
			&& number[2] == 'n' && number[3] == 'f')
		{
			if (number[0] == '+')
				value = std::numeric_limits<double>::infinity();
			else
				value = -std::numeric_limits<double>::infinity();
		}
		else if (!parseNumber(number, value))
		{
			std::cout << "char: impossible" << std::endl;
			std::cout << "int: impossible" << std::endl;
			std::cout << "float: impossible" << std::endl;
			std::cout << "double: impossible" << std::endl;
			return;
		}
	}
	printChar(value);
	printInt(value);
	printFloat(value);
	printDouble(value);
}
