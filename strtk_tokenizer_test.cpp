/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * String Tokenizer Test                                         *
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
#include <limits>
#include <cmath>

#include "strtk.hpp"


template<typename Predicate>
bool test_tokenizer_split(const Predicate& p,
                          const std::string& s,
                          const std::string& expected_result,
                          const bool compressed_delimiters = false)
{
   std::string result = "";
   std::vector< std::pair<std::string::const_iterator , std::string::const_iterator> > tok_list;
   strtk::split(p,
                s,
                std::back_inserter(tok_list),
                (compressed_delimiters) ? strtk::split_options::compress_delimiters : strtk::split_options::default_mode);

   for (std::size_t i = 0; i < tok_list.size(); ++i)
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

   for (std::size_t i = 0; i < test_count; ++i)
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

bool test_split_n()
{
   std::string delimiters = " ,|\t_:!";
   std::string data1 = "1234567890abcdefghijklmnopqrstuvwxyz";
   std::string data2 = strtk::join(delimiters,data1.begin(),data1.end());

   strtk::multiple_char_delimiter_predicate predicate(delimiters);
   std::vector<std::string> strlist;
   strlist.reserve(data1.size());

   for (std::size_t i = 0; i < data1.size(); ++i)
   {
      if (i != strtk::split_n(predicate,data2,i,strtk::range_to_type_back_inserter(strlist)))
      {
         std::cout << "test_split_n() - Failed Test: " << i << std::endl;
         return false;
      }
   }
   return true;
}

bool test_empty_filter_itr()
{
   std::string s = "a||c";
   typedef strtk::std_string::token_vector_type str_list;
   str_list sl;
   strtk::single_delimiter_predicate<std::string::value_type> p('|');
   strtk::std_string::tokenizer<>::type tok(s,p,true);

   if (2 == for_each_token(s,tok,strtk::filter_non_empty_range< std::back_insert_iterator<str_list> >(std::back_inserter(sl))))
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

bool test_double_convert()
{
   static const std::string double_str[] =
               {
                    "7379973.6624700",          "+2187496.9290991",            "384281.3720002",
                  "06603499.49182570",            "-80173.0014819",             "+00693.000285",
                         "-0.6366299",             "+0.2335175000",               "-00.6366299",
                     "+00.2335175000",              "-000.6366299",           "+000.2335175000",
                          "123.456e3",                 "123.456E3",                "123.456e+3",
                         "123.456E+3",                "123.456e03",                "123.456E03",
                        "123.456e+03",               "123.456E+03",              "123.456e0003",
                       "123.456E0003",             "123.456e+0003",             "123.456E+0003",
                         "123.4560e3",               "123.45600E3",             "123.456000e+3",
                      "123.456000E+3",            "123.4560000e03",           "123.45600000E03",
                  "123.456000000e+03",        "123.4560000000E+03",      "123.45600000000e0003",
              "123.456000000000E0003",   "123.4560000000000e+0003",  "123.45600000000000E+0003",
                      "+0123.456e+003",          "-00123.456E+003",           "+000123.45600E3",
                          "-123.456e3",               "+123.456E3",               "-123.456e+3",
                         "+123.456E+3",              "-123.456e03",               "+123.456E03",
                        "-123.456e123",             "+123.456E123",             "-123.456e+123",
                       "+123.456E+123",            "-123.456e0123",             "+123.456E0123",
                        "-123.456e+03",             "+123.456E+03",             "-123.456e0003",
                       "+123.456E0003",           "-123.456e+0003",            "+123.456E+0003",
                         "-123.4560e3",             "+123.45600E3",            "-123.456000e+3",
                      "+123.456000E+3",          "-123.4560000e03",          "+123.45600000E03",
                  "-123.456000000e+03",      "+123.4560000000E+03",     "-123.45600000000e0003",
              "+123.456000000000E0003", "-123.4560000000000e+0003", "+123.45600000000000E+0003",
                      "-0123.456e+003",          "+00123.456E+003",           "-000123.45600E3",
                "000000123.456e+0003",                       "0.0",                     "00.00",
                            "000.000",                 "0000.0000",                     "1234.",
                               "01.2",                  "0012.340",               "00012.34500"
               };

   static const double d[] =
               {
                    7379973.6624700,          +2187496.9290991,            384281.3720002,
                  06603499.49182570,            -80173.0014819,             +00693.000285,
                         -0.6366299,             +0.2335175000,               -00.6366299,
                     +00.2335175000,              -000.6366299,           +000.2335175000,
                          123.456e3,                 123.456E3,                123.456e+3,
                         123.456E+3,                123.456e03,                123.456E03,
                        123.456e+03,               123.456E+03,              123.456e0003,
                       123.456E0003,             123.456e+0003,             123.456E+0003,
                         123.4560e3,               123.45600E3,             123.456000e+3,
                      123.456000E+3,            123.4560000e03,           123.45600000E03,
                  123.456000000e+03,        123.4560000000E+03,      123.45600000000e0003,
              123.456000000000E0003,   123.4560000000000e+0003,  123.45600000000000E+0003,
                      +0123.456e+003,          -00123.456E+003,           +000123.45600E3,
                          -123.456e3,               +123.456E3,               -123.456e+3,
                         +123.456E+3,              -123.456e03,               +123.456E03,
                         -123.456e123,            +123.456E123,             -123.456e+123,
                        +123.456E+123,           -123.456e0123,             +123.456E0123,
                        -123.456e+03,             +123.456E+03,             -123.456e0003,
                       +123.456E0003,           -123.456e+0003,            +123.456E+0003,
                         -123.4560e3,             +123.45600E3,            -123.456000e+3,
                      +123.456000E+3,          -123.4560000e03,          +123.45600000E03,
                  -123.456000000e+03,      +123.4560000000E+03,     -123.45600000000e0003,
              +123.456000000000E0003, -123.4560000000000e+0003, +123.45600000000000E+0003,
                      -0123.456e+003,          +00123.456E+003,           -000123.45600E3,
                000000123.456e+0003,                       0.0,                     00.00,
                            000.000,                 0000.0000,                     1234.,
                               01.2,                  0012.340,               00012.34500

               };

   std::size_t d_size = sizeof(double_str) / sizeof(std::string);

   double v = 0.0;
   for (std::size_t i = 0; i < d_size; ++i)
   {
      strtk::string_to_type_converter(double_str[i],v);
      if (d[i] != v)
      {
         std::cout << "test_double_convert() double check[" << i << "] " << std::endl;
         return false;
      }
   }

   static const std::string bad_list[] =
                       {
                         "1234567.8A",
                          "1234567.A",
                         "12345A67.8",
                        "A1234567.8A",
                        "1234567.8Ae",
                     "1234567.AE+100",
                    "12345A67.8e+100",
                   "A1234567.8AE+100",
                        "--1.234e+10",
                        "++1.234e+10",
                         "1.234e+10-",
                         "1.234e+10+",
                         "1.234e++10",
                         "1.234E--10",
                            "1.e++10",
                            "1.E--10",
                         "1..234e+10",
                         "1.23.4E-10",
                         "1.234e-1.0",
                              "1.23f",
                         "1.23e+100f"
                       };

   static const std::size_t bad_list_size = sizeof(bad_list) / sizeof(std::string);

   for (std::size_t i = 0; i < bad_list_size; ++i)
   {
      if (strtk::string_to_type_converter(bad_list[i],v))
      {
         std::cout << "test_double_convert() bad test failure [" << i << "] " << std::endl;
         return false;
      }
   }

   return true;
}

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

bool test_parse1()
{
   std::string data = "1 ,|\t987.654 ,|\t abc ,|\t";
   int i = 0;
   double d = 0;
   std::string s = "";
   if (!strtk::parse(data,",|\t ",i,d,s))
   {
      std::cout << "test_parse() - parse fail 1" << std::endl;
      return false;
   }
   if ((i != 1) || (d != 987.654) || (s != "abc"))
   {
      std::cout << "test_parse() - parse fail 2" << std::endl;
      return false;
   }

   return true;
}

bool test_parse2()
{
   std::string data = "1 ,|\t2. ,|\t 3.3 ,|\t .4 ,|\t 123.456 ,|\t 3.30 ,|\t 1.0e+010 ,|\t 2.2e+11 ,|\t 3.0E+012 ,|\t 4.4E+13";

   double d[10] = { 0.0 };

   if (!strtk::parse(data,",|\t ",d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8],d[9]))
   {
      std::cout << "test_parse2() - parse double fail" << std::endl;
      return false;
   }

   if (d[0] !=      1.0) { std::cout << "test_parse2() double check0 " << std::endl; return false; }
   if (d[1] !=      2.0) { std::cout << "test_parse2() double check1 " << std::endl; return false; }
   if (d[2] !=      3.3) { std::cout << "test_parse2() double check2 " << std::endl; return false; }
   if (d[3] !=      0.4) { std::cout << "test_parse2() double check3 " << std::endl; return false; }
   if (d[4] !=  123.456) { std::cout << "test_parse2() double check4 " << std::endl; return false; }
   if (d[5] !=      3.3) { std::cout << "test_parse2() double check5 " << std::endl; return false; }
   if (d[6] != 1.0e+010) { std::cout << "test_parse2() double check6 " << std::endl; return false; }
   if (d[7] != 2.2e+011) { std::cout << "test_parse2() double check7 " << std::endl; return false; }
   if (d[8] != 3.0e+012) { std::cout << "test_parse2() double check8 " << std::endl; return false; }
   if (d[9] != 4.4e+013) { std::cout << "test_parse2() double check9 " << std::endl; return false; }


   float f[10]  = { 0.0 };

   if (!strtk::parse(data,",|\t ",f[0],f[1],f[2],f[3],f[4],f[5],f[6],f[7],f[8],f[9]))
   {
      std::cout << "test_parse2() - parse float fail" << std::endl;
      return false;
   }

   if (f[0] !=      1.0f) { std::cout << "test_parse2() float check0 " << std::endl; return false; }
   if (f[1] !=      2.0f) { std::cout << "test_parse2() float check1 " << std::endl; return false; }
   if (f[2] !=      3.3f) { std::cout << "test_parse2() float check2 " << std::endl; return false; }
   if (f[3] !=      0.4f) { std::cout << "test_parse2() float check3 " << std::endl; return false; }
   if (f[4] !=  123.456f) { std::cout << "test_parse2() float check4 " << std::endl; return false; }
   if (f[5] !=      3.3f) { std::cout << "test_parse2() float check5 " << std::endl; return false; }
   if (f[6] != 1.0e+010f) { std::cout << "test_parse2() float check6 " << std::endl; return false; }
   if (f[7] != 2.2e+011f) { std::cout << "test_parse2() float check7 " << std::endl; return false; }
   if (f[8] != 3.0e+012f) { std::cout << "test_parse2() float check8 " << std::endl; return false; }
   if (f[9] != 4.4e+013f) { std::cout << "test_parse2() float check9 " << std::endl; return false; }

   return true;
}

bool test_parse3()
{

   std::string data = "123.456e3,123.456E3 123.456e+3,123.456E+3 123.456e03              "
                      "123.456E03, 123.456e+03 123.456E+03,123.456e0003|123.456E0003,    "
                      "123.456e+0003 123.456E+0003,|123.4560e3 123.45600E3, 123.456000e+3"
                      "123.456000E+3,|123.4560000e03 123.45600000E03, 123.456000000e+03  "
                      "123.4560000000E+03,123.45600000000e0003|123.456000000000E0003,    "
                      "123.4560000000000e+0003 123.45600000000000E+0003                  "
                      "0123.456e+003 00123.456E+003,| 000123.45600E3, 000000123.456e+0003";

   std::vector<double> double_list;

   if (!strtk::parse(data,",|\t ",double_list))
   {
      std::cout << "test_parse2() - parse double fail" << std::endl;
      return false;
   }

   for(std::size_t i = 0; i < double_list.size(); ++i)
   {
      if (123.456e3 != double_list[i])
      {
         std::cout << "test_parse3() double check[" << i << "] " << std::endl;
         return false;
      }
   }

   std::vector<float> float_list;

   if (!strtk::parse(data,",|\t ",float_list))
   {
      std::cout << "test_parse3() - parse float fail" << std::endl;
      return false;
   }

   for(std::size_t i = 0; i < float_list.size(); ++i)
   {
      if (123.456e3f != float_list[i])
      {
         std::cout << "test_parse3() float check[" << i << "] " << std::endl;
         return false;
      }
   }

   return  true;
}

bool test_replace_pattern()
{
   typedef std::pair<std::string,std::string> sp_type;
   static const sp_type test[] = {
                                   sp_type(   "a" ,   "x"),
                                   sp_type(  "ab" ,  "xy"),
                                   sp_type( "abc" , "xyz"),
                                   sp_type(   "a" ,  "xy"),
                                   sp_type(   "b" ,  "xy"),
                                   sp_type(   "c" ,  "xy"),
                                   sp_type(  "ab" ,   "x"),
                                   sp_type(  "bc" ,   "x"),
                                   sp_type(  "ca" ,   "x"),
                                   sp_type(  "ab" , "xyz"),
                                   sp_type(  "bc" , "xyz"),
                                   sp_type(  "ca" , "xyz"),
                                   sp_type( "abc" ,   "x"),
                                   sp_type( "bca" ,   "x"),
                                   sp_type( "cab" ,   "x"),
                                   sp_type("abca" ,   "x"),
                                   sp_type("bcab" ,   "x"),
                                   sp_type("cabc" ,   "x"),
                                   sp_type( "abc" ,    ""),
                                   sp_type( "ijk" , "mno")
                                 };

   static const std::string base[] = {
                                       "",
                                       "abcabcabcabcabc",
                                       "yyabcabcabcabcabc",
                                       "yyabcabcabcabcabckkk",
                                       "yyabcabcabcabcabckkk",
                                       "yabctabcabctabcabtckk",
                                       "xyzxyzxyzxyzxyzxyzxyzxyz"
                                       "abc"
                                     };
   std::string result;
   for (std::size_t i = 0; i < sizeof(test) / sizeof(sp_type); ++i)
   {
      for (std::size_t j = 0; j < sizeof(base) / sizeof(std::string); ++j)
      {
         strtk::replace_pattern(base[j],test[i].first,test[i].second,result);
         if (test[i].first != test[i].second)
         {
            if (std::string::npos != result.find(test[i].first))
            {
               std::cout << "replace_pattern: s[" << base[j] << "] p[" << test[i].first << "] r[" << test[i].second << "]" << std::endl;
               return false;
            }
         }
      }
   }
   return true;
}

int main()
{
   test_split_and_tokenizer();
   assert(test_split_n());
   assert(test_empty_filter_itr());
   assert(test_construct_and_parse());
   assert(test_double_convert());
   assert(test_parse1());
   assert(test_parse2());
   assert(test_parse3());
   assert(test_replace_pattern());
   return 0;
}
