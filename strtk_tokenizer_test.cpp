/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * String Tokenizer Test                                           *
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
#include <algorithm>
#include <string>
#include <vector>
#include <list>

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
   if (2 == for_each_token(s,tok,strtk::filter_empty_range< strtk::add_to_sequence<str_list> >(strtk::add_to_sequence<str_list>(sl))))
      return true;
   else
   {
      std::cout << "test_empty_filter_itr() - Failed Compressed Delimiter Test" << std::endl;
      return false;
   }
}

bool test_construct_and_parse()
{
   std::string output = "";
   std::string  i1 = "abcd";
   char         i2 = 'x';
   int          i3 = -1234;
   unsigned int i4 = 78901;
   double       i5 = 4567.8901;

   strtk::construct(output,"|",i1,i2,i3,i4,i5);

   std::string  o1 = "";
   char         o2 = 0x00;
   int          o3 = 0;
   unsigned int o4 = 0;
   double       o5 = 0;

   strtk::parse(output,"|",o1,o2,o3,o4,o5);

   if (!((i1 == o1) &&
         (i2 == o2) &&
         (i3 == o3) &&
         (i4 == o4) &&
         (i5 == o5)))
   {
      std::cout << "test_construct_and_parse() - Construct and Parse Test FAILED!" << std::endl;
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
