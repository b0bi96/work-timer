#include <stdexcept>
#include <string>

class ClientException : public std::runtime_error
{
public:
    ClientException(const std::string &msg) : std::runtime_error(msg) {}
};
