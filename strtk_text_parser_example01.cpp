/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Text Parser Example 01                                          *
 * Author: Arash Partow - 2002                                     *
 * URL: http://www.partow.net/programming/strtk/index.html         *
 *                                                                 *
 * Copyright notice:                                               *
 * Free use of the String Tool Kit Library is permitted under the  *
 * guidelines and in accordance with the most current version of   *
 * the Common Public License.                                      *
 * http://www.opensource.org/licenses/cpl1.0.php                   *
 *                                                                 *
 *******************************************************************
*/


/*
  Description: The following will parse a text file line-by-line,
               tokenizing each line using the delimiters set as
               described blow. The objective is to populate the
               word_list with tokens derived from the text file.
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

   parse_line(const parse_line& pl)
   : c_(pl.c_),
     p_(pl.p_)
   {}

   parse_line& operator=(const parse_line& pl)
   {
      if (this != &pl)
      {
         c_ = pl.c_;
         p_ = pl.p_;
      }
      return *this;
   }

   inline void operator()(const std::string& s)
   {
      strtk::split(p_,s,std::back_inserter(local_list_),true);
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
   parse_line<Container,strtk::multiple_char_delimiter_predicate> pl(c,predicate);
   strtk::for_each_line(file_name,pl);
}

int main(void)
{
   std::string text_file_name = "text.txt";
   std::deque< std::string > word_list;
   parse_text(text_file_name,word_list);
   std::cout << "Token Count: " << word_list.size() << std::endl;
   return 0;
}
