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
 * http://www.opensource.org/licenses/cpl1.0.php                   *
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

   key_value_parser(const key_value_parser& kvp)
   : c_(kvp.c_),
     t_(kvp.t_)
   {}

   key_value_parser& operator=(const key_value_parser& kvp)
   {
      if (this != &kvp)
      {
         c_ = kvp.c_;
         t_ = kvp.t_;
      }
      return *this;
   }

   void operator()(const typename Tokenizer::iterator::value_type& v)
   {
      std::pair<std::string,std::string> kv;
      strtk::parse(v.first,v.second,t_,kv.first,kv.second);
      c_.push_back(kv);
   }

private:
   Container& c_;
   Tokenizer& t_;
};

int main(void)
{
   std::string data = "int=-1|unsigned int=2345|double=6.789|string=a simple string";

   typedef strtk::single_delimiter_predicate<std::string::value_type> predicate_type1;
   typedef strtk::multiple_char_delimiter_predicate predicate_type2;

   typedef strtk::std_string_tokenizer<predicate_type1>::type tokenizer_type1;
   typedef strtk::std_string_tokenizer<predicate_type2>::type tokenizer_type2;

   typedef key_value_parser<std::deque< std::pair<std::string,std::string> >,tokenizer_type2> kvp_type;

   tokenizer_type1 pair_tokenizer(data,predicate_type1('|'));
   tokenizer_type2 key_value_tokenizer(data,predicate_type2("="),true);

   std::deque< std::pair<std::string,std::string> > keyvalue_list;

   kvp_type kvp(keyvalue_list,key_value_tokenizer);

   strtk::for_each_token(data,pair_tokenizer,kvp);
   std::deque< std::pair<std::string,std::string> >::iterator it = keyvalue_list.begin();
   while(it != keyvalue_list.end())
   {
      std::cout << "<" << (*it).first << "," << (*it).second << ">" << std::endl;
      ++it;
   }

   return 0;
}
