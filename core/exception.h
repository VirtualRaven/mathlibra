
#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED
#include <string>


/**
 * Main exception class. This class contains the basic exception class used in the core and its modules. 
 *  All exceptions the program throws shall be derived from this class.
 */
class exception
{
private:
	/**
	 * String containing exception info. info contains extended information about the exception.
	 * info is returned by desc
	 * @see desc()
	 */
	const char* _info;
	const char* _type;
	unsigned short _id;
	/**
	 * Variable defining if exception is severe. 
	 * If set to true the error is consider non recoverable and as such the program will terminate
	 * @see critical()
	 */
	bool _isCritical;

public:

		
	exception(const char *  type, const char * inf,bool isCritical ,unsigned short eid); /**< 	Standrad exception constructor. @param type And string describing the category. @param inf An string containing an extended error description. @param isCritical A bool defining if error is recoverable. @param id, the exeptions unique id*/
	virtual const char * what(); /**< Returns the type of exception. what() is an abstract method which describe the exception. @pure. @return A c style string describing the exception type.  */
	bool critical(); /**< Gets the exception severity. @return True if exception is non recoverable, else false. */
	unsigned short id(); /**<Retruns exception identification number*/ 
    	const char* desc(); /**< Gets the exception description. @return A string describing the exception. @see info. */


};
#endif //EXCEPTION_H_INCLUDED
