/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Examples                                                        *
 * Author: Arash Partow (2002-2009)                                *
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
   strtk::std_string::tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type tokenizer(s,predicate);
   strtk::std_string::tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type::iterator it = tokenizer.begin();
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
   strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer(s,predicate,true);
   strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate >::type::iterator it = tokenizer.begin();
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
   strtk::std_string::tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type tokenizer(s,predicate,true);
   strtk::std_string::tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type::iterator it = tokenizer.begin();
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
   strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer(s,predicate,true);
   strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate >::type::iterator it = tokenizer.begin();
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
   strtk::std_string::tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type tokenizer(s,predicate);
   strtk::std_string::token_list_type token_list;
   std::copy(tokenizer.begin(),tokenizer.end(),std::back_inserter(token_list));
}

void tokenizer_example10()
{
   const unsigned int str_list_size = 12;
   std::string str_list[str_list_size] = { "abc" , "delimiter" , "ijk" , "delimiter" ,
                                           "lmn" , "delimiter" , "opq", "rst" ,"uvw" ,
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
   std::string s = "123|456|789|101112";
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::std_string::tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type tokenizer(s,predicate);
   strtk::std_string::tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type::iterator it = tokenizer.begin();

   std::list<int> token_list;
   std::copy(tokenizer.begin(),tokenizer.end(),strtk::range_to_type_back_inserter(token_list));
   std::copy(token_list.begin(),token_list.end(),std::ostream_iterator<int>(std::cout,"\t"));
   std::cout << std::endl;
}

void tokenizer_example12()
{
   // tokenizer_example01 with much simpler syntax.
   std::string s = "abc|123|xyz|789";
   strtk::std_string::tokenizer<>::type tokenizer(s,strtk::std_string::tokenizer<>::predicate_type('|'));
   strtk::std_string::tokenizer<>::type::iterator it = tokenizer.begin();
   while (it != tokenizer.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void tokenizer_example13()
{
   std::string s = "abc|123|xyz|789";
   strtk::std_string::tokenizer<>::type tokenizer(s,strtk::std_string::tokenizer<>::predicate_type('|'));
   strtk::std_string::tokenizer<>::type::iterator it = tokenizer.begin();
   std::cout << "[" << std::string((*it).first,(*it).second) << "]\t"; ++it;
   std::cout << "[" << std::string((*it).first,(*it).second) << "]\t"; ++it;
   std::cout << "Remaining string: " << it.remaining() << std::endl;
}

void split_example01()
{
   std::string s = "abc|123|xyz|789";
   strtk::std_string::token_list_type token_list;
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::split(predicate,s,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator it = token_list.begin();
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
   strtk::std_string::token_list_type token_list;
   strtk::multiple_char_delimiter_predicate predicate(" .;?");
   strtk::split(predicate,s,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator it = token_list.begin();
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
   strtk::std_string::token_list_type token_list;
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::split(predicate,s,std::back_inserter(token_list),strtk::split_options::compress_delimiters);
   strtk::std_string::token_list_type::iterator it = token_list.begin();
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
   strtk::std_string::token_list_type token_list;
   strtk::multiple_char_delimiter_predicate predicate(" .;?");
   strtk::split(predicate,s,std::back_inserter(token_list),strtk::split_options::compress_delimiters);
   strtk::std_string::token_list_type::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void split_example05()
{
   std::string s = "abc?123,xyz;789";
   strtk::std_string::token_list_type token_list;
   strtk::split(" .;?",s,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void split_regex_example()
{
   #ifdef ENABLE_REGEX
   std::string s = "(12)(345)(6789)(0ijkx)(yz)";
   std::list<std::string> token_list;
   strtk::split_regex("\\(.*?\\)",s,std::back_inserter(token_list));
   std::list<std::string>::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << (*it) << "]\t";
      ++it;
   }
   std::cout << std::endl;
   #endif
}

void split_n_example01()
{
   std::string s = "token1|token2|token3|token4|token5";
   strtk::std_string::token_list_type token_list;
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   const std::size_t token_count = 4;
   strtk::split_n(predicate,s,token_count,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void split_n_example02()
{
   std::string s = "token1?token2,token3;token4,token5";
   strtk::std_string::token_list_type token_list;
   strtk::multiple_char_delimiter_predicate predicate(" .;?");
   const std::size_t token_count = 4;
   strtk::split_n(predicate,s,token_count,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}

void split_n_example03()
{
   std::string s = "token1?token2,token3;token4,token5";
   strtk::std_string::token_list_type token_list;
   const std::size_t token_count = 4;
   strtk::split_n(" .;?",s,token_count,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "]\t";
      ++it;
   }
   std::cout << std::endl;
}


void split_regex_n_example()
{
   #ifdef ENABLE_REGEX
   std::string s = "(token1)(token2)(token3)(token4)(token5)";
   std::list<std::string> token_list;
   const std::size_t token_count = 4;
   strtk::split_regex_n("\\(.*?\\)",s,token_count,std::back_inserter(token_list));
   std::list<std::string>::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << (*it) << "]\t";
      ++it;
   }
   std::cout << std::endl;
   #endif
}

void offset_splitter_example01()
{
   std::string s = "abcdefghijklmnopqrstuvwxyz012";
   const int offset_list[] = {1,2,3,4,5,6,7};
   const strtk::offset_predicate<7> os_p(offset_list);
   strtk::std_string::token_list_type token_list;
   strtk::offset_splitter(s.begin(),s.end(),os_p,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator it = token_list.begin();
   while(it != token_list.end())
   {
      std::cout << "[" << std::string((*it).first,(*it).second) << "] ";
      ++it;
   }
   std::cout << std::endl;
}

void offset_splitter_example02()
{
   std::string s = "091011123";
   const int offset_list[] = {2,2,2,3};
   const strtk::offset_predicate<4> os_p(offset_list);
   strtk::std_string::token_list_type token_list;
   strtk::offset_splitter(s.begin(),s.end(),os_p,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator it = token_list.begin();
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
   strtk::std_string::token_list_type token_list;
   strtk::offset_splitter(s.begin(),s.end(),strtk::offsets(1,2,3,4,5,6,7),std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator it = token_list.begin();
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

void parse_example01()
{
   std::string input = "abcd|x|-1234|78901|4567.8901|0x75BCD15|AABB";

   std::string  o1 = "";
   char         o2 = 0x00;
   int          o3 = 0;
   unsigned int o4 = 0;
   double       o5 = 0;
   unsigned int o6 = 0;
   short int    o7 = 0;
   strtk::hex_to_number_sink<unsigned int> hns1(o6);
   strtk::hex_to_number_sink<short int> hns2(o7);
   strtk::parse(input,"|",o1,o2,o3,o4,o5,hns1,hns2);

   std::cout << o1 << "\t"
             << o2 << "\t"
             << o3 << "\t"
             << o4 << "\t"
             << o5 << "\t"
             << o6 << "\t"
             << o7 << std::endl;
}

void parse_example02()
{
   std::string int_string    = "0,1,2,3,4,5,6,7,8,9";
   std::string double_string = "0.0,1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9";

   std::vector<int> int_list;
   std::deque<double> double_list;

   strtk::parse_into_sequence(int_string,",",int_list);
   strtk::parse_into_sequence(double_string,",",double_list);

   std::copy(int_list.begin(),int_list.end(),std::ostream_iterator<int>(std::cout,"\t"));
   std::cout << std::endl;
   std::copy(double_list.begin(),double_list.end(),std::ostream_iterator<double>(std::cout,"\t"));
   std::cout << std::endl;
}

void parse_example03()
{
   std::string int_string    = "0,1,2,3,4,5,6,7,8,9";
   std::string double_string = "0.0,1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9";

   std::vector<int> int_list;
   std::deque<double> double_list;

   static const std::size_t n = 4;

   strtk::parse_into_sequence_n(int_string,",",n,int_list);
   strtk::parse_into_sequence_n(double_string,",",n,double_list);

   std::copy(int_list.begin(),int_list.end(),std::ostream_iterator<int>(std::cout,"\t"));
   std::cout << std::endl;
   std::copy(double_list.begin(),double_list.end(),std::ostream_iterator<double>(std::cout,"\t"));
   std::cout << std::endl;
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

void remove_trailing_example()
{
   std::string s = "The quick brown fox jumps over the lazy dog  , _";
   std::cout << "Before: ["<< s << "]" << std::endl;
   strtk::remove_trailing(" _,",s);
   std::cout << "After:  ["<< s << "]" << std::endl;
}

void remove_proceeding_example()
{
   std::string s = "_ ,  The quick brown fox jumps over the lazy dog";
   std::cout << "Before: ["<< s << "]" << std::endl;
   strtk::remove_proceeding(" _,",s);
   std::cout << "After:  ["<< s << "]" << std::endl;
}

void uri_extractor_example01()
{
   #ifdef ENABLE_REGEX
   std::string text = "someone@somewhere.com http://www.test.net any.one@any.where.com ftp://123.abcxyz.org";
   std::list<std::string> email_list;
   std::list<std::string> url_list;
   strtk::split_regex(strtk::email_expression,text,std::back_inserter(email_list));
   strtk::split_regex(strtk::uri_expression,text,std::back_inserter(url_list));
   std::cout << "emails: ";
   std::copy(email_list.begin(),email_list.end(),std::ostream_iterator<std::string>(std::cout," "));
   std::cout << std::endl << "urls: ";
   std::copy(url_list.begin(),url_list.end(),std::ostream_iterator<std::string>(std::cout," "));
   std::cout << std::endl;
   #endif
}

void generate_random_example01()
{
   #ifdef ENABLE_RANDOM
   const std::size_t data_size = 10;
   unsigned char* data = new unsigned char[data_size];
   strtk::generate_random_data(data,data_size,1000000);
   unsigned char* hex_data = new unsigned char[2 * data_size];
   strtk::convert_bin_to_hex(data,data + data_size, hex_data);
   std::copy(hex_data,hex_data + 2 * data_size,std::ostream_iterator<unsigned char>(std::cout));
   std::cout << std::endl;
   delete [] data;
   delete [] hex_data;
   #endif
}

void lexicographically_collate_example()
{
   std::string str_list[] = {
                              "xyzabcabc",
                              "ijkxyzabc",
                              "abcdefghijklmnopqrstuvwxyz",
                              "zyxwvutsrqponmlkjihgfedcba",
                              "The Quick Brown Fox Jumps Over The Lazy Dog"
                            };

   for(std::size_t i = 0; i < sizeof(str_list)/sizeof(std::string); ++i)
   {
      std::cout << str_list[i] << " --> ";
      strtk::lexicographically_canonicalize(str_list[i]);
      std::cout << str_list[i] << std::endl;
   };

   unsigned int uint_list[] = { 6,7,8,9,0,1,2,3,4,5 };

   std::copy(uint_list,uint_list + sizeof(uint_list) / sizeof(unsigned int),std::ostream_iterator<unsigned int>(std::cout," "));
   std::cout << " --> ";
   strtk::lexicographically_canonicalize(uint_list,uint_list + sizeof(uint_list) / sizeof(unsigned int));
   std::copy(uint_list,uint_list + sizeof(uint_list) / sizeof(unsigned int),std::ostream_iterator<unsigned int>(std::cout," "));
   std::cout << std::endl;

   std::list<int> int_list;
   int_list.push_back(6); int_list.push_back(7);
   int_list.push_back(8); int_list.push_back(9);
   int_list.push_back(0); int_list.push_back(1);
   int_list.push_back(2); int_list.push_back(3);
   int_list.push_back(4); int_list.push_back(5);

   std::copy(int_list.begin(),int_list.end(),std::ostream_iterator<int>(std::cout," "));
   std::cout << " --> ";
   strtk::lexicographically_canonicalize(int_list.begin(),int_list.end());
   std::copy(int_list.begin(),int_list.end(),std::ostream_iterator<int>(std::cout," "));
   std::cout << std::endl;
}

void hash_example()
{
            char  chr_list[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
   unsigned char uchr_list[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
            int   int_list[] = {   -4,   -3,   -2,   -1,    0,    1,    2, 3, 4 };
   unsigned int  uint_list[] = {    0,    1,    2,    3,    4,    5,    6, 7, 8 };
         double   dbl_list[] = {  1.1,  2.2,  3.3,  4.4,  5.5,  6.6,  7.7 };

   std::cout << "hash(char):   " << strtk::hash(chr_list, sizeof( chr_list) / sizeof(char))          << std::endl;
   std::cout << "hash(uchar):  " << strtk::hash(uchr_list,sizeof(uchr_list) / sizeof(unsigned char)) << std::endl;
   std::cout << "hash(int):    " << strtk::hash(int_list, sizeof( int_list) / sizeof(int))           << std::endl;
   std::cout << "hash(uint):   " << strtk::hash(uint_list,sizeof(uint_list) / sizeof(unsigned int))  << std::endl;
   std::cout << "hash(double): " << strtk::hash(dbl_list, sizeof( dbl_list) / sizeof(double))        << std::endl;
}

void join_example()
{
   const std::size_t str_list_size = 5;
   std::string str_list [] = {
                               "1",
                               "22",
                               "333",
                               "4444",
                               "55555"
                             };

   std::cout << strtk::join(",",str_list,str_list + str_list_size) << std::endl;

   std::cout << "Size equals 1: " << strtk::join_if(",", strtk::size_equal_to<1>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size equals 2: " << strtk::join_if(",", strtk::size_equal_to<2>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size equals 3: " << strtk::join_if(",", strtk::size_equal_to<3>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size equals 4: " << strtk::join_if(",", strtk::size_equal_to<4>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size equals 5: " << strtk::join_if(",", strtk::size_equal_to<5>(), str_list,str_list + str_list_size) << std::endl;

   std::cout << "Size less than 1: " << strtk::join_if(",", strtk::size_less_than<1>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size less than 2: " << strtk::join_if(",", strtk::size_less_than<2>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size less than 3: " << strtk::join_if(",", strtk::size_less_than<3>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size less than 4: " << strtk::join_if(",", strtk::size_less_than<4>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size less than 5: " << strtk::join_if(",", strtk::size_less_than<5>(), str_list,str_list + str_list_size) << std::endl;

   std::cout << "Size greater than 1: " << strtk::join_if(",", strtk::size_greater_than<1>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size greater than 2: " << strtk::join_if(",", strtk::size_greater_than<2>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size greater than 3: " << strtk::join_if(",", strtk::size_greater_than<3>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size greater than 4: " << strtk::join_if(",", strtk::size_greater_than<4>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size greater than 5: " << strtk::join_if(",", strtk::size_greater_than<5>(), str_list,str_list + str_list_size) << std::endl;
}

void combination_example()
{
   std::string s = "abcde";
   std::size_t set_size = s.size() - 1;
   std::size_t combination_index = 0;
   do
   {
      std::cout << combination_index++ << "\t";
      std::copy(s.begin(),s.begin() + set_size,std::ostream_iterator<std::string::value_type>(std::cout,""));
      std::cout << std::endl;
   }
   while (strtk::next_combination(s.begin(),s.begin() + set_size,s.end()));
}

void typename_example()
{
   char           t0;
   unsigned char  t1;
   short          t2;
   int            t3;
   long           t4;
   unsigned short t5;
   unsigned int   t6;
   unsigned long  t7;
   std::string    t8;

   std::cout << strtk::type_name(t0) << std::endl;
   std::cout << strtk::type_name(t1) << std::endl;
   std::cout << strtk::type_name(t2) << std::endl;
   std::cout << strtk::type_name(t3) << std::endl;
   std::cout << strtk::type_name(t4) << std::endl;
   std::cout << strtk::type_name(t5) << std::endl;
   std::cout << strtk::type_name(t6) << std::endl;
   std::cout << strtk::type_name(t7) << std::endl;
   std::cout << strtk::type_name(t8) << std::endl;

   std::vector<unsigned int> vui;
   std::vector<std::string> vs;

   std::deque<double> dd;
   std::deque<std::string> ds;

   std::cout << strtk::type_name(vui) << std::endl;
   std::cout << strtk::type_name(vs)  << std::endl;
   std::cout << strtk::type_name(dd)  << std::endl;
   std::cout << strtk::type_name(ds)  << std::endl;
}

int main()
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
   tokenizer_example13();
   split_example01();
   split_example02();
   split_example03();
   split_example04();
   split_example05();
   split_regex_example();
   split_n_example01();
   split_n_example02();
   split_n_example03();
   split_regex_n_example();
   offset_splitter_example01();
   offset_splitter_example02();
   offset_splitter_example03();
   construct_example();
   parse_example01();
   parse_example02();
   parse_example03();
   remove_inplace_example01();
   remove_consecutives_example01();
   remove_consecutives_example02();
   remove_consecutives_example03();
   remove_trailing_example();
   remove_proceeding_example();
   uri_extractor_example01();
   generate_random_example01();
   lexicographically_collate_example();
   hash_example();
   join_example();
   combination_example();
   typename_example();
   return 0;
}
