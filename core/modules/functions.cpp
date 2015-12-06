#include "modules/functions.h"
#include "exception_helper.h"
//Contains temporary implementation of function module
namespace math_func
{
	template<EXCEPTION T> void functionOops()
	{
		__mathlibra__raise<T,FUNCTION>();
	}

	typedef double(*double_func)(double);


	math_func::m_function::m_function(std::string name,std::string tag,std::string doc,std::string disp_name, funcPtr ptr) :type(func_type::FAST),
			name(name), 
			tag(tag),
			doc(doc),
			disp_name(disp_name),
			ptr(ptr) {}

	math_func::m_function::m_function(std::string name,std::string tag,std::string doc,std::string disp_name, generalFuncPtr ptr) : type(func_type::GENERAL), 
				name(name), 
				tag(tag),
				doc(doc),
				disp_name(disp_name),
				gptr(ptr) {}

	math_func::m_function::~m_function(){}

	math_func::m_function::m_function() :type(func_type::FAST),
					     name(), 
					     tag(),
					     doc(),
					     disp_name(),
					     ptr(nullptr){}
        math_func::m_function::m_function(std::string name, std::string tag,std::string doc,std::string disp_name,interpreted_func* ptr): type(func_type::USER),
       				  name(name),
				  tag(tag),
			       	  doc(doc),
			       	  disp_name(disp_name),
				  uptr(ptr) {}

		void function_interface::load(std::vector< m_function>& obj)
		{
			funcs.insert(funcs.end(),obj.begin(),obj.end());
		}
		void function_interface::load(m_function obj)
		{
			funcs.push_back(obj);
		}
		load_test_return  function_interface::isloaded(std::string funcName)
		{
			for (unsigned int i = 0; i < funcs.size(); i++)
			{
				if (funcs[i].name == funcName)
				{
					cache = funcs[i];
					return {true, cache.type == func_type::USER };
				}
			}
			return {false,false};
		}
		
		func_type function_interface::type()
		{
			return this->cache.type;
		}
		u_ptr function_interface::get(std::string funcName)
		{
			if (cache.name == funcName)
			{
				if(cache.type == func_type::USER)
                                {
					functionOops<FUNC_NO_CONV_FUNCTOR_VOID_P>();
                                }
                                return reinterpret_cast<u_ptr>(cache.ptr);
			}
			else
			{

				for (unsigned int i = 0; i < funcs.size(); i++)
				{
					if (funcs[i].name == funcName)
					{
                                            if(funcs[i].type == func_type::USER)
                                            {
						functionOops<FUNC_NO_CONV_FUNCTOR_VOID_P>();
                                            }
					    else return reinterpret_cast<u_ptr>(funcs[i].ptr);
					}
				}
				return nullptr;
			}
		}
                
                interpreted_func* function_interface::getFunctor(std::string funcName)
                {
                    if(cache.name ==funcName && cache.type == USER)
                    {
                            return cache.uptr;
                    }
                    else
                    {
                        for(unsigned int i =0; i < funcs.size(); i++)
                        {
                            if(funcs[i].name == funcName)
                            {
                                return cache.uptr;
                            }
                        }
                        return nullptr;
                    }   
                }
                void function_interface::display()
		{
			std::cout << "-[ function_interface {\n";
			for (unsigned int i = 0; i < funcs.size(); i++)
			{
				std::cout << this->funcs[i].name.c_str() << "\t: " << std::hex << this->funcs[i].ptr << std::dec << "\n";
			}
			std::cout << " }\nLoaded " << this->funcs.size() << " functions]\n";

		}
                function_interface::~function_interface()
                {
                        for(unsigned int i =0; i < funcs.size(); i++)
                        {
                            if(funcs[i].type == func_type::USER)
                            {
                                delete funcs[i].uptr;
                            }
                        }
                }
		std::vector<std::string> function_interface::getFunctionNames()
		{
			std::vector<std::string> vec;
			vec.reserve(this->funcs.size());
			for (unsigned int i = 0; i < funcs.size(); i++)
			{
				vec.push_back(this->funcs[i].name);
			}
			return vec;
		}
	
		std::vector<func_obj_info> function_interface::getFunctionObjs()
		{
			std::vector<func_obj_info> vec;
			vec.reserve(this->funcs.size());
			for (unsigned int i = 0; i < funcs.size(); i++)
			{
				vec.push_back( {
						this->funcs[i].name,
						this->funcs[i].tag,
						this->funcs[i].doc,
						this->funcs[i].disp_name
						});
			}
			return vec;


		}

		 std::vector< math_func::m_function> std_math_trig_func = {
			 math_func::m_function("sin","trig","sin(double), standard sinus functions","sin", static_cast<double_func>(sin)),
			 math_func::m_function("cos","trig","cos(double), standard cosinus function","cos", static_cast<double_func>(cos)),
			 math_func::m_function("tan","trig","tan(double), standard tan function","tan", static_cast<double_func>(tan)),
			 math_func::m_function("asin","trig","asindouble), inverse sin function","asin", static_cast<double_func>(asin)),
			 math_func::m_function("acos","trig","acos(double), inverse cosinus function","acos", static_cast<double_func>(acos)),
			 math_func::m_function("atan","trig","stan(double), inverse tan function","atan", static_cast<double_func>(atan))
		};

		std::vector<math_func::m_function> std_math_func = {
			//math_func::m_function("abs", static_cast<double(*)(double)>(abs)), //Only defined with double overload on certain platforms
			math_func::m_function("sqrt","numeric","sqrt(double), square root function function","\u221A", static_cast<double_func>(sqrt)),
			math_func::m_function("ln","numeric","ln(double), natural logarithm function","ln", static_cast<double_func>(log)),
			math_func::m_function("log","numeric","log(double),  logarithm base ten","log", static_cast<double_func>(log10))
		};

		std::vector<math_func::m_function> std_math_num_func = {
			math_func::m_function("ceil","numeric","ceil(double)","ceil", static_cast<double_func>(ceil)),
			math_func::m_function("floor","numeric","floor(double)","floor", static_cast<double_func>(floor)),
			math_func::m_function("round","numeric","round(double)","round", static_cast<double_func>(round))
		};
	
}

bool test::function_module_test1()
{
	math_func::function_interface func;
	func.load(math_func::std_math_trig_func);
	func.load(math_func::m_function("gen","","","", (math_func::m_function::generalFuncPtr)(nullptr)));
	if (!(func.isloaded("sin").loaded && func.isloaded("cos").loaded && func.isloaded("tan").loaded && func.isloaded("gen").loaded))
	{
		return false;
	}
	if ((math_func::m_function::generalFuncPtr)func.get("gen") != nullptr || func.get("sin") != reinterpret_cast<math_func::u_ptr>(sin))
	{
		return false;
	}
	func.isloaded("gen");
	if (func.type()  != math_func::GENERAL)
	{
		return false;
	}
	return false;
}
