// ============================================================================
//
// = CONTEXT
//    ITest Power Supplies - C++ Interface - Error & Exception Classes
//
// = FILENAME
//    Exception.h
//
// = AUTHOR
//    Nicolas Leclercq - SOLEIL
//
// ============================================================================

#ifndef _ITEST_EXCEPTION_H_
#define _ITEST_EXCEPTION_H_

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include <string>
#include <vector>

namespace itest {

// ============================================================================
// Error severity
// ============================================================================
typedef enum {
  WARN,
  ERR,
  PANIC
} ErrorSeverity;

// ============================================================================
// Error abstraction class
// ============================================================================
class Error
{
public:

  /**
   * Initialization. 
   */
  Error (void);

  /**
   * Initialization. 
   */
  Error (const char *reason,
				 const char *desc,
				 const char *origin,
	       int severity = itest::ERR,
         int err_code = -1);

  /**
   * Initialization. 
   */
  Error (const std::string& reason,
				 const std::string& desc,
				 const std::string& origin,
	       int severity = itest::ERR,
         int err_code = -1);

  /**
   * Copy constructor. 
   */
  Error (const Error& src);

  /**
   * Error details: code 
   */
  virtual ~Error (void);

  /**
   * operator= 
   */
  Error& operator= (const Error& _src);

  /**
   * Error details: reason 
   */
  std::string reason;

  /**
   * Error details: description 
   */
  std::string desc;

  /**
   * Error details: origin 
   */
  std::string origin;

  /**
   * Error details: severity 
   */
  int severity;
  
  /**
   * Error details: code 
   */
  int code;
};

// ============================================================================
// Error list
// ============================================================================
typedef std::vector<Error> ErrorList;

// ============================================================================
// Exception class
// ============================================================================
class ItestException
{
public:

  /**
   * Default constructor
   */
  ItestException (void);

  /**
   * Constructor
   */
  ItestException (const char *reason,
					        const char *desc,
					        const char *origin,
	                int severity = itest::ERR,
                  int err_code = -1);

  /**
   * Constructor
   */
  ItestException (const std::string& reason,
					        const std::string& desc,
					        const std::string& origin,
	                int severity = itest::ERR,
                  int err_code = -1);

  /**
   * Constructor
   */
  ItestException (const Error& error);

  /**
   * Constructor
   */
  ItestException (const ErrorList& errors);

  /**
   * Copy constructor
   */
  ItestException (const ItestException& src);

  /**
   * operator=
   */
  ItestException& operator= (const ItestException& _src);

  /**
   * Destructor
   */
  virtual ~ItestException (void);

  /**
   * Push the specified error into the errors list.
   */
  void push_error (const char *reason,
					         const char *desc,
						       const char *origin,
		               int severity = itest::ERR,
                   int err_code = -1);

  /**
   * Push the specified error into the errors list.
   */
  void push_error (const std::string& reason,
                   const std::string& desc,
                   const std::string& origin,
                   int severity = itest::ERR,
                   int err_code = -1);

  /**
   * Push the specified error into the errors list.
   */
  void push_error (const Error& error);

  /**
   * The errors list
   */
   ErrorList errors;
};

} // namespace itest

#endif // _ITEST_EXCEPTION_H_

