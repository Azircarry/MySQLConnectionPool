#pragma once
#include <iostream>
// 文件 行号 时间 信息
#define LOG(str) \
	std::cout << __FILE__ << ":" << __LINE__ <<" " << \
	__TIMESTAMP__ << " : " << str << std::endl;