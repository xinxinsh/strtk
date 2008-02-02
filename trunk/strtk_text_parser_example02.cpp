/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Text Parser Example 02                                          *
 * Author: Arash Partow - 2002                                     *
 * URL: http://www.partow.net/programming/strtk/index.html         *
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

const unsigned int list_size = 17;
const std::string not_of_interest_list [list_size] = { "as", "at", "but", "by", "for",
                                                       "in", "like", "next", "of", "on",
                                                       "opposite", "out", "past", "to",
                                                       "up", "via", ""};

template<typename Container, typename Predicate>
struct parse_line
{
public:
   parse_line(Container& c, Predicate& p)
   : c_(c),
     p_(p),
     tmp_(""),
     tokenizer_(tmp_,p_,true),
     filter_on_match_(reinterpret_cast<const std::string*>(not_of_interest_list),
                      reinterpret_cast<const std::string*>(not_of_interest_list + list_size),
                      strtk::range_to_string_back_inserter_iterator<Container>(c_),true,false){}

   inline void operator() (const std::string& s)
   {
      strtk::for_each_token(s,tokenizer_,filter_on_match_);
   }

private:
   Container& c_;
   Predicate& p_;
   std::string tmp_;
   typename strtk::std_string_tokenizer<Predicate>::type tokenizer_;
   typename strtk::filter_on_match< strtk::range_to_string_back_inserter_iterator<Container> > filter_on_match_;
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