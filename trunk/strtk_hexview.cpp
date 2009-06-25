/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Hex-Viewer                                                      *
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
   const unsigned int data_width = 16;
   unsigned int current_address  = 0;

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
         unsigned int read_in_width = 0;
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
