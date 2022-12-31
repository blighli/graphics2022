#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <vector>
#include <string>

class StringUtil
{
public:
	// 分割字符串（使用正则匹配）
	static std::vector<std::string> split(const std::string& in, const std::string& delim);
	// 从输入流读取文本
	static std::string readText(std::istream& input);
	// 判断是否为数字
	static bool isNumber(const std::string& in);

private:

};

#endif // !STRINGUTIL_H
