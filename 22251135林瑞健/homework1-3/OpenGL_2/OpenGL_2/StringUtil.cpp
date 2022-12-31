#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <sstream>
#include "StringUtil.h"

std::vector<std::string> StringUtil::split(const std::string& in, const std::string& delim)
{
	std::regex re{ delim };
	// 调用 std::vector::vector (InputIterator first, InputIterator last,const allocator_type& alloc = allocator_type())
	// 构造函数,完成字符串分割
	return std::vector<std::string>
	{
		std::sregex_token_iterator(in.begin(), in.end(), re, -1),
			std::sregex_token_iterator()
	};
}

std::string StringUtil::readText(std::istream& input)
{
	std::string result;
	if (input.bad())
	{
		std::cout << "Can't read the input" << '\n';
		result = "";
	}
	if (input.good())
	{
		std::stringstream ss;
		ss << input.rdbuf();
		while (!ss.eof())
		{
			std::string content;
			std::getline(ss, content);
			result += content + '\n';
		}
	}
	return result;
}

bool StringUtil::isNumber(const std::string& in)
{
	return std::regex_match(in, std::regex("^(\\d)+(.(\\d)+){0,1}$"));
}