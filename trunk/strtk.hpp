/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
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

#ifndef INCLUDE_STRTK_HPP
#define INCLUDE_STRTK_HPP

#include <cstddef>
#include <cctype>
#include <cstring>
#include <cerrno>
#include <iterator>
#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>

#include <boost/lexical_cast.hpp>

#define ENABLE_RANDOM
#ifdef ENABLE_RANDOM
   // define a TR1 compatible random library header
   #include <boost/random.hpp>
   //#include <random>
#endif

#define ENABLE_REGEX
#ifdef ENABLE_REGEX
   // define a TR1 compatible regex library header
   #include <boost/regex.hpp>
   //#include <regex>
#endif

namespace strtk
{

   static const std::size_t one_kilobyte = 1024;

   template<typename Tokenizer, class Function>
   inline std::size_t for_each_token(const std::string& buffer,
                                     Tokenizer& tokenizer,
                                     Function function)
   {
      std::size_t token_count = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenizer::iterator it = tokenizer.begin();
      while (tokenizer.end() != it)
      {
         function(*it);
         ++it;
         ++token_count;
      }
      return token_count;
   }

   template<class Function>
   inline std::size_t for_each_line(std::ifstream& stream, Function function)
   {
      std::string buffer;
      buffer.reserve(one_kilobyte);
      std::size_t line_count = 0;
      while (std::getline(stream,buffer))
      {
         function(buffer);
         ++line_count;
      }
      return line_count;
   }

   template<class Function>
   inline std::size_t for_each_line_n(std::ifstream& stream, const std::size_t& n, Function function)
   {
      std::string buffer;
      buffer.reserve(one_kilobyte);
      std::size_t line_count = 0;
      while (std::getline(stream,buffer))
      {
         function(buffer);
         if (n == ++line_count)
          break;
      }
      return line_count;
   }

   template<class Function>
   inline std::size_t for_each_line(const std::string& file_name, Function function)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line(stream,function);
      else
         return 0;
   }

   template<class Function>
   inline std::size_t for_each_line_n(const std::string& file_name, const std::size_t& n, Function function)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line_n(stream,n,function);
      else
         return 0;
   }

   template<class Function>
   inline std::size_t for_each_line_conditional(std::ifstream& stream, Function function)
   {
      std::string buffer;
      buffer.reserve(one_kilobyte);
      std::size_t line_count = 0;
      while (std::getline(stream,buffer))
      {
         if (!function(buffer))
         {
            return line_count;
         }
         ++line_count;
      }
      return line_count;
   }

   template<class Function>
   inline std::size_t for_each_line_n_conditional(std::ifstream& stream, const std::size_t& n, Function function)
   {
      std::string buffer;
      buffer.reserve(one_kilobyte);
      std::size_t line_count = 0;
      while (std::getline(stream,buffer))
      {
         if (!function(buffer))
         {
            return line_count;
         }
         if (n == ++line_count)
          break;
      }
      return line_count;
   }

   template<class Function>
   inline std::size_t for_each_line_conditional(const std::string& file_name, Function function)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line_conditional(stream,function);
      else
         return 0;
   }

   template<class Function>
   inline std::size_t for_each_line_n_conditional(const std::string& file_name, const std::size_t& n, Function function)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line_n_conditional(stream,n,function);
      else
         return 0;
   }

   template<typename T>
   static inline bool string_to_type_converter(const std::string& s, T& t)
   {
      try
      {
         t = boost::lexical_cast<T>(s);
      }
      catch(boost::bad_lexical_cast&)
      {
         return false;
      }
      return true;
   }

   template<typename T, typename InputIterator>
   inline bool string_to_type_converter(const InputIterator begin, const InputIterator end, T& t)
   {
      return string_to_type_converter(std::string(begin,end),t);
   }

   template<>
   inline bool string_to_type_converter(const std::string& s, std::string& out)
   {
      out = s;
      return true;
   }

   template<typename T>
   inline T string_to_type_converter(const std::string& s)
   {
      return boost::lexical_cast<T>(s);
   }

   template<>
   inline std::string string_to_type_converter(const std::string& s)
   {
      return s;
   }

   #define INSTANTIATE_STRING_TO_TYPE_CONVERTER(ITER_TYPE)\
   template<typename T>\
   inline T string_to_type_converter(const ITER_TYPE begin,\
                                     const ITER_TYPE end)\
   {\
      return string_to_type_converter<T>(std::string(begin,end));\
   }

   INSTANTIATE_STRING_TO_TYPE_CONVERTER(char*)
   INSTANTIATE_STRING_TO_TYPE_CONVERTER(unsigned char*)
   INSTANTIATE_STRING_TO_TYPE_CONVERTER(std::string::iterator)
   INSTANTIATE_STRING_TO_TYPE_CONVERTER(std::string::const_iterator)

   #define INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_ITR_DECL(T,ITER_TYPE)\
   template<> inline T string_to_type_converter(const ITER_TYPE begin, const ITER_TYPE end);\
   inline bool string_to_type_converter(const ITER_TYPE begin, const ITER_TYPE end, T& t);

   #define INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_DECL(T)\
   template<> inline T string_to_type_converter(const std::string& s);\
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_ITR_DECL(T,char*)\
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_ITR_DECL(T,unsigned char*)\
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_ITR_DECL(T,std::string::iterator)\
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_ITR_DECL(T,std::string::const_iterator)

   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_DECL(bool)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_DECL(char)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_DECL(short)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_DECL(int)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_DECL(long)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_DECL(unsigned char)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_DECL(unsigned short)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_DECL(unsigned int)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_DECL(unsigned long)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_DECL(float)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_DECL(double)

   template<typename T>
   inline std::string type_to_string(const T& t)
   {
      return boost::lexical_cast<std::string>(t);
   }

   template<>
   inline std::string type_to_string(const std::string& v)
   {
      return v;
   }

   template <typename T,
             class Allocator,
             template <class,class> class Sequence>
   inline std::size_t load_from_text_file(std::ifstream& stream,
                                          Sequence<T,Allocator>& sequence)
   {
     if (!stream) return 0;
     std::string buffer;
     buffer.reserve(one_kilobyte);
     std::size_t line_count = 0;
     while (std::getline(stream,buffer))
     {
        ++line_count;
        sequence.push_back(string_to_type_converter<T>(buffer));
     }
     return line_count;
   }

   template <typename T,
             class Allocator,
             template <class,class> class Sequence>
   inline std::size_t load_from_text_file(const std::string& file_name,
                                          Sequence<T,Allocator>& sequence)
   {
     std::ifstream stream(file_name.c_str());
     if (!stream)
        return 0;
     else
        return load_from_text_file(stream,sequence);
   }

   template <typename T,
             class Allocator,
             template <class,class> class Sequence>
   inline std::size_t write_to_text_file(std::ofstream& stream,
                                         const Sequence<T,Allocator>& sequence,
                                         const std::string& delimiter = "")
   {
     if (!stream) return 0;
     std::size_t count = 0;
     typename Sequence<T,Allocator>::const_iterator it = sequence.begin();
     if (!delimiter.empty())
     {
        while (sequence.end() != it)
        {
           stream << *it++ << delimiter;
           ++count;
        }
     }
     else
     {
        while (sequence.end() != it)
        {
           stream << *it++;
           ++count;
        }
     }
     return count;
   }

   template <typename T,
             class Allocator,
             template <class,class> class Sequence>
   inline std::size_t write_to_text_file(const std::string& file_name,
                                         const Sequence<T,Allocator>& sequence,
                                         const std::string& delimiter = "")
   {
     std::ofstream stream(file_name.c_str());
     if (!stream)
        return 0;
     else
        return write_to_text_file(stream,sequence,delimiter);
   }

   template<typename Predicate, typename InputIterator, typename OutputIterator>
   inline void copy_if(Predicate predicate,
                       const InputIterator begin, const InputIterator end,
                       OutputIterator out)
   {
      InputIterator it = begin;
      while (end != it)
      {
         if (predicate(*it))
         {
            *(out++)= *it;
         }
         ++it;
      }
   }

   template<typename Predicate, typename InputIterator, typename OutputIterator>
   inline InputIterator copy_while(Predicate predicate,
                                   const InputIterator begin, const InputIterator end,
                                   OutputIterator out)
   {
      InputIterator it = begin;
      while (end != it)
      {
         if (!predicate(*it))
            break;
         *(out++)= *(it++);
      }
      return it;
   }

   template<typename Predicate, typename InputIterator, typename OutputIterator>
   inline InputIterator copy_until(Predicate predicate,
                                   const InputIterator begin, const InputIterator end,
                                   OutputIterator out)
   {
      InputIterator it = begin;
      while (end != it)
      {
         if (predicate(*it))
            break;
         *(out++)= *(it++);
      }
      return it;
   }

   template<typename Predicate, typename InputIterator>
   inline bool range_only_contains(Predicate predicate, const InputIterator begin, const InputIterator end)
   {
      InputIterator it = begin;
      while (end != it)
      {
         if (!predicate(*it++))
         {
            return false;
         }
      }
      return true;
   }

   template<typename T>
   struct single_delimiter_predicate
   {
   public:
      typedef T value_type;

      single_delimiter_predicate(const T& d)
      : delimiter_(d)
      {}

      inline bool operator()(const T& d) const
      {
         return d == delimiter_;
      }

   private:
      T delimiter_;
   };

   template<typename T>
   struct multiple_delimiter_predicate
   {
   public:

      typedef T value_type;

      multiple_delimiter_predicate(const T* d_begin,const T* d_end)
      : length_(std::distance(d_begin,d_end)),
        delimiter_(new T[length_]),
        delimiter_end_(delimiter_ + length_)
      {
         std::copy(d_begin,d_end, delimiter_);
      }

      multiple_delimiter_predicate(const T d[], const std::size_t& length)
      : length_(length),
        delimiter_(new T[length_]),
        delimiter_end_(delimiter_ + length_)
      {
         std::copy(d,d + length, delimiter_);
      }

      template<typename Iterator>
      multiple_delimiter_predicate(const Iterator begin,const Iterator end)
      : length_(std::distance(begin,end)),
        delimiter_(new T[length_]),
        delimiter_end_(delimiter_ + length_)
      {
         //static_assert(T == std::iterator_traits<Iterator>::value_type);
         std::copy(begin,end, delimiter_);
      }

     ~multiple_delimiter_predicate()
      {
        delete[] delimiter_;
      }

      inline bool operator()(const T& d) const
      {
         return (std::find(delimiter_,delimiter_end_,d) != delimiter_end_);
      }

   private:

      multiple_delimiter_predicate(const multiple_delimiter_predicate<T>& mdp);
      multiple_delimiter_predicate operator=(const multiple_delimiter_predicate<T>& mdp);

   private:
      std::size_t length_;
      T* delimiter_;
      T* delimiter_end_;
   };

   struct multiple_char_delimiter_predicate
   {
   public:

      typedef unsigned char value_type;

      template<typename Iterator>
      multiple_char_delimiter_predicate(const Iterator begin,const Iterator end)
      {
         setup_delimiter_table(begin,end);
      }

      multiple_char_delimiter_predicate(const std::string& s)
      {
         setup_delimiter_table(s.begin(),s.end());
      }

      inline bool operator()(const unsigned char& c) const
      {
         return (1 == delimiter_table_[c]);
      }

      inline bool operator()(const char& c) const
      {
         return operator()(static_cast<unsigned char>(c));
      }

   private:
      static const std::size_t table_size = 256;

      template<typename Iterator>
      inline void setup_delimiter_table(const Iterator begin, const Iterator end)
      {
         std::memset(delimiter_table_,0x00,table_size);
         for (Iterator it = begin; it != end; ++it) delimiter_table_[static_cast<unsigned char>(*it)] = 1;
      }

      value_type delimiter_table_[table_size];
   };

   template<typename Iterator, class Predicate>
   inline std::size_t remove_inplace(Predicate predicate,
                                     Iterator begin,
                                     Iterator end)
   {
      Iterator it1 = begin;
      Iterator it2 = begin;
      std::size_t removal_count = 0;
      while (it1 != end)
      {
         while ((end != it1) && !predicate(*it1))
         {
            if (it1 != it2)
            {
               *it2 = *it1;
            }
            ++it1;
            ++it2;
         }
         while ((end != it1) && predicate(*it1))
         {
            ++it1;
            ++removal_count;
         }
      }
      return removal_count;
   }

   inline void remove_inplace(const std::string::value_type c, std::string& s)
   {
      std::size_t removal_count = remove_inplace(single_delimiter_predicate<std::string::value_type>(c),s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<class Predicate>
   inline void remove_inplace(Predicate predicate, std::string& s)
   {
      std::size_t removal_count = remove_inplace(predicate,s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Iterator, class Predicate>
   inline std::size_t remove_consecutives_inplace(Predicate predicate,
                                                  Iterator begin,
                                                  Iterator end)
   {
      if (0 == std::distance(begin,end)) return 0;
      Iterator it1 = (begin + 1);
      Iterator it2 = (begin + 1);
      typename std::iterator_traits<Iterator>::value_type prev = *begin;
      std::size_t removal_count = 0;
      while (it1 != end)
      {
         while ((end != it1) && (!predicate(*it1) || !predicate(prev)))
         {
            if (it1 != it2)
            {
               *it2 = *it1;
            }
            prev = *it1;
            ++it1;
            ++it2;
         }
         while ((end != it1) && predicate(*it1))
         {
            ++it1;
            ++removal_count;
         }
      }
      return removal_count;
   }

   inline void remove_consecutives_inplace(const std::string::value_type c, std::string& s)
   {
      if (s.empty()) return;
      std::size_t removal_count = remove_consecutives_inplace(single_delimiter_predicate<std::string::value_type>(c),
                                                              s.begin(),
                                                              s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_consecutives_inplace(const std::string& rem_chars, std::string& s)
   {
      if (s.empty()) return;
      std::size_t removal_count = remove_consecutives_inplace(multiple_char_delimiter_predicate(rem_chars),
                                                              s.begin(),
                                                              s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_consecutives_inplace(const char* rem_chars, std::string& s)
   {
      if (s.empty()) return;
      std::size_t removal_count = remove_consecutives_inplace(multiple_char_delimiter_predicate(
                                                              rem_chars,
                                                              rem_chars + ::strnlen(rem_chars,256)),
                                                              s.begin(),
                                                              s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<class Predicate>
   inline void remove_consecutives_inplace(Predicate predicate, std::string& s)
   {
      if (s.empty()) return;
      std::size_t removal_count = remove_consecutives_inplace(predicate,s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Iterator>
   inline std::size_t remove_consecutives_inplace(Iterator begin, Iterator end)
   {
      if (0 == std::distance(begin,end)) return 0;
      Iterator it1 = (begin + 1);
      Iterator it2 = (begin + 1);
      typename std::iterator_traits<Iterator>::value_type prev = *begin;
      std::size_t removal_count = 0;
      while (it1 != end)
      {
         while ((end != it1) && (prev != (*it1)))
         {
            if (it1 != it2)
            {
               *it2 = *it1;
            }
            prev = *it1;
            ++it1;
            ++it2;
         }
         while ((end != it1) && (prev == (*it1)))
         {
            ++it1;
            ++removal_count;
         }
      }
      return removal_count;
   }

   inline void remove_consecutives_inplace(std::string& s)
   {
      std::size_t removal_count = remove_consecutives_inplace(s.begin(),s.end());

      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Iterator, class Predicate>
   inline std::size_t remove_trailing(Predicate predicate,
                                      Iterator begin,
                                      Iterator end)
   {
      std::size_t length = std::distance(begin,end);
      if (0 == length)
         return 0;
      Iterator it = begin + (length - 1);
      std::size_t removal_count = 0;
      while ((begin != it) && predicate(*it))
      {
         it--;
         removal_count++;
      }
      return removal_count;
   }

   inline void remove_trailing(const std::string::value_type c, std::string& s)
   {
      if (s.empty()) return;
      std::size_t removal_count = remove_trailing(single_delimiter_predicate<std::string::value_type>(c),
                                                  s.begin(),
                                                  s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_trailing(const std::string& rem_chars, std::string& s)
   {
      if (s.empty()) return;
      std::size_t removal_count = remove_trailing(multiple_char_delimiter_predicate(rem_chars),
                                                  s.begin(),
                                                  s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_trailing(const char* rem_chars, std::string& s)
   {
      std::size_t removal_count = remove_trailing(multiple_char_delimiter_predicate(
                                                  rem_chars,
                                                  rem_chars + strnlen(rem_chars,256)),
                                                  s.begin(),
                                                  s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<class Predicate>
   inline void remove_trailing(Predicate predicate, std::string& s)
   {
      if (s.empty()) return;
      std::size_t removal_count = remove_trailing(predicate,s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Iterator, class Predicate>
   inline std::size_t remove_proceeding(Predicate predicate,
                                        Iterator begin,
                                        Iterator end)
   {
      std::size_t length = std::distance(begin,end);
      if (0 == length)
         return 0;
      Iterator it = begin;
      std::size_t removal_count = 0;
      while ((end != it) && predicate(*it))
      {
         it++;
         removal_count++;
      }
      std::copy(it,end,begin);
      return removal_count;
   }

   inline void remove_proceeding(const std::string::value_type c, std::string& s)
   {
      if (s.empty()) return;
      std::size_t removal_count = remove_proceeding(single_delimiter_predicate<std::string::value_type>(c),
                                                    s.begin(),
                                                    s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_proceeding(const std::string& rem_chars, std::string& s)
   {
      if (s.empty()) return;
      std::size_t removal_count = remove_proceeding(multiple_char_delimiter_predicate(rem_chars),
                                                    s.begin(),
                                                    s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_proceeding(const char* rem_chars, std::string& s)
   {
      if (s.empty()) return;
      std::size_t removal_count = remove_proceeding(multiple_char_delimiter_predicate(
                                                    rem_chars,
                                                    rem_chars + strnlen(rem_chars,256)),
                                                    s.begin(),
                                                    s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<class Predicate>
   inline void remove_proceeding(Predicate predicate, std::string& s)
   {
      if (s.empty()) return;
      std::size_t removal_count = remove_proceeding(predicate,s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Iterator>
   inline void replace(const typename std::iterator_traits<Iterator>::value_type& c1,
                       const typename std::iterator_traits<Iterator>::value_type& c2,
                       const Iterator begin,
                       const Iterator end)
   {
      for (Iterator it = begin; end != it; ++it)
      {
         if (c1 == *it) *it = c2;
      }
   }

   void replace_pattern(const std::string& s,
                        const std::string& p,
                        const std::string& r,
                        std::string& n)
   {
      if ((p == r) || p.empty())
      {
         n.assign(s);
         return;
      }

      std::size_t new_size = s.size();
      int inc = static_cast<int>(r.size()) - static_cast<int>(p.size());
      std::size_t pos = 0;
      std::size_t count = 0;
      while (std::string::npos != (pos = s.find(p,pos)))
      {
         pos += p.size();
         new_size += inc;
         ++count;
      }

      if (0 == count)
      {
         n.assign(s);
         return;
      }

      n.resize(new_size, 0x00);

      pos = 0;
      std::size_t prev_pos = 0;
      std::string::iterator sit = const_cast<std::string&>(s).begin();
      std::string::iterator nit = n.begin();

      while ((0 < count) && (std::string::npos != (pos = s.find(p,pos))))
      {
         std::size_t diff = pos - prev_pos;
         std::copy(sit,sit + diff,nit);
         nit += diff;
         std::copy(r.begin(),r.end(),nit);
         nit += r.size();
         pos += p.size();
         sit += pos - prev_pos;
         prev_pos = pos;
         --count;
      }
      while (s.end() != sit) (*nit++) = (*sit++);
   }

   template<typename InputIterator, typename OutputIterator>
   std::size_t replace_pattern(const InputIterator s_begin, const InputIterator s_end,
                               const InputIterator p_begin, const InputIterator p_end,
                               const InputIterator r_begin, const InputIterator r_end,
                               OutputIterator out)
   {
      typedef typename std::iterator_traits<InputIterator>::value_type T;

      InputIterator s_it  = s_begin;
      InputIterator r_it  = r_begin;
      InputIterator p_it  = p_begin;

      std::size_t s_len = std::distance(s_begin,s_end);
      std::size_t p_len = std::distance(p_begin,p_end);
      std::size_t r_len = std::distance(r_begin,r_end);

      if ((0 == p_len) || ((p_len == r_len) && std::equal(p_begin,p_end,r_begin)))
      {
         std::copy(s_begin,s_end,out);
         return s_len;
      }

      std::size_t pos = 0;
      std::size_t prev_pos = 0;
      std::size_t count = 0;
      std::size_t new_size = s_len;
      int inc = r_len - p_len;

      InputIterator temp_s_it = s_it;

      while (s_end != s_it)
      {
         /*
            Need to replace the following search code with
            Knuth-Pratt-Morris or Boyer-Moore string search
            algorithms.
         */
         bool found = true;
         p_it = p_begin;
         temp_s_it = s_it;
         while ((p_end != p_it) && (s_end != temp_s_it))
         {
            if (*(temp_s_it++) != *(p_it++))
            {
               found = false;
               break;
            }
         }
         if (found && (p_it == p_end))
         {
            ++count;
            new_size += inc;
            s_it = temp_s_it;
         }
         else
            ++s_it;
      }

      s_it = s_begin;
      p_it = p_begin;

      pos = 0;
      prev_pos = 0;

      temp_s_it = s_it;

      while (0 < count)
      {
         p_it = p_begin;
         bool found = true;
         InputIterator pattern_start = temp_s_it;
         while ((p_end != p_it) && (s_end != temp_s_it))
         {
            if (*(temp_s_it++) != *(p_it++))
            {
               found = false;
               temp_s_it = pattern_start;
               ++temp_s_it;
               break;
            }
         }
         if (!found || (p_it != p_end)) continue;

         pos = std::distance(s_begin,temp_s_it) - p_len;
         int diff = pos - prev_pos;

         std::copy(s_it,s_it + diff, out);
         s_it = temp_s_it;
         std::copy(r_it,r_end, out);

         pos += p_len;
         prev_pos = pos;
         --count;
      }
      std::copy(s_it,s_end,out);
      return new_size;
   }

   template<typename Iterator>
   inline bool match(const Iterator pattern_begin,
                     const Iterator pattern_end,
                     const Iterator data_begin,
                     const Iterator data_end,
                     const typename std::iterator_traits<Iterator>::value_type& zero_or_more,
                     const typename std::iterator_traits<Iterator>::value_type& zero_or_one)
   {
      /*
         Credits:
         Adapted from code provided by Jack Handy (2001 CodeProject)
      */
      if (0 == std::distance(data_begin,data_end)) return false;

      Iterator d_it = data_begin;
      Iterator p_it = pattern_begin;
      Iterator c_it = data_begin;
      Iterator m_it = data_begin;

      while ((data_end != d_it) && ((*p_it) != zero_or_more))
      {
         if (((*p_it) != (*d_it)) && (zero_or_one != (*p_it)))
         {
            return false;
         }
         ++p_it;
         ++d_it;
      }

      while (data_end != d_it)
      {
         if (zero_or_more == (*p_it))
         {
            if (pattern_end == (++p_it))
            {
               return true;
            }
            m_it = p_it;
            c_it = d_it + 1;
         }
         else if (((*p_it) == (*d_it)) || (zero_or_one == (*p_it)))
         {
            ++p_it;
            ++d_it;
         }
         else
         {
            p_it = m_it;
            d_it = c_it++;
         }
      }
      while ((p_it != pattern_end) && (zero_or_more == (*p_it))) ++p_it;
      return (p_it == pattern_end);
   }

   inline bool match(const std::string& wild_card,
                     const std::string& str)
   {
      /*
         * : Zero or more match
         ? : Zero or one match
      */
      return match(wild_card.c_str(),wild_card.c_str() + wild_card.size(),
                   str.c_str(),str.c_str() + str.size(),'*','?');
   }

   inline bool imatch(const std::string::const_iterator begin1, const std::string::const_iterator end1,
                      const std::string::const_iterator begin2, const std::string::const_iterator end2)
   {
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return false;
      }
      std::string::const_iterator it1 = begin1;
      std::string::const_iterator it2 = begin2;
      while (end1 != it1)
      {
         //if (std::toupper(*it1, std::locale::classic()) != std::toupper(*it2, std::locale::classic()))
         if (std::toupper(*it1) != std::toupper(*it2))
         {
            return false;
         }
         ++it1;
         ++it2;
      }
      return true;
   }

   inline bool imatch(const std::string& s1, const std::string s2)
   {
      return imatch(s1.begin(),s1.end(),s2.begin(),s2.end());
   }

   template<typename Iterator>
   inline bool imatch(const std::string& s, const Iterator begin, const Iterator end)
   {
      for (const std::string* it = begin; end != it; ++it)
      {
         if (imatch(s,*it))
         {
            return true;
         }
      }
      return false;
   }

   template<typename Iterator, typename DelimiterPredicate>
   class tokenizer
   {
   private:

      template<typename Iterartor, typename Predicate, typename T = std::pair<Iterator,Iterator> >
      class tokenizer_iterator : public std::iterator<std::forward_iterator_tag, T>
      {
      protected:
         typedef Iterator iterator;
         typedef const iterator const_iterator;

      public:

         inline tokenizer_iterator(const iterator begin,
                                   const iterator end,
                                   const Predicate& predicate,
                                   const bool compress_delimiters = false)
         : predicate_(predicate),
           it_(begin),
           end_(end),
           prev_(begin),
           curr_tok_begin_(end_),
           curr_tok_end_(end_),
           last_token_(false),
           compress_delimiters_(compress_delimiters)
         {
            this->operator++();
         };

         inline tokenizer_iterator& operator++()
         {
            if (end_ != it_)
            {
               prev_ = it_;
            }

            while (end_ != it_)
            {
               if (predicate_(*it_))
               {
                  curr_tok_begin_ = prev_;
                  curr_tok_end_ = it_;
                  if (compress_delimiters_)
                     while ((end_ != ++it_) && predicate_(*it_));
                  else
                     ++it_;
                  return *this;
               }
               else
                 ++it_;
            }

            if (prev_ != it_)
            {
               curr_tok_end_ = it_;
               if (!last_token_)
               {
                  if (predicate_(*(it_ - 1)))
                     curr_tok_begin_ = it_;
                  else
                     curr_tok_begin_ = prev_;
                  last_token_ = true;
               }
               else
                  prev_ = it_;
            }

            return *this;
         }

         inline tokenizer_iterator operator++(int)
         {
            tokenizer_iterator tmp = *this;
            this->operator++();
            return tmp;
         }

         inline tokenizer_iterator& operator+=(const int inc)
         {
            for (int i = 0; i < inc; ++i) ++(*this);
            return *this;
         }

         inline T operator*() const
         {
            return std::make_pair<iterator,iterator>(curr_tok_begin_,curr_tok_end_);
         }

         inline bool operator==(const tokenizer_iterator& it) const
         {
            return (it_   == it.it_  ) &&
                   (prev_ == it.prev_) &&
                   (end_  == it.end_ );
         }

         inline bool operator!=(const tokenizer_iterator& it) const
         {
            return !this->operator==(it);
         }

         inline tokenizer_iterator& operator=(const tokenizer_iterator& it)
         {
            if (this != &it)
            {
               it_             = it.it_;
               end_            = it.end_;
               prev_           = it.prev_;
               curr_tok_begin_ = it.curr_tok_begin_;
               curr_tok_end_   = it.curr_tok_end_;
               last_token_     = it.last_token_;
            }
            return *this;
         }

         inline std::string remaining() const
         {
            return std::string(curr_tok_begin_,end_);
         }

      protected:
         const Predicate& predicate_;
         iterator it_;
         iterator end_;
         iterator prev_;
         iterator curr_tok_begin_;
         iterator curr_tok_end_;
         bool last_token_;
         bool compress_delimiters_;
      };

   public:

      typedef typename std::iterator_traits<Iterator>::value_type value_type;
      typedef DelimiterPredicate predicate;
      typedef tokenizer_iterator<Iterator,DelimiterPredicate> iterator;
      typedef const iterator const_iterator;
      typedef iterator& iterator_ref;
      typedef const_iterator& const_iterator_ref;

      inline tokenizer(const Iterator begin,
                       const Iterator end,
                       const DelimiterPredicate& predicate,
                       const bool compress_delimiters = false)
      : predicate_(predicate),
        begin_(begin),
        end_(end),
        begin_itr_(begin_,end_,predicate_,compress_delimiters),
        end_itr_(end_,end_,predicate_,compress_delimiters),
        compress_delimiters_(compress_delimiters)
      {}

      inline tokenizer(const std::string& s,
                       const DelimiterPredicate& predicate,
                       const bool compress_delimiters = false)
      : predicate_(predicate),
        begin_(s.begin()),
        end_(s.end()),
        begin_itr_(begin_,end_,predicate_,compress_delimiters),
        end_itr_(end_,end_,predicate_,compress_delimiters),
        compress_delimiters_(compress_delimiters)
      {}

      inline tokenizer& operator=(const tokenizer& t)
      {
         if (this != &t)
         {
            begin_               = t.begin_;
            end_                 = t.end_;
            end_itr_             = t.end_itr_;
            begin_itr_           = t.begin_itr_;
            compress_delimiters_ = t.compress_delimiters_;
         }
         return *this;
      }

      inline void assign(const std::string& s) const
      {
         assign(s.begin(),s.end());
      }

      inline void assign(const std::string& s)
      {
         assign(s.begin(),s.end());
      }

      inline void assign(const Iterator begin, const Iterator end)
      {
        begin_ = begin;
        end_ = end;
        begin_itr_ = iterator(begin_,end_,predicate_,compress_delimiters_);
        end_itr_ = iterator(end_,end_,predicate_,compress_delimiters_);
      }

      inline const_iterator_ref begin() const
      {
         return begin_itr_;
      }

      inline const_iterator_ref end() const
      {
         return end_itr_;
      }

   private:

      const DelimiterPredicate& predicate_;
      Iterator begin_;
      Iterator end_;
      iterator begin_itr_;
      iterator end_itr_;
      bool compress_delimiters_;
   };

   namespace std_string
   {
      template<typename DelimiterPredicate = single_delimiter_predicate<std::string::value_type> >
      struct tokenizer
      {
         typedef DelimiterPredicate predicate_type;
         typedef strtk::tokenizer<std::string::const_iterator,DelimiterPredicate> type;
         typedef std::pair<std::string::const_iterator , std::string::const_iterator> iterator_type;
      };

      typedef tokenizer<>::iterator_type iterator_type;
      typedef std::vector<iterator_type> token_vector_type;
      typedef std::deque<iterator_type> token_deque_type;
      typedef std::list<iterator_type> token_list_type;
   }

   template<typename T>
   class range_adapter
   {
   public:

      typedef T value_type;
      typedef T* iterator;
      typedef const iterator const_iterator;

      range_adapter(T* const begin, T* const end)
      : begin_(begin),
        end_(end)
      {}

      range_adapter(T* const begin, const std::size_t length)
      : begin_(begin),
        end_(begin_ + length)
      {}

      inline iterator begin()
      {
         return begin_;
      }

      inline iterator end()
      {
         return end_;
      }

   private:
      iterator begin_;
      iterator end_;
   };

   template <typename Sequence>
   class range_to_type_back_inserter_iterator : public std::iterator<std::output_iterator_tag,void,void,void,void>
   {
   public:

      typedef typename Sequence::value_type T;

      explicit range_to_type_back_inserter_iterator(Sequence& sequence)
      : sequence_(sequence)
      {}

      range_to_type_back_inserter_iterator(const range_to_type_back_inserter_iterator& it)
      : sequence_(it.sequence_)
      {}

      inline range_to_type_back_inserter_iterator& operator=(const range_to_type_back_inserter_iterator& it)
      {
         if (this != &it)
         {
            this->sequence_ = it.sequence_;
         }
         return *this;
      }

      template<typename Iterator>
      inline range_to_type_back_inserter_iterator& operator=(const std::pair<Iterator,Iterator>& r)
      {
         T t;
         if (string_to_type_converter(r.first,r.second,t))
            sequence_.push_back(t);
         return (*this);
      }

      template<typename Iterator>
      inline void operator()(const std::pair<Iterator,Iterator>& r)
      {
         T t;
         if (string_to_type_converter(r.first,r.second,t))
            sequence_.push_back(t);
      }

      inline range_to_type_back_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline range_to_type_back_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline range_to_type_back_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:
      Sequence& sequence_;
   };

   template <typename Sequence>
   inline range_to_type_back_inserter_iterator<Sequence> range_to_type_back_inserter(Sequence& sequence)
   {
      return (range_to_type_back_inserter_iterator<Sequence>(sequence));
   }

   template<class Sequence>
   class back_inserter_with_valuetype_iterator : public std::iterator<std::output_iterator_tag, typename Sequence::value_type, void,void, void>
   {
   public:

      explicit back_inserter_with_valuetype_iterator(Sequence& sequence)
      : sequence_(sequence)
      {}

      back_inserter_with_valuetype_iterator(const back_inserter_with_valuetype_iterator& it)
      : sequence_(it.sequence_)
      {}

      inline back_inserter_with_valuetype_iterator& operator=(const back_inserter_with_valuetype_iterator& it)
      {
         if (this != &it)
         {
            this->sequence_ = it.sequence_;
         }
         return *this;
      }

      inline back_inserter_with_valuetype_iterator& operator=(const typename Sequence::value_type& v)
      {
         sequence_.push_back(v);
         return (*this);
      }

      inline void operator()(const typename Sequence::value_type& v)
      {
         sequence_.push_back(v);
      }

      inline back_inserter_with_valuetype_iterator& operator*()
      {
         return (*this);
      }

      inline back_inserter_with_valuetype_iterator& operator++()
      {
         return (*this);
      }

      inline back_inserter_with_valuetype_iterator operator++(int)
      {
         return (*this);
      }

   private:
      Sequence& sequence_;
   };

   template<class Sequence>
   inline back_inserter_with_valuetype_iterator<Sequence> back_inserter_with_valuetype(Sequence& sequence_)
   {
      return (back_inserter_with_valuetype_iterator<Sequence>(sequence_));
   }

   template<typename T>
   class counting_back_inserter_iterator : public std::iterator<std::output_iterator_tag,T,void,void,void>
   {
   public:

      explicit counting_back_inserter_iterator(std::size_t& counter)
      : counter_(counter)
      {}

      counting_back_inserter_iterator(const counting_back_inserter_iterator& it)
      : counter_(it.counter_)
      {}

      inline counting_back_inserter_iterator& operator=(const counting_back_inserter_iterator& it)
      {
         if (this != &it)
         {
            this->counter_ = it.counter_;
         }
         return *this;
      }

      inline counting_back_inserter_iterator& operator=(const T& v)
      {
         ++counter_;
         return (*this);
      }

      inline void operator()(const T& v)
      {
         ++counter_;
      }

      inline counting_back_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline counting_back_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline counting_back_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:
      std::size_t& counter_;
   };

   template<typename T>
   inline counting_back_inserter_iterator<T> counting_back_inserter(std::size_t& counter_)
   {
      return (counting_back_inserter_iterator<T>(counter_));
   }

   template<typename Function>
   class functional_inserter_iterator : public std::iterator<std::output_iterator_tag,void,void,void,void>
   {
   public:
      functional_inserter_iterator(Function& function)
      : function_(function)
      {}

      functional_inserter_iterator(const functional_inserter_iterator& it)
      : function_(it.function_)
      {}

      inline functional_inserter_iterator& operator=(const functional_inserter_iterator& it)
      {
         if (this != &it)
         {
            this->function_ = it.function_;
         }
         return *this;
      }

      template<typename T>
      inline functional_inserter_iterator& operator=(const T& t)
      {
         function_(t);
         return (*this);
      }

      template<typename T>
      inline void operator()(const T& t)
      {
         function_(t);
      }

      inline functional_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline functional_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline functional_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Function& function_;
   };

   template<typename Function>
   inline functional_inserter_iterator<Function> functional_inserter(Function function)
   {
      return (functional_inserter_iterator<Function>(function));
   }

   namespace split_options
   {
      typedef std::size_t type;
      enum
      {
         default_mode        = 0,
         compress_delimiters = 1,
         include_delimiters  = 2
      };
   }

   template<typename DelimiterPredicate,
            typename Iterator,
            typename OutputIterator>
   inline std::size_t split(const DelimiterPredicate& delimiter,
                            const Iterator begin,
                            const Iterator end,
                            OutputIterator out,
                            const split_options::type split_option = split_options::default_mode)
   {
      if (0 == std::distance(begin,end)) return 0;
      std::size_t token_count = 0;
      std::pair<Iterator,Iterator> range(begin,begin);
      // range.first  -> prev
      // range.second -> it
      while (end != range.second)
      {
        if (delimiter(*range.second))
        {
           if (split_option & split_options::include_delimiters)
           {
              ++range.second;
              *(out++) = range;
              if (split_option & split_options::compress_delimiters)
                 while ((end != (++range.second)) && delimiter(*range.second));
           }
           else
           {
              *(out++) = range;
              if (split_option & split_options::compress_delimiters)
                 while ((end != (++range.second)) && delimiter(*range.second));
              else
                 ++range.second;
           }
           ++token_count;
           range.first = range.second;
        }
        else
           ++range.second;
      }
      if ((range.first != range.second) || delimiter(*(range.second - 1)))
      {
         *(out++) = range;
         ++token_count;
      }
      return token_count;
   }

   template<typename OutputIterator>
   inline std::size_t split(const char* delimiters,
                            const std::string& str,
                            OutputIterator out,
                            const split_options::type& split_option = split_options::default_mode)
   {
      return split(multiple_char_delimiter_predicate(delimiters),str.begin(),str.end(),out,split_option);
   }

   template<typename OutputIterator>
   inline std::size_t split(const std::string& delimiters,
                            const std::string& str,
                            OutputIterator out,
                            const split_options::type& split_option = split_options::default_mode)
   {
      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      str.begin(), str.end(),
                      out,
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      str.begin(), str.end(),
                      out,
                      split_option);
   }

   template<typename OutputIterator>
   inline std::size_t split(const std::string::value_type delimiter,
                            const std::string& str,
                            OutputIterator out,
                            const split_options::type& split_option = split_options::default_mode)
   {
      return split(single_delimiter_predicate<std::string::value_type>(delimiter),
                   str.begin(), str.end(),
                   out,
                   split_option);
   }

   template<typename DelimiterPredicate,
            typename OutputIterator>
   inline std::size_t split(const DelimiterPredicate& delimiter,
                            const std::string& str,
                            OutputIterator out,
                            const split_options::type& split_option = split_options::default_mode)
   {
      return split(delimiter,
                   str.begin(), str.end(),
                   out,
                   split_option);
   }

   template<typename DelimiterPredicate,
            typename Iterator,
            typename OutputIterator>
   inline std::size_t split_n(const DelimiterPredicate& delimiter,
                              const Iterator begin,
                              const Iterator end,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      if (0 == std::distance(begin,end)) return 0;
      std::size_t match_count = 0;
      std::pair<Iterator,Iterator> range(begin,begin);
      // range.first  -> prev
      // range.second -> it
      while (end != range.second)
      {
        if (delimiter(*range.second))
        {
           if (split_option & split_options::include_delimiters)
           {
              ++range.second;
              *(out++) = range;
              if (++match_count >= token_count)
                 return match_count;
              if (split_option & split_options::compress_delimiters)
                 while ((end != (++range.second)) && delimiter(*range.second));
           }
           else
           {
              *(out++) = range;
              if (++match_count >= token_count)
                 return match_count;
              if (split_option & split_options::compress_delimiters)
                 while ((end != (++range.second)) && delimiter(*range.second));
              else
                 ++range.second;
           }
           range.first = range.second;
        }
        else
           ++range.second;
      }
      if ((range.first != range.second) || delimiter(*(range.second - 1)))
      {
         *(out++) = range;
         ++match_count;
      }
      return match_count;
   }

   template<typename OutputIterator>
   inline std::size_t split_n(const char* delimiters,
                              const std::string& str,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      return split_n(multiple_char_delimiter_predicate(delimiters),
                     str.begin(), str.end(),
                     token_count,
                     out,
                     split_option);
   }

   template<typename OutputIterator>
   inline std::size_t split_n(const std::string& delimiters,
                              const std::string& str,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      if (1 == delimiters.size())
         return split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                        str.begin(), str.end(),
                        token_count,
                        out,
                        split_option);
      else
         return split_n(multiple_char_delimiter_predicate(delimiters),
                        str.begin(), str.end(),
                        token_count,
                        out,
                        split_option);
   }

   #define INSTANTIATE_SPLIT_N(ITER_TYPE)\
   template<typename OutputIterator>\
   inline std::size_t split_n(const std::string& delimiters,\
                              const ITER_TYPE begin,\
                              const ITER_TYPE end,\
                              const std::size_t& token_count,\
                              OutputIterator out,\
                              const split_options::type& split_option = split_options::default_mode)\
   {\
      if (1 == delimiters.size())\
         return split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),\
                        begin,end,\
                        token_count,\
                        out,\
                        split_option);\
      else\
         return split_n(multiple_char_delimiter_predicate(delimiters),\
                        begin,end,\
                        token_count,\
                        out,\
                        split_option);\
   }

   INSTANTIATE_SPLIT_N(char*)
   INSTANTIATE_SPLIT_N(unsigned char*)
   INSTANTIATE_SPLIT_N(std::string::iterator&)
   INSTANTIATE_SPLIT_N(std::string::const_iterator&)

   template<typename OutputIterator>
   inline std::size_t split_n(const std::string::value_type delimiter,
                              const std::string& str,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      return split_n(single_delimiter_predicate<std::string::value_type>(delimiter),
                     str.begin(),str.end(),
                     token_count,
                     out,
                     split_option);
   }

   template<typename DelimiterPredicate,
            typename OutputIterator>
   inline std::size_t split_n(const DelimiterPredicate& delimiter,
                              const std::string& str,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      return split_n(delimiter,
                     str.begin(),str.end(),
                     token_count,
                     out,
                     split_option);
   }

   #ifdef ENABLE_REGEX

   static const std::string uri_expression  ("((https?|ftp)\\://((\\[?(\\d{1,3}\\.){3}\\d{1,3}\\]?)|(([-a-zA-Z0-9]+\\.)+[a-zA-Z]{2,4}))(\\:\\d+)?(/[-a-zA-Z0-9._?,+&amp;%$#=~\\\\]+)*/?)");
   static const std::string email_expression("([\\w\\-\\.]+)@((\\[([0-9]{1,3}\\.){3}[0-9]{1,3}\\])|(([\\w\\-]+\\.)+)([a-zA-Z]{2,4}))");
   static const std::string ip_expression   ("(([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+))");

   template<typename InputIterator, typename OutputIterator>
   inline std::size_t split_regex(const boost::regex& delimiter_expression,
                                  const InputIterator begin,
                                  const InputIterator end,
                                  OutputIterator out)
   {
      boost::sregex_iterator it(begin,end,delimiter_expression);
      boost::sregex_iterator it_end;
      std::string token;
      token.reserve(one_kilobyte);
      std::size_t match_count = 0;
      while (it_end != it)
      {
         token.assign((*it)[0].first,(*it)[0].second);
         *(out++) = token;
         ++it;
         ++match_count;
      }
      return match_count;
   }

   template<typename InputIterator, typename OutputIterator>
   inline std::size_t split_regex(const std::string& delimiter_expression,
                                  const InputIterator begin,
                                  const InputIterator end,
                                  OutputIterator out)
   {
      const boost::regex regex_expression(delimiter_expression);
      return split_regex(regex_expression,begin,end,out);
   }

   template<typename OutputIterator>
   inline std::size_t split_regex(const std::string& delimiter_expression,
                                  const std::string& text,
                                  OutputIterator out)
   {
      return split_regex(delimiter_expression,text.begin(),text.end(),out);
   }

   template<typename OutputIterator>
   inline std::size_t split_regex(const boost::regex& delimiter_expression,
                                  const std::string& text,
                                  OutputIterator out)
   {
      return split_regex(delimiter_expression,text.begin(),text.end(),out);
   }

   template<typename InputIterator, typename OutputIterator>
   inline std::size_t split_regex_n(const boost::regex& delimiter_expression,
                                    const InputIterator begin,
                                    const InputIterator end,
                                    const std::size_t& token_count,
                                    OutputIterator out)
   {
      boost::sregex_iterator it(begin,end,delimiter_expression);
      boost::sregex_iterator it_end;
      std::string token;
      token.reserve(one_kilobyte);
      std::size_t match_count = 0;
      while (it_end != it)
      {
         token.assign((*it)[0].first,(*it)[0].second);
         *(out++) = token;
         ++it;
         if (++match_count >= token_count)
            return match_count;
      }
      return match_count;
   }

   template<typename InputIterator, typename OutputIterator>
   inline std::size_t split_regex_n(const std::string& delimiter_expression,
                                    const InputIterator begin,
                                    const InputIterator end,
                                    const std::size_t& token_count,
                                    OutputIterator out)
   {
      const boost::regex regex_expression(delimiter_expression);
      return split_regex_n(regex_expression,
                           begin,end,
                           token_count,
                           out);
   }

   template<typename OutputIterator>
   inline std::size_t split_regex_n(const std::string& delimiter_expression,
                                    const std::string& text,
                                    const std::size_t& token_count,
                                    OutputIterator out)
   {
      return split_regex_n(delimiter_expression,
                           text.begin(),text.end(),
                           token_count,
                           out);
   }

   template<typename OutputIterator>
   inline std::size_t split_regex_n(const boost::regex& delimiter_expression,
                                    const std::string& text,
                                    const std::size_t& token_count,
                                    OutputIterator out)
   {
      return split_regex_n(delimiter_expression,
                           text.begin(),text.end(),
                           token_count,
                           out);
   }

   #endif // ENABLE_REGEX

   template<const std::size_t offset_list_size>
   class offset_predicate
   {
   public:
      offset_predicate(const int offset_list[], const bool rotate = false)
      : rotate_(rotate),
        current_index_(0)
      {
         std::copy(offset_list, offset_list + offset_list_size, offset_list_);
         offset_list_[offset_list_size] = 0;
      }

      inline bool operator!() const
      {
         return (0 == offset_list_size);
      }

      inline void reset() const
      {
         current_index_ = 0;
      }

      inline std::size_t size() const
      {
         return offset_list_size;
      }

      inline int next() const
      {
         int result = offset_list_[current_index_++];
         if (rotate_ && (current_index_ >= offset_list_size)) current_index_ = 0;
         return result;
      }

   private:
      bool rotate_;
      mutable std::size_t current_index_;
      int offset_list_[offset_list_size + 1];
   };

   inline offset_predicate<12> offsets(const int&  v1, const int&  v2, const int&  v3,
                                       const int&  v4, const int&  v5, const int&  v6,
                                       const int&  v7, const int&  v8, const int&  v9,
                                       const int& v10, const int& v11, const int& v12,
                                       const bool& rotate = false)
   {
      const int offset_list[12] = { v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12 };
      return offset_predicate<12>(offset_list,rotate);
   }

   inline offset_predicate<11> offsets(const int&  v1, const int&  v2, const int& v3,
                                       const int&  v4, const int&  v5, const int& v6,
                                       const int&  v7, const int&  v8, const int& v9,
                                       const int& v10, const int& v11,
                                       const bool& rotate = false)
   {
      const int offset_list[11] = { v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 };
      return offset_predicate<11>(offset_list,rotate);
   }

   inline offset_predicate<10> offsets(const int& v1, const int& v2, const int& v3,
                                       const int& v4, const int& v5, const int& v6,
                                       const int& v7, const int& v8, const int& v9,
                                       const int& v10, const bool& rotate = false)
   {
      const int offset_list[10] = { v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 };
      return offset_predicate<10>(offset_list,rotate);
   }

   inline offset_predicate<9> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const int& v6,
                                      const int& v7, const int& v8, const int& v9,
                                      const bool& rotate = false)
   {
      const int offset_list[9] = { v1, v2, v3, v4, v5, v6, v7, v8, v9 };
      return offset_predicate<9>(offset_list,rotate);
   }

   inline offset_predicate<8> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const int& v6,
                                      const int& v7, const int& v8, const bool& rotate = false)
   {
      const int offset_list[8] = { v1, v2, v3, v4, v5, v6, v7, v8 };
      return offset_predicate<8>(offset_list,rotate);
   }

   inline offset_predicate<7> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const int& v6,
                                      const int& v7, const bool& rotate = false)
   {
      const int offset_list[7] = { v1, v2, v3, v4, v5, v6, v7 };
      return offset_predicate<7>(offset_list,rotate);
   }

   inline offset_predicate<6> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const int& v6,
                                      const bool& rotate = false)
   {
      const int offset_list[6] = { v1, v2, v3, v4, v5, v6 };
      return offset_predicate<6>(offset_list,rotate);
   }

   inline offset_predicate<5> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const bool& rotate = false)
   {
      const int offset_list[5] = { v1, v2, v3, v4, v5 };
      return offset_predicate<5>(offset_list,rotate);
   }

   inline offset_predicate<4> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const bool& rotate = false)
   {
      const int offset_list[4] = { v1, v2, v3, v4 };
      return offset_predicate<4>(offset_list,rotate);
   }

   inline offset_predicate<3> offsets(const int& v1, const int& v2, const int& v3,
                                      const bool& rotate = false)
   {
      const int offset_list[3] = { v1, v2, v3 };
      return offset_predicate<3>(offset_list,rotate);
   }

   inline offset_predicate<2> offsets(const int& v1, const int& v2, const bool& rotate = false)
   {
      const int offset_list[2] = { v1, v2 };
      return offset_predicate<2>(offset_list,rotate);
   }

   inline offset_predicate<1> offsets(const int& v1,
                                      const bool& rotate = false)
   {
      const int offset_list[1] = { v1 };
      return offset_predicate<1>(offset_list,rotate);
   }

   template<typename OffsetPredicate,
            typename InputIterator,
            typename OutputIterator>
   inline std::size_t offset_splitter(const InputIterator begin,
                                      const InputIterator end,
                                      const OffsetPredicate& offset,
                                      OutputIterator out)
   {
      std::size_t length = 0;
      if (0 == (length = std::distance(begin,end))) return 0;
      std::pair<InputIterator,InputIterator> range(begin,begin);
      //prev -> range.first
      //it -> range.second
      std::size_t match_count = 0;
      int offset_length = 0;
      std::size_t increment_amount = 0;
      while ((end != range.second) && (0 < (offset_length = offset.next())))
      {
         increment_amount = std::min<std::size_t>(length,offset_length);
         range.first = range.second;
         range.second += increment_amount;
         length -= increment_amount;
         *(out++) = range;
         ++match_count;
      }
      return match_count;
   }

   template<typename InputIterator, typename DelimiterPredicate>
   inline std::size_t count_tokens(const InputIterator begin,
                                   const InputIterator end,
                                   const DelimiterPredicate& delimiter,
                                   const split_options::type& split_option = split_options::default_mode)
   {
      if (0 == std::distance(begin,end)) return 0;
      std::size_t token_count = 0;
      std::pair<InputIterator,InputIterator> range(begin,begin);
      while (end != range.second)
      {
        if (delimiter(*range.second))
        {
           if (split_option & split_options::include_delimiters)
           {
              ++range.second;
              if (split_option & split_options::compress_delimiters)
                 while ((end != (++range.second)) && delimiter(*range.second));
           }
           else
           {
              if (split_option & split_options::compress_delimiters)
                 while ((end != (++range.second)) && delimiter(*range.second));
              else
                 ++range.second;
           }
           ++token_count;
           range.first = range.second;
        }
        else
           ++range.second;
      }
      if ((range.first != range.second) || delimiter(*(range.second - 1)))
      {
         ++token_count;
      }
      return token_count;
   }

   template<typename InputIterator>
   inline std::size_t count_consecutive_duplicates(const InputIterator begin, const InputIterator end)
   {
      if (std::distance(begin,end) < 2) return 0;
      InputIterator prev = begin;
      InputIterator it = begin;
      std::size_t count = 0;
      while (end != ++it)
      {
         if (*prev == *it)
            ++count;
         else
            prev = it;
      }
      return count;
   }

   template<typename InputIterator>
   inline typename std::iterator_traits<InputIterator>::value_type min_in_range(const InputIterator begin, const InputIterator end)
   {
      typename std::iterator_traits<InputIterator>::value_type smallest = *begin;
      InputIterator it = begin;
      while (end != ++it)
      {
         if (*it < smallest)
            smallest = *it;
      }
      return smallest;
   }

   template<typename InputIterator>
   inline typename std::iterator_traits<InputIterator>::value_type max_in_range(const InputIterator begin, const InputIterator end)
   {
      typename std::iterator_traits<InputIterator>::value_type greatest = *begin;
      InputIterator it = begin;
      while (end != ++it)
      {
         if (*it > greatest)
            greatest = *it;
      }
      return greatest;
   }

   template<typename Iterator>
   void lexicographically_canonicalize(Iterator begin, Iterator end)
   {
      typedef typename std::iterator_traits<Iterator>::value_type type;
      typedef typename std::pair<Iterator,Iterator> iter_type;
      typedef typename std::list<iter_type> itr_list_type;
      itr_list_type itr_list;

      type smallest = min_in_range(begin,end);

      for (Iterator it = begin; it != end; ++it)
      {
         if (*it == smallest) itr_list.push_back(std::make_pair(it,it));
      }

      while (itr_list.size() > 1)
      {
         typename itr_list_type::iterator it = itr_list.begin();
         while (itr_list.end() != it)
         {
            ++(*it).first;
            if (end == (*it).first)
               it = itr_list.erase(it);
            else
               ++it;
         }

         smallest = *(*itr_list.begin()).first;

         for (it = (++itr_list.begin()); it != itr_list.end(); ++it)
         {
            if (*(*it).first < smallest)
            {
               smallest = *(*it).first;
            }
         }

         it = itr_list.begin();
         while (itr_list.end() != it)
         {
           if (*(*it).first != smallest)
              it = itr_list.erase(it);
           else
              ++it;
         }

         it = itr_list.begin();
         while (itr_list.end() != it)
         {
            if (end == (*it).first)
               it = itr_list.erase(it);
            else
               ++it;
         }
      }
      std::rotate(begin,(*itr_list.begin()).second,end);
   }

   void lexicographically_canonicalize(std::string& str)
   {
      lexicographically_canonicalize(const_cast<char*>(str.c_str()), const_cast<char*>(str.c_str() + str.size()));
   }

   inline void convert_bin_to_hex(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      static const std::size_t symbol_count = 16;
      static const unsigned char hex_symbol[symbol_count] = {
                                                             '0','1','2','3','4','5','6','7',
                                                             '8','9','A','B','C','D','E','F'
                                                            };

      for (const unsigned char* it = begin; end != it; ++it)
      {
         *(out++) = hex_symbol[((*it) >> 0x04) & 0x0F];
         *(out++) = hex_symbol[ (*it)          & 0x0F];
      }
   }

   inline void convert_bin_to_hex(const char* begin, const char* end, char* out)
   {
      convert_bin_to_hex(reinterpret_cast<const unsigned char*>(begin),
                         reinterpret_cast<const unsigned char*>(end),
                         reinterpret_cast<unsigned char*>(out));
   }

   inline void convert_bin_to_hex(const std::string& binary_data, std::string& output)
   {
      output.resize(binary_data.size() * 2);
      convert_bin_to_hex(binary_data.c_str(),binary_data.c_str() + binary_data.size(),const_cast<char*>(output.c_str()));
   }

   inline bool convert_hex_to_bin(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      if (std::distance(begin,end) % 2)
         return false;
      static const std::size_t symbol_count = 256;
      static const unsigned char hex_to_bin[symbol_count] = {
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x00 - 0x07
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x08 - 0x0F
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x10 - 0x17
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x18 - 0x1F
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x20 - 0x27
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x28 - 0x2F
                                                              0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // 0x30 - 0x37
                                                              0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x38 - 0x3F
                                                              0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, // 0x40 - 0x47
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x48 - 0x4F
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x50 - 0x57
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x58 - 0x5F
                                                              0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, // 0x60 - 0x67
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x68 - 0x6F
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x70 - 0x77
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x78 - 0x7F
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x80 - 0x87
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x88 - 0x8F
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x90 - 0x97
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x98 - 0x9F
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xA0 - 0xA7
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xA8 - 0xAF
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xB0 - 0xB7
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xB8 - 0xBF
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xC0 - 0xC7
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xC8 - 0xCF
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xD0 - 0xD7
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xD8 - 0xDF
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xE0 - 0xE7
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xE8 - 0xEF
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xF0 - 0xF7
                                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 0xF8 - 0xFF
                                                            };

      const unsigned char* it = begin;
      while (end != it)
      {
         (*out)  = static_cast<unsigned char>(hex_to_bin[*(it++)] << 4);
         (*out) |= static_cast<unsigned char>(hex_to_bin[*(it++)]     );
         ++out;
      }
      return true;
   }

   inline bool convert_hex_to_bin(const char* begin, const char* end, char* out)
   {
      return convert_hex_to_bin(reinterpret_cast<const unsigned char*>(begin),
                                reinterpret_cast<const unsigned char*>(end),
                                reinterpret_cast<unsigned char*>(out));
   }

   inline bool convert_hex_to_bin(const std::string& hex_data, std::string& output)
   {
      if (hex_data.empty() || (hex_data.size() % 2))
         return false;
      output.resize(hex_data.size() >> 1);
      return convert_hex_to_bin(hex_data.c_str(),hex_data.c_str() + hex_data.size(),const_cast<char*>(output.c_str()));
   }

   inline std::size_t convert_bin_to_base64(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      static const std::size_t symbol_count = 64;
      static const unsigned char bin_to_base64 [symbol_count]
             = {
                'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                '0','1','2','3','4','5','6','7','8','9','+','/'
               };

      const std::size_t length = std::distance(begin,end);
      std::size_t rounds = length / 3;
      const unsigned char* it = begin;
      for (std::size_t i = 0; i < rounds; ++i)
      {
         unsigned int block  = *(it++) << 16;
                      block |= *(it++) <<  8;
                      block |= *(it++)      ;
         *(out++) = bin_to_base64[( block >> 18 ) & 0x3F];
         *(out++) = bin_to_base64[( block >> 12 ) & 0x3F];
         *(out++) = bin_to_base64[( block >>  6 ) & 0x3F];
         *(out++) = bin_to_base64[( block       ) & 0x3F];
      }

      if ((rounds = (length % 3)) > 0)
      {
         switch(rounds)
         {
            case 1 : {
                       unsigned int block  = (unsigned char) (*it) << 16;
                       *(out++) = bin_to_base64[( block >> 18 ) & 0x3F];
                       *(out++) = bin_to_base64[( block >> 12 ) & 0x3F];
                       *(out++) = '=';
                       *(out++) = '=';
                     }
                     break;

            case 2 : {
                        unsigned int block  = *(it++) << 16;
                                     block |= *(it++) <<  8;
                        *(out++) = bin_to_base64[( block >> 18 ) & 0x3F];
                        *(out++) = bin_to_base64[( block >> 12 ) & 0x3F];
                        *(out++) = bin_to_base64[( block >>  6 ) & 0x3F];
                        *(out++) = '=';
                     }
                     break;
         }
      }
      return static_cast<std::size_t>((length / 3) * 4) + ((length % 3) > 0 ? 4 : 0);
   }

   inline std::size_t convert_bin_to_base64(const char* begin, const char* end, char* out)
   {
      return convert_bin_to_base64(reinterpret_cast<const unsigned char*>(begin),
                                   reinterpret_cast<const unsigned char*>(end),
                                   reinterpret_cast<unsigned char*>(out));
   }

   inline void convert_bin_to_base64(const std::string& binary_data, std::string& output)
   {
      output.resize(std::max<std::size_t>(4,binary_data.size() << 1));
      std::size_t resize = convert_bin_to_base64(binary_data.c_str(),binary_data.c_str() + binary_data.size(),const_cast<char*>(output.c_str()));
      output.resize(resize);
   }

   inline std::size_t convert_base64_to_bin(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      static const std::size_t symbol_count = 256;
      static const unsigned char base64_to_bin[symbol_count] = {
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x00 - 0x07
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x08 - 0x0F
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x10 - 0x17
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x18 - 0x1F
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x20 - 0x27
                                                                0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F, // 0x28 - 0x2F
                                                                0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, // 0x30 - 0x37
                                                                0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x38 - 0x3F
                                                                0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // 0x40 - 0x47
                                                                0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, // 0x48 - 0x4F
                                                                0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, // 0x50 - 0x57
                                                                0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x58 - 0x5F
                                                                0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, // 0x60 - 0x67
                                                                0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, // 0x68 - 0x6F
                                                                0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, // 0x70 - 0x77
                                                                0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x78 - 0x7F
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x80 - 0x87
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x88 - 0x8F
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x90 - 0x97
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x98 - 0x9F
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xA0 - 0xA7
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xA8 - 0xAF
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xB0 - 0xB7
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xB8 - 0xBF
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xC0 - 0xC7
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xC8 - 0xCF
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xD0 - 0xD7
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xD8 - 0xDF
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xE0 - 0xE7
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xE8 - 0xEF
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xF0 - 0xF7
                                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  // 0xF8 - 0xFF
                                                              };

      const unsigned char* it = (end - 1);
      while (('=' == *it) && (begin != it)) --it;
      std::size_t length = std::distance(begin,(it + 1));
      std::size_t rounds = length / 4;
      it = begin;
      for (std::size_t i = 0; i < rounds; ++i)
      {
         unsigned int block  = base64_to_bin[*(it++)] << 18;
                      block |= base64_to_bin[*(it++)] << 12;
                      block |= base64_to_bin[*(it++)] <<  6;
                      block |= base64_to_bin[*(it++)];

         *(out++) = static_cast<unsigned char>(( block >> 16 ) & 0xFF);
         *(out++) = static_cast<unsigned char>(( block >>  8 ) & 0xFF);
         *(out++) = static_cast<unsigned char>(( block       ) & 0xFF);
      }

      if ((rounds = (length % 4)) > 0)
      {
         switch(rounds)
         {
            case 2 : {
                        unsigned int block  = base64_to_bin[*(it++)] << 18;
                                     block |= base64_to_bin[*(it++)] << 12;
                        *out = static_cast<unsigned char>(( block >> 16 ) & 0xFF);
                     }
                     break;

            case 3 : {
                        unsigned int block  = base64_to_bin[*(it++)] << 18;
                                     block |= base64_to_bin[*(it++)] << 12;
                                     block |= base64_to_bin[*(it++)] <<  6;
                        *(out++) = static_cast<unsigned char>(( block >> 16 ) & 0xFF);
                        *(out  ) = static_cast<unsigned char>(( block >>  8 ) & 0xFF);
                     }
                     break;
         }
      }
      return static_cast<std::size_t>((3 * length) / 4);
   }

   inline std::size_t convert_base64_to_bin(const char* begin, const char* end, char* out)
   {
      return convert_base64_to_bin(reinterpret_cast<const unsigned char*>(begin),
                                   reinterpret_cast<const unsigned char*>(end),
                                   reinterpret_cast<unsigned char*>(out));
   }

   inline void convert_base64_to_bin(const std::string& binary_data, std::string& output)
   {
      output.resize(binary_data.size());
      std::size_t resize = convert_base64_to_bin(binary_data.c_str(),binary_data.c_str() + binary_data.size(),const_cast<char*>(output.c_str()));
      output.resize(resize);
   }

   inline void convert_to_printable_chars(unsigned char* begin, unsigned char* end)
   {
      static const std::size_t table_size = 256;
      static const unsigned char printable_char_table[table_size] = {
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x00 - 0x07
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x08 - 0x0F
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x10 - 0x17
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x18 - 0x1F
                                                                       0x2E, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, // 0x20 - 0x27
                                                                       0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, // 0x28 - 0x2F
                                                                       0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, // 0x30 - 0x37
                                                                       0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, // 0x38 - 0x3F
                                                                       0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, // 0x40 - 0x47
                                                                       0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, // 0x48 - 0x4F
                                                                       0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, // 0x50 - 0x57
                                                                       0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, // 0x58 - 0x5F
                                                                       0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, // 0x60 - 0x67
                                                                       0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, // 0x68 - 0x6F
                                                                       0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, // 0x70 - 0x77
                                                                       0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x2E, // 0x78 - 0x7F
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x80 - 0x87
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x88 - 0x8F
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x90 - 0x97
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x98 - 0x9F
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xA0 - 0xA7
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xA8 - 0xAF
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xB0 - 0xB7
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xB8 - 0xBF
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xC0 - 0xC7
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xC8 - 0xCF
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xD0 - 0xD7
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xD8 - 0xDF
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xE0 - 0xE7
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xE8 - 0xEF
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xF0 - 0xF7
                                                                       0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E  // 0xF8 - 0xFF
                                                                    };
      unsigned char* it = begin;
      while (end != it)
      {
         (*it) = printable_char_table[static_cast<unsigned int>((*it))];
         ++it;
      }
   }

   void convert_to_printable_chars(std::string& str)
   {
      convert_to_printable_chars(reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str())),
                                 reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str() + str.size())));
   }

   void convert_to_uppercase(unsigned char* begin, unsigned char* end)
   {
      std::transform(begin,end,begin,::toupper);
      /*
      unsigned char* it = begin;
      while (end != it)
      {
         //(*it) = std::toupper((*it), std::locale::classic());
         (*it) = static_cast<unsigned char>(::toupper(static_cast<int>(*it)));
         ++it;
      }
      */
   }

   void convert_to_uppercase(std::string& str)
   {
      convert_to_uppercase(reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str())),
                           reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str() + str.size())));
   }

   void convert_to_lowercase(unsigned char* begin, unsigned char* end)
   {
      std::transform(begin,end,begin,::tolower);
      /*
      unsigned char* it = begin;
      while (end != it)
      {
         //(*it) = std::tolower((*it), std::locale::classic());
         (*it) = static_cast<unsigned char>(::tolower(static_cast<int>(*it)));
         ++it;
      }
      */
   }

   void convert_to_lowercase(std::string& str)
   {
      convert_to_lowercase(reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str())),
                           reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str() + str.size())));
   }

   inline bool twoway_bitwise_interleave(const unsigned char* begin1, const unsigned char* end1,
                                         const unsigned char* begin2, const unsigned char* end2,
                                         unsigned char* out)
   {
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return false;
      }

      static const std::size_t interleave_table_size = 256;
      static const unsigned short interleave_table[interleave_table_size] =
                                  {
                                    0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015, // 0x00 - 0x07
                                    0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055, // 0x08 - 0x0F
                                    0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115, // 0x10 - 0x17
                                    0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155, // 0x18 - 0x1F
                                    0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415, // 0x20 - 0x27
                                    0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455, // 0x28 - 0x2F
                                    0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515, // 0x30 - 0x37
                                    0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555, // 0x38 - 0x3F
                                    0x1000, 0x1001, 0x1004, 0x1005, 0x1010, 0x1011, 0x1014, 0x1015, // 0x40 - 0x47
                                    0x1040, 0x1041, 0x1044, 0x1045, 0x1050, 0x1051, 0x1054, 0x1055, // 0x48 - 0x4F
                                    0x1100, 0x1101, 0x1104, 0x1105, 0x1110, 0x1111, 0x1114, 0x1115, // 0x50 - 0x57
                                    0x1140, 0x1141, 0x1144, 0x1145, 0x1150, 0x1151, 0x1154, 0x1155, // 0x58 - 0x5F
                                    0x1400, 0x1401, 0x1404, 0x1405, 0x1410, 0x1411, 0x1414, 0x1415, // 0x60 - 0x67
                                    0x1440, 0x1441, 0x1444, 0x1445, 0x1450, 0x1451, 0x1454, 0x1455, // 0x68 - 0x6F
                                    0x1500, 0x1501, 0x1504, 0x1505, 0x1510, 0x1511, 0x1514, 0x1515, // 0x70 - 0x77
                                    0x1540, 0x1541, 0x1544, 0x1545, 0x1550, 0x1551, 0x1554, 0x1555, // 0x78 - 0x7F
                                    0x4000, 0x4001, 0x4004, 0x4005, 0x4010, 0x4011, 0x4014, 0x4015, // 0x80 - 0x87
                                    0x4040, 0x4041, 0x4044, 0x4045, 0x4050, 0x4051, 0x4054, 0x4055, // 0x88 - 0x8F
                                    0x4100, 0x4101, 0x4104, 0x4105, 0x4110, 0x4111, 0x4114, 0x4115, // 0x90 - 0x97
                                    0x4140, 0x4141, 0x4144, 0x4145, 0x4150, 0x4151, 0x4154, 0x4155, // 0x98 - 0x9F
                                    0x4400, 0x4401, 0x4404, 0x4405, 0x4410, 0x4411, 0x4414, 0x4415, // 0xA0 - 0xA7
                                    0x4440, 0x4441, 0x4444, 0x4445, 0x4450, 0x4451, 0x4454, 0x4455, // 0xA8 - 0xAF
                                    0x4500, 0x4501, 0x4504, 0x4505, 0x4510, 0x4511, 0x4514, 0x4515, // 0xB0 - 0xB7
                                    0x4540, 0x4541, 0x4544, 0x4545, 0x4550, 0x4551, 0x4554, 0x4555, // 0xB8 - 0xBF
                                    0x5000, 0x5001, 0x5004, 0x5005, 0x5010, 0x5011, 0x5014, 0x5015, // 0xC0 - 0xC7
                                    0x5040, 0x5041, 0x5044, 0x5045, 0x5050, 0x5051, 0x5054, 0x5055, // 0xC8 - 0xCF
                                    0x5100, 0x5101, 0x5104, 0x5105, 0x5110, 0x5111, 0x5114, 0x5115, // 0xD0 - 0xD7
                                    0x5140, 0x5141, 0x5144, 0x5145, 0x5150, 0x5151, 0x5154, 0x5155, // 0xD8 - 0xDF
                                    0x5400, 0x5401, 0x5404, 0x5405, 0x5410, 0x5411, 0x5414, 0x5415, // 0xE0 - 0xE7
                                    0x5440, 0x5441, 0x5444, 0x5445, 0x5450, 0x5451, 0x5454, 0x5455, // 0xE8 - 0xEF
                                    0x5500, 0x5501, 0x5504, 0x5505, 0x5510, 0x5511, 0x5514, 0x5515, // 0xF0 - 0xF7
                                    0x5540, 0x5541, 0x5544, 0x5545, 0x5550, 0x5551, 0x5554, 0x5555  // 0xF8 - 0xFF
                                  };

      const unsigned char* it1 = begin1;
      const unsigned char* it2 = begin2;
      while (end1 != it1)
      {
         *(reinterpret_cast<unsigned short*>(out))  = (interleave_table[*(it2++)] << 1);
         *(reinterpret_cast<unsigned short*>(out)) |=  interleave_table[*(it1++)];
         out += 2;
      }
      return true;
   }

   inline bool twoway_bitwise_interleave(const char* begin1, const char* end1,
                                         const char* begin2, const char* end2,
                                         char* out)
   {
      return twoway_bitwise_interleave(reinterpret_cast<const unsigned char*>(begin1),
                                       reinterpret_cast<const unsigned char*>(end1),
                                       reinterpret_cast<const unsigned char*>(begin2),
                                       reinterpret_cast<const unsigned char*>(end2),
                                       reinterpret_cast<unsigned char*>(out));
   }

   inline bool twoway_bitwise_interleave(const std::string& str1,
                                         const std::string& str2,
                                         std::string& out)
   {
      if (str1.size() != str2.size())
      {
         return false;
      }
      out.resize(str1.size());
      return twoway_bitwise_interleave(str1.c_str(),str1.c_str() + str1.size(),
                                       str2.c_str(),str2.c_str() + str2.size(),
                                       const_cast<char*>(out.c_str()));
   }

   template<std::size_t n>
   struct interleave_ary;

   template<> struct interleave_ary<sizeof(unsigned short)> { typedef unsigned short type; };
   template<> struct interleave_ary<sizeof(unsigned int  )> { typedef unsigned int   type; };
   //template<> struct interleave_ary<sizeof(unsigned long )> { typedef unsigned long  type; };
   //template<> struct interleave_ary<8> { typedef long long    type; }; Note: atm not supported by ISO C++ (N1811)

   template<std::size_t n>
   inline void create_nway_interleave_table(typename interleave_ary<n>::type table[256])
   {
      typedef typename interleave_ary<n>::type type;
      const type diff = static_cast<type>(n - 1);
      for (type i = static_cast<type>(0); i < static_cast<type>(256); ++i)
      {
         table[i] = 0x00;
         for (type j = static_cast<type>(0); j < static_cast<type>(8); ++j)
         {
            table[i] |= (i & (1 << j)) << (j * diff);
         }
      }
   }

   namespace bitwise_operation { enum type { eAND, eOR, eXOR }; }

   inline void bitwise_transform(const bitwise_operation::type& operation,
                                 const unsigned char* begin1, const unsigned char* end1,
                                 const unsigned char* begin2,
                                 unsigned char* out)
   {
      const unsigned char* it1 = begin1;
      const unsigned char* it2 = begin2;

      switch(operation)
      {
         case bitwise_operation::eAND : while (it1 != end1) { *(out++) = *(it1++) & *(it2++); } return;
         case bitwise_operation::eOR  : while (it1 != end1) { *(out++) = *(it1++) | *(it2++); } return;
         case bitwise_operation::eXOR : while (it1 != end1) { *(out++) = *(it1++) ^ *(it2++); } return;
      }
   }

   inline void bitwise_transform(const bitwise_operation::type& operation,
                                 const char* begin1, const char* end1,
                                 const char* begin2,
                                 char* out)
   {
      bitwise_transform(operation,
                        reinterpret_cast<const unsigned char*>(begin1),
                        reinterpret_cast<const unsigned char*>(end1),
                        reinterpret_cast<const unsigned char*>(begin2),
                        reinterpret_cast<unsigned char*>(out));
   }

   inline void bitwise_transform(const bitwise_operation::type& operation,
                                 const std::string& str1,
                                 const std::string& str2,
                                 std::string& out)
   {
      if (str1.size() != str2.size()) return;
      out.resize(str1.size());
      bitwise_transform(operation,
                        str1.c_str(),str1.c_str() + str1.size(),
                        str2.c_str(),
                        const_cast<char*>(out.c_str()));
   }

   inline std::size_t high_bit_count(const unsigned char c)
   {
      static const std::size_t high_bits_in_char[256] = {
                                                          0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
                                                          1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                                                          1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                                                          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                                          1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                                                          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                                          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                                          3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                                                          1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                                                          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                                          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                                          3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                                                          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                                          3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                                                          3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                                                          4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
                                                        };
      return high_bits_in_char[c];
   }

   inline std::size_t high_bit_count(const unsigned short& s)
   {
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&s);
      return high_bit_count(*(ptr + 0)) + high_bit_count(*(ptr + 1));
   }

   inline std::size_t high_bit_count(const unsigned int& i)
   {
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&i);
      return high_bit_count(*(ptr + 0)) + high_bit_count(*(ptr + 1)) +
             high_bit_count(*(ptr + 2)) + high_bit_count(*(ptr + 3));
   }

   /*
   inline std::size_t high_bit_count(const long long& ll)
   {
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&ll);
      return high_bit_count(*(ptr + 0)) + high_bit_count(*(ptr + 1)) +
             high_bit_count(*(ptr + 2)) + high_bit_count(*(ptr + 3)) +
             high_bit_count(*(ptr + 4)) + high_bit_count(*(ptr + 5)) +
             high_bit_count(*(ptr + 6)) + high_bit_count(*(ptr + 7));
   }
   */

   inline std::size_t high_bit_count(const unsigned char* begin, const unsigned char* end)
   {
      std::size_t count = 0;
      const unsigned char* it1 = begin;
      while (end != it1)
      {
         count += high_bit_count(*it1++);
      }
      return count;
   }

   inline std::size_t high_bit_count(const char* begin, const char* end)
   {
      return high_bit_count(reinterpret_cast<const unsigned char*>(begin),
                            reinterpret_cast<const unsigned char*>(end));

   }

   inline std::size_t high_bit_count(const std::string& str)
   {
      return high_bit_count(str.c_str(),str.c_str() + str.size());
   }

   inline bool bit_state(const std::size_t& index, const unsigned char* ptr)
   {
      static const unsigned char bit_mask[] = {
                                                0x01, //00000001
                                                0x02, //00000010
                                                0x04, //00000100
                                                0x08, //00001000
                                                0x10, //00010000
                                                0x20, //00100000
                                                0x40, //01000000
                                                0x80  //10000000
                                              };
      return (0 != (ptr[(index >> 3)] & bit_mask[index & 7]));
   }

   inline void set_bit_high(const std::size_t& index, unsigned char* const ptr)
   {
      static const unsigned char bit_mask[] = {
                                                0x01, //00000001
                                                0x02, //00000010
                                                0x04, //00000100
                                                0x08, //00001000
                                                0x10, //00010000
                                                0x20, //00100000
                                                0x40, //01000000
                                                0x80  //10000000
                                              };
      ptr[(index >> 3)] |= bit_mask[index & 7];
   }

   inline void set_bit_low(const std::size_t& index, unsigned char* const ptr)
   {
      static const unsigned char bit_mask[] = {
                                                0xFE, //11111110
                                                0xFD, //11111101
                                                0xFB, //11111011
                                                0xF7, //11110111
                                                0xEF, //11101111
                                                0xDF, //11011111
                                                0xBF, //10111111
                                                0x7F  //01111111
                                              };
      ptr[(index >> 3)] &= bit_mask[index & 7];
   }

   inline std::size_t hamming_distance(const unsigned char* begin1, const unsigned char* end1,
                                       const unsigned char* begin2, const unsigned char* end2)
   {
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return std::numeric_limits<std::size_t>::max();
      }
      std::size_t distance = 0;
      const unsigned char* it1 = begin1;
      const unsigned char* it2 = begin2;
      while (end1 != it1)
      {
         distance += high_bit_count(static_cast<unsigned char>(((*it1++) ^ (*it2++)) & 0xFF));
      }
      return distance;
   }

   inline std::size_t hamming_distance(const char* begin1, const char* end1,
                                       const char* begin2, const char* end2)
   {
      return hamming_distance(reinterpret_cast<const unsigned char*>(begin1),
                              reinterpret_cast<const unsigned char*>(end1),
                              reinterpret_cast<const unsigned char*>(begin2),
                              reinterpret_cast<const unsigned char*>(end2));
   }

   inline std::size_t hamming_distance(const std::string& str1,const std::string& str2)
   {
      return hamming_distance(str1.c_str(),str1.c_str() + str1.size(),
                              str2.c_str(),str2.c_str() + str2.size());
   }

   namespace details
   {

      inline void compute_pod_hash(const char data[], unsigned int& hash)
      {
         hash ^=  ((hash <<  7) ^  data[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (data[1] ^ (hash >> 5)));
      }

      inline void compute_pod_hash(const unsigned char data[], unsigned int& hash)
      {
         hash ^=  ((hash <<  7) ^  data[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (data[1] ^ (hash >> 5)));
      }

      inline void compute_pod_hash(const int& data, unsigned int& hash)
      {
         const unsigned char* it = reinterpret_cast<const unsigned char*>(&data);
         hash ^=  ((hash <<  7) ^  it[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[1] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  it[2] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[3] ^ (hash >> 5)));
      }

      inline void compute_pod_hash(const unsigned int& data, unsigned int& hash)
      {
         compute_pod_hash(static_cast<int>(data),hash);
      }

      inline void compute_pod_hash(const double& data, unsigned int& hash)
      {
         const unsigned char* it = reinterpret_cast<const unsigned char*>(&data);
         hash ^=  ((hash <<  7) ^  it[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[1] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  it[2] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[3] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  it[4] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[5] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  it[6] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[7] ^ (hash >> 5)));
      }

      /*
      inline void compute_pod_hash(const long long& data, unsigned int& hash)
      {
         const unsigned char* it = reinterpret_cast<const unsigned char*>(&data);
         hash ^=  ((hash <<  7) ^  it[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[1] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  it[2] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[3] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  it[4] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[5] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  it[6] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[7] ^ (hash >> 5)));
      }
      */

      template<std::size_t block_size, typename Iterator>
      inline void compute_block(Iterator itr, std::size_t& length, unsigned int& hash)
      {
         while (length >= block_size)
         {
            for (std::size_t i = 0; i < block_size; ++i, ++itr)
            {
               compute_pod_hash((*itr),hash);
            }
            length -= block_size;
         }
      }

      template<std::size_t block_size>
      inline void compute_block(unsigned char* itr, std::size_t& length, unsigned int& hash)
      {
         while (length >= block_size)
         {
            for (std::size_t i = 0; i < block_size; ++i, ++itr)
            {
               compute_pod_hash((*itr),hash);
            }
            length -= block_size;
         }
      }

      template<std::size_t block_size>
      inline void compute_block(char* itr, std::size_t& length, unsigned int& hash)
      {
         compute_block<block_size>(reinterpret_cast<unsigned char*>(itr),length,hash);
      }

   } // namespace details

   template<typename Iterator>
   inline void hash(const Iterator itr, std::size_t length, unsigned int& hash_value)
   {
      hash_value = 0xAAAAAAAA;
      if (length >= 64) details::compute_block<64>(itr,length,hash_value);
      if (length >= 32) details::compute_block<32>(itr,length,hash_value);
      if (length >= 16) details::compute_block<16>(itr,length,hash_value);
      if (length >=  8) details::compute_block< 8>(itr,length,hash_value);
      if (length >=  4) details::compute_block< 4>(itr,length,hash_value);
      if (length >=  2) details::compute_block< 2>(itr,length,hash_value);
      if (length >   0) details::compute_block< 1>(itr,length,hash_value);
   }

   template<typename Iterator>
   inline unsigned int hash(const Iterator itr, std::size_t length)
   {
      unsigned int hash_value = 0;
      hash(itr,length,hash_value);
      return hash_value;
   }

   inline unsigned int hash(const std::string& s)
   {
      unsigned int hash_value;
      hash(s.begin(),s.size(),hash_value);
      return hash_value;
   }

   class token_grid
   {
   public:
      typedef std::pair<unsigned char*, unsigned char*> range_type;
      typedef std::deque<range_type> itr_list_type;
      typedef std::deque<itr_list_type> itr_list_list_type;
      typedef std::pair<std::size_t,std::size_t> row_range_type;

      class row_type
      {
      private:
         typedef std::pair<bool,row_type*> row_pair_type;

      public:

         row_type(const itr_list_type& token_list)
         : index_(std::numeric_limits<std::size_t>::max()),
           row_list_(0),
           token_list_(&token_list),
           next_row_(false,0),
           prev_row_(false,0)
         {}

         row_type(const std::size_t& index,
                  const itr_list_list_type& row_list)
         : index_(index),
           row_list_(&row_list),
           token_list_(&(*row_list_)[index_]),
           next_row_(index < (row_list.size() - 1),0),
           prev_row_(index > 0,0)
         {}

        ~row_type()
         {
           if (next_row_.second) { delete next_row_.second; }
           if (prev_row_.second) { delete prev_row_.second; }
         }

         template<typename T>
         inline T operator[](const std::size_t& index) const
         {
            itr_list_type::value_type curr_range = (*token_list_)[index];
            return string_to_type_converter<T>(curr_range.first,curr_range.second);
         }

         inline const row_type& next_row() const
         {
            if (next_row_.first && (0 == next_row_.second))
               next_row_.second = new row_type(index_ + 1,*row_list_);
            return *next_row_.second;
         }

         inline const row_type& prev_row() const
         {
            if (prev_row_.first && (0 == prev_row_.second))
               prev_row_.second = new row_type(index_ - 1,*row_list_);
            return *prev_row_.second;
         }

         template<typename T>
         inline T get(const std::size_t& index) const
         {
            return row_type::operator[]<T>(index);
         }

         inline range_type token(const std::size_t& index) const
         {
            return (*token_list_)[index];
         }

         inline std::size_t index() const
         {
            return index_;
         }

         inline std::size_t size() const
         {
            return token_list_->size();
         }

         inline std::size_t raw_length() const
         {
            return std::distance(token_list_->begin()->first,token_list_->back().second);
         }

         inline std::string as_string() const
         {
            return std::string(token_list_->begin()->first,token_list_->back().second);
         }

         inline void as_string(std::string& out) const
         {
            out.assign(token_list_->begin()->first,token_list_->back().second);
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8,
                  typename T9, typename T10 >
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      const std::size_t& col5, const std::size_t& col6,
                                      const std::size_t& col7, const std::size_t& col8,
                                      const std::size_t& col9, const std::size_t& col10,
                                      T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                                      T7& t7, T8& t8, T9& t9, T10& t10) const
         {
            if (!process((*token_list_)[ col1], t1)) return false;
            if (!process((*token_list_)[ col2], t2)) return false;
            if (!process((*token_list_)[ col3], t3)) return false;
            if (!process((*token_list_)[ col4], t4)) return false;
            if (!process((*token_list_)[ col5], t5)) return false;
            if (!process((*token_list_)[ col6], t6)) return false;
            if (!process((*token_list_)[ col7], t7)) return false;
            if (!process((*token_list_)[ col8], t8)) return false;
            if (!process((*token_list_)[ col9], t9)) return false;
            if (!process((*token_list_)[col10],t10)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8,
                  typename T9 >
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      const std::size_t& col5, const std::size_t& col6,
                                      const std::size_t& col7, const std::size_t& col8,
                                      const std::size_t& col9,
                                      T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                                      T7& t7, T8& t8, T9& t9) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            if (!process((*token_list_)[col4],t4)) return false;
            if (!process((*token_list_)[col5],t5)) return false;
            if (!process((*token_list_)[col6],t6)) return false;
            if (!process((*token_list_)[col7],t7)) return false;
            if (!process((*token_list_)[col8],t8)) return false;
            if (!process((*token_list_)[col9],t9)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      const std::size_t& col5, const std::size_t& col6,
                                      const std::size_t& col7, const std::size_t& col8,
                                      T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                                      T7& t7, T8& t8) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            if (!process((*token_list_)[col4],t4)) return false;
            if (!process((*token_list_)[col5],t5)) return false;
            if (!process((*token_list_)[col6],t6)) return false;
            if (!process((*token_list_)[col7],t7)) return false;
            if (!process((*token_list_)[col8],t8)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6, typename T7>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      const std::size_t& col5, const std::size_t& col6,
                                      const std::size_t& col7,
                                      T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                                      T7& t7) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            if (!process((*token_list_)[col4],t4)) return false;
            if (!process((*token_list_)[col5],t5)) return false;
            if (!process((*token_list_)[col6],t6)) return false;
            if (!process((*token_list_)[col7],t7)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      const std::size_t& col5, const std::size_t& col6,
                                      T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            if (!process((*token_list_)[col4],t4)) return false;
            if (!process((*token_list_)[col5],t5)) return false;
            if (!process((*token_list_)[col6],t6)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,typename T5>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      const std::size_t& col5,
                                      T1& t1, T2& t2, T3& t3, T4& t4, T5& t5) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            if (!process((*token_list_)[col4],t4)) return false;
            if (!process((*token_list_)[col5],t5)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      T1& t1, T2& t2, T3& t3, T4& t4) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            if (!process((*token_list_)[col4],t4)) return false;
            return true;
         }

         template<typename T1, typename T2, typename T3>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3,
                                      T1& t1, T2& t2, T3& t3) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            return true;
         }

         template<typename T1, typename T2>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      T1& t1, T2& t2) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            return true;
         }

         template<typename T1>
         inline bool parse_with_index(const std::size_t& col, T1& t) const
         {
            return process((*token_list_)[col],t);
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8,
                  typename T9, typename T10 >
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6, T7& t7, T8& t8,
                           T9& t9, T10& t10) const
         {
            if (!process((*token_list_)[0], t1)) return false;
            if (!process((*token_list_)[1], t2)) return false;
            if (!process((*token_list_)[2], t3)) return false;
            if (!process((*token_list_)[3], t4)) return false;
            if (!process((*token_list_)[4], t5)) return false;
            if (!process((*token_list_)[5], t6)) return false;
            if (!process((*token_list_)[6], t7)) return false;
            if (!process((*token_list_)[7], t8)) return false;
            if (!process((*token_list_)[8], t9)) return false;
            if (!process((*token_list_)[9],t10)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8,
                  typename T9 >
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6, T7& t7, T8& t8,
                           T9& t9) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            if (!process((*token_list_)[3],t4)) return false;
            if (!process((*token_list_)[4],t5)) return false;
            if (!process((*token_list_)[5],t6)) return false;
            if (!process((*token_list_)[6],t7)) return false;
            if (!process((*token_list_)[7],t8)) return false;
            if (!process((*token_list_)[8],t9)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8>
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6, T7& t7, T8& t8) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            if (!process((*token_list_)[3],t4)) return false;
            if (!process((*token_list_)[4],t5)) return false;
            if (!process((*token_list_)[5],t6)) return false;
            if (!process((*token_list_)[6],t7)) return false;
            if (!process((*token_list_)[7],t8)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6, typename T7>
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6, T7& t7) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            if (!process((*token_list_)[3],t4)) return false;
            if (!process((*token_list_)[4],t5)) return false;
            if (!process((*token_list_)[5],t6)) return false;
            if (!process((*token_list_)[6],t7)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6>
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            if (!process((*token_list_)[3],t4)) return false;
            if (!process((*token_list_)[4],t5)) return false;
            if (!process((*token_list_)[5],t6)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,typename T5>
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4, T5& t5) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            if (!process((*token_list_)[3],t4)) return false;
            if (!process((*token_list_)[4],t5)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4>
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            if (!process((*token_list_)[3],t4)) return false;
            return true;
         }

         template<typename T1, typename T2, typename T3>
         inline bool parse(T1& t1, T2& t2, T3& t3) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            return true;
         }

         template<typename T1, typename T2>
         inline bool parse(T1& t1, T2& t2) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            return true;
         }

         template<typename T1>
         inline bool parse(T1& t) const
         {
            return process((*token_list_)[0],t);
         }

         template<typename T, typename OutputIterator>
         inline void parse(OutputIterator out) const
         {
            itr_list_type::iterator it = const_cast<itr_list_type*>(token_list_)->begin();
            while (token_list_->end() != it)
            {
               itr_list_type::value_type& range = *it;
               *(out++) = string_to_type_converter<T>(range.first,range.second);
               ++it;
            }
         }

      private:

         template<typename T>
         inline bool process(const itr_list_type::value_type& range, T& t) const
         {
            return string_to_type_converter<T>(range.first,range.second,t);
         }

      private:

         row_type& operator=(const row_type&);

         const std::size_t index_;
         const itr_list_list_type* row_list_;
         const itr_list_type* token_list_;
         mutable row_pair_type next_row_;
         mutable row_pair_type prev_row_;
      };

      token_grid(const std::string& file_name,
            const std::string& column_delimiters = ",|;\t",
            const std::string& row_delimiters = "\n\r")
      : file_name_(file_name),
        column_delimiters_(column_delimiters),
        row_delimiters_(row_delimiters),
        buffer_(0),
        buffer_size_(0),
        min_column_count_(0),
        max_column_count_(0),
        load_from_file_(true),
        state_(load())
      {}

      token_grid(const unsigned char* input_buffer,
                 const std::size_t& input_buffer_size,
                 const std::string& column_delimiters = ",|;\t",
                 const std::string& row_delimiters = "\n\r")
      : file_name_(""),
        column_delimiters_(column_delimiters),
        row_delimiters_(row_delimiters),
        buffer_(const_cast<unsigned char*>(input_buffer)),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const char* input_buffer,
                 const std::size_t& input_buffer_size,
                 const std::string& column_delimiters = ",|;\t",
                 const std::string& row_delimiters = "\n\r")
      : file_name_(""),
        column_delimiters_(column_delimiters),
        row_delimiters_(row_delimiters),
        buffer_(reinterpret_cast<unsigned char*>(const_cast<char*>(input_buffer))),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const std::string& input_buffer,
                 const std::size_t& input_buffer_size,
                 const std::string& column_delimiters = ",;|\t",
                 const std::string& row_delimiters = "\n\r")
      : file_name_(""),
        column_delimiters_(column_delimiters),
        row_delimiters_(row_delimiters),
        buffer_(reinterpret_cast<unsigned char*>(const_cast<char*>(input_buffer.c_str()))),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        load_from_file_(false),
        state_(load())
      {}

     ~token_grid()
      {
         if ((load_from_file_) && (0 != buffer_))
         {
            delete [] buffer_;
         }
      }

      inline bool operator!()               const { return !state_;            }
      inline std::string source_file()      const { return file_name_;         }
      inline std::size_t row_count()        const { return token_list_.size(); }
      inline std::size_t min_column_count() const { return min_column_count_;  }
      inline std::size_t max_column_count() const { return max_column_count_;  }

      inline range_type token(const unsigned int& row, const std::size_t& col)
      {
         return token_list_[row][col];
      }

      template<typename T>
      inline T get(const unsigned int& row, const std::size_t& col)
      {
         range_type r = token(row,col);
         return string_to_type_converter<T>(r.first,r.second);
      }

      inline row_type row(const unsigned int& row_index) const
      {
         return row_type(row_index,token_list_);
      }

      inline row_range_type all_rows() const
      {
         return row_range_type(0,token_list_.size());
      }

      template<typename OutputIterator>
      inline void extract_column(const std::size_t& index, OutputIterator out)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         while (token_list_.end() != it)
         {
            process_column((*it++)[index],out);
         }
      }

      template<typename OutputIterator1, typename OutputIterator2>
      inline void extract_column(const std::size_t& index1,
                                 const std::size_t& index2,
                                 OutputIterator1 out1,
                                 OutputIterator2 out2)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         while (token_list_.end() != it)
         {
            process_column((*it)[index1],out1);
            process_column((*it)[index2],out2);
            ++it;
         }
      }

      template<typename OutputIterator1, typename OutputIterator2, typename OutputIterator3>
      inline void extract_column(const std::size_t& index1,
                                 const std::size_t& index2,
                                 const std::size_t& index3,
                                 OutputIterator1 out1,
                                 OutputIterator2 out2,
                                 OutputIterator3 out3)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         while (token_list_.end() != it)
         {
            process_column((*it)[index1],out1);
            process_column((*it)[index2],out2);
            process_column((*it)[index3],out3);
            ++it;
         }
      }

      template<typename OutputIterator1, typename OutputIterator2,
               typename OutputIterator3, typename OutputIterator4>
      inline void extract_column(const std::size_t& index1,
                                 const std::size_t& index2,
                                 const std::size_t& index3,
                                 const std::size_t& index4,
                                 OutputIterator1 out1,
                                 OutputIterator2 out2,
                                 OutputIterator3 out3,
                                 OutputIterator4 out4)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         while (token_list_.end() != it)
         {
            process_column((*it)[index1],out1);
            process_column((*it)[index2],out2);
            process_column((*it)[index3],out3);
            process_column((*it)[index4],out4);
            ++it;
         }
      }

      template<typename OutputIterator1, typename OutputIterator2,
               typename OutputIterator3, typename OutputIterator4,
               typename OutputIterator5>
      inline void extract_column(const std::size_t& index1,
                                 const std::size_t& index2,
                                 const std::size_t& index3,
                                 const std::size_t& index4,
                                 const std::size_t& index5,
                                 OutputIterator1 out1,
                                 OutputIterator2 out2,
                                 OutputIterator3 out3,
                                 OutputIterator4 out4,
                                 OutputIterator5 out5)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         while (token_list_.end() != it)
         {
            process_column((*it)[index1],out1);
            process_column((*it)[index2],out2);
            process_column((*it)[index3],out3);
            process_column((*it)[index4],out4);
            process_column((*it)[index5],out5);
            ++it;
         }
      }

      inline void remove_row(const std::size_t& index)
      {
         if (index < token_list_.size()) token_list_.erase(token_list_.begin() + index);
      }

      template<typename Predicate>
      inline void remove_row_if(Predicate p)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         while (token_list_.end() != it)
         {
            if (!it->empty() && p(it->front().first,it->back().second))
            {
               it = token_list_.erase(it);
            }
            else
              ++it;
         }
      }

      inline void enforce_column_count(const std::size_t& column_count)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         itr_list_list_type new_token_list;
         while (token_list_.end() != it)
         {
            if (it->size() == column_count)
            {
               new_token_list.push_back(*it);
            }
            ++it;
         }
         token_list_.swap(new_token_list);
         min_column_count_ = column_count;
         max_column_count_ = column_count;
      }

      inline void enforce_min_max_column_count(const std::size_t& min_column_count,
                                               const std::size_t& max_column_count)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         itr_list_list_type new_token_list;
         while (token_list_.end() != it)
         {
            std::size_t column_count = it->size();
            if ((min_column_count <= column_count) && (column_count <= max_column_count))
            {
               new_token_list.push_back(*it);
            }
            ++it;
         }
         token_list_.swap(new_token_list);
         min_column_count_ = min_column_count;
         max_column_count_ = max_column_count;
      }

      inline void clear()
      {
         delete[] buffer_;
         buffer_ = 0;
         buffer_size_ = 0;
         token_list_.clear();
         min_column_count_ = 0;
         max_column_count_ = 0;
         state_ = false;
         file_name_ = "";
      }

      template<typename T>
      inline bool accumulate_row(const std::size_t& row, T& result) const
      {
         if (row >= token_list_.size())
            return false;
         itr_list_type::const_iterator itr = token_list_[row].begin();
         itr_list_type::const_iterator end = token_list_[row].end();
         T current_value = T();
         while (end != itr)
         {
            if (string_to_type_converter<T>((*itr).first,(*itr).second,current_value))
              result += current_value;
            else
               return false;
            ++itr;
         }
         return true;
      }

      template<typename T>
      inline bool accumulate_column(const std::size_t& col,
                                    T& result,
                                    const row_range_type& range) const
      {
         if (col > max_column_count_)
            return false;
         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return false;
         if (range.first > range.second)
            return false;
         itr_list_list_type::const_iterator it = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;
         itr_list_list_type new_token_list;
         T current_value = T();
         while (end != it)
         {
            if (!(*it).empty())
            {
               if (string_to_type_converter<T> ((*it)[col].first, (*it)[col].second, current_value))
                  result += current_value;
               else
                  return false;
            }
            ++it;
         }
         return true;
      }

      template<typename T>
      inline bool accumulate_column(const std::size_t& col, T& result) const
      {
         return accumulate_column(col,result,all_rows());
      }

      template<typename T, typename Predicate>
      inline bool accumulate_column(const std::size_t& col,
                                    Predicate p,
                                    T& result,
                                    const row_range_type& range) const
      {
         if (col > max_column_count_)
            return false;
         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return false;
         if (range.first > range.second)
            return false;
         itr_list_list_type::const_iterator it = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;
         itr_list_list_type new_token_list;
         T current_value = T();
         while (end != it)
         {
            if (!(*it).empty() && p(row_type(*it)))
            {
               if (string_to_type_converter<T> ((*it)[col].first, (*it)[col].second, current_value))
                  result += current_value;
               else
                  return false;
            }
            ++it;
         }
         return true;
      }

      template<typename T, typename Predicate>
      inline bool accumulate_column(const std::size_t& col, Predicate p, T& result) const
      {
         return accumulate_column(col,p,result,all_rows());
      }

      inline bool join_row(const std::size_t& row, const std::string& delimiter, std::string& result)
      {
         if (row >= token_list_.size())
            return false;
         itr_list_type::const_iterator itr = token_list_[row].begin();
         itr_list_type::const_iterator end = token_list_[row].end();
         result.reserve(std::distance(token_list_[row].front().first,
                                      token_list_[row].back().second) +
                        (delimiter.size() * token_list_[row].size()) +
                        result.size());
         bool appended = false;
         while (end != itr)
         {
            if (!delimiter.empty() && appended)
               result.append(delimiter);
            appended = false;
            if ((*itr).first != (*itr).second)
            {
               result.append((*itr).first,(*itr).second);
               appended = true;
            }
            ++itr;
         }
         return true;
      }

      inline bool join_column(const std::size_t& col, const std::string& delimiter, std::string& result)
      {
         if (col > max_column_count_)
            return false;
         itr_list_list_type::const_iterator it = token_list_.begin();
         itr_list_list_type new_token_list;
         range_type range;
         bool appended = false;
         while (token_list_.end() != it)
         {
            if (!delimiter.empty() && appended)
               result.append(delimiter);
            appended = false;
            if (!(*it).empty())
            {
               range = (*it)[col];
               if (range.first != range.second)
               {
                  result.append(range.first,range.second);
                  appended = true;
               }
            }
            ++it;
         }
         return true;
      }

      template<typename Predicate>
      inline bool join_column(const std::size_t& col, Predicate p, const std::string& delimiter, std::string& result)
      {
         if (col > max_column_count_)
            return false;
         itr_list_list_type::const_iterator it = token_list_.begin();
         itr_list_list_type new_token_list;
         range_type range;
         bool appended = false;
         while (token_list_.end() != it)
         {
            if (!delimiter.empty() && appended)
               result.append(delimiter);
            appended = false;
            if (!(*it).empty() && p(row_type(*it)))
            {
               range = (*it)[col];
               if (range.first != range.second)
               {
                  result.append(range.first,range.second);
                  appended = true;
               }
            }
            ++it;
         }
         return true;
      }

      template<typename TransitionPredicate, typename Function>
      inline bool sequential_partition(TransitionPredicate p,
                                       Function f,
                                       const row_range_type& range)
      {

         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return false;
         if (range.first >= range.second)
            return false;
         itr_list_list_type::const_iterator it = token_list_.begin() + range.first;
         row_range_type r(range.first,range.first);
         for(std::size_t i = range.first; i < range.second; ++i, ++it)
         {
            if (p(row_type(*it)))
            {
               if(r.first != r.second)
               {
                  r.second = i;
                  if (!f(*this,r))
                     return false;
               }
               r.first = r.second;
            }
            else
               r.second = i;
         }

         if(r.first != range.second)
         {
            r.second = range.second;
            if (!f(*this,r))
               return false;
         }
         return true;
      }

      template<typename TransitionPredicate, typename Function>
      inline bool sequential_partition(TransitionPredicate p, Function f)
      {
         return sequential_partition(p,f,all_rows());
      }

   private:

      token_grid(const token_grid& tg);
      token_grid operator=(const token_grid& tg);

      bool load()
      {
         if (load_from_file_)
         {
            std::ifstream stream(file_name_.c_str(),std::ios::binary);
            if (!stream)
               return false;
            stream.seekg (0,std::ios::end);
            buffer_size_ = stream.tellg();
            if (0 == buffer_size_)
               return false;
            stream.seekg (0,std::ios::beg);
            buffer_ = new unsigned char[buffer_size_ + 1]; // an extra char for end iterator;
            stream.read(reinterpret_cast<char*>(buffer_),static_cast<std::streamsize>(buffer_size_));
            stream.close();
         }

         itr_list_type str_list;
         multiple_char_delimiter_predicate text_newline_predicate(row_delimiters_);
         split(text_newline_predicate,
               buffer_, buffer_ + buffer_size_,
               std::back_inserter(str_list),
               split_options::compress_delimiters);

         multiple_char_delimiter_predicate token_predicate(column_delimiters_);

         min_column_count_ = std::numeric_limits<std::size_t>::max();
         max_column_count_ = std::numeric_limits<std::size_t>::min();

         for (itr_list_type::iterator it = str_list.begin(); str_list.end() != it; ++it)
         {
            if (0 == std::distance(it->first,it->second))
               continue;
            itr_list_type current_token_list;
            split(token_predicate,
                  it->first,
                  it->second,
                  std::back_inserter(current_token_list),
                  split_options::compress_delimiters);
            if (!current_token_list.empty())
            {
               token_list_.push_back(current_token_list);
               min_column_count_ = std::min(min_column_count_,current_token_list.size());
               max_column_count_ = std::max(max_column_count_,current_token_list.size());
            }
         }
         return true;
      }

      template<typename OutputIterator>
      inline void process_column(const itr_list_type::value_type& range, OutputIterator out) const
      {
         typedef typename std::iterator_traits<OutputIterator>::value_type output_type;
         *(out++) = string_to_type_converter<output_type>(range.first,range.second);
      }

   private:

      itr_list_list_type token_list_;
      std::string file_name_;
      std::string column_delimiters_;
      std::string row_delimiters_;
      unsigned char* buffer_;
      std::size_t buffer_size_;
      std::size_t min_column_count_;
      std::size_t max_column_count_;
      bool load_from_file_;
      bool state_;
   };

   template<typename Sequence>
   struct add_to_sequence
   {
   public:
      add_to_sequence(Sequence& sequence)
      : sequence_(sequence)
      {}

      add_to_sequence(const add_to_sequence<Sequence>& ats)
      : sequence_(ats.sequence_)
      {}

      add_to_sequence<Sequence>& operator=(const add_to_sequence<Sequence>& ats)
      {
         if (this != &ats)
         {
            this->sequence_ = ats.sequence_;
         }
         return *this;
      }

      template<typename T>
      void operator()(const T& t) const
      {
         sequence_.push_back(t);
      }

   private:
      Sequence& sequence_;
   };

   template<typename T>
   inline bool convert_string_range(const std::pair<std::string::const_iterator,std::string::const_iterator> range, T& t)
   {
      if (range.first == range.second) return false;
      t = string_to_type_converter<T>(std::string(range.first,range.second));
      return true;
   }

   template<class OutputPredicate>
   struct filter_non_empty_range
   {
   public:
      filter_non_empty_range(OutputPredicate predicate)
      : predicate_(predicate)
      {}

      template<typename Iterator>
      inline void operator() (const std::pair<Iterator,Iterator>& range) const
      {
         if (range.first != range.second)
         {
            predicate_(range);
         }
      }

   private:
      OutputPredicate predicate_;
   };

   template<typename OutputPredicate>
   struct filter_on_wildcard_match
   {
   public:
      filter_on_wildcard_match(const std::string& match_pattern,
                               OutputPredicate& predicate,
                               bool allow_through_on_match = true)
      : allow_through_on_match_(allow_through_on_match),
        match_pattern_(match_pattern),
        predicate_(predicate)
      {}

      template<typename Iterator>
      inline void operator() (const std::pair<Iterator,Iterator>& range) const
      {
         if (match(match_pattern_.begin(),match_pattern_.end(),range.first,range.second,'*','?') ^ allow_through_on_match_)
         {
            predicate_(range);
         }
      }

      inline void operator() (const std::string& s) const
      {
         if (match(match_pattern_,s) ^ allow_through_on_match_)
         {
            predicate_(s);
         }
      }

   private:

      filter_on_wildcard_match(const filter_on_wildcard_match& fom);
      filter_on_wildcard_match operator=(const filter_on_wildcard_match& fom);

      bool allow_through_on_match_;
      std::string match_pattern_;
      OutputPredicate& predicate_;
   };

  template<typename OutputPredicate>
   struct filter_on_match
   {
   public:

     filter_on_match(const std::string* begin, const std::string* end,
                     OutputPredicate predicate,
                     bool case_insensitive,
                     bool allow_through_on_match = true)
     : case_insensitive_(case_insensitive),
       allow_through_on_match_(allow_through_on_match),
       begin_(begin),
       end_(end),
       predicate_(predicate)
     {}

      template<typename Iterator>
      inline void operator() (const std::pair<Iterator,Iterator>& range) const
      {
         for (const std::string* it = begin_; end_ != it; ++it)
         {
            if ((case_insensitive_ &&
               (imatch((*it).begin(),(*it).end(),range.first,range.second))) ||
               (!case_insensitive_ && std::equal((*it).begin(),(*it).end(),range.first)))
            {
               if (allow_through_on_match_)
               {
                  predicate_(range);
               }
               return;
            }
         }
         if (!allow_through_on_match_)
         {
            predicate_(range);
            return;
         }
      }

      inline void operator() (const std::string& s) const
      {
         for (const std::string* it = begin_; end_ != it; ++it)
         {
            if ((case_insensitive_ &&
               (imatch((*it).begin(),(*it).end(),s.begin(),s.end()))) ||
               (!case_insensitive_ && std::equal((*it).begin(),(*it).end(),s.begin())))
            {
               if (allow_through_on_match_)
               {
                  predicate_(s);
                  return;
               }
            }
         }
         if (!allow_through_on_match_)
         {
            predicate_(s);
            return;
         }
      }
   private:

      filter_on_match& operator=(const filter_on_match& fom);

   private:

      bool case_insensitive_;
      bool allow_through_on_match_;
      const std::string* begin_;
      const std::string* end_;
      OutputPredicate& predicate_;
   };

   template<typename MatchPredicate,
            typename Iterator>
   inline void inc_while_matching(Iterator& it,
                                  const Iterator& end,
                                  const MatchPredicate& predicate)
   {
      while (end != it)
      {
         if (predicate(*it))
            ++it;
         else
            break;
      }
   }

   template<std::size_t length>
   struct size_equal_to
   {
      template<typename Iterator>
      inline bool operator()(const Iterator begin, const Iterator end)
      {
         return length == std::distance(begin,end);
      }

      template <typename T,
                class Allocator,
                template <class,class> class Sequence>
      inline bool operator()(const Sequence<T,Allocator>& sequence)
      {
         return length == sequence.size();
      }

      inline bool operator()(const std::string& str)
      {
         return length == str.size();
      }
   };

   template<std::size_t length>
   struct size_less_than
   {
      template<typename Iterator>
      inline bool operator()(const Iterator begin, const Iterator end)
      {
         return std::distance(begin,end) < static_cast<typename std::iterator_traits<Iterator>::difference_type>(length);
      }

      template <typename T,
                class Allocator,
                template <class,class> class Sequence>
      inline bool operator()(const Sequence<T,Allocator>& sequence)
      {
         return sequence.size() < length;
      }

      inline bool operator()(const std::string& str)
      {
         return str.size() < length;
      }
   };

   template<std::size_t length>
   struct size_greater_than
   {
      template<typename Iterator>
      inline bool operator()(const Iterator begin, const Iterator end)
      {
         return std::distance(begin,end) > static_cast<typename std::iterator_traits<Iterator>::difference_type>(length);
      }

      template <typename T,
                class Allocator,
                template <class,class> class Sequence>
      inline bool operator()(const Sequence<T,Allocator>& sequence)
      {
         return sequence.size() > length;
      }

      inline bool operator()(const std::string& str)
      {
         return str.size() > length;
      }
   };

   namespace details
   {

      template<typename InputIterator,
               typename T1, typename  T2, typename  T3, typename T4,
               typename T5, typename  T6, typename  T7, typename T8,
               typename T9, typename T10, typename T11, typename T12>
      inline bool parse_impl(const InputIterator begin,
                             const InputIterator end,
                             const std::string& delimiters,
                             T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,
                             T5& t5,  T6&  t6,  T7&  t7,  T8&  t8,
                             T9& t9, T10& t10, T11& t11, T12& t12)
      {
         static const std::size_t token_count = 12;
         typedef std::pair<InputIterator,InputIterator> iterator_type;
         typedef iterator_type* iterator_type_ptr;
         iterator_type token_list[token_count];
         if (token_count != split_n(delimiters,begin,end,token_count,token_list))
            return false;
         iterator_type_ptr it = token_list;

         if (!string_to_type_converter((*it).first,(*it).second, t1)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t2)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t3)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t4)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t5)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t6)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t7)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t8)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t9)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t10)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t11)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t12)) return false;
         return true;
      }

      template<typename InputIterator,
               typename T1, typename  T2, typename  T3, typename T4,
               typename T5, typename  T6, typename  T7, typename T8,
               typename T9, typename T10, typename T11>
      inline bool parse_impl(const InputIterator begin,
                             const InputIterator end,
                             const std::string& delimiters,
                             T1& t1, T2& t2, T3& t3, T4& t4,
                             T5& t5, T6& t6, T7& t7, T8& t8,
                             T9& t9, T10& t10, T11& t11)
      {
         static const std::size_t token_count = 11;
         typedef std::pair<InputIterator,InputIterator> iterator_type;
         typedef iterator_type* iterator_type_ptr;
         iterator_type token_list[token_count];
         if (token_count != split_n(delimiters,begin,end,token_count,token_list))
            return false;
         iterator_type_ptr it = token_list;
         if (!string_to_type_converter((*it).first,(*it).second, t1)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t2)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t3)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t4)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t5)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t6)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t7)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t8)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t9)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t10)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t11)) return false;
         return true;
      }

      template<typename InputIterator,
               typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6, typename T7, typename T8,
               typename T9, typename T10>
      inline bool parse_impl(const InputIterator begin,
                             const InputIterator end,
                             const std::string& delimiters,
                             T1& t1, T2& t2, T3& t3, T4& t4,
                             T5& t5, T6& t6, T7& t7, T8& t8,
                             T9& t9, T10& t10)
      {
         static const std::size_t token_count = 10;
         typedef std::pair<InputIterator,InputIterator> iterator_type;
         typedef iterator_type* iterator_type_ptr;
         iterator_type token_list[token_count];
         if (token_count != split_n(delimiters,begin,end,token_count,token_list))
            return false;
         iterator_type_ptr it = token_list;
         if (!string_to_type_converter((*it).first,(*it).second, t1)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t2)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t3)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t4)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t5)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t6)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t7)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t8)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second, t9)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t10)) return false;
         return true;
      }

      template<typename InputIterator,
               typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6, typename T7, typename T8,
               typename T9>
      inline bool parse_impl(const InputIterator begin,
                             const InputIterator end,
                             const std::string& delimiters,
                             T1& t1, T2& t2, T3& t3, T4& t4,
                             T5& t5, T6& t6, T7& t7, T8& t8,
                             T9& t9)
      {
         static const std::size_t token_count = 9;
         typedef std::pair<InputIterator,InputIterator> iterator_type;
         typedef iterator_type* iterator_type_ptr;
         iterator_type token_list[token_count];
         if (token_count != split_n(delimiters,begin,end,token_count,token_list))
            return false;
         iterator_type_ptr it = token_list;
         if (!string_to_type_converter((*it).first,(*it).second,t1)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t2)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t3)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t4)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t5)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t6)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t7)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t8)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t9)) return false;
         return true;
      }

      template<typename InputIterator,
               typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6, typename T7, typename T8>
      inline bool parse_impl(const InputIterator begin,
                             const InputIterator end,
                             const std::string& delimiters,
                             T1& t1, T2& t2, T3& t3, T4& t4,
                             T5& t5, T6& t6, T7& t7, T8& t8)
      {
         static const std::size_t token_count = 8;
         typedef std::pair<InputIterator,InputIterator> iterator_type;
         typedef iterator_type* iterator_type_ptr;
         iterator_type token_list[token_count];
         if (token_count != split_n(delimiters,begin,end,token_count,token_list))
            return false;
         iterator_type_ptr it = token_list;
         if (!string_to_type_converter((*it).first,(*it).second,t1)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t2)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t3)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t4)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t5)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t6)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t7)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t8)) return false;
         return true;
      }

      template<typename InputIterator,
               typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6, typename T7>
      inline bool parse_impl(const InputIterator begin,
                             const InputIterator end,
                             const std::string& delimiters,
                             T1& t1, T2& t2, T3& t3, T4& t4,
                             T5& t5, T6& t6, T7& t7)
      {
         static const std::size_t token_count = 7;
         typedef std::pair<InputIterator,InputIterator> iterator_type;
         typedef iterator_type* iterator_type_ptr;
         iterator_type token_list[token_count];
         if (token_count != split_n(delimiters,begin,end,token_count,token_list))
            return false;
         iterator_type_ptr it = token_list;
         if (!string_to_type_converter((*it).first,(*it).second,t1)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t2)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t3)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t4)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t5)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t6)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t7)) return false;
         return true;
      }

      template<typename InputIterator,
               typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6>
      inline bool parse_impl(const InputIterator begin,
                             const InputIterator end,
                             const std::string& delimiters,
                             T1& t1, T2& t2, T3& t3, T4& t4,
                             T5& t5, T6& t6)
      {
         static const std::size_t token_count = 6;
         typedef std::pair<InputIterator,InputIterator> iterator_type;
         typedef iterator_type* iterator_type_ptr;
         iterator_type token_list[token_count];
         if (token_count != split_n(delimiters,begin,end,token_count,token_list))
            return false;
         iterator_type_ptr it = token_list;
         if (!string_to_type_converter((*it).first,(*it).second,t1)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t2)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t3)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t4)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t5)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t6)) return false;
         return true;
      }

      template<typename InputIterator,
               typename T1, typename T2, typename T3, typename T4,
               typename T5>
      inline bool parse_impl(const InputIterator begin,
                             const InputIterator end,
                             const std::string& delimiters,
                             T1& t1, T2& t2, T3& t3, T4& t4,
                             T5& t5)
      {
         static const std::size_t token_count = 5;
         typedef std::pair<InputIterator,InputIterator> iterator_type;
         typedef iterator_type* iterator_type_ptr;
         iterator_type token_list[token_count];
         if (token_count != split_n(delimiters,begin,end,token_count,token_list))
            return false;
         iterator_type_ptr it = token_list;
         if (!string_to_type_converter((*it).first,(*it).second,t1)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t2)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t3)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t4)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t5)) return false;
         return true;
      }

      template<typename InputIterator,
               typename T1, typename T2, typename T3, typename T4>
      inline bool parse_impl(const InputIterator begin,
                             const InputIterator end,
                             const std::string& delimiters,
                             T1& t1, T2& t2, T3& t3, T4& t4)
      {
         static const std::size_t token_count = 4;
         typedef std::pair<InputIterator,InputIterator> iterator_type;
         typedef iterator_type* iterator_type_ptr;
         iterator_type token_list[token_count];
         if (token_count != split_n(delimiters,begin,end,token_count,token_list))
            return false;
         iterator_type_ptr it = token_list;
         if (!string_to_type_converter((*it).first,(*it).second,t1)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t2)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t3)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t4)) return false;
         return true;
      }

      template<typename InputIterator,
               typename T1, typename T2, typename T3>
      inline bool parse_impl(const InputIterator begin,
                             const InputIterator end,
                             const std::string& delimiters,
                             T1& t1, T2& t2, T3& t3)
      {
         static const std::size_t token_count = 3;
         typedef std::pair<InputIterator,InputIterator> iterator_type;
         typedef iterator_type* iterator_type_ptr;
         iterator_type token_list[token_count];
         if (token_count != split_n(delimiters,begin,end,token_count,token_list))
            return false;
         iterator_type_ptr it = token_list;
         if (!string_to_type_converter((*it).first,(*it).second,t1)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t2)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t3)) return false;
         return true;
      }

      template<typename InputIterator, typename T1, typename T2>
      inline bool parse_impl(const InputIterator begin,
                             const InputIterator end,
                             const std::string& delimiters,
                             T1& t1, T2& t2)
      {
         static const std::size_t token_count = 2;
         typedef std::pair<InputIterator,InputIterator> iterator_type;
         typedef iterator_type* iterator_type_ptr;
         iterator_type token_list[token_count];
         if (token_count != split_n(delimiters,begin,end,token_count,token_list))
            return false;
         iterator_type_ptr it = token_list;
         if (!string_to_type_converter((*it).first,(*it).second,t1)) return false; ++it;
         if (!string_to_type_converter((*it).first,(*it).second,t2)) return false;
         return true;
      }

      template<typename InputIterator, typename T>
      inline bool parse_impl(const InputIterator begin,
                             const InputIterator end,
                             const std::string& delimiters,
                             T& t)
      {
         static const std::size_t token_count = 1;
         typedef std::pair<InputIterator,InputIterator> iterator_type;
         typedef iterator_type* iterator_type_ptr;
         iterator_type token_list[token_count];
         if (token_count != split_n(delimiters,begin,end,token_count,token_list))
            return false;
         iterator_type_ptr it = token_list;
         return string_to_type_converter((*it).first,(*it).second,t);
      }

   } // namespace details

   #define INSTANTIATE_PARSE(iterator_type)\
   template<typename T1, typename T2, typename T3, typename  T4,\
            typename T5, typename T6, typename T7, typename  T8,\
            typename T9, typename T10, typename T11, typename T12>\
   inline bool parse(const iterator_type begin,\
                     const iterator_type end,\
                     const std::string& delimiters,\
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,\
                     T5& t5,  T6&  t6,  T7&  t7,  T8&  t8,\
                     T9& t9, T10& t10, T11& t11, T12& t12)\
   {\
      return details::parse_impl(begin,end,delimiters,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12);\
   }\
   template<typename T1, typename T2, typename T3, typename  T4,\
            typename T5, typename T6, typename T7, typename  T8,\
            typename T9, typename T10, typename T11>\
   inline bool parse(const iterator_type begin,\
                     const iterator_type end,\
                     const std::string& delimiters,\
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,\
                     T5& t5,  T6&  t6,  T7&  t7,  T8&  t8,\
                     T9& t9, T10& t10, T11& t11)\
   {\
      return details::parse_impl(begin,end,delimiters,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11);\
   }\
   template<typename T1, typename T2, typename T3, typename  T4,\
            typename T5, typename T6, typename T7, typename  T8,\
            typename T9, typename T10>\
   inline bool parse(const iterator_type begin,\
                     const iterator_type end,\
                     const std::string& delimiters,\
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,\
                     T5& t5,  T6&  t6,  T7&  t7,  T8&  t8,\
                     T9& t9, T10& t10)\
   {\
      return details::parse_impl(begin,end,delimiters,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10);\
   }\
   template<typename T1, typename T2, typename T3, typename  T4,\
            typename T5, typename T6, typename T7, typename  T8,\
            typename T9>\
   inline bool parse(const iterator_type begin,\
                     const iterator_type end,\
                     const std::string& delimiters,\
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,\
                     T5& t5,  T6&  t6,  T7&  t7,  T8&  t8,\
                     T9& t9)\
   {\
      return details::parse_impl(begin,end,delimiters,t1,t2,t3,t4,t5,t6,t7,t8,t9);\
   }\
   template<typename T1, typename T2, typename T3, typename  T4,\
            typename T5, typename T6, typename T7, typename  T8>\
   inline bool parse(const iterator_type begin,\
                     const iterator_type end,\
                     const std::string& delimiters,\
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,\
                     T5& t5,  T6&  t6,  T7&  t7,  T8&  t8)\
   {\
      return details::parse_impl(begin,end,delimiters,t1,t2,t3,t4,t5,t6,t7,t8);\
   }\
   template<typename T1, typename T2, typename T3, typename  T4,\
            typename T5, typename T6, typename T7>\
   inline bool parse(const iterator_type begin,\
                     const iterator_type end,\
                     const std::string& delimiters,\
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,\
                     T5& t5,  T6&  t6,  T7&  t7)\
   {\
      return details::parse_impl(begin,end,delimiters,t1,t2,t3,t4,t5,t6,t7);\
   }\
   template<typename T1, typename T2, typename T3, typename  T4,\
            typename T5, typename T6>\
   inline bool parse(const iterator_type begin,\
                     const iterator_type end,\
                     const std::string& delimiters,\
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,\
                     T5& t5,  T6&  t6)\
   {\
      return details::parse_impl(begin,end,delimiters,t1,t2,t3,t4,t5,t6);\
   }\
   template<typename T1, typename T2, typename T3, typename  T4,\
            typename T5>\
   inline bool parse(const iterator_type begin,\
                     const iterator_type end,\
                     const std::string& delimiters,\
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,\
                     T5& t5)\
   {\
      return details::parse_impl(begin,end,delimiters,t1,t2,t3,t4,t5);\
   }\
   template<typename T1, typename T2, typename T3, typename  T4>\
   inline bool parse(const iterator_type begin,\
                     const iterator_type end,\
                     const std::string& delimiters,\
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4)\
   {\
      return details::parse_impl(begin,end,delimiters,t1,t2,t3,t4);\
   }\
   template<typename T1, typename T2, typename T3>\
   inline bool parse(const iterator_type begin,\
                     const iterator_type end,\
                     const std::string& delimiters,\
                     T1& t1, T2& t2, T3& t3)\
   {\
      return details::parse_impl(begin,end,delimiters,t1,t2,t3);\
   }\
   template<typename T1, typename T2>\
   inline bool parse(const iterator_type begin,\
                     const iterator_type end,\
                     const std::string& delimiters,\
                     T1& t1,  T2&  t2)\
   {\
      return details::parse_impl(begin,end,delimiters,t1,t2);\
   }\
   template<typename T>\
   inline bool parse(const iterator_type begin,\
                     const iterator_type end,\
                     const std::string& delimiters,\
                     T& t)\
   {\
      return details::parse_impl(begin,end,delimiters,t);\
   }\
   template <typename T,\
             class Allocator,\
             template <class,class> class Sequence>\
   inline std::size_t parse_into_sequence(const iterator_type begin,\
                                          const iterator_type end,\
                                          const std::string& delimiters,\
                                          Sequence<T,Allocator>& sequence,\
                                          const split_options::type& split_option = split_options::compress_delimiters)\
   {\
      if (1 == delimiters.size())\
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),\
                      begin, end,\
                      range_to_type_back_inserter(sequence),\
                      split_option);\
      else\
         return split(strtk::multiple_char_delimiter_predicate(delimiters),\
                      begin, end,\
                      range_to_type_back_inserter(sequence),\
                      split_option);\
   }

   INSTANTIATE_PARSE(char*)
   INSTANTIATE_PARSE(unsigned char*)
   INSTANTIATE_PARSE(std::string::const_iterator)
   INSTANTIATE_PARSE(std::string::iterator)

   template<typename T1, typename T2, typename T3, typename  T4,
            typename T5, typename T6, typename T7, typename  T8,
            typename T9, typename T10, typename T11, typename T12>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5& t5,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9& t9, T10& t10, T11& t11, T12& t12)
   {
      return parse(data.c_str(), data.c_str() + data.size(),delimiters,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10, typename T11>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2&   t2,  T3&  t3, T4& t4,
                     T5& t5, T6&   t6,  T7&  t7, T8& t8,
                     T9& t9, T10& t10, T11& t11)
   {
      return parse(data.c_str(), data.c_str() + data.size(),delimiters,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8,
                     T9& t9, T10& t10)
   {
      return parse(data.c_str(), data.c_str() + data.size(),delimiters,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8,
                     T9& t9)
   {
      return parse(data.c_str(), data.c_str() + data.size(),delimiters,t1,t2,t3,t4,t5,t6,t7,t8,t9);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8)
   {
      return parse(data.c_str(), data.c_str() + data.size(),delimiters,t1,t2,t3,t4,t5,t6,t7,t8);
   }

   template< typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7)
   {
      return parse(data.c_str(), data.c_str() + data.size(), delimiters,t1,t2,t3,t4,t5,t6,t7);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6)
   {
      return parse(data.c_str(), data.c_str() + data.size(), delimiters,t1,t2,t3,t4,t5,t6);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5)
   {
      return parse(data.c_str(), data.c_str() + data.size(), delimiters,t1,t2,t3,t4,t5);
   }

   template<typename T1, typename T2, typename T3, typename T4>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4)
   {
      return parse(data.c_str(), data.c_str() + data.size(), delimiters,t1,t2,t3,t4);
   }

   template<typename T1, typename T2, typename T3>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3)
   {
      return parse(data.c_str(), data.c_str() + data.size(), delimiters,t1,t2,t3);
   }

   template<typename T1, typename T2>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2)
   {
      return parse(data.c_str(), data.c_str() + data.size(), delimiters,t1,t2);
   }

   template<typename T>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T& t)
   {
      return parse(data.c_str(), data.c_str() + data.size(), delimiters,t);
   }

   template <typename T,
             class Allocator,
             template <class,class> class Sequence>
   inline std::size_t parse_into_sequence(const std::string& data,
                                          const std::string& delimiters,
                                          Sequence<T,Allocator>& sequence)
   {
      return parse_into_sequence(data.c_str(), data.c_str() + data.size(),
                                 delimiters,
                                 sequence);
   }

   template<typename T1, typename T2, typename  T3, typename  T4,
            typename T5, typename T6, typename  T7, typename  T8,
            typename T9, typename T10, typename T11, typename T12>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const  T2&  t2, const  T3&  t3, const  T4&  t4,
                         const T5& t5, const  T6&  t6, const  T7&  t7, const  T8&  t8,
                         const T9& t9, const T10& t10, const T11& t11, const T12& t12)
   {
      output += type_to_string( t1); output += delimiter;
      output += type_to_string( t2); output += delimiter;
      output += type_to_string( t3); output += delimiter;
      output += type_to_string( t4); output += delimiter;
      output += type_to_string( t5); output += delimiter;
      output += type_to_string( t6); output += delimiter;
      output += type_to_string( t7); output += delimiter;
      output += type_to_string( t8); output += delimiter;
      output += type_to_string( t9); output += delimiter;
      output += type_to_string(t10); output += delimiter;
      output += type_to_string(t11); output += delimiter;
      output += type_to_string(t12);
   }

   template<typename T1, typename T2, typename  T3, typename T4,
            typename T5, typename T6, typename  T7, typename T8,
            typename T9, typename T10, typename T11>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const  T2&  t2, const T3& t3, const T4& t4,
                         const T5& t5, const  T6&  t6, const T7& t7, const T8& t8,
                         const T9& t9, const T10& t10, const T11& t11)
   {
      output += type_to_string( t1); output += delimiter;
      output += type_to_string( t2); output += delimiter;
      output += type_to_string( t3); output += delimiter;
      output += type_to_string( t4); output += delimiter;
      output += type_to_string( t5); output += delimiter;
      output += type_to_string( t6); output += delimiter;
      output += type_to_string( t7); output += delimiter;
      output += type_to_string( t8); output += delimiter;
      output += type_to_string( t9); output += delimiter;
      output += type_to_string(t10); output += delimiter;
      output += type_to_string(t11);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                         const T9& t9, const T10& t10)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6); output += delimiter;
      output += type_to_string(t7); output += delimiter;
      output += type_to_string(t8); output += delimiter;
      output += type_to_string(t9); output += delimiter;
      output += type_to_string(t10);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                         const T9& t9)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6); output += delimiter;
      output += type_to_string(t7); output += delimiter;
      output += type_to_string(t8); output += delimiter;
      output += type_to_string(t9);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6); output += delimiter;
      output += type_to_string(t7); output += delimiter;
      output += type_to_string(t8);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6); output += delimiter;
      output += type_to_string(t7);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5,typename T6>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5);
   }

   template<typename T1, typename T2, typename T3, typename T4>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4);
   }

   template<typename T1, typename T2, typename T3>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3);
   }

   template<typename T1, typename T2>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2);
   }

   template<typename InputIterator>
   inline void join(std::string& output, const std::string& delimiter,
                    const InputIterator begin,
                    const InputIterator end)
   {
      InputIterator it = begin;
      while (end != it)
      {
         output += type_to_string(*it);
         if (end == (++it))
            break;
         else
            output += delimiter;
      }
   }

   template<typename InputIterator>
   inline std::string join(const std::string& delimiter,
                           const InputIterator begin,
                           const InputIterator end)
   {
      std::string output;
      join(output,delimiter,begin,end);
      return output;
   }

   template<typename InputIterator, typename Predicate>
   inline void join_if(std::string& output,
                       const std::string& delimiter,
                       Predicate predicate,
                       const InputIterator begin,
                       const InputIterator end)
   {
      InputIterator it = begin;
      bool first_time = true;
      while (end != it)
      {
         if (predicate(*it))
         {
            if (!first_time)
               output += delimiter;
            else
               first_time = false;
            output += type_to_string(*it);
         }
         if (end == (++it))
            break;
      }
   }

   template<typename InputIterator, typename Predicate>
   inline std::string join_if(const std::string& delimiter,
                              Predicate predicate,
                              const InputIterator begin,
                              const InputIterator end)
   {
      std::string output;
      join_if(output,delimiter,predicate,begin,end);
      return output;
   }

   inline void replicate(const std::size_t& count,
                         const std::string& str,
                         std::string& output)
   {
      if (0 == count) return;
      output.reserve(output.size() + (str.size() * count));
      for (std::size_t i = 0; i < count; ++i)
      {
         output.append(str);
      }
   }

   inline std::string replicate(const std::size_t& count,
                                const std::string& str)
   {
      std::string output;
      replicate(count,str,output);
      return output;
   }

   #ifdef ENABLE_RANDOM
   void generate_random_data(unsigned char* data, std::size_t length, unsigned int pre_gen_cnt = 0, unsigned int seed = 0xA5A5A5A5)
   {
      boost::mt19937 rng(static_cast<boost::mt19937::result_type>(seed));
      boost::uniform_int<unsigned int> dist(std::numeric_limits<unsigned int>::min(),std::numeric_limits<unsigned int>::max());
      boost::variate_generator<boost::mt19937&, boost::uniform_int<unsigned int> > rnd(rng,dist);
      if (pre_gen_cnt > 0)
      {
         unsigned int r = 0x00;
         for (unsigned int i = 0; i < pre_gen_cnt; r = rnd(), ++i);
      }
      unsigned char* it = data;
      unsigned int* x = 0;
      while (length >= sizeof(unsigned int))
      {
         x = reinterpret_cast<unsigned int*>(it);
         (*x) = rnd();
         it += sizeof(unsigned int);
         length -= sizeof(unsigned int);
      }
      if (length > 0)
      {
         it -= (sizeof(unsigned int) - length);
         x = reinterpret_cast<unsigned int*>(it);
         (*x) = rnd();
      }
   }
   #endif // ENABLE_RANDOM

   template <typename Iterator>
   bool next_combination(const Iterator first, Iterator k, const Iterator last)
   {
      /* Credits: Mark Nelson http://marknelson.us */
      if ((first == last) || (first == k) || (last == k))
         return false;
      Iterator i1 = first;
      Iterator i2 = last;
      ++i1;
      if (last == i1)
         return false;
      i1 = last;
      --i1;
      i1 = k;
      --i2;
      while (first != i1)
      {
         if (*--i1 < *i2)
         {
            Iterator j = k;
            while (!(*i1 < *j)) ++j;
            std::iter_swap(i1,j);
            ++i1;
            ++j;
            i2 = k;
            std::rotate(i1,j,last);
            while (last != j)
            {
               ++j;
               ++i2;
            }
            std::rotate(k,i2,last);
            return true;
         }
      }
      std::rotate(first,k,last);
      return false;
   }

   class serializer
   {
   public:

      serializer(char* buffer, const std::size_t& buffer_length)
      : original_buffer_(buffer),
        buffer_(buffer),
        buffer_length_(buffer_length),
        to_be_written_buffer_size_(0),
        read_buffer_size_(0)
      {}

      serializer(unsigned char* buffer, const std::size_t& buffer_length)
      : original_buffer_(reinterpret_cast<char*>(buffer)),
        buffer_(reinterpret_cast<char*>(buffer)),
        buffer_length_(buffer_length),
        to_be_written_buffer_size_(0),
        read_buffer_size_(0)
      {}

      inline bool operator!() const
      {
         return (0 == buffer_length_) || (0 == original_buffer_) || (0 == buffer_);
      }

      void reset()
      {
         to_be_written_buffer_size_ = 0;
         read_buffer_size_ = 0;
         buffer_ = original_buffer_;
      }

      void clear()
      {
         reset();
         std::memset(buffer_,0x00,buffer_length_);
      }

      std::size_t length() const
      {
         return to_be_written_buffer_size_;
      }

      template<typename T> inline bool operator >> (T& output) { return read (output); }
      template<typename T> inline bool operator << (T& output) { return write(output); }

      template<typename T>
      inline bool read(T&              output) { return output.read(*this); }
      inline bool read(char&           output) { return read_pod(output);   }
      inline bool read(short&          output) { return read_pod(output);   }
      inline bool read(unsigned short& output) { return read_pod(output);   }
      inline bool read(int&            output) { return read_pod(output);   }
      inline bool read(unsigned int&   output) { return read_pod(output);   }
      inline bool read(long&           output) { return read_pod(output);   }
      inline bool read(unsigned long&  output) { return read_pod(output);   }
      inline bool read(float&          output) { return read_pod(output);   }
      inline bool read(double&         output) { return read_pod(output);   }
      inline bool read(bool&           output) { return read_pod(output);   }

      inline bool read(std::string& output)
      {
         std::size_t length = 0;
         if (!read_pod(length))
         {
            return false;
         }

         if ((length + sizeof(std::size_t) + read_buffer_size_) > buffer_length_)
         {
            return false;
         }
         output.resize(length);
         std::copy(buffer_, buffer_ + length,output.begin());
         buffer_ += length;
         read_buffer_size_ += length;
         return true;
      }

      template<typename T>
      inline bool write(const T&              input) { return input.write(*this); }
      inline bool write(const char&           input) { return write_pod(input);   }
      inline bool write(const short&          input) { return write_pod(input);   }
      inline bool write(const unsigned short& input) { return write_pod(input);   }
      inline bool write(const int&            input) { return write_pod(input);   }
      inline bool write(const unsigned int&   input) { return write_pod(input);   }
      inline bool write(const long&           input) { return write_pod(input);   }
      inline bool write(const unsigned long&  input) { return write_pod(input);   }
      inline bool write(const float&          input) { return write_pod(input);   }
      inline bool write(const double&         input) { return write_pod(input);   }
      inline bool write(const bool&           input) { return write_pod(input);   }

      inline bool write(const std::string& input)
      {
         return write(input.c_str(),static_cast<unsigned int>(input.size()));
      }

      inline bool write(const char* data, const unsigned int& length)
      {
         if ((length + sizeof(std::size_t) + to_be_written_buffer_size_) > buffer_length_)
         {
            return false;
         }
         write(length);
         std::copy(data,data + length,buffer_);
         buffer_ += length;
         to_be_written_buffer_size_ += length;
         return true;
      }

      inline std::size_t write_to_stream(std::ofstream& stream)
      {
         stream.write(original_buffer_,static_cast<std::streamsize>(to_be_written_buffer_size_));
         return to_be_written_buffer_size_;
      }

      inline bool read_from_stream(std::ifstream& stream, const std::size_t& length)
      {
         if (length > buffer_length_) return false;
         stream.read(original_buffer_,static_cast<std::streamsize>(length));
         return true;
      }

      inline bool copy_to_internal_buffer(const char* data, const std::size_t& length)
      {
         if (length > buffer_length_) return false;
         std::copy(data, data + length,original_buffer_);
         return true;
      }

      inline void write_to_external_buffer(char* data)
      {
         std::copy(original_buffer_,original_buffer_ + to_be_written_buffer_size_,data);
      }

      template <typename T,
                class Allocator,
                template <class,class> class Sequence>
      inline bool read_into_external_sequence(Sequence<T,Allocator>& sequence)
      {
         std::size_t list_size = 0;
         if (!read(list_size))
            return false;
         T t;
         for (std::size_t i = 0; i < list_size; ++i)
         {
            if (!read(t))
               return false;
            sequence.push_back(t);
         }
         return true;
      }

      template <typename T,
                class Allocator,
                template <class,class> class Sequence>
      inline bool write_from_external_sequence(const Sequence<T,Allocator>& sequence)
      {
         if (!write(static_cast<unsigned int>(sequence.size())))
            return false;
         typename Sequence<T,Allocator>::const_iterator it = sequence.begin();
         while (sequence.end() != it)
         {
            if (!write(*it++))
               return false;
         }
         return true;
      }

   private:

      serializer();
      serializer(const serializer& s);
      serializer& operator=(const serializer& s);

      template<typename T>
      inline bool write_pod(const T& data)
      {
         static const std::size_t data_length = sizeof(T);
         if ((data_length + to_be_written_buffer_size_) > buffer_length_)
         {
            return false;
         }
         const char* ptr = reinterpret_cast<const char*>(&data);
         std::copy(ptr,ptr + data_length,buffer_);
         buffer_ += data_length;
         to_be_written_buffer_size_ += data_length;
         return true;
      }

      template<typename T>
      inline bool read_pod(T& data)
      {
         static const std::size_t data_length = sizeof(T);
         if ((data_length + read_buffer_size_) > buffer_length_)
         {
            return false;
         }
         char* ptr = reinterpret_cast<char*>(&data);
         std::copy(buffer_,buffer_ + data_length, ptr);
         buffer_ += data_length;
         read_buffer_size_ += data_length;
         return true;
      }

      char* const original_buffer_;
      char* buffer_;
      std::size_t buffer_length_;
      std::size_t to_be_written_buffer_size_;
      std::size_t read_buffer_size_;
   };

   template<typename T>
   class hex_to_number_sink
   {
      // static_assert for T either int or unsigned int and alike (could use a concept)
      class hex_value_check
      {
      public:
         inline bool operator()(const unsigned char c) const
         {
            return (('0' <= c) && (c <= '9')) ||
                   (('A' <= c) && (c <= 'F')) ||
                   (('a' <= c) && (c <= 'f'));
         }

         inline bool operator()(const char c) const
         {
            return (*this)(static_cast<unsigned char>(c));
         }
      };

   public:

      hex_to_number_sink(T& t)
      : valid_(false),
        t_(&t)
      {}

      hex_to_number_sink(const hex_to_number_sink& hns)
      : valid_(hns.valid),
        t_(hns.t_)
      {}

      inline hex_to_number_sink& operator=(const hex_to_number_sink& hns)
      {
         valid_ = hns.valid_;
         t_ = hns.t_;
         return *this;
      }

      inline hex_to_number_sink& operator=(const std::string& s)
      {
         std::size_t offset = 0;
         if ((s.size() > 2) && (s[0] == '0') && ((s[1] == 'x') || (s[1] == 'X')))
            offset = 2;
         if ((s.size() - offset) > (2 * sizeof(T)))
               return *this;
         const std::size_t buffer_size = 2 * sizeof(T);
         const std::size_t buffer_offset = ((s.size() - offset) % 2);
         char buffer[buffer_size] = { '0' };
         if (!range_only_contains(hex_value_check(),s.c_str() + offset,s.c_str() + s.size()))
         {
            valid_ = false;
            return *this;
         }
         std::copy(s.c_str() + offset, s.c_str() + s.size(), buffer + buffer_offset);
         *t_ = 0;
         valid_= strtk::convert_hex_to_bin(buffer,
                                           buffer + (s.size() - offset) + buffer_offset,
                                           reinterpret_cast<char*>(t_));
         reverse_bytes();
         return *this;
      }

      bool valid() const
      {
         return valid_;
      }

   private:

      inline void reverse_bytes()
      {
         unsigned char* it1 = reinterpret_cast<unsigned char*>(t_);
         unsigned char* it2 = it1 + (sizeof(T) - 1);
         while (it1 < it2)
         {
            char tmp = *it1;
                *it1 = *it2;
                *it2 = tmp;
            ++it1; --it2;
         }
      }

   private:
      bool valid_;
      T* t_;
   };

   template<typename T>
   class base64_to_number_sink
   {
      // static_assert for T either int or unsigned int and alike (could use a concept)
   private:
      class base64_value_check
      {
      public:
         inline bool operator()(const unsigned char c) const
         {
            return (('0' <= c) && (c <= '9')) ||
                   (('A' <= c) && (c <= 'Z')) ||
                   (('a' <= c) && (c <= 'z')) ||
                    ('+' == c) ||
                    ('/' == c) ||
                    ('=' == c);
         }

         inline bool operator()(const char c) const
         {
            return (*this)(static_cast<unsigned char>(c));
         }
      };

   public:

      base64_to_number_sink(T& t)
      : valid_(false),
        t_(&t)
      {}

      base64_to_number_sink(const base64_to_number_sink& bns)
      : valid_(bns.valid),
        t_(bns.t_)
      {}

      inline base64_to_number_sink& operator=(const base64_to_number_sink& bns)
      {
         valid_ = bns.valid_;
         t_ = bns.t_;
         return *this;
      }

      inline base64_to_number_sink& operator=(const std::string& s)
      {
         if (!range_only_contains(base64_value_check(),s.c_str(),s.c_str() + s.size()))
         {
            valid_ = false;
            return *this;
         }
         *t_ = T(0);
         strtk::convert_base64_to_bin(s.c_str(), s.c_str() + s.size(),reinterpret_cast<char*>(t_));
         reverse_bytes();
         return *this;
      }

      bool valid() const
      {
         return valid_;
      }

   private:

      inline void reverse_bytes()
      {
         unsigned char* it1 = reinterpret_cast<unsigned char*>(t_);
         unsigned char* it2 = it1 + (sizeof(T) - 1);
         while (it1 < it2)
         {
            char tmp = *it1;
                *it1 = *it2;
                *it2 = tmp;
            ++it1; --it2;
         }
      }

   private:
      bool valid_;
      T* t_;
   };

   namespace text
   {
      inline std::string center(const std::size_t& width, const std::string::value_type& pad, const std::string& str)
      {
         if (str.size() >= width) return str;
         std::size_t pad_count = width - str.size();
         std::size_t pad_count_2 = (pad_count >> 1) + (pad_count & 1);
         return std::string(pad_count >> 1,pad) + str + std::string(pad_count_2,pad);
      }

      inline std::string right_align(const std::size_t& width, const std::string::value_type& pad, const std::string& str)
      {
         if (str.size() >= width) return str;
         return std::string(width - str.size(),pad) + str;
      }

      inline std::string left_align(const std::size_t& width, const std::string::value_type& pad, const std::string& str)
      {
         if (str.size() >= width) return str;
         return str + std::string(width - str.size(),pad);
      }

      template<typename T>
      inline std::string center(const std::size_t& width, const std::string::value_type& pad, const T& t)
      {
         return center(width,pad,type_to_string(t));
      }

      template<typename T>
      inline std::string right_align(const std::size_t& width, const std::string::value_type& pad, const T& t)
      {
         return right_align(width,pad,type_to_string(t));
      }

      template<typename T>
      inline std::string left_align(const std::size_t& width, const std::string::value_type& pad, const T& t)
      {
         return left_align(width,pad,type_to_string(t));
      }

      std::string remaining_string(const std::size_t& index, const std::string& str)
      {
         return (index < str.size()) ? str.substr(index,str.size() - index) : str;
      }

      void remaining_string(const std::size_t& index, const std::string& str, std::string& result)
      {
         result = (index < str.size()) ? str.substr(index,str.size() - index) : str;
      }

   }

   namespace details
   {

      static const std::size_t digit_table_symbol_count = 256;
      static const unsigned char digit_table[digit_table_symbol_count] = {
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xFF - 0x07
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x08 - 0x0F
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x10 - 0x17
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x18 - 0x1F
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x20 - 0x27
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x28 - 0x2F
                                                               0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // 0x30 - 0x37
                                                               0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x38 - 0x3F
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x40 - 0x47
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x48 - 0x4F
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x50 - 0x57
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x58 - 0x5F
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x60 - 0x67
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x68 - 0x6F
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x70 - 0x77
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x78 - 0x7F
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x80 - 0x87
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x88 - 0x8F
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x90 - 0x97
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x98 - 0x9F
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xA0 - 0xA7
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xA8 - 0xAF
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xB0 - 0xB7
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xB8 - 0xBF
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xC0 - 0xC7
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xC8 - 0xCF
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xD0 - 0xD7
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xD8 - 0xDF
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xE0 - 0xE7
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xE8 - 0xEF
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xF0 - 0xF7
                                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  // 0xF8 - 0xFF
                                                             };

      template<typename InputIterator, typename T>
      inline bool string_to_signed_type_converter_impl_itr(InputIterator begin, InputIterator end, T& v)
      {
         if (0 == std::distance(begin,end))
            return false;
         v = 0;
         InputIterator it = begin;
         bool negative = false;
         if ('+' == *it)
            ++it;
         else if ('-' == *it)
         {
            ++it;
            negative = true;
         }
         if (end == it)
            return false;
         while (end != it)
         {
            const T digit = static_cast<T>(digit_table[static_cast<unsigned int>(*it++)]);
            if (0xFF == digit)
               return false;
            v = (10 * v) + digit;
         }
         if (negative)
            v *= -1;
         return true;
      }

      template<typename T>
      inline bool string_to_signed_type_converter_impl(const std::string& s, T& v)
      {
         return string_to_signed_type_converter_impl_itr(s.c_str(), s.c_str() + s.size(),v);
      }

      template<typename Iterator, typename T>
      inline bool string_to_unsigned_type_converter_impl_itr(Iterator begin,Iterator end, T& v)
      {
         if (0 == std::distance(begin,end))
            return false;
         v = 0;
         Iterator it = begin;
         if ('+' == *it)
            ++it;
         if (end == it)
            return false;
         while (end != it)
         {
            const T digit = static_cast<T>(digit_table[static_cast<unsigned int>(*it++)]);
            if (0xFF == digit)
               return false;
            v = (10 * v) + digit;
         }
         return true;
      }

      template<typename T>
      inline bool string_to_unsigned_type_converter_impl(const std::string& s, T& v)
      {
         return string_to_unsigned_type_converter_impl_itr(s.c_str(), s.c_str() + s.size(),v);
      }

      static const unsigned char digit[10] = {
                                               '0','1','2','3','4',
                                               '5','6','7','8','9'
                                             };

      template<typename T>
      inline void signed_number_to_string_impl(T value, std::string& result)
      {
         if (result.size() < (3 * sizeof(value) + 1))
            result.resize(3 * sizeof(value) + 1);
         char* it = const_cast<char*>(result.c_str());
         bool negative = (value < 0);
         value = static_cast<T>(std::abs(value));
         int tmp_value = value;
         do
         {
            tmp_value = value;
            value /= 10;
            *(it++) = digit[(tmp_value - value * 10)];
         }
         while (value);
         if (negative) *(it++) = '-';
         result.resize(std::distance(result.c_str(),const_cast<const char*>(it)));
         it = const_cast<char*>(result.c_str());
         char* it2 = it + (result.size() - 1);
         while (it < it2)
         {
            char tmp = *it;
            *it = *it2;
            *it2 = tmp;
            ++it; --it2;
         }
      }

      template<typename T>
      inline void unsigned_number_to_string_impl(T value, std::string& result)
      {
         if (result.size() < (3 * sizeof(value) + 1))
            result.resize(3 * sizeof(value) + 1);
         char* it = const_cast<char*>(result.c_str());
         unsigned int tmp_value = value;
         do
         {
            tmp_value = value;
            value /= 10;
            *(it++) = digit[(tmp_value - value * 10)];
         }
         while (value);
         result.resize(std::distance(result.c_str(),const_cast<const char*>(it)));
         it = const_cast<char*>(result.c_str());
         char* it2 = it + (result.size() - 1);
         while (it < it2)
         {
            char tmp = *it;
            *it = *it2;
            *it2 = tmp;
            ++it; --it2;
         }
      }

      #define INSTANTIATE_STRING_TO_SIGNED_TYPE_CONVERTER_IMPL(ITER_TYPE)\
      template<typename T>\
      inline bool string_to_signed_type_converter_impl(const ITER_TYPE begin, const ITER_TYPE end, T& v)\
      {\
         return string_to_signed_type_converter_impl_itr(begin,end,v);\
      }

      INSTANTIATE_STRING_TO_SIGNED_TYPE_CONVERTER_IMPL(char*)
      INSTANTIATE_STRING_TO_SIGNED_TYPE_CONVERTER_IMPL(unsigned char*)
      INSTANTIATE_STRING_TO_SIGNED_TYPE_CONVERTER_IMPL(std::string::iterator)
      INSTANTIATE_STRING_TO_SIGNED_TYPE_CONVERTER_IMPL(std::string::const_iterator)

      #define INSTANTIATE_STRING_TO_UNSIGNED_TYPE_CONVERTER_IMPL(ITER_TYPE)\
      template<typename T>\
      inline bool string_to_unsigned_type_converter_impl(const ITER_TYPE begin, const ITER_TYPE end, T& v)\
      {\
         return string_to_unsigned_type_converter_impl_itr(begin,end,v);\
      }

      INSTANTIATE_STRING_TO_UNSIGNED_TYPE_CONVERTER_IMPL(char*)
      INSTANTIATE_STRING_TO_UNSIGNED_TYPE_CONVERTER_IMPL(unsigned char*)
      INSTANTIATE_STRING_TO_UNSIGNED_TYPE_CONVERTER_IMPL(std::string::iterator)
      INSTANTIATE_STRING_TO_UNSIGNED_TYPE_CONVERTER_IMPL(std::string::const_iterator)

      template<typename InputIterator, typename T>
      inline bool string_to_byte_converter_impl_itr(InputIterator begin, InputIterator end, T& v)
      {
         if (1 != std::distance(begin,end))
            return false;
         v = static_cast<T>(*begin);
         return true;
      }

      #define INSTANTIATE_STRING_TO_BYTE_CONVERTER_IMPL(ITER_TYPE)\
      template<typename T>\
      inline bool string_to_byte_converter_impl(const ITER_TYPE begin, const ITER_TYPE end, T& v)\
      {\
         return string_to_byte_converter_impl_itr(begin,end,v);\
      }

      INSTANTIATE_STRING_TO_BYTE_CONVERTER_IMPL(char*)
      INSTANTIATE_STRING_TO_BYTE_CONVERTER_IMPL(unsigned char*)
      INSTANTIATE_STRING_TO_BYTE_CONVERTER_IMPL(std::string::iterator)
      INSTANTIATE_STRING_TO_BYTE_CONVERTER_IMPL(std::string::const_iterator)

      template<typename InputIterator>
      inline bool string_to_bool_converter_impl(InputIterator begin, InputIterator end, bool& v)
      {
         if (1 != std::distance(begin,end))
            return false;
         v = (('0' == *begin) ? false : true);
         return true;
      }

   } // namespace details

   #define INSTANTIATE_STRING_TO_TYPE_CONVERTER_FOR_STD_STRING_RETURN(ITER_TYPE)\
   template<> inline std::string string_to_type_converter(const ITER_TYPE begin, const ITER_TYPE end)\
   {\
      return std::string(begin,end);\
   }\
   template<> inline bool string_to_type_converter(const ITER_TYPE begin, const ITER_TYPE end, std::string& out)\
   {\
      out.assign(begin,end);\
      return true;\
   }

   INSTANTIATE_STRING_TO_TYPE_CONVERTER_FOR_STD_STRING_RETURN(char*)
   INSTANTIATE_STRING_TO_TYPE_CONVERTER_FOR_STD_STRING_RETURN(unsigned char*)
   INSTANTIATE_STRING_TO_TYPE_CONVERTER_FOR_STD_STRING_RETURN(std::string::iterator)
   INSTANTIATE_STRING_TO_TYPE_CONVERTER_FOR_STD_STRING_RETURN(std::string::const_iterator)

   #define INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_ITR(T,ITER_TYPE)\
   template<> inline T string_to_type_converter(const ITER_TYPE begin, const ITER_TYPE end)\
   {\
      T result = static_cast<T>(0);\
      if (string_to_type_converter(begin,end,result))\
         return result;\
      else\
         throw;\
   }

   #define INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN(T)\
   template<> inline T string_to_type_converter(const std::string& s)\
   {\
      T result = static_cast<T>(0);\
      if (string_to_type_converter(s,result))\
         return result;\
      else\
         throw;\
   }\
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_ITR(T,char*)\
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_ITR(T,unsigned char*)\
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_ITR(T,std::string::iterator)\
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_ITR(T,std::string::const_iterator)

   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN(short)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN(int)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN(long)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN(unsigned short)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN(unsigned int)
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN(unsigned long)

   #define INSTANTIATE_STRING_TO_SIGNED_NUMBER_CONVERTER_T_ITER(T,ITER_TYPE)\
   inline bool string_to_type_converter(const ITER_TYPE begin, const ITER_TYPE end, T& v)\
   {\
      return details::string_to_signed_type_converter_impl(begin,end,v);\
   }

   #define INSTANTIATE_STRING_TO_SIGNED_NUMBER_CONVERTER(T)\
   inline bool string_to_type_converter(const std::string& s, T& v)\
   {\
      return details::string_to_signed_type_converter_impl(s,v);\
   }\
   INSTANTIATE_STRING_TO_SIGNED_NUMBER_CONVERTER_T_ITER(T,char*)\
   INSTANTIATE_STRING_TO_SIGNED_NUMBER_CONVERTER_T_ITER(T,unsigned char*)\
   INSTANTIATE_STRING_TO_SIGNED_NUMBER_CONVERTER_T_ITER(T,std::string::iterator)\
   INSTANTIATE_STRING_TO_SIGNED_NUMBER_CONVERTER_T_ITER(T,std::string::const_iterator)

   INSTANTIATE_STRING_TO_SIGNED_NUMBER_CONVERTER(short)
   INSTANTIATE_STRING_TO_SIGNED_NUMBER_CONVERTER(int)
   INSTANTIATE_STRING_TO_SIGNED_NUMBER_CONVERTER(long)

   #define INSTANTIATE_STRING_TO_UNSIGNED_NUMBER_CONVERTER_T_ITER(T,ITER_TYPE)\
   inline bool string_to_type_converter(const ITER_TYPE begin, const ITER_TYPE end, T& v)\
   {\
      return details::string_to_unsigned_type_converter_impl(begin,end,v);\
   }\

   #define INSTANTIATE_STRING_TO_UNSIGNED_NUMBER_CONVERTER(T)\
   inline bool string_to_type_converter(const std::string& s, T& v)\
   {\
      return details::string_to_unsigned_type_converter_impl(s,v);\
   }\
   INSTANTIATE_STRING_TO_UNSIGNED_NUMBER_CONVERTER_T_ITER(T,char*)\
   INSTANTIATE_STRING_TO_UNSIGNED_NUMBER_CONVERTER_T_ITER(T,unsigned char*)\
   INSTANTIATE_STRING_TO_UNSIGNED_NUMBER_CONVERTER_T_ITER(T,std::string::iterator)\
   INSTANTIATE_STRING_TO_UNSIGNED_NUMBER_CONVERTER_T_ITER(T,std::string::const_iterator)

   INSTANTIATE_STRING_TO_UNSIGNED_NUMBER_CONVERTER(unsigned short)
   INSTANTIATE_STRING_TO_UNSIGNED_NUMBER_CONVERTER(unsigned int)
   INSTANTIATE_STRING_TO_UNSIGNED_NUMBER_CONVERTER(unsigned long)

   #define INSTANTIATE_STRING_TO_BYTE_CONVERTER_T_ITER(T,ITER_TYPE)\
   inline bool string_to_type_converter(const ITER_TYPE begin, const ITER_TYPE end, T& v)\
   {\
      return details::string_to_byte_converter_impl(begin,end,v);\
   }

   #define INSTANTIATE_STRING_TO_BYTE_CONVERTER(T)\
   inline bool string_to_type_converter(const std::string& s, T& v)\
   {\
      return details::string_to_byte_converter_impl(s.c_str(),s.c_str() + s.size(),v);\
   }\
   INSTANTIATE_STRING_TO_BYTE_CONVERTER_T_ITER(T,char*)\
   INSTANTIATE_STRING_TO_BYTE_CONVERTER_T_ITER(T,unsigned char*)\
   INSTANTIATE_STRING_TO_BYTE_CONVERTER_T_ITER(T,std::string::iterator)\
   INSTANTIATE_STRING_TO_BYTE_CONVERTER_T_ITER(T,std::string::const_iterator)

   INSTANTIATE_STRING_TO_BYTE_CONVERTER(char)
   INSTANTIATE_STRING_TO_BYTE_CONVERTER(unsigned char)

   #define INSTANTIATE_STRING_TO_BOOL_CONVERTER_ITER(ITER_TYPE)\
   inline bool string_to_type_converter(const ITER_TYPE begin, const ITER_TYPE end, bool& v)\
   {\
      return details::string_to_bool_converter_impl(begin,end,v);\
   }

   INSTANTIATE_STRING_TO_BOOL_CONVERTER_ITER(char*)
   INSTANTIATE_STRING_TO_BOOL_CONVERTER_ITER(unsigned char*)
   INSTANTIATE_STRING_TO_BOOL_CONVERTER_ITER(std::string::iterator)
   INSTANTIATE_STRING_TO_BOOL_CONVERTER_ITER(std::string::const_iterator)

   #define INSTANTIATE_STRING_TO_BOOL_CONVERTER(T)\
   inline bool string_to_type_converter(const std::string& s, bool& v)\
   {\
      return details::string_to_bool_converter_impl(s.c_str(),s.c_str() + s.size(),v);\
   }\
   INSTANTIATE_STRING_TO_BOOL_CONVERTER(0)


   #define INSTANTIATE_STRING_TO_DECIMAL_NUMBER_CONVERTER_ITER(T,ITER_TYPE)\
   inline bool string_to_type_converter(const ITER_TYPE begin, const ITER_TYPE end, T& v)\
   {\
      std::size_t length = std::distance(begin,end);\
      static const std::size_t buffer_size = 129;\
      if (length >= buffer_size) return false;\
      char buffer[buffer_size];\
      char* endptr = buffer + length;\
      std::memcpy(buffer,begin,length);\
      v = static_cast<T>(strtod(buffer,&endptr));\
      return (errno != ERANGE);\
   }

   INSTANTIATE_STRING_TO_DECIMAL_NUMBER_CONVERTER_ITER(float,char*)
   INSTANTIATE_STRING_TO_DECIMAL_NUMBER_CONVERTER_ITER(float,unsigned char*)
   INSTANTIATE_STRING_TO_DECIMAL_NUMBER_CONVERTER_ITER(double,char*)
   INSTANTIATE_STRING_TO_DECIMAL_NUMBER_CONVERTER_ITER(double,unsigned char*)

   #define INSTANTIATE_STRING_TO_DECIMAL_NUMBER_CONVERTER_RETURN_ITR(T,ITER_TYPE)\
   template<> inline T string_to_type_converter(const ITER_TYPE begin, const ITER_TYPE end)\
   {\
      T result = static_cast<T>(0);\
      if (string_to_type_converter(begin,end,result))\
         return result;\
      else\
         throw;\
   }

   #define INSTANTIATE_STRING_TO_DECIMAL_NUMBER_CONVERTER_RETURN(T)\
   template<> inline T string_to_type_converter(const std::string& s)\
   {\
      T result = static_cast<T>(0);\
      if (string_to_type_converter(s,result))\
         return result;\
      else\
         throw;\
   }\
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_ITR(T,char*)\
   INSTANTIATE_STRING_TO_NUMBER_CONVERTER_RETURN_ITR(T,unsigned char*)\

   INSTANTIATE_STRING_TO_DECIMAL_NUMBER_CONVERTER_RETURN(float)
   INSTANTIATE_STRING_TO_DECIMAL_NUMBER_CONVERTER_RETURN(double)

   #define INSTANTIATE_SIGNED_NUMBER_TO_STRING(T)\
   inline void number_to_string(T value, std::string& result)\
   {\
      details::signed_number_to_string_impl(value,result);\
   }

   INSTANTIATE_SIGNED_NUMBER_TO_STRING(short)
   INSTANTIATE_SIGNED_NUMBER_TO_STRING(int)
   INSTANTIATE_SIGNED_NUMBER_TO_STRING(long)

   #define INSTANTIATE_UNSIGNED_NUMBER_TO_STRING(T)\
   inline void number_to_string(T value, std::string& result)\
   {\
      details::unsigned_number_to_string_impl(value,result);\
   }

   INSTANTIATE_UNSIGNED_NUMBER_TO_STRING(unsigned short)
   INSTANTIATE_UNSIGNED_NUMBER_TO_STRING(unsigned int)
   INSTANTIATE_UNSIGNED_NUMBER_TO_STRING(unsigned long)

   #define INSTANTIATE_NUMBER_TYPE_TO_STRING(T)\
   template<> inline std::string type_to_string(const T& v)\
   {\
      std::string result;\
      number_to_string(v,result);\
      return result;\
   }

   INSTANTIATE_NUMBER_TYPE_TO_STRING(short)
   INSTANTIATE_NUMBER_TYPE_TO_STRING(int)
   INSTANTIATE_NUMBER_TYPE_TO_STRING(long)
   INSTANTIATE_NUMBER_TYPE_TO_STRING(unsigned short)
   INSTANTIATE_NUMBER_TYPE_TO_STRING(unsigned int)
   INSTANTIATE_NUMBER_TYPE_TO_STRING(unsigned long)

   #define INSTANTIATE_STRING_TO_TYPE_HEX_AND_BASE64_TO_NUMBER_SINK(T)\
   template<> bool string_to_type_converter(const std::string& s, hex_to_number_sink<T>& out)\
   {\
      out = s;\
      return out.valid();\
   }\
   template<> bool string_to_type_converter(const std::string& s, base64_to_number_sink<T>& out)\
   {\
      out = s;\
      return out.valid();\
   }

   INSTANTIATE_STRING_TO_TYPE_HEX_AND_BASE64_TO_NUMBER_SINK(short)
   INSTANTIATE_STRING_TO_TYPE_HEX_AND_BASE64_TO_NUMBER_SINK(int)
   INSTANTIATE_STRING_TO_TYPE_HEX_AND_BASE64_TO_NUMBER_SINK(long)
   INSTANTIATE_STRING_TO_TYPE_HEX_AND_BASE64_TO_NUMBER_SINK(unsigned short)
   INSTANTIATE_STRING_TO_TYPE_HEX_AND_BASE64_TO_NUMBER_SINK(unsigned int)
   INSTANTIATE_STRING_TO_TYPE_HEX_AND_BASE64_TO_NUMBER_SINK(unsigned long)

   #define INSTANTIATE_PARSE_SEQUENCE_TYPE(Type)\
   INSTANTIATE_PARSE(std::vector<Type>::iterator)\
   INSTANTIATE_PARSE(std::vector<Type>::const_iterator)\
   INSTANTIATE_PARSE(std::deque<Type>::iterator)\
   INSTANTIATE_PARSE(std::deque<Type>::const_iterator)\
   INSTANTIATE_PARSE(std::list<Type>::iterator)\
   INSTANTIATE_PARSE(std::list<Type>::const_iterator)

   INSTANTIATE_PARSE_SEQUENCE_TYPE(char)
   INSTANTIATE_PARSE_SEQUENCE_TYPE(unsigned char)
   INSTANTIATE_PARSE_SEQUENCE_TYPE(short)
   INSTANTIATE_PARSE_SEQUENCE_TYPE(int)
   INSTANTIATE_PARSE_SEQUENCE_TYPE(long)
   INSTANTIATE_PARSE_SEQUENCE_TYPE(unsigned short)
   INSTANTIATE_PARSE_SEQUENCE_TYPE(unsigned int)
   INSTANTIATE_PARSE_SEQUENCE_TYPE(unsigned long)

} // namespace strtk

#endif
