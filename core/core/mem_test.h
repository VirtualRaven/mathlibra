
namespace debug
{
	void check_tree_mem_leak(); /**< Uses util::coubtable to detect memory leaks. It Checks if the total number of allocated mathnode objects corresponds to the total deletion count. If check fails it frows an exception.**/
        bool mem_error_occured() noexcept;
}

