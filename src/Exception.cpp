 // ============================================================================
//
// = CONTEXT
//    ITest Power Supplies - C++ Interface - Error & ItestException Classes
//
// = FILENAME
//    ItestException.cpp
//
// = AUTHOR
//    Nicolas Leclercq - SOLEIL
//
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "Exception.h"

namespace itest {

// ============================================================================
// Error::Error
// ============================================================================
Error::Error (void)
  :  reason ("unknown error"),
     desc ("unknown error"),
     origin ("unknown"),
     severity (itest::ERR),
     code (-1)
{
 //- noop ctor 
}

// ============================================================================
// Error::Error
// ============================================================================
Error::Error (const char *_reason,
              const char *_desc,
              const char *_origin,
              int _severity,
              int _code)
  :  reason (_reason),
     desc (_desc),
     origin (_origin),
     severity (_severity),
     code (_code)
{
 //- noop ctor 
}

// ============================================================================
// Error::Error
// ============================================================================
Error::Error (const std::string& _reason,
	            const std::string& _desc,
	            const std::string& _origin,
              int _severity,
              int _code)
  :  reason (_reason),
     desc (_desc),
     origin (_origin),
     severity (_severity),
     code (_code)
{
 //- noop ctor 
}


// ============================================================================
// Error::Error
// ============================================================================
Error::Error (const Error& _src)
  :  reason (_src.reason),
     desc (_src.desc),
     origin (_src.origin),
     severity (_src.severity),
     code (_src.code)
{
 //- noop ctor 
}

// ============================================================================
// Error::~Error
// ============================================================================
Error::~Error (void)
{
 //- noop dtor
}

// ============================================================================
// Error::operator=
// ============================================================================
Error& Error::operator= (const Error& _src) 
{
  //- no self assignment
  if (this == &_src)
    return *this;

  this->reason = _src.reason;
  this->desc = _src.desc;
  this->origin = _src.origin;
  this->severity = _src.severity;
  this->code = _src.code;

  return *this;
}

// ============================================================================
// ItestException::ItestException
// ============================================================================
ItestException::ItestException (void) 
  : errors(0)
{
  this->push_error(Error());
}

// ============================================================================
// ItestException::ItestException
// ============================================================================
ItestException::ItestException (const char *_reason,
                      const char *_desc,
                      const char *_origin,
                      int _severity,
                      int _code)
  : errors(0)
{
  this->push_error(_reason, _desc, _origin, _severity, _code);
}

// ============================================================================
// ItestException::ItestException
// ============================================================================
ItestException::ItestException (const std::string& _reason,
                      const std::string& _desc,
                      const std::string& _origin,
                      int _severity,
                      int _code)
  : errors(0)
{
  this->push_error(_reason, _desc, _origin, _severity, _code);
}

// ============================================================================
// ItestException::ItestException
// ============================================================================
ItestException::ItestException (const ItestException& _src) 
  : errors(0)
{
  this->errors = _src.errors;
}

// ============================================================================
// ItestException::ItestException
// ============================================================================
ItestException::ItestException (const ErrorList& _errors) 
{
  this->errors = _errors;
}

// ============================================================================
// ItestException::ItestException
// ============================================================================
ItestException& ItestException::operator= (const ItestException& _src) 
{
  //- no self assignment
  if (this == &_src) {
    return *this;
  }

  this->errors.clear();

  for (size_t i = 0; i < _src.errors.size();  i++)
    this->push_error(_src.errors[i]);

  return *this;
}

// ============================================================================
// ItestException::~ItestException
// ============================================================================
ItestException::~ItestException (void)
{
  this->errors.clear();
}

// ============================================================================
// ItestException::push_error
// ============================================================================
void ItestException::push_error (const char *_reason,
    					              const char *_desc,
					                  const char *_origin,
                            int _severity,
                            int _err_code)
{
  this->errors.push_back(Error(_reason, _desc, _origin, _severity, _err_code));
}

// ============================================================================
// ItestException::push_error
// ============================================================================
void ItestException::push_error (const std::string& _reason,
    					              const std::string& _desc,
					                  const std::string& _origin, 
                            int _err_code, 
                            int _severity)
{
  this->errors.push_back(Error(_reason, _desc, _origin, _severity, _err_code));
}

// ============================================================================
// ItestException::push_error
// ============================================================================
void ItestException::push_error (const Error& _error)
{
  this->errors.push_back(_error);
}

} // namespace itest


