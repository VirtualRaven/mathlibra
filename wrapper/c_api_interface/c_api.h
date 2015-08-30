extern "C"
{
	struct handle_obj;
	typedef handle_obj*  handle;
	handle create_handle();
	void free_handle(handle);
	void interpret_arg(handle,char *);
	double execute_arg(handle);
	bool mathlibra_error(handle);
	const char* mathlibra_error_info(handle);
	void enable_plugins(handle);	
}
