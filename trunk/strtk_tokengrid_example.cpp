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
         case  0 : grid.row(i).parse(0,lst[0]);
                   break;
         case  1 : grid.row(i).parse(0,1,lst[0],lst[1]);
                   break;
         case  2 : grid.row(i).parse(0,1,2,lst[0],lst[1],lst[2]);
                   break;
         case  3 : grid.row(i).parse(0,1,2,3,lst[0],lst[1],lst[2],lst[3]);
                   break;
         case  4 : grid.row(i).parse(0,1,2,3,4,lst[0],lst[1],lst[2],lst[3],lst[4]);
                   break;
         case  5 : grid.row(i).parse(0,1,2,3,4,5,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5]);
                   break;
         case  6 : grid.row(i).parse(0,1,2,3,4,5,6,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6]);
                   break;
         case  7 : grid.row(i).parse(0,1,2,3,4,5,6,7,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6],lst[7]);
                   break;
         case  8 : grid.row(i).parse(0,1,2,3,4,5,6,7,8,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6],lst[7],lst[8]);
                   break;
         case  9 : grid.row(i).parse(0,1,2,3,4,5,6,7,8,9,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6],lst[7],lst[8],lst[9]);
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

int main(void)
{
   token_grid_test01();
   token_grid_test02();
   token_grid_test03();
   token_grid_test04();
   token_grid_test05();
   return 0;
}
