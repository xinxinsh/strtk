#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "strtk.hpp"

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
   long           d11;
   unsigned long  d12;
};


void parse_test01()
{
   data_block i1;
   i1.d1  = "The1 quick2 brown3 fox4 jumps5 over6 the7 lazy8 dog9";
   i1.d2  = 'x';
   i1.d3  = -1234;
   i1.d4  = 78901;
   i1.d5  = 4567.8901;
   i1.d6  = 123.456f;
   i1.d7  = -16000;
   i1.d8  = 15000;
   i1.d9  = true;
   i1.d10 = 0xEE;
   i1.d11 = -737373;
   i1.d12 = 333777l;

   data_block i2;
   i2.d1  = "The9 quick8 brown7 fox6 jumps5 over4 the3 lazy2 dog1";
   i2.d2  = 'A';
   i2.d3  = -4321;
   i2.d4  = 11111;
   i2.d5  = 98765.12345;
   i2.d6  = 123.456f;
   i2.d7  = -11111;
   i2.d8  = 13333;
   i2.d9  = true;
   i2.d10 = 0xA5;
   i2.d11 = -737373;
   i2.d12 = 333777l;

   std::string str1 = "";
   std::string str2 = "";

   strtk::construct(str1,"|",i1.d1,i1.d2,i1.d3,i1.d4,i1.d5,i1.d6,i1.d7,i1.d8,i1.d9,i1.d10,i1.d11,i1.d12);
   strtk::construct(str2,"|",i2.d1,i2.d2,i2.d3,i2.d4,i2.d5,i2.d6,i2.d7,i2.d8,i2.d9,i2.d10,i2.d11,i2.d12);

   const std::size_t dblist_size = 20;

   std::vector<data_block> db_list(dblist_size);
   std::vector<std::string> s_list;

   for (std::size_t i = 0; i < (dblist_size >> 1); ++i)
   {
      s_list.push_back(str1);
      s_list.push_back(str2);
   }

   const std::size_t count = 1000000;
   timer t;
   t.start();
   for (std::size_t i = 0; i < count; ++i)
   {
      for (std::size_t j = 0; j < s_list.size(); ++j)
      {
         strtk::parse(s_list[j],
                      "|",
                       db_list[j].d1,  db_list[j].d2,  db_list[j].d3,
                       db_list[j].d4,  db_list[j].d5,  db_list[j].d6,
                       db_list[j].d7,  db_list[j].d8,  db_list[j].d9,
                       db_list[j].d10, db_list[j].d11, db_list[j].d12);
      }
   }
   t.stop();
   printf("Token Count: %u\tTotal time: %8.4f\tRate: %8.4fprs/s\n",
          s_list.size() * 12 * count,
          t.time(),
          (s_list.size() * count) / (1.0 * t.time()));
}

int main()
{
   parse_test01();
   return 0;
}