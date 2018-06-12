#ifndef AO_EXCEPTION_HPP
#define AO_EXCEPTION_HPP

#include <exception>

class AOException : public std::exception
{
public:
  enum AOError
  {
     badfile = 0, // couldn't open 
     badalloc,    // couldn't allocate memory
     unknown      // something really, really bad happened
  };
  AOError err;


  AOException(const char *message, AOError _err);
  ~AOException() = default;
};

#endif //AO_EXCEPTION_HPP