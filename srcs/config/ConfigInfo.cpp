#include "../../include/ConfigInfo.hpp"
#include "../../include/configStructure.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>

static std::vector<std::string>	ft_split(const std::string& str, const std::string& delimiter);
static std::string 				ft_strtrim(const std::string& str, const std::string& set);

ConfigInfo::ValidateFieldInfo::ValidateFieldInfo(void)
	:count(0)
{}

ConfigInfo::ValidateFieldInfo &ConfigInfo::ValidateFieldInfo::operator++(int)
{
	this->count++;
	return (*this);
}

void ConfigInfo::ValidateFieldInfo::setValidateFieldType(const VaildateFieldType& _validate_field_type)
{
	this->validate_field_type = _validate_field_type;
}

void ConfigInfo::ValidateFieldInfo::setCount(const size_t &_count)
{
	this->count = _count;
}

const ConfigInfo::VaildateFieldType &ConfigInfo::ValidateFieldInfo::getValidateFieldType(void) const
{
	return (this->validate_field_type);
}

const size_t &ConfigInfo::ValidateFieldInfo::getCount(void) const
{
	return (this->count);
}

void ConfigInfo::printContent(const std::string &str, const std::string &str_name, const std::string &color)
{
	std::cout << color << str_name << " : $" << str << "$" << WHI << std::endl;
}

void ConfigInfo::printVector(std::vector<std::string> &word_list, const std::string &str_name, const std::string &color)
{
	std::vector<std::string>::iterator	iter;

	for(iter = word_list.begin(); iter != word_list.end(); iter++)
		printContent(*iter, str_name, color);
}

ConfigInfo::ConfigInfo(void)
	: whitespace(" \t\n\v\f\r")
{}

/*
	config 중복여부 확인필요
	host:port
*/
void ConfigInfo::parseConfig(const char *file_name)
{
	std::string		file_content;

	// check file name rule
	if (this->checkFileNameExtension(file_name))
		std::cout << "file name is correct to the config file name rule" << std::endl;
	else
	{
		std::cout << "file name is not correct to the config file name rule!!!" << std::endl;
		exit(1);
	}
	// open and read file
	file_content = this->readFile(file_name);

	// validation
	if (this->validateConfigFile(file_content))
		std::cout << "Config file content is validate" << std::endl;
	else
	{
		std::cout << "Config file content is N.O.T validate" << std::endl;
		exit(1);
	}
	
	// this->parse(file_content);
	if (this->parse(file_content))
		std::cout << "Config parse is OK" << std::endl;
	else
		std::cout << "Config parsing something is wrong" << std::endl;
	// ConfigInfo ConfigInfo;
	// parse
	// ParseConfig(); // 결국 이 안으로 들어가야함
	// ConfigInfo.parseConfig(argv[1]);
	//std::cout << ConfigInfo.config << std::endl;
	// ConfigInfo.parseServer();
}

// ServerInfo ConfigInfo::parseServerBlock(size_t it) {
// 	ServerInfo res;
// 	std::string key;
// 	std::string value;

// 	while (it != std::string::npos) {
// 		// aa
// 	}

// 	return (res);
// }

/*
	throw하는 식으로 변경 필요
	throw 이후 exit()
*/
bool	ConfigInfo::checkFileNameExtension(const char *file_name_parms)
{
	std::string	file_name(file_name_parms);
	size_t		pos;

	pos = file_name.rfind(".conf");
	if (pos != std::string::npos && \
		file_name.compare(pos, file_name.size() - pos, ".conf") == 0)
		return (true);
	return (false);
}

std::string ConfigInfo::readFile(std::string file_name)
{
	std::string     s;
	std::ifstream   ifs(file_name);

	if (ifs.is_open())
	{
		ifs.seekg(0, std::ios::end);
		int size = ifs.tellg();
		s.resize(size);
		ifs.seekg(0, std::ios::beg);
		ifs.read(&s[0], size);
	}
	else
	{
		// error_handling("Fail to open file", 1);
		exit(1);
	}
	return (s);
}

static bool	in_str(const std::string& str, const char& ch)
{
	// for(std::string::iterator iter = str.begin(); iter != str.end(); iter++)
	for(size_t idx = 0; idx < str.size(); idx++)
	{
		if (ch == str[idx])
			return (true);
	}
	return (false);
}

// char delimiter -> std::string delimiter
static std::vector<std::string>    ft_split(const std::string& str, const std::string& delimiter)
{
	std::vector<std::string>	word_list;
	size_t						idx;

	idx = 0;
	while (idx < str.size())
	{
		if (in_str(delimiter, str[idx]))
			idx++;
		else
		{
			size_t  begin_of_word;
			size_t  len;

			begin_of_word = idx;
			while (str[idx] && (in_str(delimiter, str[idx]) == false))
				idx++;
			len = idx - begin_of_word;
			word_list.push_back(str.substr(begin_of_word, len));
		}
	}
	return (word_list);
}

static std::string ft_strtrim(const std::string& str, const std::string& set)
{
	std::string clean_str;
	size_t      start_of_str;
	size_t      end_of_str;

	if (str == "" || set == "")
		return (str);
	start_of_str = str.find_first_not_of(set);
	end_of_str = str.find_last_not_of(set);
	if (start_of_str == std::string::npos || end_of_str == std::string::npos)
		return ("");
	clean_str = str.substr(start_of_str, end_of_str - start_of_str + 1);
	return (clean_str);
}

/*
	여러번 파일을 읽을것이라서 const불가, 
	reference하면 파일 pos가 맨 밑으로 갈것으로 예상되어서 deep copy되게 놔둠
	- 짝은 맞지만 불필요한 곳에 {}가 있는 경우에 대한 처리 필요.
	- split된 인자의시작이 
*/
bool	ConfigInfo::checkCurlyBracketPair(const std::string& file_content)
{
	std::stringstream		file_content_stream(file_content);
	std::string				str;
	std::stack<std::string>	curly_stack;

	while (std::getline(file_content_stream, str))
	{
		std::string					clean_str;
		std::vector<std::string>	word_list;

		clean_str = ft_strtrim(str, this->whitespace);
		// printContent(clean_str, "clean_str", BLU);
		if (clean_str == "" || clean_str[0] == '#')
			continue ;
		word_list = ft_split(clean_str, this->whitespace);
		for (std::vector<std::string>::iterator word_iter = word_list.begin(); word_iter != word_list.end(); word_iter++)
		{
			std::string	word;
			
			word = *word_iter;
			if ((word)[0] == '#')
				break ;
			// printContent(word, "\tword_iter");
			if ((word == "{" || word == "}"))
			{
				if (word_iter + 1 != word_list.end() && (word_iter + 1)->at(0) != '#') // '{', '}' ,다음 단어가 있다면 "#"로 시작해야함
					return (false);
				// printContent(*word_iter, "\t###word_iter ", GRN);
				if (word == "{")
					curly_stack.push(word);
				else if (!curly_stack.empty())
					curly_stack.pop();
				else
					return (false);
			}// '{' '}'로 시작하는데 길이가 1보다 크고, #이 ';' 바로 다음에 없으면 error처리
			else if ((word.at(0) == '{' || word.at(0) == '}') &&  
				(word.size() > 1 && word.at(1) != '#'))
				return (false);
		}
	}
	return (curly_stack.empty());
}

/*
case
- server {
- server { #4242 sdf 
- server
  { # comment dsf erw tdfg
- serverr test {
*/
bool	findOpenCurlyBracket(const std::vector<std::string>::iterator& begin_iter,
								const std::vector<std::string>::iterator& end_iter,
								bool& server_flag,
								bool& location_flag,
								bool& error_flag)
{
	// printContent(*(begin_iter + 1) , "*(begin_iter + 1) ");
	if (*begin_iter == "server")
	{
		if ((((begin_iter + 1) != end_iter &&*(begin_iter + 1) == "{") && ((begin_iter + 2) == end_iter || (*(begin_iter + 2))[0] == '#')) || \
			(*begin_iter == "{" && (begin_iter + 1 == end_iter || (*(begin_iter + 1))[0] == '#')))
			server_flag = true;
		else if ((begin_iter + 1) == end_iter)
			;
		else
		{
			error_flag = false;
			return (false);
		}
		return (true);
	}
	else if (*begin_iter == "location")
	{
		if (((end_iter - begin_iter) > 2 && ((begin_iter + 2) != end_iter &&*(begin_iter + 2) == "{") && ((begin_iter + 3) == end_iter || (*(begin_iter + 3))[0] == '#')) || \
		(*begin_iter == "{" && (begin_iter + 1 == end_iter || (*(begin_iter + 1))[0] == '#')))
			location_flag = true;
		else
		{
			error_flag = false;
			return (false);
		}
		return (true);
	}
	return (false);
}

bool	findCloseCurlyBracket(const std::vector<std::string>::iterator& begin_iter,
								const std::vector<std::string>::iterator& end_iter,
								bool& error_flag)
{
	if (*begin_iter == "}")
	{
		if (((begin_iter + 1) == end_iter || (*(begin_iter + 1))[0] == '#'))
			return (true);
		error_flag = false;
	}
	return (false);
}

bool	ConfigInfo::findLocationBlock(std::vector<std::string>::iterator& iter, \
													const std::vector<std::string>::iterator& src_end_iter, \
													std::vector<std::string>::iterator& begin_iter, \
													std::vector<std::string>::iterator& end_iter)
{
	std::stack<std::string> 			location_stack;
	bool								server_flag = false; // 여기서 쓰지는 않지만 같이 써야하므로 통일성을 위해 놔둠
	bool								location_flag = false;
	bool								error_flag = true;

	// findServerBlock
	for (;iter != src_end_iter; iter++)
	{
		std::vector<std::string>	word_list;
		std::string					clean_str;

		clean_str = ft_strtrim(*iter, this->whitespace);
		// printContent(clean_str, "clean_str", CYN);
		// 만약 block 짝이 안맞을때 에러케이스 거르기 위함
		if (clean_str == "" || clean_str[0] == '#')
			continue ;
		word_list = ft_split(clean_str, this->whitespace);
		// printVector(word_list, "word_list", GRN);
		// for (std::vector<std::string>::iterator tmp_iter = word_list.begin(); tmp_iter != word_list.end(); tmp_iter++)
		// 	printContent(*tmp_iter, "#*tmp_iter in findServerBlock", CYN);
	 	if (findOpenCurlyBracket(word_list.begin(), word_list.end(), server_flag, location_flag, error_flag))
		{
			// std::cout << WHI << std::boolalpha << "server_flag : " << server_flag << WHI << std::endl;
			// std::cout << WHI << std::boolalpha << "location_flag : " << location_flag << WHI << std::endl;
			if (location_flag == true)
			{
				location_flag = false;
				location_stack.push("{");
				begin_iter = iter;
			}
			// std::cout << BLU << std::boolalpha << "server_flag : " << server_flag << WHI << std::endl;
			// std::cout << BLU << std::boolalpha << "location_flag : " << location_flag << WHI << std::endl;
		}
		else if (findCloseCurlyBracket(word_list.begin(), word_list.end(), error_flag))
		{
			if (location_stack.empty() == false)
			{
				location_stack.pop();
				// begin_iter++; // block내부만 가리키게 만듦
				end_iter = iter;
				// for (std::vector<std::string>::iterator iter = begin_iter; iter != end_iter; iter++)
				// 	printContent(*iter, "#*iter about findServerBlock :", CYN);
				// printContent(*begin_iter, "begin_iter");
				// printContent(*end_iter, "end_iter");
				return (true);
			}
		}
		if (server_flag || error_flag == false)
			return (false);
	}
	return (false);
}

/*
	ValidateFieldInfo 내부에count가 1인지 확인
	count를 증가시키기 전에 확인할것이므로 중복 필드가 아니면 
	count가 0이어야 정상이다.
*/
bool	ConfigInfo::checkDuplicateConfigField(const ValidateFieldInfo& validate_field_info)
{
	if (validate_field_info.getValidateFieldType() == NESSARY_UNIQUE || \
		validate_field_info.getValidateFieldType() == OPTION_UNIQUE)
	{
		if (validate_field_info.getCount() > 0)
			return (false);
	}
	return (true);
}

/*
	# 주석은 상관없다
	이미 이전에 확인한 상태이므로 무조건 ;이 #
*/
std::string	removeAfterSemicolon(const std::string& origin_value)
{
	std::string	clean_value(origin_value);
	size_t		end_of_str;

	end_of_str = origin_value.find(';');
	if (end_of_str != std::string::npos)
		clean_value = origin_value.substr(0, end_of_str);
	return (clean_value);
}

/*
IPv4
[123].[123].[123].[123];
- ;이 있다면 제거
	; or # 이전까지만 value에 담아둠
- 각 '.'과 '.'사이는 최대 3자리 숫자
- '.'의 개수는 3개
*/
bool ConfigInfo::checkHostConfigField(std::string field_value)
{
	std::string	ip_class;
	size_t		dot_count;

	field_value = removeAfterSemicolon(field_value);
	dot_count = 0;
	for (size_t	idx = 0; idx < field_value.length(); idx++)
	{
		if (field_value[idx] == '.')
		{
			if (checkIpClass(ip_class) == false)
				return (false);
			ip_class = "";
			dot_count++;
			continue ;
		}	
		ip_class += field_value[idx];
	}
	if (checkIpClass(ip_class) == false)
		return (false);
	if (dot_count != 3)
		return (false);
    return (true);
}

bool ConfigInfo::checkIpClass(const std::string& ip_class)
{
	std::stringstream	ip_class_strstream;
	int					ip_class_numeric;

	ip_class_strstream << std::atoi(ip_class.c_str());
	ip_class_strstream >> ip_class_numeric;
	if (ip_class_strstream.str() != ip_class)
		return (false);
	else if (ip_class_numeric > 255 || ip_class_numeric < 0)
		return (false);
    return (true);
}

/*
port 
0 ~ 65526
*/
bool ConfigInfo::checkPortConfigField(std::string port)
{
	std::stringstream	port_strstream;
	int					port_numeric;

	port = removeAfterSemicolon(port);
	port_strstream << std::atoi(port.c_str());
	port_strstream >> port_numeric;
	if (port_strstream.str() != port)
		return (false);
	else if (port_numeric > std::numeric_limits<uint16_t>::max() || port_numeric < 0)
		return (false);
    return (true);
}

bool ConfigInfo::checkClientMaxBodySizeConfigField(std::string client_body_size)
{
	std::stringstream	client_body_size_strstream;
	int					client_body_size_numeric;

	client_body_size = removeAfterSemicolon(client_body_size);
	client_body_size_strstream << std::atoi(client_body_size.c_str());
	client_body_size_strstream >> client_body_size_numeric;
	if (client_body_size_strstream.str() != client_body_size)
		return (false);
	else if (client_body_size_numeric > std::numeric_limits<int>::max() || client_body_size_numeric < 0)
		return (false);
    return (true);
}

/*
Error page code
400~599
*/
bool ConfigInfo::checkErrorPageStatusCodde(const std::string& status_code)
{
	std::stringstream	status_code_strstream;
	int					status_code_numeric;

	status_code_strstream << std::atoi(status_code.c_str());
	status_code_strstream >> status_code_numeric;
	if (status_code_strstream.str() != status_code)
		return (false);
	else if (status_code_numeric > 599 || status_code_numeric < 400)
		return (false);
    return (true);
}

/*
	allow_method GET;
	allow_method GET ;
	allow_method GET POST;
	allow_method DELETE GET POST;
*/
bool ConfigInfo::checkAllowMethodConfigField(std::string allow_method)
{
	std::string							value;
	size_t 								semicolon_pos;
	size_t								comment_pos;
	std::vector<std::string>			allow_method_vector;
	std::vector<std::string>::iterator	allow_method_value_iter;

	semicolon_pos = allow_method.find(';');
	if (semicolon_pos == std::string::npos)
		return (false);
	// printContent(allow_method, "allow_method before removeAfterSemicolon", GRN);
	allow_method = removeAfterSemicolon(allow_method);
	// printContent(allow_method, "allow_method after removeAfterSemicolon", RED);
	comment_pos = allow_method.find('#');
	if (comment_pos != std::string::npos)
	{
		if (semicolon_pos > comment_pos)
			return (false);
		else if (semicolon_pos + 1 != comment_pos)
			return (false);
	}

	allow_method = ft_strtrim(allow_method, this->whitespace);
	allow_method_vector = ft_split(allow_method, this->whitespace);
	allow_method_value_iter = allow_method_vector.begin() + 1;
	if (allow_method_value_iter == allow_method_vector.end()) // allow_method test ;
		return (false);
	for (std::vector<std::string>::iterator iter = allow_method_value_iter; iter != allow_method_vector.end(); iter++)
	{
		if (*iter != "GET" && \
			*iter != "POST" && \
			*iter != "DELETE")
			return (false);
	}
    return (true);
}

/*
    필수항목에서 빠진 것은 없는지 확인
    필수항목 포함하여 중복된 것은 없는지 확인 이미 확인하기에 필요없을 듯
    NESSARY_UNIQUE,
    NESSARY_MULTI,
	OPTION_UNIQUE,
*/
bool ConfigInfo::checkNessaryOrUniqueField(std::map<std::string, ConfigInfo::ValidateFieldInfo> validate_server_field_map)
{
	std::map<std::string, ValidateFieldInfo>::iterator	map_iter;

	for (map_iter = validate_server_field_map.begin(); map_iter != validate_server_field_map.end(); map_iter++)
	{
		if (map_iter->second.getValidateFieldType() == NESSARY_UNIQUE)
		{
			if (map_iter->second.getCount() != 1) // 중복 및 누락여부 확인 가능 
				return (false);
		}
		if (map_iter->second.getValidateFieldType() == NESSARY_MULTI)
		{
			if (map_iter->second.getCount() == 0) // 누락여부 확인 가능 
				return (false);
		}
		if (map_iter->second.getValidateFieldType() == OPTION_UNIQUE)
		{
			if (map_iter->second.getCount() != 1) // 누락여부 확인 가능 
				return (false);
		}
	}
    return (true);
}

/*
    error_page 403 404 400 500 /50x.html
    형식이 제대로 맞는지도 확인필요
    ;이 있는가?
    마지막 단어에 있어야한다.
	;다음에 #이나 문자가 있는 경우 문제됨
*/
bool ConfigInfo::checkErrorPageConfigField(std::string error_page)
{
	std::string							value;
	size_t 								semicolon_pos;
	size_t								comment_pos;
	std::vector<std::string>			error_page_vector;
	std::vector<std::string>::iterator	error_page_end_value_iter;
	std::vector<std::string>::iterator	error_page_value_iter;

	semicolon_pos = error_page.find(';');
	if (semicolon_pos == std::string::npos)
		return (false);
	error_page = removeAfterSemicolon(error_page);
	comment_pos = error_page.find('#');
	if (comment_pos != std::string::npos)
	{
		if (semicolon_pos > comment_pos)
			return (false);
		else if (semicolon_pos + 1 != comment_pos)
			return (false);
	}
	error_page = ft_strtrim(error_page, this->whitespace);
	error_page_vector = ft_split(error_page, this->whitespace);
	error_page_value_iter = error_page_vector.begin() + 1;
	error_page_end_value_iter = error_page_vector.end() - 1;
	if (error_page_value_iter == error_page_vector.end() ||  // error_page;
		error_page_end_value_iter == error_page_vector.begin() || // error_page test;
		error_page_value_iter == error_page_end_value_iter) // error_page test ;
		return (false);

	for (std::vector<std::string>::iterator iter = error_page_value_iter; iter != error_page_end_value_iter; iter++)
	{
		if (checkErrorPageStatusCodde(*iter) == false)
			return (false);
	}
    return (true);
}

/*
- key value;
- key value;#;#;#;; #ㅇㄹㅁㄴㅇ라ㅣㅓㅁㅇ니러
- key value ;
- key value; #
- key value ; #
2개 이상
key는 이미 validate한지 확인한 이후
*/
// bool	ConfigInfo::checkCommonConfigLineForm(std::string config_line)
bool	ConfigInfo::checkCommonConfigLineForm(std::vector<std::string> word_list)
{
	std::vector<std::string>::iterator	value_iter;
	std::vector<std::string>::iterator	semicolon_iter;
	std::string							value;
	size_t 								semicolon_pos;
	size_t								comment_pos;

	// for (std::vector<std::string>::iterator iter = word_list.begin(); iter != word_list.end(); iter++)
	// 	printContent(*iter, "*iter", BRW);
	value_iter = word_list.begin() + 1;
	// printContent(*value_iter, "*value_iter", BRW);
	if (value_iter == word_list.end())
		return (false);

	value = *value_iter;
	// printContent(value, "value", BRW);
	if (value.size() == 0 || value[0] == '#')
		return (false);
	// value; ,value ;#
	// value;#sdfjklasdjklfj 이런 케이스는 음
	// ';'찾은 경우
	// ';'가 마지막에 있거나 ';'바로 다음이 '#'이어야 한다.
	semicolon_pos = value.find(';');
	// std::cout << "semicolon_pos : " << semicolon_pos << std::endl;
	comment_pos = value.find('#');
	// std::cout << BRW << "comment_pos : " << comment_pos << WHI << std::endl;
	// value안에 ';'이 있는 경우 "value;"
	// value안에 주석이 있었다면 ;이 주석 왼쪽에 있어야한다.
		// 없다면 value 다음 문자를 확인해야한다.
	// 주석이 없다면 ;이 "value;" 마지막에 있거나 "value" ";"처럼 value 바로 다음 단어에 첫 글자로 와야한다.

	// "value#;" ";"
	// "value#;" "#;"
	if (comment_pos != std::string::npos)
	{
		// 이미 주석처리 된것이므로 뒤에 단어에 ';'있든 없든 상관없어짐
		if (semicolon_pos > comment_pos)
			return (false);
		else if (semicolon_pos + 1 != comment_pos)
			return (false);
	}
	else
	{
		// "value"에 주석이 없으면서 "value"에 ';'로 없는 경우
		if (semicolon_pos == std::string::npos) // 다음 단어에 ';'이 처음에 있는지 확인
		{
			std::vector<std::string>::iterator	next_iter;
			size_t								next_word_comment_pos;
			size_t								next_word_semicolon_pos;

			next_iter = value_iter + 1;
			next_word_comment_pos = next_iter->find('#');
			next_word_semicolon_pos = next_iter->find(';');

			// ';'이 다음단어 처음에 나타나는 경우
			// ';'다음은 없거나 '#'이 ';' 바로 뒤에 나와야함
			// ";#"허용
			if (next_word_semicolon_pos == 0)
			{
				if (next_iter->size() > 1 && next_iter->at(1) != '#')
					return (false);
			}
			else // 다음단어 처음에 ';'가 나타나지 않으면 error
				return (false);
		}
		else //주석이 없다면 ';'이 value 마지막에 나와야함
		{
			if (semicolon_pos != (value.size() - 1))
				return (false);
			else if (value_iter + 1 != word_list.end())
			{
				if ((value_iter + 1)->at(0) != '#')
					return (false);
			}
		}
	}
	value = removeAfterSemicolon(value);
	if (value.size() == 0)
		return (false);
	return (true);
}

/*
- Locatoin block

*/
bool	ConfigInfo::findServerBlock(std::vector<std::string>::iterator& iter, \
													const std::vector<std::string>::iterator& src_end_iter, \
													std::vector<std::string>::iterator& begin_iter, \
													std::vector<std::string>::iterator& end_iter)
{
	std::stack<std::string> 			server_stack;
	std::stack<std::string> 			location_stack;
	bool								server_flag = false; // for stack.push() 해도되는지 확인용
	bool								location_flag = false;
	bool								error_flag = true;

	// findServerBlock
	for (;iter != src_end_iter; iter++)
	{
		std::vector<std::string>	word_list;
		std::string					clean_str;

		clean_str = ft_strtrim(*iter, this->whitespace);
		// printContent(clean_str, "clean_str", CYN);
		// 만약 block 짝이 안맞을때 에러케이스 거르기 위함
		if (clean_str == "" || clean_str[0] == '#')
			continue ;
		word_list = ft_split(clean_str, this->whitespace);
		// for (std::vector<std::string>::iterator tmp_iter = word_list.begin(); tmp_iter != word_list.end(); tmp_iter++)
		// 	printContent(*tmp_iter, "#*tmp_iter in findServerBlock", CYN);
	 	if (findOpenCurlyBracket(word_list.begin(), word_list.end(), server_flag, location_flag, error_flag))
		{
			// std::cout << WHI << std::boolalpha << "server_flag : " << server_flag << WHI << std::endl;
			// std::cout << WHI << std::boolalpha << "location_flag : " << location_flag << WHI << std::endl;
			if (server_flag == true)
			{
				server_flag = false;
				server_stack.push("{");
				begin_iter = iter;
			}
			else if (location_flag == true)
			{
				location_flag = false;
				location_stack.push("{");
			}
			// std::cout << BLU << std::boolalpha << "server_flag : " << server_flag << WHI << std::endl;
			// std::cout << BLU << std::boolalpha << "location_flag : " << location_flag << WHI << std::endl;
		}
		else if (findCloseCurlyBracket(word_list.begin(), word_list.end(), error_flag))
		{
			if (location_stack.empty() == false)
			{
				location_stack.pop();
				location_flag = false;
			}
			// else if (location_stack.empty() && server_stack.empty() == false)
			else if (server_stack.empty() == false)
			{
				server_stack.pop();
				begin_iter++; // block내부만 가리키게 만듦
				end_iter = iter;
				// for (std::vector<std::string>::iterator iter = begin_iter; iter != end_iter; iter++)
				// 	printContent(*iter, "#*iter about findServerBlock :", CYN);
				// printContent(*begin_iter, "begin_iter");
				// printContent(*end_iter, "end_iter");
				return (true);
				// validateServerBlock(begin_iter, end_iter); // block위치를 알게됨
			}
		}
		if (error_flag == false)
			return (false);
	}
	return (false);
}

std::map<std::string, ConfigInfo::ValidateFieldInfo>	ConfigInfo::getValidateServerFieldMap()
{
	std::map<std::string, ConfigInfo::ValidateFieldInfo>	validate_server_field_map;
	ValidateFieldInfo										validate_field_info;

	validate_field_info.setValidateFieldType(NESSARY_UNIQUE);
	validate_server_field_map["host"] = validate_field_info;
	validate_server_field_map["port"] = validate_field_info;
	validate_server_field_map["root"] = validate_field_info;
	validate_server_field_map["index"] = validate_field_info;
	validate_server_field_map["client_max_body_size"] = validate_field_info;
	validate_field_info.setValidateFieldType(OPTION_UNIQUE);
	validate_server_field_map["server_name"] = validate_field_info;
	validate_field_info.setValidateFieldType(OPTION_MULTI);
	validate_server_field_map["error_page"] = validate_field_info;
	validate_server_field_map["location"] = validate_field_info;
	return (validate_server_field_map);
}

std::map<std::string, ConfigInfo::ValidateFieldInfo>	ConfigInfo::getValidateLocationFieldMap()
{
	std::map<std::string, ConfigInfo::ValidateFieldInfo>	validate_location_field_map;
	ValidateFieldInfo										validate_field_info;

	validate_field_info.setValidateFieldType(NESSARY_UNIQUE);
	validate_location_field_map["location"] = validate_field_info;
	validate_field_info.setValidateFieldType(OPTION_UNIQUE);
	validate_location_field_map["allow_method"] = validate_field_info;
	validate_location_field_map["autoindex"] = validate_field_info;
	validate_location_field_map["root"] = validate_field_info;
	validate_location_field_map["index"] = validate_field_info;
	validate_location_field_map["redirect"] = validate_field_info;
	return (validate_location_field_map);
}

/*
- 유효성 검사 이후이므로 바로 넣어주면 된다.
*/
bool	ConfigInfo::parse(const std::string &file_content)
{
	std::string							str;
	std::vector<std::string>			file_content_vector;
	std::vector<std::string>::iterator 	cur_iter;
	std::vector<std::string>::iterator 	begin_iter;
	std::vector<std::string>::iterator 	end_iter;
	
	file_content_vector = ft_split(file_content, "\n");
	cur_iter = file_content_vector.begin();
	while (cur_iter != file_content_vector.end())
	{
		std::string	clean_str;

		clean_str = ft_strtrim(*cur_iter, this->whitespace);
		if (clean_str.size() == 0 || clean_str[0] == '#')
		{
			cur_iter++;
			continue ;
		}
		std::vector<std::string>	word_list;
		
		word_list = ft_split(*cur_iter, this->whitespace);
		// server block만 확인하므로 server block 이외에 라인에서는 "server"로 시작하지 않으면 error이다.
		if (word_list[0] != "server")
			return (false);
		if (findServerBlock(cur_iter, file_content_vector.end(), begin_iter, end_iter) == false)
			return (false);
		std::vector<std::string>	serverBlockVector(begin_iter, end_iter);

		if (parseServerBlock(serverBlockVector) == false)
			return (false);
		// std::cout << std::endl << std::endl << std::endl;
		cur_iter++;
	}
	return (true);
}

/*
- std::vector<ServerConfig>	webserv_config;
- struct ServerConfig
	{
		std::string								host;					// 필수 필드, IPv4 style "0.0.0.0"
		uint16_t								port;					// 필수 필드, 0~65525 범위 내에 존재해야함
		std::string								root;					// 필수 필드, 빈 문자열 입력은 허용하지 않음
		std::string								index;					// 필수 필드, 빈 문자열 입력은 허용하지 않음
		size_t									client_max_body_size;	// 필수 필드, 양수만 허용, atoi로 변환하여 처리, 숫자가 아닌 문자열인 경우는?, isdigit()사용
		std::map<int, std::string>				default_error_page;		// config에는 없지만 error_page에 없는 status code에 대한 default error page 경로를 담고 있음, 항상 값이 있음
		std::map<int, std::string>				error_page;				// 선택 필드, 복수값 입력가능, 입력값 없으면 size() == 0
		std::string								server_name;			// 선택 필드, 유일값, 입력 없으면 ""
		std::map<std::string, LocationConfig>	locations;				// 선택 필드, 입력값 없으면 size() == 0
	};

- struct LocationConfig
	{
		// std::string					location_path;	// 필수 필드 key로 이동 (std::map<std::string, LocationConfig>	locations)
		std::map<Method, bool>		allow_method;	// 선택 필드, 입력값 없으면 default를 MethodType enum으로 초기화
		bool						autoindex;		// 선택 필드, 입력값 없으면 default를 false -> "off"를 의미
		std::string					root;			// 선택 필드, 입력값 없으면 ""
		std::string					index;			// 선택 필드, 입력값 없으면 ""
		std::string					redirect;		// 선택 필드, 입력값 없으면 "" , default로 301 code만 들어온다고 가정
		// to be added cgi infomation!!!
		//예상 std::map<std::string, CgiInfo>	cgis;
	};
*/
bool ConfigInfo::parseServerBlock(std::vector<std::string> server_block_vec)
{
	ServerConfig										server_config;
	std::map<std::string, ValidateFieldInfo>			server_field_map;
	std::map<std::string, ValidateFieldInfo>::iterator	map_iter;
	std::map<std::string, ValidateFieldInfo>::iterator	server_field_map_iter;
	std::vector<std::string>::iterator 					cur_iter;
	std::vector<std::string>::iterator 					begin_iter;
	std::vector<std::string>::iterator 					end_iter;
	std::vector<std::string>::iterator 					src_begin_iter;
	std::vector<std::string>::iterator 					src_end_iter;

	src_begin_iter = server_block_vec.begin();
	src_end_iter = server_block_vec.end();
	server_field_map = getValidateServerFieldMap();
	// std::cout << "field_map.size() : " << server_field_map.size() << std::endl;
	cur_iter = src_begin_iter;
	// for (; cur_iter != src_end_iter; cur_iter++)
	while (cur_iter != src_end_iter)
	{
		std::string					clean_str;
		std::vector<std::string>	word_list;
		std::string					first_word;

		printContent(*cur_iter, "cur_iter", PUP);
		clean_str = ft_strtrim(*cur_iter, this->whitespace);
		// if (clean_str.size() == 0 || clean_str[0] == '#')
		if (clean_str.size() == 0 || clean_str[0] == '#' || clean_str[0] == '{' || clean_str[0] == '}')
		{
			cur_iter++;
			continue ;
		}
		// printContent(clean_str, "clean_str", CYN);
		word_list = ft_split(clean_str, this->whitespace);
		first_word = *(word_list.begin());
		// printVector(word_list, "parseServerBlock", GRN);
		// printContent(first_word, "first_word", BLU);
		if (first_word == "location")
		{
			if (findLocationBlock(cur_iter, src_end_iter, begin_iter, end_iter) == false)
				return (false);
			// for (std::vector<std::string>::iterator tmp_iter = begin_iter; tmp_iter != end_iter; tmp_iter++)
			// 	printContent(*tmp_iter, "Locatoin", RED);
			// if (parseLocationBlock(begin_iter, end_iter) == false)
			// 	return (false);
			server_field_map[first_word]++;
			// std::cout << std::endl << std::endl << std::endl;
		}
		else
		{
			server_field_map_iter = server_field_map.find(first_word);
			if (server_field_map_iter == server_field_map.end())
			{
				std::cerr << RED <<  "Could not found field " << first_word << WHI<<std::endl;
				return (false);
			}
			else
			{
				std::vector<std::string>::iterator	vec_iter;
				ValidateFieldInfo					field_info;
				std::string							field_name;
				std::string							field_value;

				field_name = server_field_map_iter->first;
				field_info = server_field_map_iter->second;
				// printContent(field_name, "field_name", GRN);
				// printVector(word_list, "word_list", PUP);
				// for(vec_iter = word_list.begin(); vec_iter != word_list.end(); vec_iter++)
				// 	printContent(*vec_iter, "*vec_iter", PUP);
				// printContent(first_word, "first_word", BLU);
				
				/*
					key value; #
					key value ; #
					error_page를 제외하고는 getCount == 0 이어야 정상
					location은 별도 block 에서 확인하며
					parse 할때 혹은 default_error_page는 생성자에서 값을 넣어준다.
				*/
				field_value = *(word_list.begin() + 1);
				if (first_word == "host")
					parseHostConfigField(server_config, field_value);
				else if (first_word == "port")
				{
					// parsePortConfigField(field_value);
				}
				else if (first_word == "root")
				{
				}
				else if (first_word == "index")
				{
				}
				else if (first_word == "client_max_body_size")
				{
					// if (parseClientMaxBodySizeConfigField(field_value) == false)
					// 	return (false);
				}
				else if (first_word == "server_name")
				{
				}
				else if (first_word == "error_page")
				{
					// if (parseErrorPageConfigField(clean_str) == false)
					// 	return (false);
				}
				else
				{
					//find()이후라 없을 것이지만 혹시 모르니 체크
					std::cerr << RED <<  "Could not found field " << first_word << WHI<<std::endl;
					return (false);
				}
				// printContent(first_word, "first_word", RED);
			}
		}
		
		// first_word.at(first_word);
		cur_iter++;
	}
	return (true);
}


bool        ConfigInfo::validateServerBlock(std::vector<std::string> server_block_vec)
{
	std::map<std::string, ValidateFieldInfo>			validate_server_field_map;
	std::map<std::string, ValidateFieldInfo>::iterator	map_iter;
	std::map<std::string, ValidateFieldInfo>::iterator	validate_server_field_map_iter;
	std::vector<std::string>::iterator 					cur_iter;
	std::vector<std::string>::iterator 					begin_iter;
	std::vector<std::string>::iterator 					end_iter;
	std::vector<std::string>::iterator 					src_begin_iter;
	std::vector<std::string>::iterator 					src_end_iter;


	src_begin_iter = server_block_vec.begin();
	src_end_iter = server_block_vec.end();
	validate_server_field_map = getValidateServerFieldMap();
	// std::cout << "validate_field_map.size() : " << validate_server_field_map.size() << std::endl;
	cur_iter = src_begin_iter;
	// for (; cur_iter != src_end_iter; cur_iter++)
	while (cur_iter != src_end_iter)
	{
		std::string					clean_str;
		std::vector<std::string>	word_list;
		std::string					first_word;

		// printContent(*iter, "iter", PUP);
		clean_str = ft_strtrim(*cur_iter, this->whitespace);
		// if (clean_str.size() == 0 || clean_str[0] == '#')
		if (clean_str.size() == 0 || clean_str[0] == '#' || clean_str[0] == '{' || clean_str[0] == '}')
		{
			cur_iter++;
			continue ;
		}
		// printContent(clean_str, "clean_str", CYN);
		word_list = ft_split(clean_str, this->whitespace);
		// printVector(word_list, "word", GRN);
		// key value";" comment
		// #comment
		// if (word_list.begin() != word_list.end())
		first_word = *(word_list.begin());
		// printContent(first_word, "first_word", BLU);
		if (first_word == "location")
		{
			// printContent(first_word, "first_word", RED);
			// printContent(*cur_iter, "before *cur_iter", GRN);
			if (findLocationBlock(cur_iter, src_end_iter, begin_iter, end_iter) == false)
				return (false);
			// printContent(*begin_iter, "*begin_iter", PUP);
			// printContent(*end_iter, "*end_iter", PUP);
			// printContent(*cur_iter, "*cur_iter", PUP);
			// for (std::vector<std::string>::iterator	tmp_iter = begin_iter; tmp_iter != end_iter; tmp_iter++)
			// 	printContent(*tmp_iter, "Locatoin Block", BRW);
			if (validateLocationBlock(begin_iter, end_iter) == false)
				return (false);
			validate_server_field_map[first_word]++;
			// std::cout << std::endl << std::endl << std::endl;
		}
		else
		{
			validate_server_field_map_iter = validate_server_field_map.find(first_word);
			if (validate_server_field_map_iter == validate_server_field_map.end())
			{
				std::cerr << RED <<  "Could not found field " << first_word << WHI<<std::endl;
				return (false);
			}
			else
			{
				std::vector<std::string>::iterator	vec_iter;
				ValidateFieldInfo					validate_field_info;
				std::string							field_name;
				std::string							field_value;

				field_name = validate_server_field_map_iter->first;
				validate_field_info = validate_server_field_map_iter->second;
				// printContent(field_name, "field_name", GRN);
				// printVector(word_list, "word_list", PUP);
				// for(vec_iter = word_list.begin(); vec_iter != word_list.end(); vec_iter++)
				// 	printContent(*vec_iter, "*vec_iter", PUP);
				// printContent(first_word, "first_word", BLU);
				
				/*
					key value; #
					key value ; #
					error_page를 제외하고는 getCount == 0 이어야 정상
					location은 별도 block 에서 확인하며
					parse 할때 혹은 default_error_page는 생성자에서 값을 넣어준다.
				*/
				field_value = *(word_list.begin() + 1);
				if (first_word == "host")
				{
					if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
					if (checkCommonConfigLineForm(word_list) == false)
						return (false);
					if (checkHostConfigField(field_value) == false)
						return (false);
					validate_server_field_map[field_name]++; // count increament!!!
				}
				else if (first_word == "port")
				{
					if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
					if (checkCommonConfigLineForm(word_list) == false)
						return (false);
					if (checkPortConfigField(field_value) == false)
						return (false);
					validate_server_field_map[field_name]++; // count increament!!!
				}
				else if (first_word == "root")
				{
					if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
					if (checkCommonConfigLineForm(word_list) == false)
						return (false);
					validate_server_field_map[field_name]++; // count increament!!!
				}
				else if (first_word == "index")
				{
					if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
					if (checkCommonConfigLineForm(word_list) == false)
						return (false);
					validate_server_field_map[field_name]++; // count increament!!!
				}
				else if (first_word == "client_max_body_size")
				{
					if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
					if (checkCommonConfigLineForm(word_list) == false)
						return (false);
					if (checkClientMaxBodySizeConfigField(field_value) == false)
						return (false);
					validate_server_field_map[field_name]++; // count increament!!!
				}
				else if (first_word == "server_name")
				{
					if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
					if (checkCommonConfigLineForm(word_list) == false)
						return (false);
					validate_server_field_map[field_name]++; // count increament!!!
				}
				else if (first_word == "error_page")
				{
					if (checkErrorPageConfigField(clean_str) == false)
						return (false);
				}
				else
				{
					//find()이후라 없을 것이지만 혹시 모르니 체크
					std::cerr << RED <<  "Could not found field " << first_word << WHI<<std::endl;
					return (false);
				}
				// printContent(first_word, "first_word", RED);
			}
		}
		
		// first_word.at(first_word);
		cur_iter++;
	}
	

	if (checkNessaryOrUniqueField(validate_server_field_map) == false)
		return (false);
	return (true);
}

bool ConfigInfo::parseLocationBlock(std::vector<std::string>::iterator &src_begin_iter, 
									const std::vector<std::string>::iterator &src_end_iter)
{
	std::map<std::string, ValidateFieldInfo>			validate_location_field_map;
	std::map<std::string, ValidateFieldInfo>::iterator	map_iter;
	std::map<std::string, ValidateFieldInfo>::iterator	validate_location_field_map_iter;
	std::vector<std::string>::iterator 					cur_iter;

	validate_location_field_map = getValidateLocationFieldMap();
	cur_iter = src_begin_iter;
	for (; cur_iter != src_end_iter; cur_iter++)
	{
		std::string					clean_str;
		std::vector<std::string>	word_list;
		std::string					first_word;

		clean_str = ft_strtrim(*cur_iter, this->whitespace);
		// if (clean_str.size() == 0 || clean_str[0] == '#')
		if (clean_str.size() == 0 || clean_str[0] == '#' || clean_str[0] == '{' || clean_str[0] == '}')
			continue ;
		// printContent(clean_str, "clean_str", BRW);
		word_list = ft_split(clean_str, this->whitespace);
		// printVector(word_list, "word", GRN);
		// key value";" comment
		// #comment
		first_word = *(word_list.begin());
		validate_location_field_map_iter = validate_location_field_map.find(first_word);
		if (validate_location_field_map_iter == validate_location_field_map.end())
			return (false);
		else
		{
			std::vector<std::string>::iterator	vec_iter;
			ValidateFieldInfo					validate_field_info;
			std::string							field_name;

			field_name = validate_location_field_map_iter->first;
			validate_field_info = validate_location_field_map_iter->second;

			/*
				key value; #
				key value ; #
				error_page를 제외하고는 getCount == 0 이어야 정상
				location은 별도 block 에서 확인하며
				parse 할때 혹은 default_error_page는 생성자에서 값을 넣어준다.
			*/
			if (first_word == "allow_method")
			{
				if (checkDuplicateConfigField(validate_field_info) == false)
					return (false);
				if (checkAllowMethodConfigField(clean_str) == false)
					return (false);
				// allow method에 맞는 validate 함수 필요
				// 여러개의 값이 들어올 수 있으며 마지막 값이 경로로 들어간다.
				// 중간에 오는 값들은 GET, POST, DELTE가 될 수 있으며 
				// GET, POST, DELET가 중복되어서는 안된다.
					//-> 중복을 허용해줄지는 추가 고려 필요
				// if (checkCommonConfigLineForm(word_list) == false)
				// 	return (false);
				validate_location_field_map[field_name]++; // count increament!!!
			}
			else if (first_word == "autoindex")
			{
				if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
				if (checkCommonConfigLineForm(word_list) == false)
					return (false);
				std::string 	value_with_semicolon_comment;
				std::string 	value;
				
				// checkCommonConfigLineForm() 이후라서 ';'이 무조건 있다. 어디에 있느냐에 차이, "value;" or "value" ";#"
				// ';', '#' 이후를 잘라내서 value만 남도록 하는 것이 목적.
				// 이미 각 줄마다 checkCommonConfigLineForm() 통해 유효성 검사를 한 이후이므로 #은 ;이후에 있긴하다
				value_with_semicolon_comment = *(word_list.begin() + 1);
				value = removeAfterSemicolon(value_with_semicolon_comment);
				if (value != "on" && value != "off") // 입력값에 대해서는 대소문자 구분없이도 되게할지는 추후 고민 후 수정하거나 그대로 놔둠.
					return (false);
				validate_location_field_map[field_name]++; // count increament!!!
			}
			else if (first_word == "root")
			{
				if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
				if (checkCommonConfigLineForm(word_list) == false)
					return (false);
				validate_location_field_map[field_name]++; // count increament!!!
			}
			else if (first_word == "index")
			{
				if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
				if (checkCommonConfigLineForm(word_list) == false)
					return (false);
				validate_location_field_map[field_name]++; // count increament!!!
			}
			else if (first_word == "redirect")
			{
				if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
				if (checkCommonConfigLineForm(word_list) == false)
					return (false);
				validate_location_field_map[field_name]++; // count increament!!!
			}
			else if (first_word == "location")
			{
				if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
					validate_location_field_map[field_name]++; // count increament!!!
			}
			else
			{
				//find()이후라 없을 것이지만 혹시 모르니 체크
				std::cerr << RED <<  "Could not found field in validateLocationBlock()" << first_word << WHI<<std::endl;
				return (false);
			}
		}
	}
	return (true);
}

/*

*/
/*
IPv4
[123].[123].[123].[123];
- ;이 있다면 제거
	; or # 이전까지만 value에 담아둠
- 각 '.'과 '.'사이는 최대 3자리 숫자
- '.'의 개수는 3개
*/
void ConfigInfo::parseHostConfigField(ServerConfig& server_config, std::string field_value)
{
	std::string	ip_class;
	size_t		dot_count;

	printContent(field_value, "field_value before removeAfterSemicolon", GRN);
	field_value = removeAfterSemicolon(field_value);
	printContent(field_value, "field_value after removeAfterSemicolon", BRW);
	server_config.setHost(field_value);
}

/*
	1. 유효필드 검사를 위해 map<std::string, ValidateFieldInfo>를 사용한다.
		struct ValidateFieldInfo
		{
			VaildateFieldType	validate_field_type;
			size_t			count;
		};

		enum	VaildateFieldType
		{
			NESSARY_UNIQUE
			NESSARY_MULTI
			OPTION_UNIQUE
			OPTION_MULTI
		};

	2. 각 줄을 읽으면서 validate한지 확인한다.
		- 각 field 마다 value type, value range 등의 제한조건이 다르다.
		- ;
		- # 확인 필요
	3. locatoin이 나오면 find location block을 쓴다.

*/
bool        ConfigInfo::validateLocationBlock(std::vector<std::string>::iterator& src_begin_iter,
										const std::vector<std::string>::iterator& src_end_iter)
{
	std::map<std::string, ValidateFieldInfo>			validate_location_field_map;
	std::map<std::string, ValidateFieldInfo>::iterator	map_iter;
	std::map<std::string, ValidateFieldInfo>::iterator	validate_location_field_map_iter;
	std::vector<std::string>::iterator 					cur_iter;

	validate_location_field_map = getValidateLocationFieldMap();
	cur_iter = src_begin_iter;
	for (; cur_iter != src_end_iter; cur_iter++)
	{
		std::string					clean_str;
		std::vector<std::string>	word_list;
		std::string					first_word;

		clean_str = ft_strtrim(*cur_iter, this->whitespace);
		// if (clean_str.size() == 0 || clean_str[0] == '#')
		if (clean_str.size() == 0 || clean_str[0] == '#' || clean_str[0] == '{' || clean_str[0] == '}')
			continue ;
		// printContent(clean_str, "clean_str", BRW);
		word_list = ft_split(clean_str, this->whitespace);
		// printVector(word_list, "word", GRN);
		// key value";" comment
		// #comment
		first_word = *(word_list.begin());
		validate_location_field_map_iter = validate_location_field_map.find(first_word);
		if (validate_location_field_map_iter == validate_location_field_map.end())
		{
			printContent(first_word, "first_word in validateLocationBlock()", RED);
			return (false);
		}
		else
		{
			std::vector<std::string>::iterator	vec_iter;
			ValidateFieldInfo					validate_field_info;
			std::string							field_name;

			field_name = validate_location_field_map_iter->first;
			validate_field_info = validate_location_field_map_iter->second;

			/*
				key value; #
				key value ; #
				error_page를 제외하고는 getCount == 0 이어야 정상
				location은 별도 block 에서 확인하며
				parse 할때 혹은 default_error_page는 생성자에서 값을 넣어준다.
			*/
			if (first_word == "allow_method")
			{
				if (checkDuplicateConfigField(validate_field_info) == false)
					return (false);
				if (checkAllowMethodConfigField(clean_str) == false)
					return (false);
				// allow method에 맞는 validate 함수 필요
				// 여러개의 값이 들어올 수 있으며 마지막 값이 경로로 들어간다.
				// 중간에 오는 값들은 GET, POST, DELTE가 될 수 있으며 
				// GET, POST, DELET가 중복되어서는 안된다.
					//-> 중복을 허용해줄지는 추가 고려 필요
				// if (checkCommonConfigLineForm(word_list) == false)
				// 	return (false);
				validate_location_field_map[field_name]++; // count increament!!!
			}
			else if (first_word == "autoindex")
			{
				if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
				if (checkCommonConfigLineForm(word_list) == false)
					return (false);
				std::string 	value_with_semicolon_comment;
				std::string 	value;
				
				// checkCommonConfigLineForm() 이후라서 ';'이 무조건 있다. 어디에 있느냐에 차이, "value;" or "value" ";#"
				// ';', '#' 이후를 잘라내서 value만 남도록 하는 것이 목적.
				// 이미 각 줄마다 checkCommonConfigLineForm() 통해 유효성 검사를 한 이후이므로 #은 ;이후에 있긴하다
				value_with_semicolon_comment = *(word_list.begin() + 1);
				value = removeAfterSemicolon(value_with_semicolon_comment);
				if (value != "on" && value != "off") // 입력값에 대해서는 대소문자 구분없이도 되게할지는 추후 고민 후 수정하거나 그대로 놔둠.
					return (false);
				validate_location_field_map[field_name]++; // count increament!!!
			}
			else if (first_word == "root")
			{
				if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
				if (checkCommonConfigLineForm(word_list) == false)
					return (false);
				validate_location_field_map[field_name]++; // count increament!!!
			}
			else if (first_word == "index")
			{
				if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
				if (checkCommonConfigLineForm(word_list) == false)
					return (false);
				validate_location_field_map[field_name]++; // count increament!!!
			}
			else if (first_word == "redirect")
			{
				if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
				if (checkCommonConfigLineForm(word_list) == false)
					return (false);
				validate_location_field_map[field_name]++; // count increament!!!
			}
			else if (first_word == "location")
			{
				if (checkDuplicateConfigField(validate_field_info) == false)
						return (false);
					validate_location_field_map[field_name]++; // count increament!!!
			}
			else
			{
				//find()이후라 없을 것이지만 혹시 모르니 체크
				std::cerr << RED <<  "Could not found field in validateLocationBlock()" << first_word << WHI<<std::endl;
				return (false);
			}
		}
	}
	return (true);
}


/*
	- #주석이후로는 제외
	- key value 짝이 맞아야함
		- value가 list로 나올 수도 있다.
		- 들어온 key가 유효한지, 중복이 된것은 없는지 확인해야한다.
			- 소문자만 허용!!
			- host, port index, server_name, root, client_max_body_size
		- value역시 해당 key에 허용하는 형식인지 확인해야한다
	- vector 요소의 가장 처음 것은 key이거나 {, 주석이다
	- 혹은 비어있거나
	- {, } 이후에 무언가 있다면 바로 다음 것이 주석 이어야한다.
	- 주석이나 {,} 기호가 없다면 ";"으로 끝나야한다
	- server, location block 에서 확인하는 validation이 다르다.

	block을 찾는다.
	server,location이 나오면 그 다음에 나오는 word_list에서 값은 '{' 이어야한다.
	그렇지 않다면 error
	begin_iter는 server, location 이후에 나오는 '{'부터이다.
	end_iter는 '}'
*/
bool	ConfigInfo::checkWhole(const std::string& file_content)
{
	std::string							str;
	std::vector<std::string>			file_content_vector;
	std::vector<std::string>::iterator 	cur_iter;
	std::vector<std::string>::iterator 	begin_iter;
	std::vector<std::string>::iterator 	end_iter;
	
	file_content_vector = ft_split(file_content, "\n");
	cur_iter = file_content_vector.begin();
	while (cur_iter != file_content_vector.end())
	{
		std::string	clean_str;

		clean_str = ft_strtrim(*cur_iter, this->whitespace);
		if (clean_str.size() == 0 || clean_str[0] == '#')
		{
			cur_iter++;
			continue ;
		}
		std::vector<std::string>	word_list;
		
		word_list = ft_split(*cur_iter, this->whitespace);
		// server block만 확인하므로 server block 이외에 라인에서는 "server"로 시작하지 않으면 error이다.
		if (word_list[0] != "server")
			return (false);
		if (findServerBlock(cur_iter, file_content_vector.end(), begin_iter, end_iter) == false)
			return (false);
		std::vector<std::string>	serverBlockVector(begin_iter, end_iter);

		if (validateServerBlock(serverBlockVector) == false)
			return (false);
		std::cout << std::endl << std::endl << std::endl;
		cur_iter++;
	}
	return (true);
}

/*
	curly bracket pair
	block이 제대로 되었는지 확인을 추가시킬지 포함 
		- server, locatoin 이후에 {}이 왔는지
		- {}이 아예 안오는 경우
			다른 key에 {가 붙는 경우
		- {}만 들어오는 케이스에 대한 처리
			- "{" "}" 다음에는 아무것도 없거나 주석만 가능
		뜬금없는 위치이지만 {}만 있는 케이스는 놔둔다.
		이 부분까지 checkBracket에서 처리할려면 "{trtlkfldaf}", "234234{trtlkfldaf}23424" 같은 케이스도 확인해야함
		- 한 줄씩 확인할때 유효키인지 정도만 확인하며 value값이 경우는 문자열을 받는 value는 확인하지 않.는다.
			e.g) index "{}"
				- root "{sdf}"
				- location_path "{}"
				이런 경우들은 확인하지 않음.
*/
bool	ConfigInfo::validateConfigFile(const std::string& file_content)
{
	if (checkCurlyBracketPair(file_content) == false)
		return (false);
	if (checkWhole(file_content) == false)
		return (false);
	return (true);
}


//TODO remove
void	test(void)
{
	system("leaks ConfigInfo");
}

int main(int argc, char *argv[])
{
	// std::string line;
	ConfigInfo	configPaser;

	if (argc != 2) {
		std::cout << "Usage : string" << std::endl;
		return (1);
	}
	try
	{
		configPaser.parseConfig(argv[1]);
		/* code */
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	// atexit(test);
	return (0);
}
