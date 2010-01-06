/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Serializer Example                                              *
 * Author: Arash Partow (2002-2010)                                *
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


#include <cstddef>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

#include "strtk.hpp"


struct person
{
public:
   std::string  name;
   unsigned int age;
   double       height;
   float        weight;
   bool         state;

   person()
   {
      clear();
   }

   bool operator==(const person& p)
   {
      return (p.name   ==   name) &&
             (p.age    ==    age) &&
             (p.weight == weight) &&
             (p.height == height) &&
             (p.state  ==  state);
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
      state = false;
   }

   void read(strtk::serializer& s)
   {
      s >> name;
      s >> age;
      s >> height;
      s >> weight;
      s >> state;
   }

   void write(strtk::serializer& s)
   {
      s << name;
      s << age;
      s << height;
      s << weight;
      s << state;
   }
};


bool test01(char* buffer, const unsigned int buffer_size)
{
   strtk::serializer s(buffer,buffer_size);
   s.clear();

   person p1;
   person p2;

   p1.name   = "Mr. Rumpelstilzchen";
   p1.age    = 637;
   p1.height = 123.4567;
   p1.weight = 765.345f;
   p1.state  = true;

   p1.write(s);

   s.reset();

   p2.read(s);

   if (p1 != p2)
   {
      std::cout << "Test01 failed!" << std::endl;
      return false;
   }
   return true;
}

bool test02(char* buffer, const unsigned int buffer_size)
{
   strtk::serializer s(buffer,buffer_size);
   s.clear();

   person p1;

   p1.name   = "Mr. Rumpelstilzchen";
   p1.age    = 0;
   p1.height = 123.4567;
   p1.weight = 765.345f;
   p1.state  = true;

   const std::size_t rounds = 1000;

   for(unsigned int i = 0; i < rounds; ++i)
   {
      p1.write(s);
      p1.age++;
      p1.height += 1.23;
      p1.weight += 4.567f;
      p1.state = !p1.state;
   }

   s.reset();

   p1.name   = "Mr. Rumpelstilzchen";
   p1.age    = 0;
   p1.height = 123.4567;
   p1.weight = 765.345f;
   p1.state  = true;

   person p2;

   for(unsigned int i = 0; i < rounds; ++i)
   {
      p2.clear();
      p2.read(s);
      if (p1 != p2)
      {
         std::cout << "Test02 failed!" << std::endl;
         return false;
      }
      p1.age++;
      p1.height += 1.23;
      p1.weight += 4.567f;
      p1.state = !p1.state;
   }
   return true;
}

bool test03(char* buffer, const unsigned int buffer_size)
{
   strtk::serializer s(buffer,buffer_size);
   s.clear();

   person p1;

   p1.name   = "Mr. Rumpelstilzchen";
   p1.age    = 0;
   p1.height = 123.4567;
   p1.weight = 765.345f;
   p1.state  = true;

   const std::size_t rounds = 1000;

   for(unsigned int i = 0; i < rounds; ++i)
   {
      p1.write(s);
      p1.age++;
      p1.height += 1.23;
      p1.weight += 4.567f;
      p1.state = !p1.state;
   }

   std::ofstream o_stream("data.txt",std::ios::binary);
   if (!o_stream)
   {
      std::cout << "Test03() ERROR - Could not open file!(1)" << std::endl;
      return false;
   }

   s.write_to_stream(o_stream);
   o_stream.close();

   std::size_t length = s.length();

   std::ifstream i_stream("data.txt",std::ios::binary);
   if(!i_stream)
   {
      std::cout << "Test03() ERROR - Could not open file!(2)" << std::endl;
      return false;
   }

   s.read_from_stream(i_stream,length);
   s.reset();

   p1.name   = "Mr. Rumpelstilzchen";
   p1.age    = 0;
   p1.height = 123.4567;
   p1.weight = 765.345f;
   p1.state  = true;

   person p2;

   for(unsigned int i = 0; i < rounds; ++i)
   {
      p2.clear();
      p2.read(s);
      if (p1 != p2)
      {
         std::cout << "Test03 failed!" << std::endl;
         return false;
      }
      p1.age++;
      p1.height += 1.23;
      p1.weight += 4.567f;
      p1.state = !p1.state;
   }
   return true;
}


bool test04(char* buffer, const unsigned int buffer_size)
{
   strtk::serializer s(buffer,buffer_size);
   s.clear();

   std::deque<unsigned int> lst;
   const unsigned int max_count = 100;
   for(unsigned int i = 0; i < max_count; lst.push_back(i++));

   s.write_from_external_sequence(lst);

   lst.clear();
   s.reset();

   s.read_into_external_sequence(lst);

   for(unsigned int i = 0; i < max_count; ++i)
   {
      if (lst[i] != i)
      {
         std::cout << "failure at index: " << i << std::endl;
         return false;
      }
   }
   return true;
}


int main()
{
   static const std::size_t max_buffer_size = 64 * strtk::one_kilobyte; // 64KB
   char* buffer = new char[max_buffer_size];
   test01(buffer,max_buffer_size);
   test02(buffer,max_buffer_size);
   test03(buffer,max_buffer_size);
   test04(buffer,max_buffer_size);
   delete[] buffer;
   return 0;
}
