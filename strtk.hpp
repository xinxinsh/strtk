/*
 *******************************************************************
 *                                                                 *
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Author: Arash Partow - 2002                                     *
 * URL: http://www.partow.net                                      *
 *                                                                 *
 * Copyright notice:                                               *
 * Free use of the String Tool Kit Library is permitted under the  *
 * guidelines and in accordance with the most current version of   *
 * the Common Public License.                                      *
 * http://www.opensource.org/licenses/cpl.php                      *
 *                                                                 *
 *******************************************************************
*/


#ifndef INCLUDE_STRTK_HPP
#define INCLUDE_STRTK_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>

#include <boost/lexical_cast.hpp>


namespace strtk
{

   template<typename Tokenizer, typename Handler>
   unsigned int for_each_token(const std::string& buffer,
                               Tokenizer& tokenizer,
                               const Handler& handler)
   {
      unsigned int result = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      for(typename Tokenizer::iterator it = tokenizer.begin();
          it != tokenizer.end();
          ++it, ++result)
      {
         handler(*it);
      }
      return result;
   }

   template<typename Handler>
   unsigned int for_each_line(std::ifstream& stream, const Handler& handler)
   {
      std::string buffer(1024,0x0);
      unsigned int result = 0;
      while(std::getline(stream,buffer))
      {
         result++;
         handler(buffer);
      }
      return result;
   }

   template<typename Handler>
   unsigned int for_each_line(const std::string& file_name, const Handler& handler)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line(stream,handler);
      else
         return 0;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9, typename T10>
   inline unsigned int parse(const std::string& buffer,
                             Tokenzier& tokenizer,
                             T1& t1, T2& t2, T3& t3, T4& t4,
                             T5& t5, T6& t6, T7& t7, T8& t8,
                             T9& t9, T10& t10)
   {
      unsigned int result = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
       t1 = boost::lexical_cast< T1>(*it); ++it; ++result;
       t2 = boost::lexical_cast< T2>(*it); ++it; ++result;
       t3 = boost::lexical_cast< T3>(*it); ++it; ++result;
       t4 = boost::lexical_cast< T4>(*it); ++it; ++result;
       t5 = boost::lexical_cast< T5>(*it); ++it; ++result;
       t6 = boost::lexical_cast< T6>(*it); ++it; ++result;
       t7 = boost::lexical_cast< T7>(*it); ++it; ++result;
       t8 = boost::lexical_cast< T8>(*it); ++it; ++result;
       t9 = boost::lexical_cast< T9>(*it); ++it; ++result;
      t10 = boost::lexical_cast<T10>(*it); ++it; ++result;
      return result;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9>
   inline unsigned int parse(const std::string& buffer,
                             Tokenzier& tokenizer,
                             T1& t1, T2& t2, T3& t3, T4& t4,
                             T5& t5, T6& t6, T7& t7, T8& t8,
                             T9& t9)
   {
      unsigned int result = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(*it); ++it; ++result;
      t2 = boost::lexical_cast<T2>(*it); ++it; ++result;
      t3 = boost::lexical_cast<T3>(*it); ++it; ++result;
      t4 = boost::lexical_cast<T4>(*it); ++it; ++result;
      t5 = boost::lexical_cast<T5>(*it); ++it; ++result;
      t6 = boost::lexical_cast<T6>(*it); ++it; ++result;
      t7 = boost::lexical_cast<T7>(*it); ++it; ++result;
      t8 = boost::lexical_cast<T8>(*it); ++it; ++result;
      t9 = boost::lexical_cast<T9>(*it); ++it; ++result;
      return result;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8>
   inline unsigned int parse(const std::string& buffer,
                             Tokenzier& tokenizer,
                             T1& t1, T2& t2, T3& t3, T4& t4,
                             T5& t5, T6& t6, T7& t7, T8& t8)
   {
      unsigned int result = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(*it);
      ++it; ++result;
      t2 = boost::lexical_cast<T2>(*it); ++it; ++result;
      t3 = boost::lexical_cast<T3>(*it); ++it; ++result;
      t4 = boost::lexical_cast<T4>(*it); ++it; ++result;
      t5 = boost::lexical_cast<T5>(*it); ++it; ++result;
      t6 = boost::lexical_cast<T6>(*it); ++it; ++result;
      t7 = boost::lexical_cast<T7>(*it); ++it; ++result;
      t8 = boost::lexical_cast<T8>(*it); ++it; ++result;
      return result;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7>
   inline unsigned int parse(const std::string& buffer,
                             Tokenzier& tokenizer,
                             T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7)
   {
      unsigned int result = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(*it); ++it; ++result;
      t2 = boost::lexical_cast<T2>(*it); ++it; ++result;
      t3 = boost::lexical_cast<T3>(*it); ++it; ++result;
      t4 = boost::lexical_cast<T4>(*it); ++it; ++result;
      t5 = boost::lexical_cast<T5>(*it); ++it; ++result;
      t6 = boost::lexical_cast<T6>(*it); ++it; ++result;
      t7 = boost::lexical_cast<T7>(*it); ++it; ++result;
      return result;
   }


   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5,typename T6 >
   inline unsigned int parse(const std::string& buffer,
                             Tokenzier& tokenizer,
                             T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6)
   {
      unsigned int result = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(*it); ++it; ++result;
      t2 = boost::lexical_cast<T2>(*it); ++it; ++result;
      t3 = boost::lexical_cast<T3>(*it); ++it; ++result;
      t4 = boost::lexical_cast<T4>(*it); ++it; ++result;
      t5 = boost::lexical_cast<T5>(*it); ++it; ++result;
      t6 = boost::lexical_cast<T6>(*it); ++it; ++result;
      return result;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5 >
   inline unsigned int parse(const std::string& buffer,
                             Tokenzier& tokenizer,
                             T1& t1, T2& t2, T3& t3, T4& t4, T5& t5)
   {
      unsigned int result = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(*it); ++it; ++result;
      t2 = boost::lexical_cast<T2>(*it); ++it; ++result;
      t3 = boost::lexical_cast<T3>(*it); ++it; ++result;
      t4 = boost::lexical_cast<T4>(*it); ++it; ++result;
      t5 = boost::lexical_cast<T5>(*it); ++it; ++result;
      return result;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4>
   inline unsigned int parse(const std::string& buffer,
                             Tokenzier& tokenizer,
                             T1& t1, T2& t2, T3& t3, T4& t4)
   {
      unsigned int result = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(*it); ++it; ++result;
      t2 = boost::lexical_cast<T2>(*it); ++it; ++result;
      t3 = boost::lexical_cast<T3>(*it); ++it; ++result;
      t4 = boost::lexical_cast<T4>(*it); ++it; ++result;
      return result;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3 >
   inline unsigned int parse(const std::string& buffer,
                             Tokenzier& tokenizer,
                             T1& t1, T2& t2, T3& t3)
   {
      unsigned int result = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(*it); ++it; ++result;
      t2 = boost::lexical_cast<T2>(*it); ++it; ++result;
      t3 = boost::lexical_cast<T3>(*it); ++it; ++result;
      return result;
   }

   template<typename Tokenzier, typename T1, typename T2>
   inline unsigned int parse(const std::string& buffer,
                             Tokenzier& tokenizer,
                             T1& t1, T2& t2)
   {
      unsigned int result = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(*it); ++it; ++result;
      t2 = boost::lexical_cast<T2>(*it); ++it; ++result;
      return result;
   }

   template<typename Tokenzier, typename T>
   inline unsigned int parse(const std::string& buffer,
                             Tokenzier& tokenizer,
                             T& t)
   {
      unsigned int result = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t = boost::lexical_cast<T>(*it); ++it; ++result;
      return result;
   }

   template< typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9, typename T10 >
   inline void construct(const std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                         const T9& t9, const T10& t10)
   {
      output += boost::lexical_cast<std::string>(t1);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t2);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t3);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t4);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t5);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t6);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t7);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t8);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t9);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t10);
   }

   template< typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9 >
   inline void construct(const std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                         const T9& t9)
   {
      output += boost::lexical_cast<std::string>(t1);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t2);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t3);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t4);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t5);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t6);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t7);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t8);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t9);
   }

   template< typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8>
   inline void construct(const std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8)
   {
      output += boost::lexical_cast<std::string>(t1);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t2);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t3);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t4);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t5);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t6);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t7);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t8);
   }

   template< typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7 >
   inline void construct(const std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7)
   {
      output += boost::lexical_cast<std::string>(t1);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t2);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t3);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t4);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t5);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t6);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t7);
   }


   template< typename T1, typename T2, typename T3, typename T4,
             typename T5,typename T6>
   inline void construct(const std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6)
   {
      output += boost::lexical_cast<std::string>(t1);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t2);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t3);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t4);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t5);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t6);
   }

   template< typename T1, typename T2, typename T3, typename T4,
             typename T5 >
   inline void construct(const std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5)
   {
      output += boost::lexical_cast<std::string>(t1);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t2);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t3);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t4);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t5);
   }

   template< typename T1, typename T2, typename T3, typename T4 >
   inline void construct(const std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4)
   {
      output += boost::lexical_cast<std::string>(t1);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t2);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t3);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t4);
   }

   template< typename T1, typename T2, typename T3 >
   inline void construct(const std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3)
   {
      output += boost::lexical_cast<std::string>(t1);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t2);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t3);
   }

   template< typename T1, typename T2 >
   inline void construct(const std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2)
   {
      output += boost::lexical_cast<std::string>(t1);
      output += delimiter;
      output += boost::lexical_cast<std::string>(t2);
   }

   struct remove_inplace
   {
   public:

      template<typename ForwardIterator>
      remove_inplace(const typename std::iterator_traits<ForwardIterator>::value_type& c,
                     ForwardIterator begin,
                     ForwardIterator end,
                     unsigned int& removal_count)
      {
         ForwardIterator it1 = begin;
         ForwardIterator it2 = begin;
         removal_count = 0;
         while(it1 != end)
         {
            while((it1 != end) && (c != (*it1)))
            {
               if (it1 != it2)
               {
                  *it2 = *it1;
               }
               ++it1;
               ++it2;
            }
            while((it1 != end) && (c == (*it1)))
            {
               ++it1;
               removal_count++;
            }
         }
      }

      template<typename ForwardIterator>
      remove_inplace(const typename std::iterator_traits<ForwardIterator>::value_type c[],
                     const unsigned int c_length,
                     ForwardIterator begin,
                     ForwardIterator end,
                     unsigned int& removal_count)
      {
         ForwardIterator it1 = begin;
         ForwardIterator it2 = begin;
         removal_count = 0;
         while(it1 != end)
         {
            while((it1 != end) && !exist_in_list((*it1),c,c_length))
            {
               if (it1 != it2)
               {
                  *it2 = *it1;
               }
               ++it1;
               ++it2;
            }
            while((it1 != end) && exist_in_list((*it1),c,c_length))
            {
               ++it1;
               removal_count++;
            }
         }
      }

      remove_inplace(const std::string::value_type c, std::string& s)
      {
         unsigned int removal_count = 0;
         remove_inplace(c,s.begin(),s.end(),removal_count);
         if (removal_count > 0)
         {
            s.resize(s.size() - removal_count);
         }
      }

      remove_inplace(const std::string::value_type c[],
                     const unsigned int& c_length,
                     std::string& s)
      {
         unsigned int removal_count = 0;
         remove_inplace(c,c_length,s.begin(),s.end(),removal_count);
         if (removal_count > 0)
         {
            s.resize(s.size() - removal_count);
         }
      }

   private:

      template<typename T>
      inline bool exist_in_list(const T c,
                                const T list[],
                                const unsigned int& length)
      {
         for(unsigned int i = 0; i < length; ++i)
         {
            if (list[i] == c) return true;
         }
         return false;
      }
   };

   inline void replace(const unsigned char c1,
                       const unsigned char c2,
                       std::string& s)
   {
      for(std::string::iterator it = s.begin(); it != s.end(); ++it)
      {
         if (c1 == *it) *it = c2;
      }
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
            Credits: Adapted entirely from code provided
            by Jack Handy - 2001 on CodeProject (jakkhandy@hotmail.com)
         */

         Iterator d_it = data_begin;
         Iterator p_it = pattern_begin;
         Iterator c_it = data_begin;
         Iterator m_it = data_begin;

         while ((d_it != data_end) && ((*p_it) != zero_or_more))
         {
            if (((*p_it)!= (*d_it)) && (zero_or_one != (*p_it)))
            {
               return false;
            }
            p_it++;
            d_it++;
         }

         while (d_it != data_end)
         {
            if (zero_or_more == (*p_it))
            {
               if (!*++p_it)
               {
                  return true;
               }
               m_it = p_it;
               c_it = d_it + 1;
            }
            else if (((*p_it) == (*d_it)) || (zero_or_one == (*p_it)))
            {
               p_it++;
               d_it++;
            }
            else
            {
               p_it = m_it;
               d_it = c_it++;
            }
         }
         while ((p_it != pattern_end) && (zero_or_more == (*p_it))) p_it++;
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

   template<typename T>
   class range_adapter
   {
   public:

      typedef T value_type;
      typedef T* iterator;
      typedef const iterator const_iterator;

      range_adapter(T* const begin,T* const end)
      : begin_(begin),
        end_(end){}

      range_adapter(T* const begin, const std::size_t length)
      : begin_(begin),
        end_(begin_ + length){}

      const_iterator begin() const { return begin_; }
      const_iterator end() const { return end_; }

      iterator begin() { return begin_; }
      iterator end() { return end_; }

   private:

      iterator begin_;
      iterator end_;
   };

   template<typename T>
   struct single_delimiter_predicate
   {
   public:
      single_delimiter_predicate(const T& d) : delimiter_(d) {}
      bool operator()(const T& d) { return d == delimiter_; }
   private:
      T delimiter_;
   };

   template<typename T>
   struct multiple_delimiter_predicate
   {
   public:

      multiple_delimiter_predicate(const T* d_begin,const T* d_end)
      {
         length_ = std::distance(d_begin,d_end);
         delimiter_ = new T[length_];
         delimiter_end_ = delimiter_ + length_;
         std::copy(d_begin,d_end, delimiter_);
      }

      multiple_delimiter_predicate(const T d[], const std::size_t& length)
      {
         length_ = length;
         delimiter_ = new T[length_];
         delimiter_end_ = delimiter_ + length_;
         std::copy(d,d + length, delimiter_);
      }

      template<typename Iterator>
      multiple_delimiter_predicate(const Iterator begin,const Iterator end)
      {
         length_ = std::distance(begin,end);
         delimiter_ = new T[length_];
         delimiter_end_ = delimiter_ + length_;
         std::copy(begin,end, delimiter_);
      }

     ~multiple_delimiter_predicate() { delete[] delimiter_; }

      bool operator()(const T& d) const
      {
         return (std::find(delimiter_,delimiter_end_,d) != delimiter_end_);
      }

   private:
      T* delimiter_;
      T* delimiter_end_;
      std::size_t length_;
   };

   template<typename T,
            typename DelimiterPredicate,
            typename Iterator = T*>
   class splitter
   {
   public:

      splitter(const DelimiterPredicate& delimiter) : delimiter_(delimiter){}

      void assign(const Iterator begin, const Iterator end)
      {
         begin_ = begin;
         end_   = end;
      }

      template<typename OutputIterator>
      void split(OutputIterator out)
      {
         Iterator it1 = begin_;
         Iterator prev = begin_;
         while(it1 != end_)
         {
           if(delimiter_(*it1))
           {
              out = std::make_pair<Iterator,Iterator>(prev,it1);
              ++out;
              ++it1;
              prev = it1;
           }
           else
              ++it1;
         }
         if (prev != it1)
         {
            out = std::make_pair<Iterator,Iterator>(prev,it1);
            ++out;
         }
      }

      template<typename OutputIterator>
      void split_and_compress_delimiters(OutputIterator out)
      {
         Iterator it1  = begin_;
         Iterator prev = begin_;
         while(it1 != end_)
         {
           if(delimiter_(*it1))
           {
              if (std::distance(prev,it1) >= 1)
              {
                 out = std::make_pair<Iterator,Iterator>(prev,it1);
                 ++out;
              }
              do { ++it1; } while((it1 != end_) && delimiter_(*it1));
              prev = it1;
           }
           else
              ++it1;
         }
         if (prev != it1)
         {
            out = std::make_pair<Iterator,Iterator>(prev,it1);
            ++out;
         }
      }

   private:
      const DelimiterPredicate& delimiter_;
      Iterator begin_;
      Iterator end_;
   };

}

#endif
