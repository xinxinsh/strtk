/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Token Grid Example                                              *
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
#include <numeric>
#include <utility>
#include <string>
#include <vector>

#include "strtk.hpp"


void token_grid_test01()
{
   std::string data;
   data += "1,2,3,4,5\n";
   data += "0,2,4,6,8\n";
   data += "1,3,5,7,9\n";

   strtk::token_grid grid(data,data.size(),",");

   for(std::size_t i = 0; i < grid.row_count(); ++i)
   {
      strtk::token_grid::row_type r = grid.row(i);
      for(std::size_t j = 0; j < r.size(); ++j)
      {
         std::cout << r.get<int>(j) << "\t";
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
}

void token_grid_test02()
{
   std::string data;

   data += "1\n";
   data += "1,2\n";
   data += "1,2,3\n";
   data += "1,2,3,4\n";
   data += "1,2,3,4,5\n";
   data += "1,2,3,4,5,6\n";
   data += "1,2,3,4,5,6,7\n";
   data += "1,2,3,4,5,6,7,8\n";
   data += "1,2,3,4,5,6,7,8,9\n";
   data += "1,2,3,4,5,6,7,8,9,10\n";

   strtk::token_grid grid(data,data.size(),",");

   std::vector<int> lst(10);

   for(std::size_t i = 0; i < grid.row_count(); ++i)
   {
      switch(i)
      {
         case  0 : grid.row(i).parse_with_index(0,lst[0]);
                   break;
         case  1 : grid.row(i).parse_with_index(0,1,lst[0],lst[1]);
                   break;
         case  2 : grid.row(i).parse_with_index(0,1,2,lst[0],lst[1],lst[2]);
                   break;
         case  3 : grid.row(i).parse_with_index(0,1,2,3,lst[0],lst[1],lst[2],lst[3]);
                   break;
         case  4 : grid.row(i).parse_with_index(0,1,2,3,4,lst[0],lst[1],lst[2],lst[3],lst[4]);
                   break;
         case  5 : grid.row(i).parse_with_index(0,1,2,3,4,5,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5]);
                   break;
         case  6 : grid.row(i).parse_with_index(0,1,2,3,4,5,6,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6]);
                   break;
         case  7 : grid.row(i).parse_with_index(0,1,2,3,4,5,6,7,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6],lst[7]);
                   break;
         case  8 : grid.row(i).parse_with_index(0,1,2,3,4,5,6,7,8,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6],lst[7],lst[8]);
                   break;
         case  9 : grid.row(i).parse_with_index(0,1,2,3,4,5,6,7,8,9,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6],lst[7],lst[8],lst[9]);
                   break;
      }
      std::cout << strtk::join(",",lst.begin(),lst.begin() + (i+1)) << std::endl;
   }
}

template<typename Container>
void output_containter(const Container& c)
{
   std::copy(c.begin(),c.end(),std::ostream_iterator<int>(std::cout,"\t"));
}

void token_grid_test03()
{
   std::string data;
   data += "1,2,3,4,5\n";
   data += "1,2,3,4,5\n";
   data += "1,2,3,4,5\n";
   data += "1,2,3,4,5\n";

   std::vector<int> lst0;
   std::vector<int> lst1;
   std::vector<int> lst2;
   std::vector<int> lst3;
   std::vector<int> lst4;

   strtk::token_grid grid(data,data.size(),",");

   grid.extract_column(0,strtk::back_inserter_with_valuetype(lst0));
   output_containter(lst0); std::cout << std::endl;
   lst0.clear();

   grid.extract_column(0,1,strtk::back_inserter_with_valuetype(lst0),
                           strtk::back_inserter_with_valuetype(lst1));
   output_containter(lst0); std::cout << std::endl;
   output_containter(lst1); std::cout << std::endl;
   lst0.clear(); lst1.clear();

   grid.extract_column(0,1,2,strtk::back_inserter_with_valuetype(lst0),
                             strtk::back_inserter_with_valuetype(lst1),
                             strtk::back_inserter_with_valuetype(lst2));
   output_containter(lst0); std::cout << std::endl;
   output_containter(lst1); std::cout << std::endl;
   output_containter(lst2); std::cout << std::endl;
   lst0.clear(); lst1.clear();
   lst2.clear();

   grid.extract_column(0,1,2,3,strtk::back_inserter_with_valuetype(lst0),
                               strtk::back_inserter_with_valuetype(lst1),
                               strtk::back_inserter_with_valuetype(lst2),
                               strtk::back_inserter_with_valuetype(lst3));
   output_containter(lst0); std::cout << std::endl;
   output_containter(lst1); std::cout << std::endl;
   output_containter(lst2); std::cout << std::endl;
   output_containter(lst3); std::cout << std::endl;
   lst0.clear(); lst1.clear();
   lst2.clear(); lst3.clear();

   grid.extract_column(0,1,2,3,4,strtk::back_inserter_with_valuetype(lst0),
                                 strtk::back_inserter_with_valuetype(lst1),
                                 strtk::back_inserter_with_valuetype(lst2),
                                 strtk::back_inserter_with_valuetype(lst3),
                                 strtk::back_inserter_with_valuetype(lst4));
   output_containter(lst0); std::cout << std::endl;
   output_containter(lst1); std::cout << std::endl;
   output_containter(lst2); std::cout << std::endl;
   output_containter(lst3); std::cout << std::endl;
   output_containter(lst4); std::cout << std::endl;
   lst0.clear(); lst1.clear();
   lst2.clear(); lst3.clear();
   lst4.clear();
}

void token_grid_test04()
{
   std::string data;
   data.reserve(160);
   data += "1,2,3,4,5\n1,2,3,4\n1,2,3\n1,2\n1\n";
   for(unsigned int i = 0; i < 10; data += "1,2,3,4,5\n1,2,3,4\n1,2,3\n1,2\n1\n", ++i);
   strtk::token_grid grid(data,data.size(),",");
   grid.enforce_column_count(5);
   for(unsigned int i = 0; i < grid.row_count(); ++i)
   {
      strtk::token_grid::row_type row = grid.row(i);
      for(unsigned j = 0; j < row.size(); ++j)
      {
         strtk::token_grid::range_type r = row.token(j);
         std::cout << std::string(r.first,r.second) << "\t";
      }
      std::cout << std::endl;
   }
}

void token_grid_test05()
{
   std::string data;
   data.reserve(160);
   data += "1,2,3,4,5\n1,2,3,4\n1,2,3\n1,2\n1\n";
   for(unsigned int i = 0; i < 10; data += "1,2,3,4,5\n1,2,3,4\n1,2,3\n1,2\n1\n", ++i);
   strtk::token_grid grid(data,data.size(),",");

   for(unsigned int i = 0; i < grid.row_count(); ++i)
   {
      strtk::token_grid::row_type row = grid.row(i);
      for(unsigned j = 0; j < row.size(); ++j)
      {
         std::cout << grid.get<int>(i,j) << "\t";
      }
      std::cout << std::endl;
   }
}


void token_grid_test06()
{
   std::string data;
   data += "1.1,1.1,1.1,1.1,1.1,1.1\n"
           "2.2,2.2,2.2,2.2,2.2,2.2\n"
           "3.3,3.3,3.3,3.3,3.3,3.3\n"
           "4.4,4.4,4.4,4.4,4.4,4.4\n"
           "5.5,5.5,5.5,5.5,5.5,5.5\n"
           "6.6,6.6,6.6,6.6,6.6,6.6\n"
           "7.7,7.7,7.7,7.7,7.7,7.7\n";

   strtk::token_grid grid(data,data.size(),",");

   std::vector<double> avg_c(grid.row(0).size(),0.0);
   std::vector<double> avg_r(grid.row_count(),0.0);
   std::vector<double> tmp(grid.row(0).size(),0.0);
   std::fill(avg_c.begin(),avg_c.end(),0.0);

   for(unsigned int i = 0; i < grid.row_count(); ++i)
   {
      grid.row(i).parse<double>(tmp.begin());
      std::transform(avg_c.begin(),avg_c.end(),tmp.begin(),avg_c.begin(),std::plus<double>());
      avg_r[i] = std::accumulate(tmp.begin(),tmp.end(),0.0) / tmp.size();
   }

   for(unsigned int i = 0; i < avg_c.size(); avg_c[i++] /= grid.row_count());

   std::cout << "Column Averages:\t";
   std::copy(avg_c.begin(),avg_c.end(),std::ostream_iterator<double>(std::cout,"\t"));
   std::cout << "\n";

   std::cout << "Row Averages:\t";
   std::copy(avg_r.begin(),avg_r.end(),std::ostream_iterator<double>(std::cout,"\t"));
   std::cout << "\n";
}


void token_grid_test07()
{
   std::string data;
   data += "The The  The The  The   The The The The\n";
   data += "quick quick  quick quick  quick   quick quick  quick quick\n";
   data += "brown  brown brown brown brown brown brown brown brown\n";
   data += "fox fox  fox  fox fox   fox fox  fox    fox\n";
   data += "jumps jumps jumps jumps jumps jumps jumps jumps jumps\n";
   data += "over  over  over  over  over  over  over  over  over\n";
   data += "the   the the    the   the   the the  the   the\n";
   data += "lazy  lazy lazy lazy  lazy  lazy lazy  lazy  lazy\n";
   data += "dog   dog  dog   dog   dog   dog   dog   dog dog\n\n\n";

   strtk::token_grid grid(data,data.size()," ");

   if (grid.min_column_count() != grid.max_column_count())
   {
      std::cout << "token_grid_test07 - ERROR!" << std::endl;
      std::cout << "min column count: " << grid.min_column_count() << std::endl;
      std::cout << "max column count: " << grid.max_column_count() << std::endl;
   }

   std::vector<std::string> words;
   words.reserve(grid.row_count());

   for(std::size_t i = 0; i < grid.min_column_count(); ++i)
   {
      words.clear();
      grid.extract_column(i,strtk::back_inserter_with_valuetype(words));
      std::cout << strtk::join(" ",words.begin(),words.end()) << std::endl;
   }
}


int main()
{
   token_grid_test01();
   token_grid_test02();
   token_grid_test03();
   token_grid_test04();
   token_grid_test05();
   token_grid_test06();
   token_grid_test07();
   return 0;
}
