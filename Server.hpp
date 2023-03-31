#ifndef SERVER_HPP
# define SERVER_HPP

# include "Webserv.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"

class Server {
	private:
		Socket						serverSock;
		std::vector<struct kevent>	changeList;
		struct kevent				eventList[8];
		std::map<int, std::string>	clientData;

	public:
		Server() {};
		Server(const Server& copy) {};
		virtual ~Server() { close(serverSock.getFD()); };

		void	CreateListenSock();
		void	Run();
		void	ChangeEvent(uintptr_t ident,
							int16_t filter,
							uint16_t flags,
							uint32_t fflags,
							intptr_t data,
							void* udata);
};

#endif

