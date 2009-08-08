/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * String Tokenizer Test                                           *
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


/*
   Description: The String Tokenizer Test performs some very basic
                "unit test"-like testing and code coverage upon the
                String Toolkit library's various tokenization,
                splitting and parsing routines.
                A silent/blank return indicates a positive test sweep,
                otherwise any output indicates test failures.
*/

#include <cstddef>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <cassert>

#include "strtk.hpp"

template<typename Predicate>
bool test_tokenizer_split(const Predicate& p,
                          const std::string& s,
                          const std::string& expected_result,
                          const bool compressed_delimiters = false)
{
   std::string result = "";
   std::vector< std::pair< std::string::const_iterator , std::string::const_iterator> > tok_list;
   strtk::split(p,s,std::back_inserter(tok_list),compressed_delimiters);

   for(std::size_t i = 0; i < tok_list.size(); ++i)
   {
      if (tok_list[i].first == tok_list[i].second)
         result += "<>";
      else
         result += std::string(tok_list[i].first,tok_list[i].second);
      if (i != (tok_list.size() - 1))
      {
         result += ",";
      }
   }

   if (result != expected_result)
   {
      std::cout << "ERROR: result: " << result << "\t expected: " << expected_result << "\t";
   }
   return result == expected_result;
}

template<typename Tokenizer,typename Predicate>
bool test_tokenizer_itr(const Predicate& p,
                        const std::string& s,
                        const std::string& expected_result,
                        const bool compressed_delimiters = false)
{
   std::string result = "";
   Tokenizer stk(s,p,compressed_delimiters);
   typename Tokenizer::iterator it = stk.begin();
   while (it != stk.end())
   {
      if ((*it).first == (*it).second)
         result += "<>";
      else
         result += std::string((*it).first,(*it).second);
      ++it;
      if (it != stk.end()) result += ",";
   }
   if (result != expected_result)
   {
      std::cout << "ERROR: result: " << result << "\t expected: " << expected_result << "\t";
   }
   return result == expected_result;
}

void test_split_and_tokenizer()
{
   struct test_pairs
   {
      std::string input;
      std::string output;
   };

   /*
      Test IO 1 and 2 : Single and Multi predicate tokenizing
      Test IO 3 and 4 : Single and Multi predicate compressed delimiter tokenizing
   */

   const std::size_t test_count = 40;
   const test_pairs test_input_output1[test_count]
     = {
         {"d",                                             "d"},
         {"d|x",                                         "d,x"},
         {"d||x",                                     "d,<>,x"},
         {"|d||x",                                 "<>,d,<>,x"},
         {"d||x|",                                 "d,<>,x,<>"},
         {"|d||x|",                             "<>,d,<>,x,<>"},
         {"||d||x||",                     "<>,<>,d,<>,x,<>,<>"},
         {"||",                                     "<>,<>,<>"},
         {"|",                                         "<>,<>"},
         {"|||",                                 "<>,<>,<>,<>"},
         {"",                                               ""},
         {"dxd",                                         "dxd"},
         {"dx|xd",                                     "dx,xd"},
         {"dxd||xdx",                             "dxd,<>,xdx"},
         {"|dx||x",                               "<>,dx,<>,x"},
         {"d||xd|",                               "d,<>,xd,<>"},
         {"|xd||dx|",                         "<>,xd,<>,dx,<>"},
         {"||dd||xx||",                 "<>,<>,dd,<>,xx,<>,<>"},
         {"abc",                                         "abc"},
         {"abc|xyz",                                 "abc,xyz"},
         {"abc||xyz",                             "abc,<>,xyz"},
         {"|abc||xyz",                         "<>,abc,<>,xyz"},
         {"abc||xyz|",                         "abc,<>,xyz,<>"},
         {"|abc||xyz|",                     "<>,abc,<>,xyz,<>"},
         {"||abc||xyz||",             "<>,<>,abc,<>,xyz,<>,<>"},
         {"||",                                     "<>,<>,<>"},
         {"|",                                         "<>,<>"},
         {"|||",                                 "<>,<>,<>,<>"},
         {"",                                               ""},
         {"abcxyzabc",                             "abcxyzabc"},
         {"abcxyz|xyzabc",                     "abcxyz,xyzabc"},
         {"abcxyzabc||xyzabcxyz",     "abcxyzabc,<>,xyzabcxyz"},
         {"|abcxyz||xyz",                   "<>,abcxyz,<>,xyz"},
         {"abc||xyzabc|",                   "abc,<>,xyzabc,<>"},
         {"|xyzabc||abcxyz|",         "<>,xyzabc,<>,abcxyz,<>"},
         {"||abcabc||xyzxyz||", "<>,<>,abcabc,<>,xyzxyz,<>,<>"},
         {"a|ij|xyz",                               "a,ij,xyz"},
         {"a||ij||xyz",                       "a,<>,ij,<>,xyz"},
         {"|a||ij|||xyz",               "<>,a,<>,ij,<>,<>,xyz"},
         {"|a||ij|||xyz|",           "<>,a,<>,ij,<>,<>,xyz,<>"}
      };

   const test_pairs test_input_output2[test_count]
     = {
         {"d",                                             "d"},
         {"d|x",                                         "d,x"},
         {"d?|x",                                     "d,<>,x"},
         {",d?-x",                                 "<>,d,<>,x"},
         {"d;?x|",                                 "d,<>,x,<>"},
         {"|d;;x-",                             "<>,d,<>,x,<>"},
         {"_|d?|x;-",                     "<>,<>,d,<>,x,<>,<>"},
         {"|_",                                     "<>,<>,<>"},
         {";",                                         "<>,<>"},
         {"?_,",                                 "<>,<>,<>,<>"},
         {"",                                               ""},
         {"dxd",                                         "dxd"},
         {"dx,xd",                                     "dx,xd"},
         {"dxd|,xdx",                             "dxd,<>,xdx"},
         {";dx||x",                               "<>,dx,<>,x"},
         {"d|;xd?",                               "d,<>,xd,<>"},
         {"_xd,,dx;",                         "<>,xd,<>,dx,<>"},
         {"__dd|_xx,;",                 "<>,<>,dd,<>,xx,<>,<>"},
         {"abc",                                         "abc"},
         {"abc|xyz",                                 "abc,xyz"},
         {"abc?|xyz",                             "abc,<>,xyz"},
         {",abc?-xyz",                         "<>,abc,<>,xyz"},
         {"abc;?xyz|",                         "abc,<>,xyz,<>"},
         {"|abc;;xyz-",                     "<>,abc,<>,xyz,<>"},
         {"_|abc?|xyz;-",             "<>,<>,abc,<>,xyz,<>,<>"},
         {"|_",                                     "<>,<>,<>"},
         {";",                                         "<>,<>"},
         {"?_,",                                 "<>,<>,<>,<>"},
         {"",                                               ""},
         {"abcxyzabc",                             "abcxyzabc"},
         {"abcxyz,xyzabc",                     "abcxyz,xyzabc"},
         {"abcxyzabc|,xyzabcxyz",     "abcxyzabc,<>,xyzabcxyz"},
         {";abcxyz||xyz",                   "<>,abcxyz,<>,xyz"},
         {"abc|;xyzabc?",                   "abc,<>,xyzabc,<>"},
         {"_xyzabc,,abcxyz;",         "<>,xyzabc,<>,abcxyz,<>"},
         {"__abcabc|_xyzxyz,;", "<>,<>,abcabc,<>,xyzxyz,<>,<>"},
         {"a|ij?xyz",                               "a,ij,xyz"},
         {"a|_ij,;xyz",                       "a,<>,ij,<>,xyz"},
         {"_a??ij;,|xyz",               "<>,a,<>,ij,<>,<>,xyz"},
         {"_a||ij,,?xyz_",           "<>,a,<>,ij,<>,<>,xyz,<>"}
      };

   const test_pairs test_input_output3[test_count]
     = {
         {"d",                                      "d"},
         {"d|x",                                  "d,x"},
         {"d||x",                                 "d,x"},
         {"|d||x",                             "<>,d,x"},
         {"d||x|",                             "d,x,<>"},
         {"|d||x|",                         "<>,d,x,<>"},
         {"||d||x||",                       "<>,d,x,<>"},
         {"||",                                 "<>,<>"},
         {"|",                                  "<>,<>"},
         {"|||",                                "<>,<>"},
         {"",                                        ""},
         {"dxd",                                  "dxd"},
         {"dx|xd",                              "dx,xd"},
         {"dxd||xdx",                         "dxd,xdx"},
         {"|dx||x",                           "<>,dx,x"},
         {"d||xd|",                           "d,xd,<>"},
         {"|xd||dx|",                     "<>,xd,dx,<>"},
         {"||dd||xx||",                   "<>,dd,xx,<>"},
         {"abc",                                  "abc"},
         {"abc|xyz",                          "abc,xyz"},
         {"abc||xyz",                         "abc,xyz"},
         {"|abc||xyz",                     "<>,abc,xyz"},
         {"abc||xyz|",                     "abc,xyz,<>"},
         {"|abc||xyz|",                 "<>,abc,xyz,<>"},
         {"||abc||xyz||",               "<>,abc,xyz,<>"},
         {"||",                                 "<>,<>"},
         {"|",                                  "<>,<>"},
         {"|||",                                "<>,<>"},
         {"",                                        ""},
         {"abcxyzabc",                      "abcxyzabc"},
         {"abcxyz|xyzabc",              "abcxyz,xyzabc"},
         {"abcxyzabc||xyzabcxyz", "abcxyzabc,xyzabcxyz"},
         {"|abcxyz||xyz",               "<>,abcxyz,xyz"},
         {"abc||xyzabc|",               "abc,xyzabc,<>"},
         {"|xyzabc||abcxyz|",     "<>,xyzabc,abcxyz,<>"},
         {"||abcabc||xyzxyz||",   "<>,abcabc,xyzxyz,<>"},
         {"a|ij|xyz",                        "a,ij,xyz"},
         {"a||ij||xyz",                      "a,ij,xyz"},
         {"|a||ij|||xyz",                 "<>,a,ij,xyz"},
         {"|a||ij|||xyz|",             "<>,a,ij,xyz,<>"}
      };

   const test_pairs test_input_output4[test_count]
     = {
         {"d",                                      "d"},
         {"d;x",                                  "d,x"},
         {"d|?x",                                 "d,x"},
         {",d_|x",                             "<>,d,x"},
         {"d|;x|",                             "d,x,<>"},
         {";d||x|",                         "<>,d,x,<>"},
         {"|,d?|x;;",                       "<>,d,x,<>"},
         {"|?",                                 "<>,<>"},
         {"|",                                  "<>,<>"},
         {"?,|",                                "<>,<>"},
         {"",                                        ""},
         {"dxd",                                  "dxd"},
         {"dx,xd",                              "dx,xd"},
         {"dxd?,xdx",                         "dxd,xdx"},
         {"|dx;|x",                           "<>,dx,x"},
         {"d|,xd_",                           "d,xd,<>"},
         {";xd||dx|",                     "<>,xd,dx,<>"},
         {"|?dd|,xx?_",                   "<>,dd,xx,<>"},
         {"abc",                                  "abc"},
         {"abc;xyz",                          "abc,xyz"},
         {"abc,,xyz",                         "abc,xyz"},
         {"|abc;|xyz",                     "<>,abc,xyz"},
         {"abc?|xyz,",                     "abc,xyz,<>"},
         {"|abc||xyz|",                 "<>,abc,xyz,<>"},
         {"||abc?|xyz_|",               "<>,abc,xyz,<>"},
         {"|,",                                 "<>,<>"},
         {"|",                                  "<>,<>"},
         {";,|",                                "<>,<>"},
         {"",                                        ""},
         {"abcxyzabc",                      "abcxyzabc"},
         {"abcxyz;xyzabc",              "abcxyz,xyzabc"},
         {"abcxyzabc|,xyzabcxyz", "abcxyzabc,xyzabcxyz"},
         {"_abcxyz;?xyz",               "<>,abcxyz,xyz"},
         {"abc,|xyzabc|",               "abc,xyzabc,<>"},
         {"|xyzabc|?abcxyz,",     "<>,xyzabc,abcxyz,<>"},
         {"?|abcabc_|xyzxyz|_",   "<>,abcabc,xyzxyz,<>"},
         {"a,ij|xyz",                        "a,ij,xyz"},
         {"a?|ij|,xyz",                      "a,ij,xyz"},
         {"|a||ij|,?xyz",                 "<>,a,ij,xyz"},
         {"?a|,ij|;_xyz|",             "<>,a,ij,xyz,<>"}
      };

   typedef strtk::single_delimiter_predicate<std::string::value_type> single_predicate_type;
   typedef strtk::multiple_char_delimiter_predicate multiple_predicate_type;
   typedef strtk::std_string::tokenizer<single_predicate_type>::type tokenizer_type1;
   typedef strtk::std_string::tokenizer<multiple_predicate_type>::type tokenizer_type2;

   single_predicate_type single_predicate('|');
   multiple_predicate_type multi_predicate("|?-,;_");

   for(std::size_t i = 0; i < test_count; ++i)
   {
      std::string in = test_input_output1[i].input;
      std::string out = test_input_output1[i].output;

      if (!test_tokenizer_split(single_predicate,in,out)) std::cout << "Failed Split Test01 " << i << std::endl;
      if (!test_tokenizer_itr<tokenizer_type1>(single_predicate,in,out)) std::cout << "Failed Iterator Test01 " << i << std::endl;

      in = test_input_output2[i].input;
      out = test_input_output2[i].output;

      if (!test_tokenizer_split(multi_predicate,in,out)) std::cout << "Failed Split Test02 " << i << std::endl;
      if (!test_tokenizer_itr<tokenizer_type2>(multi_predicate,in,out)) std::cout << "Failed Iterator Test02 " << i << std::endl;

      in = test_input_output3[i].input;
      out = test_input_output3[i].output;

      if (!test_tokenizer_split(single_predicate,in,out,true)) std::cout << "Failed Compressed Delimiter Split Test01 " << i << std::endl;
      if (!test_tokenizer_itr<tokenizer_type1>(single_predicate,in,out,true)) std::cout << "Failed Compressed Delimiter Iterator Test01 " << i << std::endl;

      in = test_input_output4[i].input;
      out = test_input_output4[i].output;

      if (!test_tokenizer_split(multi_predicate,in,out,true)) std::cout << "Failed Compressed Delimiter Split Test02 " << i << std::endl;
      if (!test_tokenizer_itr<tokenizer_type2>(multi_predicate,in,out,true)) std::cout << "Failed Compressed Delimiter Iterator Test02 " << i << std::endl;
   }
}

bool test_empty_filter_itr()
{
   std::string s = "a||c";
   typedef std::vector< std::pair<std::string::const_iterator,std::string::const_iterator> > str_list;
   str_list sl;
   strtk::single_delimiter_predicate<std::string::value_type> p('|');
   strtk::std_string::tokenizer< strtk::single_delimiter_predicate<std::string::value_type> >::type tok(s,p,true);
   if (2 == for_each_token(s,tok,strtk::filter_non_empty_range< strtk::add_to_sequence<str_list> >(strtk::add_to_sequence<str_list>(sl))))
      return true;
   else
   {
      std::cout << "test_empty_filter_itr() - Failed Compressed Delimiter Test" << std::endl;
      return false;
   }
}

struct data_block
{
   std::string    d1;
   char           d2;
   int            d3;
   unsigned int   d4;
   double         d5;
   float          d6;
   short          d7;
   unsigned short d8;
   bool           d9;
   unsigned char  d10;

   void clear(const std::size_t& i)
   {
      if (i >=  1) d1  = "";
      if (i >=  2) d2  = 0x00;
      if (i >=  3) d3  = 0;
      if (i >=  4) d4  = 0;
      if (i >=  5) d5  = 0;
      if (i >=  6) d6  = 0;
      if (i >=  7) d7  = 7;
      if (i >=  8) d8  = 0;
      if (i >=  9) d9  = false;
      if (i >= 10) d10 = 0x00;
   }

   bool operator==(const data_block& db)
   {
      return d1 == db.d1 &&
             d2 == db.d2 &&
             d3 == db.d3 &&
             d4 == db.d4 &&
             d5 == db.d5 &&
             d6 == db.d6 &&
             d7 == db.d7 &&
             d8 == db.d8 &&
             d9 == db.d9 &&
             d10 == db.d10;
   }

   bool operator!=(const data_block& db)
   {
      return !this->operator ==(db);
   }

};

bool test_construct_and_parse()
{
   data_block i;
   std::string output = "";
   i.d1  = "The quick brown fox jumps over the lazy dog";
   i.d2  = 'x';
   i.d3  = -1234;
   i.d4  = 78901;
   i.d5  = 4567.8901;
   i.d6  = 123.456f;
   i.d7  = -16000;
   i.d8  = 15000;
   i.d9  = true;
   i.d10 = 0xEE;

   strtk::construct(output,"|",i.d1,i.d2,i.d3,i.d4,i.d5,i.d6,i.d7,i.d8,i.d9,i.d10);
   data_block o = i;

   o.clear(1);
   if (!strtk::parse(output,"|",o.d1) || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 1" << std::endl;
      return false;
   }

   o.clear(2);
   if (!strtk::parse(output,"|",o.d1,o.d2) || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 2" << std::endl;
      return false;
   }

   o.clear(3);
   if (!strtk::parse(output,"|",o.d1,o.d2,o.d3) || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 3" << std::endl;
      return false;
   }

   o.clear(4);
   if (!strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4) || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 4" << std::endl;
      return false;
   }

   o.clear(5);
   if (!strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4,o.d5) || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 5" << std::endl;
      return false;
   }

   o.clear(6);
   if (!strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4,o.d5,o.d6) || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 6" << std::endl;
      return false;
   }

   o.clear(7);
   if (!strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4,o.d5,o.d6,o.d7) || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 7" << std::endl;
      return false;
   }

   o.clear(8);
   if (!strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4,o.d5,o.d6,o.d7,o.d8) || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 8" << std::endl;
      return false;
   }

   o.clear(9);
   if (!strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4,o.d5,o.d6,o.d7,o.d8,o.d9) || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 9" << std::endl;
      return false;
   }

   o.clear(10);
   if (!strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4,o.d5,o.d6,o.d7,o.d8,o.d9,o.d10) || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 10" << std::endl;
      return false;
   }

   return true;
}

int main()
{
   test_split_and_tokenizer();
   assert(test_empty_filter_itr());
   assert(test_construct_and_parse());
   return 0;
}
