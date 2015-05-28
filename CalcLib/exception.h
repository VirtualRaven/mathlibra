/**
 * Main exception class. This class contains the basic exception class used in the core and its modules. 
 *  All exceptions the program throws shall be derived from this class.
 */

#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED
#include <string>
#include "main.h"


class exception
{
protected:
	/**
	 * String containing exception info. info contains extended information about the exception.
	 * info is returned by desc
	 * @see desc()
	 */
	std::string info;
	/**
	 * Variable defining if exception is severe. 
	 * If set to true the error is consider non recoverable and as such the program will terminate
	 * @see critical()
	 */
	bool _isCritical;
public:

		
	exception(std::string inf,bool isCritical); /**< 	Standrad exception construct. @param inf An string containing an extended error description. @param isCritical A bool defining if error is recoverable. */
	exception(); /**< Empty constructor. Used to create an empty exception. */
	virtual ~exception(); /**< Standrad destructor */
	virtual const char * what()=0; /**< Returns the type of exception. what() is an abstract method which describe the exception. @pure. @return A c style string describing the exception type.  */
	bool critical(); /**< Gets the exception severity. @return True if exception is non recoverable, else false. */
    std::string desc(); /**< Gets the exception description. @return A string describing the exception. @see info. */


};
#endif //EXCEPTION_H_INCLUDED
