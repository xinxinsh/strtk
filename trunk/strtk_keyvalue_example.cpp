/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Key-Value Pair Example 01                                       *
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

template<typename Container, typename Tokenizer>
struct key_value_parser
{
public:

   key_value_parser(Container& c, Tokenizer& t)
   : c_(c),
     t_(t){}

   void operator()(const typename Tokenizer::iterator::value_type& v)
   {
      t_.assign(v.first,v.second);
      std::pair<std::string,std::string> kv;
      strtk::parse(t_,kv.first,kv.second);
      c_.push_back(kv);
   }

private:

   key_value_parser(const key_value_parser& kvp){}
   key_value_parser& operator=(const key_value_parser& kvp){}

   Container& c_;
   Tokenizer& t_;
};

int main(void)
{
   std::string data = "int=-1|unsigned int=2345|double=6.789|string=a simple string";

   typedef strtk::single_delimiter_predicate<std::string::value_type> predicate_type1;
   typedef strtk::multiple_char_delimiter_predicate predicate_type2;

   strtk::std_string_tokenizer<predicate_type1>::type pair_tokenizer(data,predicate_type1('|'));
   strtk::std_string_tokenizer<predicate_type2>::type key_value_tokenizer(data,predicate_type2("="),true);

   std::deque< std::pair<std::string,std::string> > keyvalue_list;

   typedef key_value_parser<std::deque< std::pair<std::string,std::string> >,strtk::std_string_tokenizer<predicate_type2>::type> kv_parser_type;

   strtk::for_each_token<strtk::std_string_tokenizer<predicate_type1>::type,kv_parser_type>(data,pair_tokenizer,kv_parser_type(keyvalue_list,key_value_tokenizer));

   std::deque< std::pair<std::string,std::string> >::iterator it = keyvalue_list.begin();

   while(it != keyvalue_list.end())
   {
      std::cout << "<" << (*it).first << "," << (*it).second << ">" << std::endl;
      ++it;
   }

   return 0;
}