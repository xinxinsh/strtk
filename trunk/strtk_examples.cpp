/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Examples                                                        *
 * Author: Arash Partow - 2002                                     *
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
#include <string>
#include <list>
#include "strtk.hpp"


void tokenizer_example01()
{
   std::string s = "abc|123|xyz|789";
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::std_string_tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type tokenizer(s,predicate);
   strtk::std_string_tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type::iterator it = tokenizer.begin();
   while (it != tokenizer.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void tokenizer_example02()
{
   std::string s = "abc.123 xyz?789";
   strtk::multiple_char_delimiter_predicate predicate(" .;?");
   strtk::std_string_tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer(s,predicate);
   strtk::std_string_tokenizer<strtk::multiple_char_delimiter_predicate >::type::iterator it = tokenizer.begin();
   while (it != tokenizer.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void tokenizer_example03()
{
   std::string s = "abc||123|||||xyz|789";
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::std_string_tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type tokenizer(s,predicate,true);
   strtk::std_string_tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type::iterator it = tokenizer.begin();
   while (it != tokenizer.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void tokenizer_example04()
{
   std::string s = "abc.;123? xyz;?789";
   strtk::multiple_char_delimiter_predicate predicate(" .;?");
   strtk::std_string_tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer(s,predicate,true);
   strtk::std_string_tokenizer<strtk::multiple_char_delimiter_predicate >::type::iterator it = tokenizer.begin();
   while (it != tokenizer.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void split_example01()
{
   std::string s = "abc|123|xyz|789";
   std::list< std::pair< std::string::const_iterator,std::string::const_iterator > > token_list;
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::split(s,predicate,std::back_inserter(token_list));
   std::list< std::pair< std::string::const_iterator,std::string::const_iterator > >::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void split_example02()
{
   std::string s = "abc?123,xyz;789";
   std::list< std::pair< std::string::const_iterator,std::string::const_iterator > > token_list;
   strtk::multiple_char_delimiter_predicate predicate(" .;?");
   strtk::split(s,predicate,std::back_inserter(token_list));
   std::list< std::pair< std::string::const_iterator,std::string::const_iterator > >::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void split_example03()
{
   std::string s = "abc|123|xyz|789";
   std::list< std::pair< std::string::const_iterator,std::string::const_iterator > > token_list;
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::split(s,predicate,std::back_inserter(token_list),true);
   std::list< std::pair< std::string::const_iterator,std::string::const_iterator > >::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void split_example04()
{
   std::string s = "abc?123,xyz;789";
   std::list< std::pair< std::string::const_iterator,std::string::const_iterator > > token_list;
   strtk::multiple_char_delimiter_predicate predicate(" .;?");
   strtk::split(s,predicate,std::back_inserter(token_list),true);
   std::list< std::pair< std::string::const_iterator,std::string::const_iterator > >::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void construct_example()
{
   std::string  i1 = "abcd";
   char         i2 = 'x';
   int          i3 = -1234;
   unsigned int i4 = 78901;
   double       i5 = 4567.8901;
   std::string output = "";
   strtk::construct(output,"|",i1,i2,i3,i4,i5);
   std::cout << "construct_example() - " << output << std::endl;
}

void parse_example()
{
   std::string input = "abcd|x|-1234|78901|4567.8901";
   typedef strtk::single_delimiter_predicate<std::string::value_type> single_predicate_type;
   typedef strtk::std_string_tokenizer<single_predicate_type>::type tokenizer_type;

   single_predicate_type single_predicate('|');
   tokenizer_type tokenizer(input,single_predicate);

   std::string  o1 = "";
   char         o2 = 0x00;
   int          o3 = 0;
   unsigned int o4 = 0;
   double       o5 = 0;

   strtk::parse(input,tokenizer,o1,o2,o3,o4,o5);

   std::cout << o1 << "\t"
             << o2 << "\t"
             << o3 << "\t"
             << o4 << "\t"
             << o5 << std::endl;
}

int main(void)
{
   tokenizer_example01();
   tokenizer_example02();
   tokenizer_example03();
   tokenizer_example04();
   split_example01();
   split_example02();
   split_example03();
   split_example04();
   construct_example();
   parse_example();
   return 0;
}
