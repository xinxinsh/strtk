/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * String Tokenizer Test                                         *
 * Author: Arash Partow (2002-2011)                              *
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
   Description: The String Tokenizer Test performs some very basic "unit test"-like
                testing and code coverage upon the String Toolkit library's various
                tokenization, splitting and parsing routines.
                A silent/blank return indicates a positive test sweep, otherwise
                any output indicates test failures.
*/


#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <limits>
#include <cmath>
#include <cassert>

#include "strtk.hpp"


template<typename Predicate>
bool test_tokenizer_split(const Predicate& p,
                          const std::string& s,
                          const std::string& expected_result,
                          const bool compressed_delimiters = false)
{
   std::string result;
   strtk::std_string::token_vector_type tok_list;
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
   std::string result;
   Tokenizer stk(s,p,compressed_delimiters);
   typename Tokenizer::iterator it = stk.begin();
   while (stk.end() != it)
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

bool test_split_and_tokenizer()
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

      if (!test_tokenizer_split(single_predicate,in,out))
      {
         std::cout << "Failed Split Test01 " << i << std::endl;
         return false;
      }

      if (!test_tokenizer_itr<tokenizer_type1>(single_predicate,in,out))
      {
         std::cout << "Failed Iterator Test01 " << i << std::endl;
         return false;
      }

      in = test_input_output2[i].input;
      out = test_input_output2[i].output;

      if (!test_tokenizer_split(multi_predicate,in,out))
      {
         std::cout << "Failed Split Test02 " << i << std::endl;
         return false;
      }

      if (!test_tokenizer_itr<tokenizer_type2>(multi_predicate,in,out))
      {
         std::cout << "Failed Iterator Test02 " << i << std::endl;
         return false;
      }

      in = test_input_output3[i].input;
      out = test_input_output3[i].output;

      if (!test_tokenizer_split(single_predicate,in,out,true))
      {
         std::cout << "Failed Compressed Delimiter Split Test01 " << i << std::endl;
         return false;
      }

      if (!test_tokenizer_itr<tokenizer_type1>(single_predicate,in,out,true))
      {
         std::cout << "Failed Compressed Delimiter Iterator Test01 " << i << std::endl;
         return false;
      }

      in = test_input_output4[i].input;
      out = test_input_output4[i].output;

      if (!test_tokenizer_split(multi_predicate,in,out,true))
      {
         std::cout << "Failed Compressed Delimiter Split Test02 " << i << std::endl;
         return false;
      }

      if (!test_tokenizer_itr<tokenizer_type2>(multi_predicate,in,out,true))
      {
         std::cout << "Failed Compressed Delimiter Iterator Test02 " << i << std::endl;
         return false;
      }
   }

   return true;
}

bool test_tokenizer_options()
{
   {
      std::string s = "1xyz23ijk456abc";
      std::string expected_token[] = { "1x", "23i", "456a", "" };
      std::size_t expected_token_size = sizeof(expected_token) / sizeof(std::string);

      std::deque<std::string> token_list;

      strtk::tokenize_options::type tokenize_options = strtk::tokenize_options::include_1st_delimiter +
                                                       strtk::tokenize_options::compress_delimiters;

      typedef strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer_type;

      tokenizer_type tokenizer(s,strtk::multiple_char_delimiter_predicate("abcijkxyz"),tokenize_options);
      tokenizer_type::iterator itr = tokenizer.begin();
      tokenizer_type::iterator end = tokenizer.end();

      while (end != itr)
      {
         token_list.push_back(std::string((*itr).first,(*itr).second));
         ++itr;
      }

      if (token_list.size() != expected_token_size)
      {
         std::cout << "test_tokenizer_options() - I1stD expected number of tokens does not match parsed amount!" << std::endl;
         return false;
      }

      for (std::size_t i = 0; i < expected_token_size; ++i)
      {
         if (expected_token[i] != token_list[i])
         {
            std::cout << "test_tokenizer_options() - I1stD Failed match @ " << i << std::endl;
            return false;
         }
      }
   }

   {
      std::string s = "1xyz23ijk456abc";
      std::string expected_token[] = { "1xyz", "23ijk", "456abc", "" };
      std::size_t expected_token_size = sizeof(expected_token) / sizeof(std::string);

      std::deque<std::string> token_list;

      strtk::tokenize_options::type tokenize_options = strtk::tokenize_options::include_all_delimiters;

      typedef strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer_type;

      tokenizer_type tokenizer(s,strtk::multiple_char_delimiter_predicate("abcijkxyz"),tokenize_options);
      tokenizer_type::iterator itr = tokenizer.begin();
      tokenizer_type::iterator end = tokenizer.end();

      while (end != itr)
      {
         token_list.push_back(std::string((*itr).first,(*itr).second));
         ++itr;
      }

      if (token_list.size() != expected_token_size)
      {
         std::cout << "test_tokenizer_options() - IAD expected number of tokens does not match parsed amount!" << std::endl;
         return false;
      }

      for (std::size_t i = 0; i < expected_token_size; ++i)
      {
         if (expected_token[i] != token_list[i])
         {
            std::cout << "test_tokenizer_options() - IAD Failed match @ " << i << std::endl;
            return false;
         }
      }
   }

   return true;
}

bool test_split_options()
{
   {
      std::string s = "1xyz23ijk456abc";
      std::string expected_token[] = { "1x", "23i", "456a", "" };
      std::size_t expected_token_size = sizeof(expected_token) / sizeof(std::string);

      std::deque<std::string> token_list;

      strtk::split_options::type split_options = strtk::split_options::include_1st_delimiter +
                                                 strtk::split_options::compress_delimiters;

      strtk::split(strtk::multiple_char_delimiter_predicate("abcijkxyz"),
                   s,
                   strtk::range_to_type_back_inserter(token_list),
                   split_options);

      if (token_list.size() != expected_token_size)
      {
         std::cout << "test_split_options() - I1stD expected number of tokens does not match parsed amount!" << std::endl;
         return false;
      }

      for (std::size_t i = 0; i < expected_token_size; ++i)
      {
         if (expected_token[i] != token_list[i])
         {
            std::cout << "test_split_options() - I1stD Failed match @ " << i << std::endl;
            return false;
         }
      }
   }

   {
      std::string s = "1xyz23ijk456abc";
      std::string expected_token[] = { "1xyz", "23ijk", "456abc", "" };
      std::size_t expected_token_size = sizeof(expected_token) / sizeof(std::string);

      std::deque<std::string> token_list;

      strtk::split_options::type split_options = strtk::split_options::include_all_delimiters;

      strtk::split(strtk::multiple_char_delimiter_predicate("abcijkxyz"),
                   s,
                   strtk::range_to_type_back_inserter(token_list),
                   split_options);

      if (token_list.size() != expected_token_size)
      {
         std::cout << "test_split_options() - IAD expected number of tokens does not match parsed amount!" << std::endl;
         return false;
      }

      for (std::size_t i = 0; i < expected_token_size; ++i)
      {
         if (expected_token[i] != token_list[i])
         {
            std::cout << "test_split_options() - IAD Failed match @ " << i << std::endl;
            return false;
         }
      }
   }

   return true;
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

inline void std_double_to_string(const double& d, std::string& s)
{
   char buffer[128];
   std::size_t sz = sprintf(buffer,"%20.19e",d);
   buffer[sz] = 0;
   s.assign(buffer);
}

inline bool strtk_isnan(const double& v)
{
   volatile double d = v;
   return d != d;
}

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
                             "0.0e10",                     "0e+10",                     "0e-10",
                            "-0.0e10",                     "+0e10",                    "00e010",
                            "000.000",                 "0000.0000",                     "1234.",
                               "01.2",                  "0012.340",               "00012.34500",
                                  "0",                       "123",                      "0123",
                              "00123",                      "-123",                      "+123",
                    "0",    "1",    "2",    "3",    "4",    "5",    "6",    "7",    "8",    "9",
                   "+0",   "+1",   "+2",   "+3",   "+4",   "+5",   "+6",   "+7",   "+8",   "+9",
                   "-0",   "-1",   "-2",   "-3",   "-4",   "-5",   "-6",   "-7",   "-8",   "-9",
                  "0.0",  "1.0",  "2.0",  "3.0",  "4.0",  "5.0",  "6.0",  "7.0",  "8.0",  "9.0",
                 "+0.0", "+1.0", "+2.0", "+3.0", "+4.0", "+5.0", "+6.0", "+7.0", "+8.0", "+9.0",
                 "-0.0", "-1.0", "-2.0", "-3.0", "-4.0", "-5.0", "-6.0", "-7.0", "-8.0", "-9.0",
        ".0e+0",  ".1e+1",  ".2e+2",  ".3e+3",  ".4e+4",  ".5e+5",  ".6e+6",  ".7e+7",  ".8e+8",  ".9e+9",
       ".00e+0", ".01e+1", ".02e+2", ".03e+3", ".04e+4", ".05e+5", ".06e+6", ".07e+7", ".08e+8", ".09e+9",
       ".0e+00", ".1e+01", ".2e+02", ".3e+03", ".4e+04", ".5e+05", ".6e+06", ".7e+07", ".8e+08", ".9e+09",
        ".0E+0",  ".1E+1",  ".2E+2",  ".3E+3",  ".4E+4",  ".5E+5",  ".6E+6",  ".7E+7",  ".8E+8",  ".9E+9",
       ".00E+0", ".01E+1", ".02E+2", ".03E+3", ".04E+4", ".05E+5", ".06E+6", ".07E+7", ".08E+8", ".09E+9",
       ".0E+00", ".1E+01", ".2E+02", ".3E+03", ".4E+04", ".5E+05", ".6E+06", ".7E+07", ".8E+08", ".9E+09",
       "+.0e+0", "+.1e+1", "+.2e+2", "+.3e+3", "+.4e+4", "+.5e+5", "+.6e+6", "+.7e+7", "+.8e+8", "+.9e+9",
       "-.0e+0", "-.1e+1", "-.2e+2", "-.3e+3", "-.4e+4", "-.5e+5", "-.6e+6", "-.7e+7", "-.8e+8", "-.9e+9",
                "0.",  "1.",  "2.",  "3.",  "4.",  "5.",  "6.",  "7.",  "8.",  "9.",
               "+0.", "+1.", "+2.", "+3.", "+4.", "+5.", "+6.", "+7.", "+8.", "+9.",
               "-0.", "-1.", "-2.", "-3.", "-4.", "-5.", "-6.", "-7.", "-8.", "-9.",
               ".0",  ".1",  ".2",  ".3",  ".4",  ".5",  ".6",  ".7",  ".8",  ".9",
               "-.0",  "-.1",  "-.2",  "-.3",  "-.4",  "-.5",  "-.6",  "-.7",  "-.8",  "-.9",
               "+.0",  "+.1",  "+.2",  "+.3",  "+.4",  "+.5",  "+.6",  "+.7",  "+.8",  "+.9",
       "+0.0f", "+1.0f", "+2.0f", "+3.0f", "+4.0f", "+5.0f", "+6.0f", "+7.0f", "+8.0f", "+9.0f",
       "+0.0F", "+1.0F", "+2.0F", "+3.0F", "+4.0F", "+5.0F", "+6.0F", "+7.0F", "+8.0F", "+9.0F",
       "+0.0l", "+1.0l", "+2.0l", "+3.0l", "+4.0l", "+5.0l", "+6.0l", "+7.0l", "+8.0l", "+9.0l",
       "+0.0L", "+1.0L", "+2.0L", "+3.0L", "+4.0L", "+5.0L", "+6.0L", "+7.0L", "+8.0L", "+9.0L",
       "-0.0f", "-1.0f", "-2.0f", "-3.0f", "-4.0f", "-5.0f", "-6.0f", "-7.0f", "-8.0f", "-9.0f",
       "-0.0F", "-1.0F", "-2.0F", "-3.0F", "-4.0F", "-5.0F", "-6.0F", "-7.0F", "-8.0F", "-9.0F",
       "-0.0l", "-1.0l", "-2.0l", "-3.0l", "-4.0l", "-5.0l", "-6.0l", "-7.0l", "-8.0l", "-9.0l",
       "-0.0L", "-1.0L", "-2.0L", "-3.0L", "-4.0L", "-5.0L", "-6.0L", "-7.0L", "-8.0L", "-9.0L",
       "+0.0e+1f", "+1.0e+1f", "+2.0e+1f", "+3.0e+1f", "+4.0e+1f", "+5.0e+1f", "+6.0e+1f", "+7.0e+1f", "+8.0e+1f", "+9.0e+1f",
       "+0.0E-2F", "+1.0E-2F", "+2.0E-2F", "+3.0E-2F", "+4.0E-2F", "+5.0E-2F", "+6.0E-2F", "+7.0E-2F", "+8.0E-2F", "+9.0E-2F",
       "+0.0e+3l", "+1.0e+3l", "+2.0e+3l", "+3.0e+3l", "+4.0e+3l", "+5.0e+3l", "+6.0e+3l", "+7.0e+3l", "+8.0e+3l", "+9.0e+3l",
       "+0.0E-4L", "+1.0E-4L", "+2.0E-4L", "+3.0E-4L", "+4.0E-4L", "+5.0E-4L", "+6.0E-4L", "+7.0E-4L", "+8.0E-4L", "+9.0E-4L",
       "-0.0e+5f", "-1.0e+5f", "-2.0e+5f", "-3.0e+5f", "-4.0e+5f", "-5.0e+5f", "-6.0e+5f", "-7.0e+5f", "-8.0e+5f", "-9.0e+5f",
       "-0.0E-6F", "-1.0E-6F", "-2.0E-6F", "-3.0E-6F", "-4.0E-6F", "-5.0E-6F", "-6.0E-6F", "-7.0E-6F", "-8.0E-6F", "-9.0E-6F",
       "-0.0e+7l", "-1.0e+7l", "-2.0e+7l", "-3.0e+7l", "-4.0e+7l", "-5.0e+7l", "-6.0e+7l", "-7.0e+7l", "-8.0e+7l", "-9.0e+7l",
       "-0.0E-8L", "-1.0E-8L", "-2.0E-8L", "-3.0E-8L", "-4.0E-8L", "-5.0E-8L", "-6.0E-8L", "-7.0E-8L", "-8.0E-8L", "-9.0E-8L",
      "0.1", "0.01", "0.001", "0.0001", "0.00001", "0.000001", "0.0000001", "0.00000001", "0.000000001",
      "0.2", "0.02", "0.002", "0.0002", "0.00002", "0.000002", "0.0000002", "0.00000002", "0.000000002",
      "0.3", "0.03", "0.003", "0.0003", "0.00003", "0.000003", "0.0000003", "0.00000003", "0.000000003",
      "0.4", "0.04", "0.004", "0.0004", "0.00004", "0.000004", "0.0000004", "0.00000004", "0.000000004",
      "0.5", "0.05", "0.005", "0.0005", "0.00005", "0.000005", "0.0000005", "0.00000005", "0.000000005",
      "0.6", "0.06", "0.006", "0.0006", "0.00006", "0.000006", "0.0000006", "0.00000006", "0.000000006",
      "0.7", "0.07", "0.007", "0.0007", "0.00007", "0.000007", "0.0000007", "0.00000007", "0.000000007",
      "0.8", "0.08", "0.008", "0.0008", "0.00008", "0.000008", "0.0000008", "0.00000008", "0.000000008",
      "0.9", "0.09", "0.009", "0.0009", "0.00009", "0.000009", "0.0000009", "0.00000009", "0.000000009",
      "+0.1", "+0.01", "+0.001", "+0.0001", "+0.00001", "+0.000001", "+0.0000001", "+0.00000001", "+0.000000001",
      "+0.2", "+0.02", "+0.002", "+0.0002", "+0.00002", "+0.000002", "+0.0000002", "+0.00000002", "+0.000000002",
      "+0.3", "+0.03", "+0.003", "+0.0003", "+0.00003", "+0.000003", "+0.0000003", "+0.00000003", "+0.000000003",
      "+0.4", "+0.04", "+0.004", "+0.0004", "+0.00004", "+0.000004", "+0.0000004", "+0.00000004", "+0.000000004",
      "+0.5", "+0.05", "+0.005", "+0.0005", "+0.00005", "+0.000005", "+0.0000005", "+0.00000005", "+0.000000005",
      "+0.6", "+0.06", "+0.006", "+0.0006", "+0.00006", "+0.000006", "+0.0000006", "+0.00000006", "+0.000000006",
      "+0.7", "+0.07", "+0.007", "+0.0007", "+0.00007", "+0.000007", "+0.0000007", "+0.00000007", "+0.000000007",
      "+0.8", "+0.08", "+0.008", "+0.0008", "+0.00008", "+0.000008", "+0.0000008", "+0.00000008", "+0.000000008",
      "+0.9", "+0.09", "+0.009", "+0.0009", "+0.00009", "+0.000009", "+0.0000009", "+0.00000009", "+0.000000009",
      "-0.1", "-0.01", "-0.001", "-0.0001", "-0.00001", "-0.000001", "-0.0000001", "-0.00000001", "-0.000000001",
      "-0.2", "-0.02", "-0.002", "-0.0002", "-0.00002", "-0.000002", "-0.0000002", "-0.00000002", "-0.000000002",
      "-0.3", "-0.03", "-0.003", "-0.0003", "-0.00003", "-0.000003", "-0.0000003", "-0.00000003", "-0.000000003",
      "-0.4", "-0.04", "-0.004", "-0.0004", "-0.00004", "-0.000004", "-0.0000004", "-0.00000004", "-0.000000004",
      "-0.5", "-0.05", "-0.005", "-0.0005", "-0.00005", "-0.000005", "-0.0000005", "-0.00000005", "-0.000000005",
      "-0.6", "-0.06", "-0.006", "-0.0006", "-0.00006", "-0.000006", "-0.0000006", "-0.00000006", "-0.000000006",
      "-0.7", "-0.07", "-0.007", "-0.0007", "-0.00007", "-0.000007", "-0.0000007", "-0.00000007", "-0.000000007",
      "-0.8", "-0.08", "-0.008", "-0.0008", "-0.00008", "-0.000008", "-0.0000008", "-0.00000008", "-0.000000008",
      "-0.9", "-0.09", "-0.009", "-0.0009", "-0.00009", "-0.000009", "-0.0000009", "-0.00000009", "-0.000000009",
                      "1.01", "0.101", "1.0101", "0.10101", "1.010101", "0.1010101", "1.01010101", "1.101010101",
                      "2.02", "0.202", "2.0202", "0.20202", "2.020202", "0.2020202", "2.02020202", "2.202020202",
                      "3.03", "0.303", "3.0303", "0.30303", "3.030303", "0.3030303", "3.03030303", "3.303030303",
                      "4.04", "0.404", "4.0404", "0.40404", "4.040404", "0.4040404", "4.04040404", "4.404040404",
                      "5.05", "0.505", "5.0505", "0.50505", "5.050505", "0.5050505", "5.05050505", "5.505050505",
                      "6.06", "0.606", "6.0606", "0.60606", "6.060606", "0.6060606", "6.06060606", "6.606060606",
                      "7.07", "0.707", "7.0707", "0.70707", "7.070707", "0.7070707", "7.07070707", "7.707070707",
                      "8.08", "0.808", "8.0808", "0.80808", "8.080808", "0.8080808", "8.08080808", "8.808080808",
                      "9.09", "0.909", "9.0909", "0.90909", "9.090909", "0.9090909", "9.09090909", "9.909090909",
              "+1.01", "+0.101", "+1.0101", "+0.10101", "+1.010101", "+0.1010101", "+1.01010101", "+1.101010101",
              "+2.02", "+0.202", "+2.0202", "+0.20202", "+2.020202", "+0.2020202", "+2.02020202", "+2.202020202",
              "+3.03", "+0.303", "+3.0303", "+0.30303", "+3.030303", "+0.3030303", "+3.03030303", "+3.303030303",
              "+4.04", "+0.404", "+4.0404", "+0.40404", "+4.040404", "+0.4040404", "+4.04040404", "+4.404040404",
              "+5.05", "+0.505", "+5.0505", "+0.50505", "+5.050505", "+0.5050505", "+5.05050505", "+5.505050505",
              "+6.06", "+0.606", "+6.0606", "+0.60606", "+6.060606", "+0.6060606", "+6.06060606", "+6.606060606",
              "+7.07", "+0.707", "+7.0707", "+0.70707", "+7.070707", "+0.7070707", "+7.07070707", "+7.707070707",
              "+8.08", "+0.808", "+8.0808", "+0.80808", "+8.080808", "+0.8080808", "+8.08080808", "+8.808080808",
              "+9.09", "+0.909", "+9.0909", "+0.90909", "+9.090909", "+0.9090909", "+9.09090909", "+9.909090909",
              "-1.01", "-0.101", "-1.0101", "-0.10101", "-1.010101", "-0.1010101", "-1.01010101", "-1.101010101",
              "-2.02", "-0.202", "-2.0202", "-0.20202", "-2.020202", "-0.2020202", "-2.02020202", "-2.202020202",
              "-3.03", "-0.303", "-3.0303", "-0.30303", "-3.030303", "-0.3030303", "-3.03030303", "-3.303030303",
              "-4.04", "-0.404", "-4.0404", "-0.40404", "-4.040404", "-0.4040404", "-4.04040404", "-4.404040404",
              "-5.05", "-0.505", "-5.0505", "-0.50505", "-5.050505", "-0.5050505", "-5.05050505", "-5.505050505",
              "-6.06", "-0.606", "-6.0606", "-0.60606", "-6.060606", "-0.6060606", "-6.06060606", "-6.606060606",
              "-7.07", "-0.707", "-7.0707", "-0.70707", "-7.070707", "-0.7070707", "-7.07070707", "-7.707070707",
              "-8.08", "-0.808", "-8.0808", "-0.80808", "-8.080808", "-0.8080808", "-8.08080808", "-8.808080808",
              "-9.09", "-0.909", "-9.0909", "-0.90909", "-9.090909", "-0.9090909", "-9.09090909", "-9.909090909",
               "+inf",         "-inf",
               "+INF",         "-INF",
               "+1.0#inf", "-1.0#inf",
               "+1.0#INF", "-1.0#INF",
               "+infinity",         "-infinity",
               "+INFINITY",         "-INFINITY",
               "+1.0#infinity", "-1.0#infinity",
               "+1.0#INFINITY", "-1.0#INFINITY",
                    "NAN",    "nan",
                "1.0#NAN", "1.0#nan"
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
                         -123.456e123,             +123.456E123,             -123.456e+123,
                        +123.456E+123,            -123.456e0123,             +123.456E0123,
                         -123.456e+03,             +123.456E+03,             -123.456e0003,
                        +123.456E0003,           -123.456e+0003,            +123.456E+0003,
                          -123.4560e3,             +123.45600E3,            -123.456000e+3,
                       +123.456000E+3,          -123.4560000e03,          +123.45600000E03,
                   -123.456000000e+03,      +123.4560000000E+03,     -123.45600000000e0003,
               +123.456000000000E0003, -123.4560000000000e+0003, +123.45600000000000E+0003,
                        -123.456e+003,            +123.456E+003,              -123.45600E3,
                        123.456e+0003,                      0.0,                       0.0,
                                  0.0,                      0.0,                       0.0,
                                  0.0,                      0.0,                       0.0,
                                  0.0,                      0.0,                     1234.,
                                  1.2,                    12.34,                  12.34500,
                                    0,                      123,                       123,
                                  123,                     -123,                      +123,
                                 0.0,  1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,  9.0,
                                +0.0, +1.0, +2.0, +3.0, +4.0, +5.0, +6.0, +7.0, +8.0, +9.0,
                                -0.0, -1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0,
                                 0.0,  1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,  9.0,
                                +0.0, +1.0, +2.0, +3.0, +4.0, +5.0, +6.0, +7.0, +8.0, +9.0,
                                -0.0, -1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0,
              .0e+0,  .1e+1,  .2e+2,  .3e+3,  .4e+4,  .5e+5,  .6e+6,  .7e+7,  .8e+8,  .9e+9,
             .00e+0, .01e+1, .02e+2, .03e+3, .04e+4, .05e+5, .06e+6, .07e+7, .08e+8, .09e+9,
             .0e+00, .1e+01, .2e+02, .3e+03, .4e+04, .5e+05, .6e+06, .7e+07, .8e+08, .9e+09,
              .0E+0,  .1E+1,  .2E+2,  .3E+3,  .4E+4,  .5E+5,  .6E+6,  .7E+7,  .8E+8,  .9E+9,
             .00E+0, .01E+1, .02E+2, .03E+3, .04E+4, .05E+5, .06E+6, .07E+7, .08E+8, .09E+9,
             .0E+00, .1E+01, .2E+02, .3E+03, .4E+04, .5E+05, .6E+06, .7E+07, .8E+08, .9E+09,
             +.0e+0, +.1e+1, +.2e+2, +.3e+3, +.4e+4, +.5e+5, +.6e+6, +.7e+7, +.8e+8, +.9e+9,
             -.0e+0, -.1e+1, -.2e+2, -.3e+3, -.4e+4, -.5e+5, -.6e+6, -.7e+7, -.8e+8, -.9e+9,
                                  0.0,  1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,  9.0,
                                 +0.0, +1.0, +2.0, +3.0, +4.0, +5.0, +6.0, +7.0, +8.0, +9.0,
                                 -0.0, -1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0,
                         0.0,   0.1,   0.2,   0.3,   0.4,   0.5 ,  0.6,   0.7,   0.8,   0.9,
                        -0.0,  -0.1,  -0.2,  -0.3,  -0.4,  -0.5,  -0.6,  -0.7,  -0.8,  -0.9,
                        +0.0,  +0.1,  +0.2,  +0.3,  +0.4,  +0.5,  +0.6,  +0.7,  +0.8,  +0.9,
                                 +0.0, +1.0, +2.0, +3.0, +4.0, +5.0, +6.0, +7.0, +8.0, +9.0,
                                 +0.0, +1.0, +2.0, +3.0, +4.0, +5.0, +6.0, +7.0, +8.0, +9.0,
                                 +0.0, +1.0, +2.0, +3.0, +4.0, +5.0, +6.0, +7.0, +8.0, +9.0,
                                 +0.0, +1.0, +2.0, +3.0, +4.0, +5.0, +6.0, +7.0, +8.0, +9.0,
                                 -0.0, -1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0,
                                 -0.0, -1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0,
                                 -0.0, -1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0,
                                 -0.0, -1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0,
   +0.0e+1, +1.0e+1, +2.0e+1, +3.0e+1, +4.0e+1, +5.0e+1, +6.0e+1, +7.0e+1, +8.0e+1, +9.0e+1,
   +0.0E-2, +1.0E-2, +2.0E-2, +3.0E-2, +4.0E-2, +5.0E-2, +6.0E-2, +7.0E-2, +8.0E-2, +9.0E-2,
   +0.0e+3, +1.0e+3, +2.0e+3, +3.0e+3, +4.0e+3, +5.0e+3, +6.0e+3, +7.0e+3, +8.0e+3, +9.0e+3,
   +0.0E-4, +1.0E-4, +2.0E-4, +3.0E-4, +4.0E-4, +5.0E-4, +6.0E-4, +7.0E-4, +8.0E-4, +9.0E-4,
   -0.0e+5, -1.0e+5, -2.0e+5, -3.0e+5, -4.0e+5, -5.0e+5, -6.0e+5, -7.0e+5, -8.0e+5, -9.0e+5,
   -0.0E-6, -1.0E-6, -2.0E-6, -3.0E-6, -4.0E-6, -5.0E-6, -6.0E-6, -7.0E-6, -8.0E-6, -9.0E-6,
   -0.0e+7, -1.0e+7, -2.0e+7, -3.0e+7, -4.0e+7, -5.0e+7, -6.0e+7, -7.0e+7, -8.0e+7, -9.0e+7,
   -0.0E-8, -1.0E-8, -2.0E-8, -3.0E-8, -4.0E-8, -5.0E-8, -6.0E-8, -7.0E-8, -8.0E-8, -9.0E-8,
            0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001, 0.0000001, 0.00000001, 0.000000001,
            0.2, 0.02, 0.002, 0.0002, 0.00002, 0.000002, 0.0000002, 0.00000002, 0.000000002,
            0.3, 0.03, 0.003, 0.0003, 0.00003, 0.000003, 0.0000003, 0.00000003, 0.000000003,
            0.4, 0.04, 0.004, 0.0004, 0.00004, 0.000004, 0.0000004, 0.00000004, 0.000000004,
            0.5, 0.05, 0.005, 0.0005, 0.00005, 0.000005, 0.0000005, 0.00000005, 0.000000005,
            0.6, 0.06, 0.006, 0.0006, 0.00006, 0.000006, 0.0000006, 0.00000006, 0.000000006,
            0.7, 0.07, 0.007, 0.0007, 0.00007, 0.000007, 0.0000007, 0.00000007, 0.000000007,
            0.8, 0.08, 0.008, 0.0008, 0.00008, 0.000008, 0.0000008, 0.00000008, 0.000000008,
            0.9, 0.09, 0.009, 0.0009, 0.00009, 0.000009, 0.0000009, 0.00000009, 0.000000009,
   +0.1, +0.01, +0.001, +0.0001, +0.00001, +0.000001, +0.0000001, +0.00000001, +0.000000001,
   +0.2, +0.02, +0.002, +0.0002, +0.00002, +0.000002, +0.0000002, +0.00000002, +0.000000002,
   +0.3, +0.03, +0.003, +0.0003, +0.00003, +0.000003, +0.0000003, +0.00000003, +0.000000003,
   +0.4, +0.04, +0.004, +0.0004, +0.00004, +0.000004, +0.0000004, +0.00000004, +0.000000004,
   +0.5, +0.05, +0.005, +0.0005, +0.00005, +0.000005, +0.0000005, +0.00000005, +0.000000005,
   +0.6, +0.06, +0.006, +0.0006, +0.00006, +0.000006, +0.0000006, +0.00000006, +0.000000006,
   +0.7, +0.07, +0.007, +0.0007, +0.00007, +0.000007, +0.0000007, +0.00000007, +0.000000007,
   +0.8, +0.08, +0.008, +0.0008, +0.00008, +0.000008, +0.0000008, +0.00000008, +0.000000008,
   +0.9, +0.09, +0.009, +0.0009, +0.00009, +0.000009, +0.0000009, +0.00000009, +0.000000009,
   -0.1, -0.01, -0.001, -0.0001, -0.00001, -0.000001, -0.0000001, -0.00000001, -0.000000001,
   -0.2, -0.02, -0.002, -0.0002, -0.00002, -0.000002, -0.0000002, -0.00000002, -0.000000002,
   -0.3, -0.03, -0.003, -0.0003, -0.00003, -0.000003, -0.0000003, -0.00000003, -0.000000003,
   -0.4, -0.04, -0.004, -0.0004, -0.00004, -0.000004, -0.0000004, -0.00000004, -0.000000004,
   -0.5, -0.05, -0.005, -0.0005, -0.00005, -0.000005, -0.0000005, -0.00000005, -0.000000005,
   -0.6, -0.06, -0.006, -0.0006, -0.00006, -0.000006, -0.0000006, -0.00000006, -0.000000006,
   -0.7, -0.07, -0.007, -0.0007, -0.00007, -0.000007, -0.0000007, -0.00000007, -0.000000007,
   -0.8, -0.08, -0.008, -0.0008, -0.00008, -0.000008, -0.0000008, -0.00000008, -0.000000008,
   -0.9, -0.09, -0.009, -0.0009, -0.00009, -0.000009, -0.0000009, -0.00000009, -0.000000009,
                 1.01, 0.101, 1.0101, 0.10101, 1.010101, 0.1010101, 1.01010101, 1.101010101,
                 2.02, 0.202, 2.0202, 0.20202, 2.020202, 0.2020202, 2.02020202, 2.202020202,
                 3.03, 0.303, 3.0303, 0.30303, 3.030303, 0.3030303, 3.03030303, 3.303030303,
                 4.04, 0.404, 4.0404, 0.40404, 4.040404, 0.4040404, 4.04040404, 4.404040404,
                 5.05, 0.505, 5.0505, 0.50505, 5.050505, 0.5050505, 5.05050505, 5.505050505,
                 6.06, 0.606, 6.0606, 0.60606, 6.060606, 0.6060606, 6.06060606, 6.606060606,
                 7.07, 0.707, 7.0707, 0.70707, 7.070707, 0.7070707, 7.07070707, 7.707070707,
                 8.08, 0.808, 8.0808, 0.80808, 8.080808, 0.8080808, 8.08080808, 8.808080808,
                 9.09, 0.909, 9.0909, 0.90909, 9.090909, 0.9090909, 9.09090909, 9.909090909,
         +1.01, +0.101, +1.0101, +0.10101, +1.010101, +0.1010101, +1.01010101, +1.101010101,
         +2.02, +0.202, +2.0202, +0.20202, +2.020202, +0.2020202, +2.02020202, +2.202020202,
         +3.03, +0.303, +3.0303, +0.30303, +3.030303, +0.3030303, +3.03030303, +3.303030303,
         +4.04, +0.404, +4.0404, +0.40404, +4.040404, +0.4040404, +4.04040404, +4.404040404,
         +5.05, +0.505, +5.0505, +0.50505, +5.050505, +0.5050505, +5.05050505, +5.505050505,
         +6.06, +0.606, +6.0606, +0.60606, +6.060606, +0.6060606, +6.06060606, +6.606060606,
         +7.07, +0.707, +7.0707, +0.70707, +7.070707, +0.7070707, +7.07070707, +7.707070707,
         +8.08, +0.808, +8.0808, +0.80808, +8.080808, +0.8080808, +8.08080808, +8.808080808,
         +9.09, +0.909, +9.0909, +0.90909, +9.090909, +0.9090909, +9.09090909, +9.909090909,
         -1.01, -0.101, -1.0101, -0.10101, -1.010101, -0.1010101, -1.01010101, -1.101010101,
         -2.02, -0.202, -2.0202, -0.20202, -2.020202, -0.2020202, -2.02020202, -2.202020202,
         -3.03, -0.303, -3.0303, -0.30303, -3.030303, -0.3030303, -3.03030303, -3.303030303,
         -4.04, -0.404, -4.0404, -0.40404, -4.040404, -0.4040404, -4.04040404, -4.404040404,
         -5.05, -0.505, -5.0505, -0.50505, -5.050505, -0.5050505, -5.05050505, -5.505050505,
         -6.06, -0.606, -6.0606, -0.60606, -6.060606, -0.6060606, -6.06060606, -6.606060606,
         -7.07, -0.707, -7.0707, -0.70707, -7.070707, -0.7070707, -7.07070707, -7.707070707,
         -8.08, -0.808, -8.0808, -0.80808, -8.080808, -0.8080808, -8.08080808, -8.808080808,
         -9.09, -0.909, -9.0909, -0.90909, -9.090909, -0.9090909, -9.09090909, -9.909090909,
         +std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
         +std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
         +std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
         +std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
         +std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
         +std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
         +std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
         +std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
          std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(),
          std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(),
        };

   std::size_t d_size = sizeof(double_str) / sizeof(std::string);

   double v = 0.0;
   for (std::size_t i = 0; i < d_size; ++i)
   {
      v = 0.0;
      if (!strtk::string_to_type_converter(double_str[i],v))
      {
         std::cout << "test_double_convert() double convert[" << i << "]" << std::endl;
         return false;
      }

      if ((d[i] != v) && (!strtk_isnan(v)))
      {
         std::cout << "test_double_convert() double check[" << i << "]" << std::endl;
         return false;
      }
   }

   static const std::string bad_list[] =
                       {
                          "1234567.8A",      "1234567.A",      "12345A67.8",      "A1234567.8A",
                         "1234567.8Ae", "1234567.AE+100", "12345A67.8e+100", "A1234567.8AE+100",
                         "--1.234e+10",    "++1.234e+10",      "1.234e+10-",       "1.234e+10+",
                          "1.234e++10",     "1.234E--10",         "1.e++10",          "1.E--10",
                          "1..234e+10",     "1.23.4E-10",      "1.234e-1.0",            "1.23x",
                          "1.23e+100y",               "",            "123+",             "123-",
                               "123.+",          "123.-",           "123+.",            "123-.",
                            "123..123",       "123.-123",        "123.+123",           "123-.0",
                                                          "+", "-", "+.", "-.", ".+", ".-", ".",
                                                   "++", "--", "++.", "--.", ".++", ".--", "..",
                                                   "+f", "-f", "+.f", "-.f", ".+f", ".-f", ".f",
                                                   "+l", "-l", "+.l", "-.l", ".+l", ".-l", ".l",
                           ".+0", ".+1", ".+2", ".+3", ".+4", ".+5", ".+6", ".+7", ".+8", ".+9",
                           ".-0", ".-1", ".-2", ".-3", ".-4", ".-5", ".-6", ".-7", ".-8", ".-9",
                 "..+0", "..+1", "..+2", "..+3", "..+4", "..+5", "..+6", "..+7", "..+8", "..+9",
                 "..-0", "..-1", "..-2", "..-3", "..-4", "..-5", "..-6", "..-7", "..-8", "..-9",
                 ".+0f", ".+1F", ".+2f", ".+3F", ".+4f", ".+5F", ".+6f", ".+7F", ".+8f", ".+9F",
                 ".-0l", ".-1L", ".-2l", ".-3L", ".-4l", ".-5L", ".-6l", ".-7L", ".-8l", ".-9L"
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

   {
      static const std::size_t count = 10000000;
      std::string s;
      s.reserve(32);
      double t = 0.0;
      for (int i = -static_cast<int>(count); i < static_cast<int>(count); ++i)
      {
         if (!strtk::type_to_string(i,s))
         {
            std::cout << "test_double_convert() - Failed int to string @ " << i << std::endl;
            return false;
         }

         if (!strtk::string_to_type_converter(s,t))
         {
            std::cout << "test_double_convert() - Failed string to double @ " << i << std::endl;
            return false;
         }

         if (static_cast<double>(i) != t)
         {
            std::cout << "test_double_convert() - Failed i == t @ " << i << std::endl;
            return false;
         }

         s.clear();
      }
   }

   {
      std::string s;
      s.reserve(256);
      double v = -111111.11;
      double t = 0.0;
      while (v < +111111.11)
      {
         std_double_to_string(v,s);
         if (!strtk::string_to_type_converter(s,t))
         {
            std::cout << "test_double_convert() - Failed decimal to string,  t: " << t << std::endl;
            return false;
         }

         if (std::abs(t - v) > 0.0000000001)
         {
            std::cout << "test_double_convert() - Failed decimal r == t  r: " << t << std::endl;
            return false;
         }

         v += 0.01;
         s.clear();
         t = 0.0;
      }
   }

   {
      double d1 = +123.456;
      double d2 = -123.456;
      double delta[] =
             {
               0.333,   0.555,   0.777,
               0.999, 0.13579, 0.97531
             };
      std::size_t delta_size = sizeof(delta) / sizeof(double);
      std::string s;
      s.reserve(256);
      double tmp = 0.0;
      for (int i = 0; i < 1000000; ++i)
      {
         std_double_to_string(d1,s);
         if (!strtk::string_to_type_converter(s,tmp))
         {
            std::cout << "test_double_convert() - (1) final test string to double failure @ " << i << std::endl;
            return false;
         }

         if (std::abs(d1 - tmp) > 0.000000001)
         {
            std::cout << "test_double_convert() - (1) final test compare failure @ " << i << std::endl;
            return false;
         }

         d1 += delta[std::abs(i) % delta_size];

         std_double_to_string(d2,s);
         if (!strtk::string_to_type_converter(s,tmp))
         {
            std::cout << "test_double_convert() - (2) final test string to double failure @ " << i << std::endl;
            return false;
         }

         if (std::abs(d2 - tmp) > 0.000000001)
         {
            std::cout << "test_double_convert() - (2) final test compare failure @ " << i << std::endl;
            return false;
         }

         d2 -= delta[std::abs(i) % delta_size];
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

   bool result = true;

   o.clear(1);
   result = strtk::parse(output,"|",o.d1);
   if (!result || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 1" << std::endl;
      return false;
   }

   o.clear(2);
   result = strtk::parse(output,"|",o.d1,o.d2);
   if (!result || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 2" << std::endl;
      return false;
   }

   o.clear(3);
   result = strtk::parse(output,"|",o.d1,o.d2,o.d3);
   if (!result || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 3" << std::endl;
      return false;
   }

   o.clear(4);
   result = strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4);
   if (!result || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 4" << std::endl;
      return false;
   }

   o.clear(5);
   result = strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4,o.d5);
   if (!result || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 5" << std::endl;
      return false;
   }

   o.clear(6);
   result = strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4,o.d5,o.d6);
   if (!result || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 6" << std::endl;
      return false;
   }

   o.clear(7);
   result = strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4,o.d5,o.d6,o.d7);
   if (!result || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 7" << std::endl;
      return false;
   }

   o.clear(8);
   result = strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4,o.d5,o.d6,o.d7,o.d8);
   if (!result || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 8" << std::endl;
      return false;
   }

   o.clear(9);
   result = strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4,o.d5,o.d6,o.d7,o.d8,o.d9);
   if (!result || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 9" << std::endl;
      return false;
   }

   o.clear(10);
   result = strtk::parse(output,"|",o.d1,o.d2,o.d3,o.d4,o.d5,o.d6,o.d7,o.d8,o.d9,o.d10);
   if (!result || (o != i))
   {
      std::cout << "test_construct_and_parse() - parse fail 10" << std::endl;
      return false;
   }

   return true;
}

bool test_int_uint_convert()
{
   static const std::size_t count = 20000000;
   std::string s;
   s.reserve(32);

   {
      int t = 0;
      for (int i = -static_cast<int>(count); i < static_cast<int>(count); ++i)
      {
         if (!strtk::type_to_string(i,s))
         {
            std::cout << "test_int_uint_convert() - Failed int to string @ " << i << std::endl;
            return false;
         }

         if (!strtk::string_to_type_converter(s,t))
         {
            std::cout << "test_int_uint_convert() - Failed string to int @ " << i << std::endl;
            return false;
         }

         if (i != t)
         {
            std::cout << "test_int_uint_convert() - Failed (int) i == t @ " << i << std::endl;
            return false;
         }

         s.clear();
      }
   }

   {
      unsigned int t = 0;
      for (unsigned int i = 0; i < (2 * count); ++i)
      {
         if (!strtk::type_to_string(i,s))
         {
            std::cout << "test_int_uint_convert() - Failed uint to string @ " << i << std::endl;
            return false;
         }

         if (!strtk::string_to_type_converter(s,t))
         {
            std::cout << "test_int_uint_convert() - Failed string to uint @ " << i << std::endl;
            return false;
         }

         if (i != t)
         {
            std::cout << "test_int_uint_convert() - Failed (uint) i == t @ " << i << std::endl;
            return false;
         }

         s.clear();
      }
   }

   static const std::string bad_list[]
                = {
                     "12345678901",            "+12345678901",
                     "123456789012",           "+123456789012",
                     "1234567890123",          "+1234567890123",
                     "12345678901234",         "+12345678901234",
                     "123456789012345",        "+123456789012345",
                     "00000123456789012345",   "+00000123456789012345",
                     "12345678901234500000",   "+12345678901234500000",
                     "1234A",                  "+1234A",
                     "12A34",                  "+12A34",
                     "A1234",                  "+A1234",
                     "-12345678901",           "012345678901",
                     "-123456789012",          "00123456789012",
                     "-1234567890123",         "0001234567890123",
                     "-12345678901234",        "00012345678901234",
                     "-123456789012345",       "000123456789012345",
                     "-00000123456789012345",  "-012345678901",
                     "-12345678901234500000",  "-00123456789012",
                     "-1234A",                 "-0001234567890123",
                     "-12A34",                 "-00012345678901234",
                     "-A1234",                 "-000123456789012345",
                     "+012345678901",
                     "+00123456789012",
                     "+0001234567890123",
                     "+00012345678901234",
                     "+000123456789012345",
                     ""
                  };
   static const std::size_t bad_list_size = sizeof(bad_list) / sizeof(std::string);

   {
      int t = 0;
      for (std::size_t i = 0; i < bad_list_size; ++i)
      {
         if (strtk::string_to_type_converter(bad_list[i],t))
         {
            std::cout << "test_int_uint_convert() - Failed bad test for int @ " << i << std::endl;
            return false;
         }
      }
   }

   {
      unsigned int t = 0;
      for (std::size_t i = 0; i < bad_list_size; ++i)
      {
         if (strtk::string_to_type_converter(bad_list[i],t))
         {
            std::cout << "test_int_uint_convert() - Failed bad test for uint @ " << i << std::endl;
            return false;
         }
      }
   }

   {
      short t = 0;
      for (std::size_t i = 0; i < bad_list_size; ++i)
      {
         if (strtk::string_to_type_converter(bad_list[i],t))
         {
            std::cout << "test_int_uint_convert() - Failed bad test for short @ " << i << std::endl;
            return false;
         }
      }
   }

   {
      unsigned short t = 0;
      for (std::size_t i = 0; i < bad_list_size; ++i)
      {
         if (strtk::string_to_type_converter(bad_list[i],t))
         {
            std::cout << "test_int_uint_convert() - Failed bad test for ushort @ " << i << std::endl;
            return false;
         }
      }
   }

   {
      std::string s1 =  "65536";
      std::string s2 = "+65536";
      std::string s3 =  "99999";
      unsigned short t = 0;

      if (strtk::string_to_type_converter(s1,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for max1 ushort str: " << s << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s2,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for max2 ushort str: " << s << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s3,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for extreme ushort str: " << s << std::endl;
         return false;
      }
   }

   {
      std::string s1 = "-32769";
      std::string s2 = "+32768";
      std::string s3 =  "32768";
      std::string s4 =  "99999";
      short int t = 0;

      if (strtk::string_to_type_converter(s1,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for min short str: " << s << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s2,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for max1 short str: " << s << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s3,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for max2 short str: " << s << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s4,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for extreme short str: " << s << std::endl;
         return false;
      }
   }

   {
      std::string s1 =  "4294967296";
      std::string s2 = "+4294967296";
      std::string s3 =  "9999999999";
      unsigned int t = 0;

      if (strtk::string_to_type_converter(s1,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for max1 uint str: " << s << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s2,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for max2 uint str: " << s << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s3,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for extreme uint str: " << s << std::endl;
         return false;
      }
   }

   {
      std::string s1 = "-2147483649";
      std::string s2 = "+2147483648";
      std::string s3 =  "2147483648";
      std::string s4 =  "9999999999";
      int t = 0;

      if (strtk::string_to_type_converter(s1,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for min int str: " << s1 << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s2,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for max1 int str: " << s2 << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s3,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for max2 int str: " << s3 << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s4,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for extreme int str: " << s3 << std::endl;
         return false;
      }
   }

   {
      std::string s1 = "-0009223372036854775809";
      std::string s2 = "+0009223372036854775808";
      std::string s3 =  "0009223372036854775808";
      std::string s4 =  "0009999999999999999999";
      long long t = 0;

      if (strtk::string_to_type_converter(s1,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for min long long str: " << s1 << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s2,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for max1 long long str: " << s2 << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s3,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for max2 long long str: " << s3 << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s4,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for extreme long long str: " << s4 << std::endl;
         return false;
      }
   }

   {
      std::string s1 = "-00018446744073709551616";
      std::string s2 = "+00018446744073709551616";
      std::string s3 =  "00018446744073709551616";
      std::string s4 =  "00099999999999999999999";

      unsigned long long t = 0;

      if (strtk::string_to_type_converter(s1,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for min unsigned long long str: " << s1 << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s2,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for max1 unsigned long long str: " << s2 << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s3,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for max2 unsigned long long str: " << s3 << std::endl;
         return false;
      }

      if (strtk::string_to_type_converter(s4,t))
      {
         std::cout << "test_int_uint_convert() - Failed bad test for extreme unsigned long long str: " << s4 << std::endl;
         return false;
      }
   }

   return true;
}

bool test_parse1()
{
   std::string data = "1 ,|\t987.654 ,|\t abc ,|\t";
   int i = 0;
   double d = 0;
   std::string s;
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

   for (std::size_t i = 0; i < double_list.size(); ++i)
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

   for (std::size_t i = 0; i < float_list.size(); ++i)
   {
      if (123.456e3f != float_list[i])
      {
         std::cout << "test_parse3() float check[" << i << "] " << std::endl;
         return false;
      }
   }

   return  true;
}

bool test_parse4()
{
   {
      std::string data = "0,1,2,3|+0,+1,+2,+3|-0,-1,-2,-3";
      strtk::vector_sink<int>::type vec_sink(",");
      strtk::deque_sink<int>::type deq_sink(",");
      strtk::list_sink<int>::type lst_sink(",");

      std::vector<int> int_vec;
      std::deque<int> int_deq;
      std::list<int> int_lst;

      strtk::parse(data,"|",vec_sink(int_vec),
                            deq_sink(int_deq),
                            lst_sink(int_lst));

      if (4 != int_vec.size())
      {
         std::cout << "test_parse4() - failed int_vec.size()" << std::endl;
         return false;
      }

      if (4 != int_deq.size())
      {
         std::cout << "test_parse4() - failed int_deq.size()" << std::endl;
         return false;
      }

      if (4 != int_lst.size())
      {
         std::cout << "test_parse4() - failed int_lst.size()" << std::endl;
         return false;
      }

      std::list<int>::iterator int_lst_itr = int_lst.begin();

      for (int i = 0; i <= 3; ++i)
      {
         if (int_vec[i] != i)
         {
            std::cout << "test_parse4() - failed int_vec[" << i <<"] == " << i << std::endl;
            return false;
         }

         if (int_deq[i] != i)
         {
            std::cout << "test_parse4() - failed int_deq[" << i <<"] == " << i << std::endl;
            return false;
         }

         if (*(int_lst_itr) != -i)
         {
            std::cout << "test_parse4() - failed int_lst[" << i <<"] == " << i << std::endl;
            return false;
         }
         else
            ++int_lst_itr;
      }
   }

   {
      std::string data = "0.0,1.1,2.2,3.3|+0.0,+1.1,+2.2,+3.3|-0.0,-1.1,-2.2,-3.3";
      strtk::vector_sink<double>::type vec_sink(",");
      strtk::deque_sink<double>::type  deq_sink(",");
      strtk::list_sink<double>::type  lst_sink(",");

      std::vector<double> double_vec;
      std::deque<double> double_deq;
      std::list<double> double_lst;

      strtk::parse(data,"|",vec_sink(double_vec),
                            deq_sink(double_deq),
                            lst_sink(double_lst));

      if (4 != double_vec.size())
      {
         std::cout << "test_parse4() - failed int_vec.size()" << std::endl;
         return false;
      }

      if (4 != double_deq.size())
      {
         std::cout << "test_parse4() - failed int_deq.size()" << std::endl;
         return false;
      }

      if (4 != double_lst.size())
      {
         std::cout << "test_parse4() - failed int_lst.size()" << std::endl;
         return false;
      }

      std::list<double>::iterator double_lst_itr = double_lst.begin();

      for (int i = 0; i <= 3; ++i)
      {
         double expected_value = i +  (i / 10.0);

         if (double_vec[i] != expected_value)
         {
            std::cout << "test_parse4() - failed double_vec[" << i <<"] == " << i << std::endl;
            return false;
         }

         if (double_deq[i] != expected_value)
         {
            std::cout << "test_parse4() - failed double_deq[" << i <<"] == " << i << std::endl;
            return false;
         }

         if (*(double_lst_itr) != -expected_value)
         {
            std::cout << "test_parse4() - failed double_lst[" << i <<"] == " << i << std::endl;
            return false;
         }
         else
            ++double_lst_itr;
      }
   }

   return true;
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

   static const std::size_t test_size = sizeof(test) / sizeof(sp_type);


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

   static const std::size_t base_size = sizeof(base) / sizeof(std::string);

   std::string result;
   for (std::size_t i = 0; i < test_size; ++i)
   {
      for (std::size_t j = 0; j < base_size; ++j)
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

bool test_n_choose_k()
{
   strtk::initialize_n_choose_k();
   assert(strtk::n_choose_k(10, 0) ==   1LL);
   assert(strtk::n_choose_k(10, 1) ==  10LL);
   assert(strtk::n_choose_k(10, 2) ==  45LL);
   assert(strtk::n_choose_k(10, 3) == 120LL);
   assert(strtk::n_choose_k(10, 4) == 210LL);
   assert(strtk::n_choose_k(10, 5) == 252LL);
   assert(strtk::n_choose_k(10, 6) == 210LL);
   assert(strtk::n_choose_k(10, 7) == 120LL);
   assert(strtk::n_choose_k(10, 8) ==  45LL);
   assert(strtk::n_choose_k(10, 9) ==  10LL);
   assert(strtk::n_choose_k(10,10) ==   1LL);
   assert(strtk::n_choose_k(11, 0) ==   1LL);
   assert(strtk::n_choose_k(11, 1) ==  11LL);
   assert(strtk::n_choose_k(11, 2) ==  55LL);
   assert(strtk::n_choose_k(11, 3) == 165LL);
   assert(strtk::n_choose_k(11, 4) == 330LL);
   assert(strtk::n_choose_k(11, 5) == 462LL);
   assert(strtk::n_choose_k(11, 6) == 462LL);
   assert(strtk::n_choose_k(11, 7) == 330LL);
   assert(strtk::n_choose_k(11, 8) == 165LL);
   assert(strtk::n_choose_k(11, 9) ==  55LL);
   assert(strtk::n_choose_k(11,10) ==  11LL);
   assert(strtk::n_choose_k(11,11) ==   1LL);
   assert(strtk::n_choose_k(100,   0) ==              1LL);
   assert(strtk::n_choose_k(100,   1) ==            100LL);
   assert(strtk::n_choose_k(100,   2) ==           4950LL);
   assert(strtk::n_choose_k(100,   3) ==         161700LL);
   assert(strtk::n_choose_k(100,   4) ==        3921225LL);
   assert(strtk::n_choose_k(100,   5) ==       75287520LL);
   assert(strtk::n_choose_k(100,   6) ==     1192052400LL);
   assert(strtk::n_choose_k(100,   7) ==    16007560800LL);
   assert(strtk::n_choose_k(100,   8) ==   186087894300LL);
   assert(strtk::n_choose_k(100,   9) ==  1902231808400LL);
   assert(strtk::n_choose_k(100,  10) == 17310309456440LL);
   assert(strtk::n_choose_k(100, 100) ==              1LL);
   assert(strtk::n_choose_k(100,  99) ==            100LL);
   assert(strtk::n_choose_k(100,  98) ==           4950LL);
   assert(strtk::n_choose_k(100,  97) ==         161700LL);
   assert(strtk::n_choose_k(100,  96) ==        3921225LL);
   assert(strtk::n_choose_k(100,  95) ==       75287520LL);
   assert(strtk::n_choose_k(100,  94) ==     1192052400LL);
   assert(strtk::n_choose_k(100,  93) ==    16007560800LL);
   assert(strtk::n_choose_k(100,  92) ==   186087894300LL);
   assert(strtk::n_choose_k(100,  91) ==  1902231808400LL);
   assert(strtk::n_choose_k(100,  90) == 17310309456440LL);
   assert(strtk::n_choose_k(500,   0) ==                    1LL);
   assert(strtk::n_choose_k(500,   1) ==                  500LL);
   assert(strtk::n_choose_k(500,   2) ==               124750LL);
   assert(strtk::n_choose_k(500,   3) ==             20708500LL);
   assert(strtk::n_choose_k(500,   4) ==           2573031125LL);
   assert(strtk::n_choose_k(500,   5) ==         255244687600LL);
   assert(strtk::n_choose_k(500,   6) ==       21057686727000LL);
   assert(strtk::n_choose_k(500,   7) ==     1486071034734000LL);
   assert(strtk::n_choose_k(500,   8) ==    91579127515482750LL);
   assert(strtk::n_choose_k(500,   9) ==  5006325637513057000LL);
   assert(strtk::n_choose_k(500, 500) ==                    1LL);
   assert(strtk::n_choose_k(500, 499) ==                  500LL);
   assert(strtk::n_choose_k(500, 498) ==               124750LL);
   assert(strtk::n_choose_k(500, 497) ==             20708500LL);
   assert(strtk::n_choose_k(500, 496) ==           2573031125LL);
   assert(strtk::n_choose_k(500, 495) ==         255244687600LL);
   assert(strtk::n_choose_k(500, 494) ==       21057686727000LL);
   assert(strtk::n_choose_k(500, 493) ==     1486071034734000LL);
   assert(strtk::n_choose_k(500, 492) ==    91579127515482750LL);
   assert(strtk::n_choose_k(500, 491) ==  5006325637513057000LL);

   return true;
}

int main()
{
   bool result = true;
   result &= test_split_and_tokenizer();
   result &= test_tokenizer_options();
   result &= test_split_options();
   result &= test_split_n();
   result &= test_empty_filter_itr();
   result &= test_construct_and_parse();
   result &= test_double_convert();
   result &= test_int_uint_convert();
   result &= test_parse1();
   result &= test_parse2();
   result &= test_parse3();
   result &= test_parse4();
   result &= test_replace_pattern();
   result &= test_n_choose_k();
   return (false == result ? 1 : 0);
}
