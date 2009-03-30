/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Examples                                                        *
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
#include <string>
#include <iterator>
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
   strtk::std_string_tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer(s,predicate,true);
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

void tokenizer_example05()
{
   const unsigned int data_size = 12;
   unsigned int data[data_size] = {1,2,3,0,4,5,6,0,7,8,0,9};
   strtk::single_delimiter_predicate<unsigned int> predicate(0);
   strtk::tokenizer< unsigned int*,strtk::single_delimiter_predicate<unsigned int> > tokenizer(data,data + data_size,predicate);
   strtk::tokenizer< unsigned int*,strtk::single_delimiter_predicate<unsigned int> >::iterator it = tokenizer.begin();
   while (it != tokenizer.end())
   {
      std::cout << "[";
      std::copy((*it).first,(*it).second,std::ostream_iterator<unsigned int>(std::cout," "));
      std::cout << "]";
      ++it;
   }
   std::cout << std::endl;
}

void tokenizer_example06()
{
   const unsigned int data_size = 12;
   unsigned int data[data_size] = {1,2,3,0,4,5,6,10,7,8,0,9};
   unsigned int delimiters[2] = {0,10};
   strtk::multiple_delimiter_predicate<unsigned int> predicate(delimiters,delimiters + 2);
   strtk::tokenizer< unsigned int*,strtk::multiple_delimiter_predicate<unsigned int> > tokenizer(data,data + data_size,predicate);
   strtk::tokenizer< unsigned int*,strtk::multiple_delimiter_predicate<unsigned int> >::iterator it = tokenizer.begin();
   while (it != tokenizer.end())
   {
      std::cout << "[";
      std::copy((*it).first,(*it).second,std::ostream_iterator<unsigned int>(std::cout," "));
      std::cout << "]";
      ++it;
   }
   std::cout << std::endl;
}

void tokenizer_example07()
{
   const unsigned int data_size = 12;
   double data[data_size] = {1.1,2.2,3.3,0.0,4.4,5.5,6.6,0,7.7,8.8,0,9.9};
   strtk::single_delimiter_predicate<double> predicate(0);
   strtk::tokenizer< double*,strtk::single_delimiter_predicate<double> > tokenizer(data,data + data_size,predicate);
   strtk::tokenizer< double*,strtk::single_delimiter_predicate<double> >::iterator it = tokenizer.begin();
   while (it != tokenizer.end())
   {
      std::cout << "[";
      std::copy((*it).first,(*it).second,std::ostream_iterator<double>(std::cout," "));
      std::cout << "]";
      ++it;
   }
   std::cout << std::endl;
}

void tokenizer_example08()
{
   const unsigned int data_size = 12;
   double data[data_size] = {1.1,2.2,3.3,0.0,4.4,5.5,6.6,10.0,7.7,8.8,10.0,9.9};
   double delimiters[2] = {0.0,10.0};
   strtk::multiple_delimiter_predicate<double> predicate(delimiters,delimiters + 2);
   strtk::tokenizer< double*,strtk::multiple_delimiter_predicate<double> > tokenizer(data,data + data_size,predicate);
   strtk::tokenizer< double*,strtk::multiple_delimiter_predicate<double> >::iterator it = tokenizer.begin();
   while (it != tokenizer.end())
   {
      std::cout << "[";
      std::copy((*it).first,(*it).second,std::ostream_iterator<double>(std::cout," "));
      std::cout << "]";
      ++it;
   }
   std::cout << std::endl;
}

void tokenizer_example09()
{
   std::string s = "abc|123|xyz|789";
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::std_string_tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type tokenizer(s,predicate);
   std::list<strtk::std_string_tokenizer<std::string::value_type>::iterator_type> token_list;
   std::copy(tokenizer.begin(),tokenizer.end(),std::back_inserter(token_list));
}

void tokenizer_example10()
{
   const unsigned int str_list_size = 12;
   std::string str_list[str_list_size] = { "abc" , "delimiter" , "ijk" , "delimiter" ,
                                           "lmn" , "delimiter" , "opq", "rst"  ,"uvw" ,
                                           "delimiter" , "xyz" , "123" };

   strtk::range_adapter<std::string> range(str_list,str_list_size);
   strtk::single_delimiter_predicate<std::string> predicate("delimiter");
   strtk::tokenizer< std::string*,strtk::single_delimiter_predicate<std::string> > tokenizer(range.begin(),range.end(),predicate);
   strtk::tokenizer< std::string*,strtk::single_delimiter_predicate<std::string> >::iterator it = tokenizer.begin();
   while(it != tokenizer.end())
   {
      std::cout << "[" << strtk::join(" ",(*it).first,(*it).second) << "]";
      ++it;
   }
   std::cout << std::endl;
}

void tokenizer_example11()
{
   std::string s = "abc|123|xyz|789";
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::std_string_tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type tokenizer(s,predicate);
   strtk::std_string_tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type::iterator it = tokenizer.begin();

   std::list<std::string> token_list;
   std::copy(tokenizer.begin(),tokenizer.end(),strtk::range_to_string_back_inserter(token_list));
   std::copy(token_list.begin(),token_list.end(),std::ostream_iterator<std::string>(std::cout,"\t"));
   std::cout << std::endl;
}

void tokenizer_example12()
{
   // tokenizer_example01 with much simpler syntax.
   std::string s = "abc|123|xyz|789";
   strtk::std_string_tokenizer<>::type tokenizer(s,strtk::std_string_tokenizer<>::predicate_type('|'));
   strtk::std_string_tokenizer<>::type::iterator it = tokenizer.begin();
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
   std::list< strtk::std_string_tokenizer<>::iterator_type > token_list;
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::split(s,predicate,std::back_inserter(token_list));
   std::list<strtk::std_string_tokenizer<>::iterator_type>::iterator it = token_list.begin();
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
   std::list< strtk::std_string_tokenizer<>::iterator_type > token_list;
   strtk::multiple_char_delimiter_predicate predicate(" .;?");
   strtk::split(s,predicate,std::back_inserter(token_list));
   std::list<strtk::std_string_tokenizer<>::iterator_type>::iterator it = token_list.begin();
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
   std::list< strtk::std_string_tokenizer<>::iterator_type > token_list;
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::split(s,predicate,std::back_inserter(token_list),true);
   std::list<strtk::std_string_tokenizer<>::iterator_type>::iterator it = token_list.begin();
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
   std::list< strtk::std_string_tokenizer<>::iterator_type > token_list;
   strtk::multiple_char_delimiter_predicate predicate(" .;?");
   strtk::split(s,predicate,std::back_inserter(token_list),true);
   std::list<strtk::std_string_tokenizer<>::iterator_type>::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void offset_splitter_example01()
{
   std::string s = "abcdefghijklmnopqrstuvwxyz012";
   const int offset_list[] = {1,2,3,4,5,6,7};
   const strtk::offset_predicate<7> os_p(offset_list);
   std::list< strtk::std_string_tokenizer<>::iterator_type> token_list;
   strtk::offset_splitter(s.begin(),s.end(),os_p,std::back_inserter(token_list));
   std::list<strtk::std_string_tokenizer<>::iterator_type>::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "] ";
      ++it;
   }
   std::cout << std::endl;
}

void offset_splitter_example02()
{
   std::string s = "09:10:11.123";
   const int offset_list[] = {2,1,2,1,2,1,3};
   const strtk::offset_predicate<7> os_p(offset_list);
   std::list< strtk::std_string_tokenizer<>::iterator_type > token_list;
   strtk::offset_splitter(s.begin(),s.end(),os_p,std::back_inserter(token_list));
   std::list<strtk::std_string_tokenizer<>::iterator_type>::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      if (1 != std::distance((*it).first,(*it).second))
      {
         std::cout << "[" << std::string((*it).first,(*it).second) << "] ";
      }
      ++it;
   }
   std::cout << std::endl;
}

void offset_splitter_example03()
{
   std::string s = "abcdefghijklmnopqrstuvwxyz012";
   std::list< strtk::std_string_tokenizer<>::iterator_type > token_list;
   strtk::offset_splitter(s.begin(),s.end(),strtk::offsets(1,2,3,4,5,6,7),std::back_inserter(token_list));
   std::list<strtk::std_string_tokenizer<>::iterator_type>::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "] ";
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
   std::cout << output << std::endl;
}

void parse_example()
{
   std::string input = "abcd|x|-1234|78901|4567.8901";
   typedef strtk::std_string_tokenizer<>::predicate_type single_predicate_type;
   typedef strtk::std_string_tokenizer<>::type tokenizer_type;

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

void remove_inplace_example01()
{
   std::string s = "aa abb cdd  ee fg";
   std::cout << s << " -> ";
   strtk::remove_inplace(' ',s);
   std::cout << s << std::endl;
}

void remove_consecutives_example01()
{
   std::string s = "aaabbcddeefg";
   std::cout << s << " -> ";
   strtk::remove_consecutives_inplace(s);
   std::cout << s << std::endl;
}

void remove_consecutives_example02()
{
   std::string s = "aaabbcaaaddeeafg";
   std::cout << s << " -> ";
   strtk::remove_consecutives_inplace('a',s);
   std::cout << s << std::endl;
}

void remove_consecutives_example03()
{
   std::string s = "aaabbcaaaddeeafg";
   std::cout << s << " -> ";
   strtk::remove_consecutives_inplace(strtk::multiple_char_delimiter_predicate("abcdefg"),s);
   std::cout << s << std::endl;
}

void uri_extractor_example01()
{
   std::string text = "someone@somewhere.com http://www.test.com any.one@any.where.com ftp://123.abcxyz.com";
   std::list<std::string> email_list;
   std::list<std::string> url_list;
   strtk::split(strtk::email_expression,text,std::back_inserter(email_list));
   strtk::split(strtk::uri_expression,text,std::back_inserter(url_list));
   std::cout << "emails: ";
   std::copy(email_list.begin(),email_list.end(),std::ostream_iterator<std::string>(std::cout," "));
   std::cout << std::endl << "urls: ";
   std::copy(url_list.begin(),url_list.end(),std::ostream_iterator<std::string>(std::cout," "));
   std::cout << std::endl;
}

void generate_random_example01()
{
   const std::size_t data_size = 10;
   unsigned char* data = new unsigned char[data_size];
   strtk::generate_random_data(data,data_size,1000000);
   unsigned char* hex_data = new unsigned char[2 * data_size];
   strtk::convert_bin_to_hex(data,data + data_size, hex_data);
   std::copy(hex_data,hex_data + 2 * data_size,std::ostream_iterator<unsigned char>(std::cout));
   std::cout << std::endl;
   delete [] data;
   delete [] hex_data;
}

int main(void)
{
   tokenizer_example01();
   tokenizer_example02();
   tokenizer_example03();
   tokenizer_example04();
   tokenizer_example05();
   tokenizer_example06();
   tokenizer_example07();
   tokenizer_example08();
   tokenizer_example09();
   tokenizer_example10();
   tokenizer_example11();
   tokenizer_example12();
   split_example01();
   split_example02();
   split_example03();
   split_example04();
   offset_splitter_example01();
   offset_splitter_example02();
   offset_splitter_example03();
   construct_example();
   parse_example();
   remove_inplace_example01();
   remove_consecutives_example01();
   remove_consecutives_example02();
   remove_consecutives_example03();
   uri_extractor_example01();
   generate_random_example01();
   return 0;
}
