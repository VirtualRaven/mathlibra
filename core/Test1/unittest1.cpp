#include "stdafx.h"
#include "CppUnitTest.h"
#include "tests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test1
{		
	TEST_CLASS(UnitTest1)
	{
		Assert assert;
	public:
		
		TEST_METHOD(MemoryTest)
		{
			assert.IsTrue(test::memory_module_test1());
			
		}
		TEST_METHOD(EvaluationTest)
		{
			try
			{
				assert.IsTrue(test::argEvaluation() == 100);
			}
			catch (exception& e)
			{

				Logger::WriteMessage(std::string("[\n Exception: " + std::string(e.what()) + "\n").c_str());
				Logger::WriteMessage(std::string(" Description: " + std::string(e.desc()) + "\n]\n").c_str());
			
			}
			
			
		}

	};
}