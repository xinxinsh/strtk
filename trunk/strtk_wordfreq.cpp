/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Word Frequency Calculator                                     *
 * Author: Arash Partow (2002-2010)                              *
 * URL: http://www.partow.net/programming/strtk/index.html       *
 *                                                               *
 * Copyright notice:                                             *
 * Free use of the String Toolkit Library is permitted under the *
 * guidelines and in accordance with the most current version of *
 * the Common Public License.                                    *
 * http://www.opensource.org/licenses/cpl1.0.php                 *
 *                                                               *
 *****************************************************************
*/


/*
  Description: This example demonstrates the how one can calculate the
               word frequency model of given piece of text. Input is taken
               either from stdin or a user specified file. Once the text
               has been processed, the frequencies of each word is printed
               out to stdout.
*/


#include <cstdio>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <map>

#include "strtk.hpp"


typedef std::map<std::string,unsigned int> map_t;

template<typename Predicate>
struct line_parser
{
public:

   line_parser(unsigned long long& word_count,
               map_t& map,
               Predicate& p)
   : word_count_(word_count),
     map_(map),
     p_(p)
   {}

   inline void operator() (const std::string& s)
   {
      if (s.empty()) return;

      static map_t& m = map_;
      static unsigned long long& wc = word_count_;

      class map_insert_or_inc
      {
      public:
         void operator()(const std::pair<const char*,const char*>& r)
         {
            if (r.first == r.second) return;
            ++wc;
            std::string str(r.first,r.second);
            strtk::convert_to_lowercase(str);
            map_t::iterator itr = m.find(str);
            if (m.end() != itr)
               ++itr->second;
            else
               m.insert(std::make_pair(str,1));
         }
      };

      strtk::split(p_,
                   s.c_str(), s.c_str() + s.size(),
                   strtk::functional_inserter(map_insert_or_inc()),
                   strtk::split_options::compress_delimiters);
   }

private:

   unsigned long long& word_count_;
   map_t& map_;
   Predicate& p_;
};

int main(int argc, char* argv[])
{
   typedef strtk::multiple_char_delimiter_predicate predicate_t;
   typedef line_parser<const predicate_t> lp_t;

   strtk::ext_string delimiters = strtk::ext_string::all_chars() -
                                  strtk::ext_string::all_lowercase_letters() -
                                  strtk::ext_string::all_uppercase_letters();

   static const predicate_t predicate(delimiters.as_string());

   map_t word_list;
   unsigned long long word_count = 0;

   switch (argc)
   {
                // Consume input from stdin
      case 1  : strtk::for_each_line<lp_t>(std::cin,lp_t(word_count,word_list,predicate));
                break;

                // Consume input from user specified file
      case 2  : strtk::for_each_line<lp_t>(argv[1],lp_t(word_count,word_list,predicate));
                break;

      default :
               {
                  std::cout << "usage: strtk_wordfreq <file name>" << std::endl;
                  std::cout << "usage: cat words.txt | strtk_wordfreq" << std::endl;
                  return 1;
               }
   }

   std::cout << "Word count: " << word_count << std::endl;
   map_t::iterator itr = word_list.begin();
   while (word_list.end() != itr)
   {
      printf("%s %10d %10.9f\n",
             strtk::text::right_align(15,' ',itr->first).c_str(),
             itr->second,
             (1.0 * itr->second) / word_count);
      ++itr;
   }

   return 0;
}
