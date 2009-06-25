/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * StrTk Tokenizer Vs BOOST Tokenizer                              *
 * Author: Arash Partow - 2003                                     *
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

#include "strtk.hpp"
#include <boost/tokenizer.hpp>

#ifdef WIN32
 #include <windows.h>
#else
 #include <sys/time.h>
 #include <sys/types.h>
#endif

#ifdef WIN32

class timer
{
public:
   timer()      { QueryPerformanceFrequency(&clock_frequency); }
   void start() { QueryPerformanceCounter(&start_time);        }
   void stop()  { QueryPerformanceCounter(&stop_time);         }
   double time(){ return (1.0 *(stop_time.QuadPart - start_time.QuadPart)) / (1.0 * clock_frequency.QuadPart); }
 private:
  LARGE_INTEGER start_time;
  LARGE_INTEGER stop_time;
  LARGE_INTEGER clock_frequency;
};

#else

class timer
{
public:
   void start() { gettimeofday(&start_time, 0); }
   void stop()  { gettimeofday(&stop_time,  0); }
   double time()
   {
      double diff = (stop_time.tv_sec - start_time.tv_sec) * 1000000.0;
      if (stop_time.tv_usec > start_time.tv_usec)
         diff += (1.0 * (stop_time.tv_usec - start_time.tv_usec));
      else if (stop_time.tv_usec < start_time.tv_usec)
         diff -= (1.0 * (start_time.tv_usec - stop_time.tv_usec));

      return (diff / 1000000.0);
   }
 private:
  struct timeval start_time;
  struct timeval stop_time;
  struct timeval clock_frequency;
};

#endif

const std::string base = "a|bc?def,ghij;klmno_p|qr?stu,vwxy;z1234_56789 0 A|BC?DEF,GHIJ;KLMNO_P|QR?STU,VWXY;Z1234_56789";
const std::size_t replicate_count = 1000000;
const std::string delimiters = "|?,;_ ";

void strtk_tokenizer_timed_test()
{
   std::string s = "";
   std::string so = "";
   s.reserve(base.size() * replicate_count);
   so.reserve(base.size() * replicate_count);
   for(unsigned int i = 0; i < replicate_count; ++i) s += base;
   strtk::multiple_char_delimiter_predicate predicate(delimiters);
   strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer(s,predicate);
   strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type::iterator it = tokenizer.begin();
   unsigned int token_count = 0;
   timer t;
   t.start();
   while (it != tokenizer.end())
   {
      so += std::string((*it).first,(*it).second);
      ++it;
      ++token_count;
   }
   t.stop();
   printf("[strtk] Token Count: %d\tTotal time: %8.4f\tRate: %8.4ftks/s\n",token_count,t.time(),(1.0 * token_count) / (1.0 * t.time()));
}

void boost_tokenizer_timed_test()
{
   std::string s = "";
   std::string so  ="";
   s.reserve(base.size() * replicate_count);
   so.reserve(base.size() * replicate_count);
   for(unsigned int i = 0; i < replicate_count; ++i) s += base;
   typedef boost::tokenizer<boost::char_separator<char> > tokenizer_type;
   tokenizer_type tokenizer(s,boost::char_separator<char>(delimiters.c_str()));
   tokenizer_type::iterator it = tokenizer.begin();
   unsigned int token_count = 0;
   timer t;
   t.start();
   while (it != tokenizer.end())
   {
      so += *it;
      ++it;
      ++token_count;
   }
   t.stop();
   printf("[boost] Token Count: %d\tTotal time: %8.4f\tRate: %8.4ftks/s\n",token_count,t.time(),(1.0 * token_count) / (1.0 * t.time()));
}

int main()
{
   boost_tokenizer_timed_test();
   strtk_tokenizer_timed_test();
   return 0;
}
