/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Key-Value Pair Parser Example                                 *
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
   Description: This example demonstrates parsing of a sequence of key-value
                pairs. An example data object named "data_store" is used which
                contains 11 various types of members that need populating. Each
                member is mapped via a key to value which is registered with the
                parser. The examples below demonstrate parsing of keys as
                unsigned int and of std::string types.

                An array of 5 sequences of key-value pairs, in various orderings,
                are passed to the key-value parser. The parser processes each
                sequence, splitting them up into pairs and subsequently
                processing each pair, mapping parsed values to the appropriate
                registered value. The whole process is timed, and details such as
                total time and rate of pairs parsed per second is displayed. Note
                a running total is maintained so as to overcome the effects of
                overzealous optimizers.
*/


#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <deque>

#include "strtk.hpp"


struct data_store
{
   static const std::size_t member_count = 11;

   data_store()
   :   c(0),
      uc(0),
       s(0),
      us(0),
       i(0),
      ui(0),
      ll(0),
     ull(0),
     d(0.0),
     f(0.0),
    str("")
   {}

   char                 c; // key = 121
   unsigned char       uc; // key = 122
   short                s; // key = 123
   unsigned short      us; // key = 124
   int                  i; // key = 125
   unsigned int        ui; // key = 126
   long long           ll; // key = 127
   unsigned long long ull; // key = 128
   double               d; // key = 129
   float                f; // key = 110
   std::string        str; // key = 111

};

int main()
{
   static const std::size_t rounds = 1000000;

   // Format: key_0=value_1|key_2=value_2|......|key_n=value_n
   static const std::string data[] =
                      {
                         //Seq 00
                         "121=A|122=z|123=-123|124=456|125=-12345678|126=789012345|127=-43654364634645235|128=777777777777777777|129=1234.56789|110=0.0003|111=abcdefghijklmnopqrstuvwxyz",
                         //Seq 01
                         "121=A|123=-111|125=-17171717|127=-43654364634645235|129=1234.56789|111=abcdefghijklmnopqrstuvwxyz|122=z|124=999|126=789012345|128=777777777777777777|110=0.0003",
                         //Seq 02
                         "122=z|124=999|126=789012345|128=777777777777777777|110=0.0003|121=A|123=-111|125=-17171717|127=-43654364634645235|129=1234.56789|111=abcdefghijklmnopqrstuvwxyz",
                         //Seq 03
                         "121=A|122=z|123=-123|124=456|125=-12345678|126=789012345|127=-43654364634645235|128=777777777777777777|129=1234.56789|110=0.0003|111=abcdefghijklmnopqrstuvwxyz",
                         //Seq 04
                         "111=abcdefghijklmnopqrstuvwxyz|110=0.0003|129=1234.56789|128=777777777777777777|127=-43654364634645235|126=789012345|125=-12345678|124=456|123=-123|122=z|121=A"
                       };

   static const std::size_t data_size = sizeof(data) / sizeof(std::string);

   {
      typedef strtk::keyvalue::parser<strtk::keyvalue::uintkey_map> kvp_type;

      strtk::keyvalue::uintkey_map::options options;

      options.key_count            = 130; //[110,129]
      options.pair_block_delimiter = '|';
      options.pair_delimiter       = '=';

      kvp_type kvp(options);

      data_store ds;

      kvp.register_keyvalue(121,ds.  c);
      kvp.register_keyvalue(122,ds. uc);
      kvp.register_keyvalue(123,ds.  s);
      kvp.register_keyvalue(124,ds. us);
      kvp.register_keyvalue(125,ds.  i);
      kvp.register_keyvalue(126,ds. ui);
      kvp.register_keyvalue(127,ds. ll);
      kvp.register_keyvalue(128,ds.ull);
      kvp.register_keyvalue(129,ds.  d);
      kvp.register_keyvalue(110,ds.  f);
      kvp.register_keyvalue(111,ds.str);

      unsigned int total = 0;

      strtk::util::timer timer;

      timer.start();
      for (std::size_t i = 0; i < rounds; ++i)
      {
         for (std::size_t j = 0; j < data_size; ++j)
         {
            if (kvp(data[j]))
               total += ds.ui;
            else
               return 1;
         }
      }
      timer.stop();

      printf("[uintkeymap] Total: %d Time %8.5fsec Rate: %12.3fpairs/sec\n",
             total,
             timer.time(),
             (rounds * data_store::member_count * data_size) / timer.time ());
   }

   {
      typedef unsigned char char_type;
      typedef strtk::keyvalue::parser<strtk::keyvalue::stringkey_map> kvp_type;
      typedef strtk::keyvalue::options<char_type> opts_type;

      opts_type options;

      options.pair_block_delimiter = '|';
      options.pair_delimiter       = '=';

      kvp_type kvp(options);

      data_store ds;

      kvp.register_keyvalue("121",ds.  c);
      kvp.register_keyvalue("122",ds. uc);
      kvp.register_keyvalue("123",ds.  s);
      kvp.register_keyvalue("124",ds. us);
      kvp.register_keyvalue("125",ds.  i);
      kvp.register_keyvalue("126",ds. ui);
      kvp.register_keyvalue("127",ds. ll);
      kvp.register_keyvalue("128",ds.ull);
      kvp.register_keyvalue("129",ds.  d);
      kvp.register_keyvalue("110",ds.  f);
      kvp.register_keyvalue("111",ds.str);

      unsigned int total = 0;

      strtk::util::timer timer;

      timer.start();
      for (std::size_t i = 0; i < rounds; ++i)
      {
         for (std::size_t j = 0; j < data_size; ++j)
         {
            if (kvp(data[j]))
               total += ds.ui;
            else
               return 1;
         }
      }
      timer.stop();

      printf("[strkeymap ] Total: %d Time %8.5fsec Rate: %12.3fpairs/sec\n",
             total,
             timer.time(),
             (rounds * data_store::member_count * data_size) / timer.time ());
   }

   return 0;
}

