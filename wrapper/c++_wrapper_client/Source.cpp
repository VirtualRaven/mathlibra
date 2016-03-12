#include "test.h"
#include "main_wrapper.h"
#include "c++_wrapper/export.h"
#include "ptr_protect.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <map>

void trim(std::string& str)
{
    for(auto it =str.begin(); it != str.end(); it++)
    {
       if(*it==' ')
       {
            str.erase(it); 
       } else break;
    }
    for(auto it =--str.end(); it != --str.begin() ; it--)
    {
       if(*it==' ')
       {
            str.erase(it); 
       } else break;
    }
}
bool startsWith(std::string str, std::string prefix)
{
    auto it = str.begin();
   for(char s : prefix)
   {
        if(s != *it++)
        {
         return false;   
        }    
   }
  return true; 
}

bool run_test_cases(std::map<string,vector<string>> test)
{
        bool test_failed = false;
        for(auto it = test.begin(); it != test.end(); it++)
        {
               auto test_res = runTest(it->second); 
               if(test_res == RETURN_VAL::SUCCESS)
               {
                    std::cout << "[ TEST: " << it->first  << " (OK) ]" << std::endl; 
               }
               else if (test_res == RETURN_VAL::MEM_ERROR)
               {

                   std::cout << "[ TEST: " << it->first<< " (mem leak) ]" << std::endl;
                   test_failed = true;
               }
               else 
               {
                   std::cout << "[ TEST: " << it->first<< " (failed) ]" << std::endl;
                   test_failed = true;
               }
        }
        return !test_failed;
}

bool read_test_file(std::string filename)
{
    std::ifstream infile(filename, std::istream::in); 
    std::map<string,vector<string>> tests;   
    int linecounter =0;
    if(infile.good())
    {
        while(!infile.eof())
        {
            std::string line;
            std::getline(infile,line);
            trim(line);
            if(line.empty())
            {
                continue;
            }
            linecounter++;
            if(startsWith(line,"#test-"))
            {   
                if(line.size() <= 6)
                {
                    std::cerr << "(Line "<< linecounter << ") Expected test name after test declaration" << std::endl; 
                    return false; 
                }
                std::string test_name = line.substr(6,string::npos);
                if ( tests.count("test_name") != 0)
                {
                    std::cerr << "(Line " << linecounter << ") Test "<< test_name << " already defined" << std::endl;
                    return false;
                }
                std::vector<string> testLines;
                while(infile.good())
                {
                    std::getline(infile,line);
                    linecounter++;
                    if(startsWith(line,"#"))
                    {   
                        break;
                    }
                    else
                    {
                        testLines.push_back(std::move(line)); 
                        
                    }
                }
                if(testLines.size() ==0)
                {
                    std::cerr << "(Line " << linecounter << ") Expected atleat one test statement in " << test_name << std::endl;
                    return false;

                }
                if(line != "#endtest")
                {
                   std::cerr << "(Line " << linecounter << ") Expected end of test statment" << std::endl;
                   return false;
                } 
                tests[test_name] = std::move(testLines);
                    

            }
            else
            {
                std::cerr << "(Line " << linecounter << ") Expected test declaration" << std::endl;
                return false;
            }   

        }
        return run_test_cases(tests);
                
    }
    else
    {
        std::cerr << "Failed to open " << filename << "\n";
        return false;
    }

}


int main(int argc, char* argv[]  )
{
    std::vector<std::string> tests; 
    const std::string flag = "-f";
    for(int i =1; i< argc; i++)
    {
        std::string arg(argv[i]);
        trim(arg);

        if(arg==flag)
        {
              if(i+1<argc)
              {
                  std::string file = argv[i+1];
                  trim(file);
                  tests.push_back(file);
                  ++i;
              } 
              else
              {
                std::cout << "Expected filename after f flag" << std::endl;
                return 1;
              }
        }
        else
        {
            std::cout << "Unknown flag" << std::endl;  
            return 1;
        }
    }
    std::ofstream out("testlog.txt");
    if(!out.good())
    {
        std::cout << "Failed to create log file\n";
        return 1;
    }
    auto cerrbuf = std::cerr.rdbuf(out.rdbuf());
    bool test_success=true;
    for(std::string item : tests)
    {
        std::cout << "[ Running " << item << " ]\n";
        test_success &= read_test_file(item); 
    }
    if(!test_success){
        std::cerr.rdbuf(cerrbuf);
        return 1;
    }
    else
    {
        std::cerr.rdbuf(cerrbuf);
        return 0;
    }
}
