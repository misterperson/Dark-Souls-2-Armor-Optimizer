#include "AOException.hpp"

AOException::AOException(const char * message, AOError _err)
  : std::exception{message}, err{_err}
{
}
