#include "native_class_wrapper.h"
using namespace System;
using namespace System::Collections::Generic;

namespace calculator
{
	public ref class calc_engine
	{
		native::core_native_wrapper* wrp;
		void throwIfFailed();
	public:
		calc_engine();
		~calc_engine();
		
		///<summary>
		///Sets the expression which the interpreter shall work upon 
		///</summary>
		void SetArg(String^ str);
		
		///<summary>
		///Preformce analysis of provided expression to make it executable
		///</summary>
		bool InterpretArg();
		
		///<summary>
		///Executes the expression provided
		///</summary>
		///<returns>
		/// Returns the expressions eveluated value. If assigment, the assigned value is returned
		///</returns>
		double ExecuteArg();
	
			List<String^> ^ GetVariableNames();
			double GetVariableValue( String^ name);
			void SetVariable( String^ name, double value);
			void CreateVariable( String^ name, double value);
			void ClearVariables();

		

	};

	
}