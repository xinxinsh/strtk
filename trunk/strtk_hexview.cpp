/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Hex-Viewer                                                      *
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
   Description: This example demonstrates a simple hex-viewer
                application using the String Toolkit Library's
                bin to hex conversion functionality. The viewer
                takes input from the stdin, processes the input
                and outputs it to stdout. An example of the
                output:

   00000000e5  2020202020202020737472746B3A3A63 ........strtk::c
   00000000e6  6F6E766572745F62696E5F746F5F6865 onvert_bin_to_he
   00000000e7  78286275666665722C62756666657220 x(buffer,buffer.
   00000000e8  2B20726561645F696E5F77696474682C +.read_in_width,
   00000000e9  6865785F627566666572293B0A202020 hex_buffer);....
   00000000ea  2020202020202020207374643A3A636F .........std::co
   00000000eb  7574203C3C207374643A3A686578203C ut.<<.std::hex.<
   00000000ec  3C207374643A3A736574772831302920 <.std::setw(10).
   00000000ed  3C3C2063757272656E745F6164647265 <<.current_addre
   00000000ee  7373203C3C20222020223B0A20202020 ss.<<."..";.....
   00000000ef  20202020202020207374643A3A636F70 ........std::cop
   00000000f0  79286865785F6275666665722C686578 y(hex_buffer,hex
   00000000f1  5F627566666572202B202832202A2072 _buffer.+.(2.*.r
   00000000f2  6561645F696E5F7769647468292C7374 ead_in_width),st
   00000000f3  643A3A6F73747265616D5F6974657261 d::ostream_itera
   00000000f4  746F723C756E7369676E656420636861 tor<unsigned.cha
   00000000f5  723E287374643A3A636F75742C222229 r>(std::cout,"")

*/


#include <cstddef>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include "strtk.hpp"

int main()
{
   /*
      Note: process takes input from stdin.
      usage: cat file.txt | strtk_hexview
   */
   const std::size_t data_width = 16;
   std::size_t current_address  = 0;

   unsigned char* buffer = new unsigned char[data_width];
   unsigned char* hex_buffer = new unsigned char[2 * data_width];

   while (true)
   {
      std::cin.read(reinterpret_cast<char*>(buffer),data_width);
      if (!(std::cin.eof() || std::cin.bad()))
      {
         strtk::convert_bin_to_hex(buffer,buffer + data_width,hex_buffer);
         std::cout << std::hex << std::setfill('0') << std::setw(10) << current_address << "  ";
         std::copy(hex_buffer,hex_buffer + (2 * data_width),std::ostream_iterator<unsigned char>(std::cout,""));
         std::cout << " ";
         strtk::convert_to_printable_chars(buffer,buffer + data_width);
         std::copy(buffer,buffer + data_width,std::ostream_iterator<unsigned char>(std::cout,""));
         std::cout << std::endl;
         ++current_address;
      }
      else
      {
         std::size_t read_in_width = 0;
         if (0 != (read_in_width = std::cin.gcount()))
         {
            strtk::convert_bin_to_hex(buffer,buffer + read_in_width,hex_buffer);
            std::cout << std::hex << std::setw(10) << current_address << "  ";
            std::copy(hex_buffer,hex_buffer + (2 * read_in_width),std::ostream_iterator<unsigned char>(std::cout,""));
            std::cout << std::string(2 * (16 - read_in_width) + 1, ' ');
            strtk::convert_to_printable_chars(buffer,buffer + read_in_width);
            std::copy(buffer,buffer + read_in_width,std::ostream_iterator<unsigned char>(std::cout,""));
            std::cout << std::endl;
         }
         break;
      }
   }

   return 0;
}
