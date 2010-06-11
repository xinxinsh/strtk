/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Serializer Example                                            *
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


#include <cstddef>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <list>

#include "strtk.hpp"


struct person
{
public:
   unsigned long long id;
   std::string      name;
   unsigned int      age;
   double         height;
   float          weight;
   bool        is_insane;

   person()
   {
      clear();
   }

   bool operator==(const person& p)
   {
      return (p.id         ==        id) &&
             (p.name       ==      name) &&
             (p.age        ==       age) &&
             (p.weight     ==    weight) &&
             (p.height     ==    height) &&
             (p.is_insane  == is_insane);
   }

   bool operator!=(const person& p)
   {
      return !operator==(p);
   }

   void clear()
   {
      name = "";
      age = 0;
      height = 0.0;
      weight = 0.0f;
      is_insane = false;
   }

   strtk_binary_reader_begin()
     strtk_binary_reader(id)
     strtk_binary_reader(name)
     strtk_binary_reader(age)
     strtk_binary_reader(height)
     strtk_binary_reader(weight)
     strtk_binary_reader(is_insane)
   strtk_binary_reader_end()

   strtk_binary_writer_begin()
     strtk_binary_writer(id)
     strtk_binary_writer(name)
     strtk_binary_writer(age)
     strtk_binary_writer(height)
     strtk_binary_writer(weight)
     strtk_binary_writer(is_insane)
   strtk_binary_writer_end()

};

bool test01(char* buffer, const unsigned int buffer_size)
{
   person p_out;
   person p_in;

   {
      strtk::binary::writer writer(buffer,buffer_size);
      writer.clear();

      p_out.id        = 12345678901234567890ULL;
      p_out.name      = "Mr. Rumpelstilzchen";
      p_out.age       = 637;
      p_out.height    = 123.4567;
      p_out.weight    = 765.345f;
      p_out.is_insane = true;

      writer(p_out);
   }

   {
      strtk::binary::reader reader(buffer,buffer_size);
      reader(p_in);
   }

   if (p_in != p_out)
   {
      std::cout << "test01() - Failed p-in to p-out comparison!" << std::endl;
      return false;
   }

   return true;
}

bool test02(char* buffer, const unsigned int buffer_size)
{
   {
      strtk::binary::writer writer(buffer,buffer_size);
      writer.clear();

      person p_out;
      p_out.id        = 12345678901234567890ULL;
      p_out.name      = "Mr. Rumpelstilzchen";
      p_out.age       = 0;
      p_out.height    = 0.0;
      p_out.weight    = 0.0f;
      p_out.is_insane = false;

      const std::size_t rounds = 1000;
      for (std::size_t i = 0; i < rounds; ++i)
      {
         p_out.id++;
         p_out.age++;
         p_out.height += 1.23;
         p_out.weight += 4.567f;
         p_out.is_insane = !p_out.is_insane;
         if (!writer(p_out))
         {
            std::cout << "test02() - Failed to write person:" << i << std::endl;
            return false;
         }
      }
   }

   {
      strtk::binary::reader reader(buffer,buffer_size);
      person p_in;
      person p_expected;
      p_expected.id        = 12345678901234567890ULL;
      p_expected.name      = "Mr. Rumpelstilzchen";
      p_expected.age       = 0;
      p_expected.height    = 0.0;
      p_expected.weight    = 0.0f;
      p_expected.is_insane = false;

      const std::size_t rounds = 1000;
      for (std::size_t i = 0; i < rounds; ++i)
      {
         p_expected.id++;
         p_expected.age++;
         p_expected.height += 1.23;
         p_expected.weight += 4.567f;
         p_expected.is_insane = !p_expected.is_insane;
         if (!reader(p_in))
         {
            std::cout << "test02() - Failed to read person:" << i << std::endl;
            return false;
         }

         if (p_in != p_expected)
         {
            std::cout << "test02() - Comparison between expected and read failed @ " << i << std::endl;
            return false;
         }
      }
   }

   return true;
}

bool test03(char* buffer, const unsigned int buffer_size)
{
   const std::string file_name = "data.txt";
   const std::size_t rounds = 1000;

   {
      strtk::binary::writer writer(buffer,buffer_size);
      writer.clear();

      person p;
      p.id        = 12345678901234567890ULL;
      p.name      = "Mr. Rumpelstilzchen";
      p.age       = 0;
      p.height    = 123.4567;
      p.weight    = 765.345f;
      p.is_insane = true;

      for (unsigned int i = 0; i < rounds; ++i)
      {
         if (!writer(p))
         {
            std::cout << "test03() - Failed to write person:" << i << std::endl;
            return false;
         }
         p.id++;
         p.age++;
         p.height += 1.23;
         p.weight += 4.567f;
         p.is_insane = !p.is_insane;
      }

      std::ofstream o_stream(file_name.c_str(),std::ios::binary);
      if (!o_stream)
      {
         std::cout << "test03() - ERROR Could not open file!(1)" << std::endl;
         return false;
      }

      writer(o_stream);
      o_stream.close();
   }

   {
      strtk::binary::reader reader(buffer,buffer_size);
      reader.clear();

      const std::size_t length = strtk::fileio::file_size(file_name);

      std::ifstream i_stream(file_name.c_str(),std::ios::binary);
      if (!i_stream)
      {
         std::cout << "test03() - ERROR Could not open file!(2)" << std::endl;
         return false;
      }

      reader(i_stream,length);
      reader.reset();

      person p_expected;
      p_expected.id        = 12345678901234567890ULL;
      p_expected.name      = "Mr. Rumpelstilzchen";
      p_expected.age       = 0;
      p_expected.height    = 123.4567;
      p_expected.weight    = 765.345f;
      p_expected.is_insane = true;

      person p_in;

      for (unsigned int i = 0; i < rounds; ++i)
      {
         p_in.clear();
         if (!reader(p_in))
         {
            std::cout << "test03() - Failed to read person:" << i << std::endl;
            return false;
         }

         if (p_in != p_expected)
         {
            std::cout << "test03() - Comparison between expected and read failed @ " << i << std::endl;
            return false;
         }
         p_expected.id++;
         p_expected.age++;
         p_expected.height += 1.23;
         p_expected.weight += 4.567f;
         p_expected.is_insane = !p_expected.is_insane;
      }
   }

   return true;
}

bool test04(char* buffer, const unsigned int buffer_size)
{
   {
      // Write out and then read back in an array of unsigned ints.
      std::deque<unsigned int> lst;
      const unsigned int max_count = 2000;

      {
         strtk::binary::writer writer(buffer,buffer_size);
         writer.clear();
         for (unsigned int i = 0; i < max_count; lst.push_back(i++)) ;
         if (!writer(lst))
         {
            std::cout << "test04() - Failed to write list of 'unsigned int'" << std::endl;
            return false;
         }
         lst.clear();
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);
         if (!reader(lst))
         {
            std::cout << "test04() - Failed to read list of 'unsigned int'" << std::endl;
            return false;
         }
         for (unsigned int i = 0; i < max_count; ++i)
         {
            if (lst[i] != i)
            {
               std::cout << "test04() - 'unsigned int' failure at index: " << i << std::endl;
               return false;
            }
         }
      }
   }

   {
      // Write out and then read back in an array of floats.
      std::vector<float> lst;
      const unsigned int max_count = 2000;
      const float magic[] = {
                              111.111f, 333.333f, 555.555f,
                              777.777f, 135.531f, 357.753f
                            };
      const std::size_t magic_count = sizeof(magic) / sizeof(float);

      {
         strtk::binary::writer writer(buffer,buffer_size);

         for (std::size_t i = 0; i < max_count; ++i)
         {
            lst.push_back(magic[i % magic_count] * i);
         }
         if (!writer(lst))
         {
            std::cout << "test04() - Failed to write list of " << strtk::type_name(lst) << std::endl;
            return false;
         }
         lst.clear();
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);
         if (!reader(lst))
         {
            std::cout << "test04() - Failed to read list of " << strtk::type_name(lst) << std::endl;
            return false;
         }

         for (std::size_t i = 0; i < max_count; ++i)
         {
            const float d = magic[i % magic_count] * i;
            if (lst[i] != d)
            {
               std::cout << "test04() - 'float' failure at index: " << i
                         << " expected value: "                     << d << std::endl;
               return false;
            }
         }
      }
   }

   {
      // Write out and then read back in an array of doubles.
      std::list<double> lst;
      const unsigned int max_count = 1000;
      const double magic[] = {
                                111.111, 333.333, 555.555,
                                777.777, 135.531, 357.753
                             };
      const std::size_t magic_count = sizeof(magic) / sizeof(double);

      {
         strtk::binary::writer writer(buffer,buffer_size);
         writer.clear();
         for (std::size_t i = 0; i < max_count; ++i)
         {
            lst.push_back(magic[i % magic_count] * i);
         }
         if (!writer(lst))
         {
            std::cout << "test04() - Failed to write list of " << strtk::type_name(lst) << std::endl;
            return false;
         }
         lst.clear();
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);
         if (!reader(lst))
         {
            std::cout << "test04() - Failed to read list of " << strtk::type_name(lst) << std::endl;
            return false;
         }

         std::list<double>::iterator itr = lst.begin();
         for (std::size_t i = 0; i < max_count; ++i, ++itr)
         {
            const double d = magic[i % magic_count] * i;
            if (*itr != d)
            {
               std::cout << "test04() - 'double' failure at index: " << i
                         << " expected value: "                      << d << std::endl;
               return false;
            }
         }
      }
   }

   {
      // Write out and then read back in a set of int.
      std::set<int> lst;
      const int max_count = 10000;
      {
         strtk::binary::writer writer(buffer,buffer_size);
         writer.clear();
         for (int i = -(max_count / 2); i < (max_count / 2); ++i)
         {
            lst.insert(i);
         }
         if (!writer(lst))
         {
            std::cout << "test04() - Failed to write list of " << strtk::type_name(lst) << std::endl;
            return false;
         }
         lst.clear();
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);
         if (!reader(lst))
         {
            std::cout << "test04() - Failed to read list of " << strtk::type_name(lst) << std::endl;
            return false;
         }
         int i = -(max_count / 2);
         std::set<int>::iterator itr = lst.begin();
         while (lst.end() != itr)
         {
            if (i != *itr)
            {
               std::cout << "test04() - expected value: " << i
                         << " read value: " << *itr << std::endl;
               return false;

            }
            ++i;
            ++itr;
         }
      }
   }

   return true;
}

bool test05(char* buffer, const unsigned int buffer_size)
{
   const std::size_t rounds = 100000;
   const std::size_t person_count = 1000;
   person p;
   p.id        = 12345678901234567890ULL;
   p.name      = "Mr. Rumpelstilzchen";
   p.age       = 637;
   p.height    = 123.4567;
   p.weight    = 765.345f;
   p.is_insane = true;

   {
      strtk::binary::writer writer(buffer,buffer_size);
      unsigned long long total_written = 0;
      strtk::util::timer t;
      t.start();
      for (std::size_t r = 0; r < rounds; ++r)
      {
         writer.reset();
         for (std::size_t i = 0; i < person_count; ++i)
         {
            if (!writer(p))
            {
               std::cout << "test05() - Failed to write index " << i << " @ round " << r << std::endl;
               return false;
            }
         }
         total_written += writer.write_size();
      }
      t.stop();
      printf("[strtk::binary::writer] Person Count:%10llu  Total time:%8.4f  Rate:%14.4fpersons/s %7.3fMB/s\n",
             static_cast<unsigned long long>(rounds * person_count),
             t.time(),
             (rounds * person_count) / t.time(),
             total_written / (1048576.0 * t.time()));
   }

   {
      strtk::binary::reader reader(buffer,buffer_size);
      unsigned long long total_read = 0;
      strtk::util::timer t;
      t.start();
      for (std::size_t r = 0; r < rounds; ++r)
      {
         reader.reset();
         for (std::size_t i = 0; i < person_count; ++i)
         {
            if (!reader(p))
            {
               std::cout << "test05() - Failed to read index " << i << " @ round " << r << std::endl;
               return false;
            }
         }
         total_read += reader.read_size();
      }
      t.stop();
      printf("[strtk::binary::reader] Person Count:%10llu  Total time:%8.4f  Rate:%14.4fpersons/s %7.3fMB/s\n",
             static_cast<unsigned long long>(rounds * person_count),
             t.time(),
             (rounds * person_count) / t.time(),
             total_read / (1048576.0 * t.time()));
   }

   {
      const std::size_t rounds = 100000;
      const std::size_t max_count = 8000;
      const double magic[] = {
                                111.111, 333.333, 555.555,
                                777.777, 135.531, 357.753
                             };
      const std::size_t magic_count = sizeof(magic) / sizeof(double);

      std::vector<double> dbl_list;
      dbl_list.reserve(max_count);

      for (std::size_t i = 0; i < max_count; ++i)
      {
         dbl_list.push_back(magic[i % magic_count] * i);
      }

      {
         strtk::binary::writer writer(buffer,buffer_size);
         unsigned long long total_written = 0;
         strtk::util::timer t;
         t.start();
         for(std::size_t r = 0; r < rounds; ++r)
         {
            writer.reset();
            if (!writer(dbl_list))
            {
               std::cout << "test05() - Failed to write " << strtk::type_name(dbl_list) << " @ round " << r << std::endl;
               return false;
            }
            total_written += writer.write_size();
         }
         t.stop();
         printf("[strtk::binary::writer] Double Count:%10llu  Total time:%8.4f  Rate:%14.4fpersons/s %7.3fMB/s\n",
                static_cast<unsigned long long>(rounds * max_count),
                t.time(),
                (rounds * max_count) / t.time(),
                total_written / (1048576.0 * t.time()));
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);
         unsigned long long total_read = 0;
         strtk::util::timer t;
         t.start();
         for(std::size_t r = 0; r < rounds; ++r)
         {
            reader.reset();
            if (!reader(dbl_list))
            {
               std::cout << "test05() - Failed to read " << strtk::type_name(dbl_list) << " @ round " << r << std::endl;
               return false;
            }
            dbl_list.clear();
            total_read += reader.read_size();
         }
         t.stop();
         printf("[strtk::binary::reader] Double Count:%10llu  Total time:%8.4f  Rate:%14.4fpersons/s %7.3fMB/s\n",
                static_cast<unsigned long long>(rounds * max_count),
                t.time(),
                (rounds * max_count) / t.time(),
                total_read / (1048576.0 * t.time()));
      }
   }

   return true;
}

bool test06(char* buffer)
{
   char in_char = -17;
   unsigned char in_uchar = 200;
   short in_short = -20000;
   unsigned short in_ushort = 55555;
   int in_int = -1111111;
   unsigned int in_uint = 79797979;
   long in_long = -43294761;
   unsigned long in_ulong = 78292365;
   float in_float = 1234.5678f;
   double in_double = 9876.54321;
   long double in_ldouble = 456789.123456;

   char out_char = 0;
   unsigned char out_uchar = 0;
   short out_short = 0;
   unsigned short out_ushort = 0;
   int out_int = 0;
   unsigned int out_uint = 0;
   long out_long = 0;
   unsigned long out_ulong = 0;
   float out_float = 0.0f;
   double out_double = 0.0;
   long double out_ldouble = 0.0;

   unsigned char* ptr = reinterpret_cast<unsigned char*>(buffer);

   strtk::write_pod(ptr,
                    in_char,
                    in_uchar,
                    in_short,
                    in_ushort,
                    in_int,
                    in_uint,
                    in_long,
                    in_ulong,
                    in_float,
                    in_double,
                    in_ldouble);

   strtk::read_pod(ptr,
                   out_char,
                   out_uchar,
                   out_short,
                   out_ushort,
                   out_int,
                   out_uint,
                   out_long,
                   out_ulong,
                   out_float,
                   out_double,
                   out_ldouble);

   if (in_char    != out_char)    { std::cout << "test06() - Failed char"    << std::endl; return false; }
   if (in_uchar   != out_uchar)   { std::cout << "test06() - Failed uchar"   << std::endl; return false; }
   if (in_short   != out_short)   { std::cout << "test06() - Failed short"   << std::endl; return false; }
   if (in_ushort  != out_ushort)  { std::cout << "test06() - Failed ushort"  << std::endl; return false; }
   if (in_int     != out_int)     { std::cout << "test06() - Failed int"     << std::endl; return false; }
   if (in_uint    != out_uint)    { std::cout << "test06() - Failed uint"    << std::endl; return false; }
   if (in_long    != out_long)    { std::cout << "test06() - Failed long"    << std::endl; return false; }
   if (in_ulong   != out_ulong)   { std::cout << "test06() - Failed ulong"   << std::endl; return false; }
   if (in_float   != out_float)   { std::cout << "test06() - Failed float"   << std::endl; return false; }
   if (in_double  != out_double)  { std::cout << "test06() - Failed double"  << std::endl; return false; }
   if (in_ldouble != out_ldouble) { std::cout << "test06() - Failed ldouble" << std::endl; return false; }

   return true;
}

bool test07(char* buffer)
{
   const size_t size = 10;
   const int intlst[size] = { -1, 2, -3, 4, -5, 6, -7, 8, -9, 10 };
   const unsigned int uintlst[size] = {
                                        734594, 1375762, 5432543,     3454, 32132,
                                         65463,  976765, 2355754, 74239542, 32523
                                      };
   const float fltlst[size] = { 1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f, 10.10f };
   const double dbllst[size] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10 };

   int r_intlst[size] = { 0 };
   unsigned int r_uintlst[size] = { 0 };
   float r_fltlst[size] = { 0.0f };
   double r_dbllst[size] = { 0.0 };

   unsigned char* ptr = reinterpret_cast<unsigned char*>(buffer);
   ptr = strtk::write_pod(ptr,intlst);
   ptr = strtk::write_pod(ptr,uintlst);
   ptr = strtk::write_pod(ptr,fltlst);
   ptr = strtk::write_pod(ptr,dbllst);

   ptr = reinterpret_cast<unsigned char*>(buffer);
   ptr = strtk::read_pod(ptr,r_intlst);
   ptr = strtk::read_pod(ptr,r_uintlst);
   ptr = strtk::read_pod(ptr,r_fltlst);
   ptr = strtk::read_pod(ptr,r_dbllst);

   if (!std::equal(intlst, intlst + size, r_intlst))
   {
      std::cout << "test07() - failed int list compare." << std::endl;
      return false;
   }

   if (!std::equal(uintlst, uintlst + size, r_uintlst))
   {
      std::cout << "test07() - failed unsigned int list compare." << std::endl;
      return false;
   }

   if (!std::equal(fltlst, fltlst + size, r_fltlst))
   {
      std::cout << "test07() - failed float list compare." << std::endl;
      return false;
   }

   if (!std::equal(dbllst, dbllst + size, r_dbllst))
   {
      std::cout << "test07() - failed double list compare." << std::endl;
      return false;
   }

   return true;
}

int main()
{
   static const std::size_t max_buffer_size = 512 * strtk::one_kilobyte; // 512KB
   char* buffer = new char[max_buffer_size];
   test01(buffer,max_buffer_size);
   test02(buffer,max_buffer_size);
   test03(buffer,max_buffer_size);
   test04(buffer,max_buffer_size);
   test05(buffer,max_buffer_size);
   test06(buffer);
   test07(buffer);
   delete[] buffer;
   return 0;
}
