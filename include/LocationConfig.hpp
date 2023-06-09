#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <map>
# include <string>
# include "HTTPMessage.hpp"

class LocationConfig
{
	private:
		std::string						location_path;	// 필수 필드 key로 이동 (std::map<std::string, LocationConfig>	locations)
		std::string						root;			// 선택 필드, 입력값 없으면 ""
		std::string						index;			// 선택 필드, 입력값 없으면 ""
		std::string						redirect;		// 선택 필드, 입력값 없으면 "" , default로 301 code만 들어온다고 가정
		bool							autoindex;		// 선택 필드, 입력값 없으면 default를 false -> "off"를 의미
		std::map<MethodType, bool>		allow_method;	// 선택 필드, 입력값 없으면 default를 MethodType enum으로 초기화

	public:
		LocationConfig(void);
		virtual ~LocationConfig();

		// setter()
		void	setLocationPath(const std::string& _location_path);
		void	setRoot(const std::string& _root);
		void	setIndex(const std::string& _index);
		void	setRedirect(const std::string& _redirect);
		void	setAutoindex(const bool& _autoindex);
		void	setAllowMethod(const std::map<MethodType, bool>& _allow_method);

		// getter()
		const std::string&					getLocationPath(void) const;
		const std::string&					getRoot(void) const;
		const std::string&					getIndex(void) const;
		const std::string&					getRedirect(void) const;
		const bool&							getAutoindex(void) const;
		const std::map<MethodType, bool>&	getAllowMethod(void) const;

		// print 
		template <typename T>
		void	printContent(const T &val, std::string name, std::string color) const;

		void	printMapContent(const std::map<MethodType, bool> &pair_map, std::string name, std::string color) const ;
		void	printLocationConfingContent(std::string color)  const;
};


template <typename T>
inline void LocationConfig::printContent(const T &val, std::string name, std::string color) const
{
	std::cout << color << name << " : $" <<  val << "$" << WHI << std::endl;
}

inline void LocationConfig::printMapContent(const std::map<MethodType, bool> &pair_map, std::string name, std::string color) const
{
	std::cout << color << "-----" << name << ": " << pair_map.size() << "-----" << std::endl;
	for (std::map<MethodType, bool>::const_iterator iter = pair_map.begin(); iter != pair_map.end(); iter++)
	{
		std::cout << "$" << iter->first << ", " << iter->second << "$" << std::endl;
	}
	std::cout << WHI << std::endl;
}

#endif