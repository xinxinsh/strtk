#include <iostream>
#include <iterator>
#include <string>
#include <deque>
#include <ctime>

#include "strtk.hpp"

int main(int argc, char* argv[])
{
   std::deque<std::string> str_lst;

   switch (argc)
   {
      case 1 : strtk::load_from_text_file(std::cin,str_lst);
               break;

      case 2 : strtk::load_from_text_file(argv[1],str_lst);
               break;

      default :
               {
                  std::cout << "usage: randomizer <file name>" << std::endl;
                  std::cout << "usage: cat data.txt | randomizer" << std::endl;
                  return 1;
               }
   }

   if (!str_lst.empty())
   {
      strtk::random_permutation(str_lst.begin(),str_lst.end(),
                                std::ostream_iterator<std::string>(std::cout,"\n"),
                                static_cast<std::size_t>(::time(0)));
   }
   return 0;
}
