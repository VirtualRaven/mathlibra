
namespace util
{	
	template<typename T>class countable;

	template<typename T> struct object_stats /**<provides methods to query allocation stats regarding the the object T*/
	{
		unsigned int get_total()
		{
			return countable<T>::__total_count;	
		}
		unsigned int get_current()
		{
			return countable<T>::__current_count;
		}
				
		unsigned int get_deleted()
		{
			return countable<T>::__deleted_count;
		}
	};

		
	template<typename T> class countable /**< A class that implements an counter. All objects that derives this class can be queried for allocation stats. @seealso object_stats*/
	{
		static unsigned int __current_count;
		static unsigned int __total_count;
		static unsigned int __deleted_count;
		friend object_stats<T>;
		public:
		countable()
		{
			__total_count++;
			__current_count++;
		}

		~countable()
		{
			__current_count--;
			__deleted_count++;
		}
		
	};



	template<typename T> unsigned int util::countable<T>::__current_count=0;
	template<typename T> unsigned int util::countable<T>::__total_count=0;
	template<typename T> unsigned int util::countable<T>::__deleted_count=0;
}
