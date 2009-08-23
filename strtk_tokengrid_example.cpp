/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Token Grid Example                                              *
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
   Description: This example aims to demonstrate the usage patterns
                and functionality of the Token Grid class.
                The example test cases construct a string that
                represents a token grid - It should be noted the
                class can just as easily use a text file as input.
                The examples range from determining sums and averages
                of rows and columns of numeric values, iterating down
                columns, outputting rows/columns in specific orderings
                etc.
*/


#include <iostream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <utility>
#include <string>
#include <vector>
#include <utility>

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

void token_grid_test08()
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

   for(std::size_t r = 0; r < grid.row_count(); ++r)
   {
      double sum = 0.0;
      if (grid.accumulate_row(r,sum))
         std::cout << "sum(row["<< r <<"]) = " << sum << std::endl;
      else
         std::cout << "failed row["<< r <<"]" << std::endl;
   }

   for(std::size_t c = 0; c < grid.max_column_count(); ++c)
   {
      double sum = 0.0;
      if (grid.accumulate_column(c,sum))
         std::cout << "sum(col["<< c <<"]) = " << sum << std::endl;
      else
         std::cout << "failed col["<< c <<"]" << std::endl;
   }
}

void token_grid_test09()
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

   for(std::size_t r = 0; r < grid.row_count(); ++r)
   {
      std::string row = "";
      if (grid.join_row(r,"|",row))
         std::cout << "row["<< r <<"] = " << row << std::endl;
      else
         std::cout << "failed row["<< r <<"]" << std::endl;
   }

   for(std::size_t c = 0; c < grid.max_column_count(); ++c)
   {
      std::string col = "";
      if (grid.join_column(c,"|",col))
         std::cout << "col["<< c <<"] = " << col << std::endl;
      else
         std::cout << "failed col["<< c <<"]" << std::endl;
   }
}

class symbol_predicate
{
public:
   symbol_predicate(const std::string& symbol)
   : symbol_(symbol)
   {}
   bool operator()(const strtk::token_grid::row_type& row)
   {
      return symbol_ == row.get<std::string>(1);
   }

private:
   std::string symbol_;
};

void token_grid_test10()
{
   std::string market_data;
                 //Date,Symbol,Open,Close,High,Low,Volume
   market_data += "20090701,GOOG,424.2000,418.9900,426.4000,418.1500,2310768\n"
                  "20090701,MSFT,24.0500,24.0400,24.3000,23.9600,54915127\n"
                  "20090702,GOOG,415.4100,408.4900,415.4100,406.8100,2517630\n"
                  "20090702,MSFT,23.7600,23.3700,24.0400,23.2100,65427699\n"
                  "20090703,GOOG,408.4900,408.4900,408.4900,408.4900,0\n"
                  "20090703,MSFT,23.3700,23.3700,23.3700,23.3700,0\n"
                  "20090706,GOOG,406.5000,409.6100,410.6400,401.6600,2262557\n"
                  "20090706,MSFT,23.2100,23.2000,23.2800,22.8700,49207638\n"
                  "20090707,GOOG,408.2400,396.6300,409.1900,395.9801,3260307\n"
                  "20090707,MSFT,23.0800,22.5300,23.1400,22.4600,52842412\n"
                  "20090708,GOOG,400.0000,402.4900,406.0000,398.0600,3441854\n"
                  "20090708,MSFT,22.3100,22.5600,22.6900,2200000,73023306\n"
                  "20090709,GOOG,406.1200,410.3900,414.4500,405.8000,3275816\n"
                  "20090709,MSFT,22.6500,22.4400,22.8100,22.3700,46981174\n"
                  "20090710,GOOG,409.5700,414.4000,417.3700,408.7000,2929559\n"
                  "20090710,MSFT,22.1900,22.3900,22.5400,22.1500,43238698\n";

   strtk::token_grid grid(market_data,market_data.size(),",");
   std::size_t total_volume = 0;  //should be long long.

   grid.accumulate_column(6,symbol_predicate("GOOG"),total_volume);
   std::cout << "Total Volume (GOOG): " << total_volume << std::endl;

   grid.accumulate_column(6,symbol_predicate("MSFT"),total_volume);
   std::cout << "Total Volume (MSFT): " << total_volume << std::endl;
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
   token_grid_test08();
   token_grid_test09();
   token_grid_test10();
   return 0;
}
