//#include "asyncsoc.h"

//using namespace std;

//AsyncSoc::handle AsyncSoc::connect(std::size_t block_size)
//{
//	static int id = 0;
//	m_handlers.emplace(++id, Handler(block_size));
//	return id;
//}

//void AsyncSoc::receive(handle handle, const char *data, std::size_t size)
//{
//	m_handlers[handle].input(string(data, size));
//}

//void AsyncSoc::disconnect(handle handle)
//{
//	m_handlers.erase(m_handlers.find(handle));
//}
