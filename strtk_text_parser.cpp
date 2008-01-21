/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Text File Parser Example                                        *
 * Author: Arash Partow - 2003                                     *
 * URL: http://www.partow.net                                      *
 *                                                                 *
 * Copyright notice:                                               *
 * Free use of the String Tool Kit Library is permitted under the  *
 * guidelines and in accordance with the most current version of   *
 * the Common Public License.                                      *
 * http://www.opensource.org/licenses/cpl.php                      *
 *                                                                 *
 *******************************************************************
*/


#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <deque>

#include "strtk.hpp"

template<typename Container, typename Predicate>
struct parse_line
{
private:
   typedef std::deque<strtk::std_string_tokenizer<char>::iterator_type> local_list_type;

public:
   parse_line(Container& c, const Predicate& p)
   : c_(c),
     p_(p)
   {}

   inline void operator() (const std::string& s)
   {
      strtk::split(s,p_,std::back_inserter(local_list_),true);
      for(local_list_type::const_iterator it = local_list_.begin();
          it != local_list_.end();
          ++it)
      {
         c_.push_back(std::string((*it).first,(*it).second));
      }
      local_list_.resize(0);
   }

private:
   local_list_type local_list_;
   Container& c_;
   const Predicate& p_;
};

template<typename Container>
void parse_text(const std::string& file_name, Container& c)
{
   std::string delimiters = " ,.;:<>'[]{}()_?/'`~!@#$%^&*|-_\"=+";
   strtk::multiple_char_delimiter_predicate predicate(delimiters);
   unsigned int line_count = for_each_line(file_name,parse_line<Container,strtk::multiple_char_delimiter_predicate>(c,predicate));
}

int main(void)
{
   std::string text_file_name = "text.txt";
   std::deque< std::string > word_list;
   parse_text(text_file_name,word_list);
   std::cout << "Token Count: " << word_list.size() << std::endl;
   return 0;
}
