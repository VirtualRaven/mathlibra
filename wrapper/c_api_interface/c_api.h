extern "C"
{
	typedef void* handle;
	handle create_handle();
	void free_handle(handle);
	void interpret_arg(handle,char *);
	double execute_arg(handle);
	bool mathlibra_error(handle);
	void enable_plugins(handle);	
}
