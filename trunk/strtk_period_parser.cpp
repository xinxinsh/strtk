/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Period Parser                                                 *
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
   Description: This example demonstrates how to build a simple parser
                that takes input in the form of days, hours, minutes and
                seconds, validates each of the values and then proceeds
                to combine them all into a millisecond period of time.
*/


#include <cstddef>
#include <vector>
#include <string>

#include "strtk.hpp"


class period_parser
{
public:

   period_parser()
   : predicate_("DHRMINSECdhrminsec :")
   {}

   bool operator()(const std::string& s, long long& period)
   {
      if (s.empty())
         return false;

      static const strtk::split_options::type split_options = strtk::split_options::compress_delimiters +
                                                              strtk::split_options::include_delimiters;
      std::size_t token_count = split_n(predicate_,
                                        const_cast<char*>(s.c_str()),
                                        const_cast<char*>(s.c_str() + s.size()),
                                        max_token_count,
                                        token_list,
                                        split_options);
      period = 0;
      double t = 0.0;
      for (std::size_t i = 0; i < token_count; ++i)
      {
         if (!strtk::string_to_type_converter(token_list[i].first,token_list[i].second - 1,t))
            return false;
         switch (std::toupper(*(token_list[i].second - 1)))
         {
            case 'D' : t = 86400000.0 * t; break;
            case 'H' : t =  3600000.0 * t; break;
            case 'M' : t =    60000.0 * t; break;
            case 'S' : t =     1000.0 * t; break;
            default  : return false;
         }
         if (('D' != std::toupper(*(token_list[i].second - 1))) && (t < 0.0))
            return false;
         period += static_cast<long long>(t);
      }
      return true;
   }

private:
   static const std::size_t max_token_count = 4;
   typedef std::pair<char*,char*> iterator_type;

   strtk::multiple_char_delimiter_predicate predicate_;
   iterator_type token_list[max_token_count];
};

int main(int argc, char* argv[])
{
   period_parser parser;
   const std::string period_string[] =
               {
                  "123.456D789.012Hr345.678Min901.234Sec",
                  "789.012Hr345.678Min901.234Sec123.456D",
                  "345.678Min901.234Sec123.456D789.012Hr",
                  "901.234Sec123.456D789.012Hr345.678Min",
                  "123.456D 789.012Hr 345.678Min 901.234Sec",
                  "789.012Hr 345.678Min 901.234Sec 123.456D",
                  "345.678Min 901.234Sec 123.456D 789.012Hr",
                  "901.234Sec 123.456D 789.012Hr 345.678Min",
                  "123.456D:789.012Hr:345.678Min:901.234Sec",
                  "789.012Hr:345.678Min:901.234Sec:123.456D",
                  "345.678Min:901.234Sec:123.456D:789.012Hr",
                  "901.234Sec:123.456D:789.012Hr:345.678Min",
                  "0123.456D0789.012Hr0345.678Min0901.234Sec",
                  "0789.012Hr0345.678Min0901.234Sec0123.456D",
                  "0345.678Min0901.234Sec0123.456D0789.012Hr",
                  "0901.234Sec0123.456D0789.012Hr0345.678Min",
                  "0123.456D 0789.012Hr 0345.678Min 0901.234Sec",
                  "0789.012Hr 0345.678Min 0901.234Sec 0123.456D",
                  "0345.678Min 0901.234Sec 0123.456D 0789.012Hr",
                  "0901.234Sec 0123.456D 0789.012Hr 0345.678Min"
               };

   const std::size_t period_string_size = sizeof(period_string) / sizeof(std::string);

   long long t = 0;
   for (std::size_t i = 0; i < period_string_size; ++i)
   {
      if (parser(period_string[i],t))
      {
         std::cout << "Period: " << t << "msec" << std::endl;
      }
   }
   return 0;
}
