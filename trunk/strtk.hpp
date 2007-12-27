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

   template<typename Sequence>
   struct add_to_sequence
   {
   public:
      add_to_sequence(Sequence& sequence) : sequence_(sequence){}

      template<typename T>
      void operator()(const T& t) { sequence_.push_back(t); }
   private:
      Sequence sequence_;
   };

   template<typename Tokenizer, typename Handler>
   unsigned int for_each_token(const std::string& buffer,
                               Tokenizer& tokenizer,
                               Handler& handler)
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
   inline unsigned int for_each_line(std::ifstream& stream, Handler& handler)
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
   inline unsigned int for_each_line(const std::string& file_name, Handler& handler)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line(stream,handler);
      else
         return 0;
   }

   template<typename Handler>
   inline unsigned int for_each_line_conditional(std::ifstream& stream, Handler& handler)
   {
      std::string buffer(1024,0x0);
      unsigned int result = 0;
      while(std::getline(stream,buffer))
      {
         result++;
         if(!handler(buffer))
         {
            return result;
         }
      }
      return result;
   }

   template<typename Handler>
   inline unsigned int for_each_line_conditional(const std::string& file_name, Handler& handler)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line(stream,handler);
      else
         return 0;
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

   struct remove_consecutives_inplace
   {
   public:

      template<typename ForwardIterator>
      remove_consecutives_inplace(const typename std::iterator_traits<ForwardIterator>::value_type& c,
                                  ForwardIterator begin,
                                  ForwardIterator end,
                                  unsigned int& removal_count)
      {
         if (0 == std::distance(begin,end)) return;
         ForwardIterator it1 = begin;
         ForwardIterator it2 = begin;
         typename std::iterator_traits<ForwardIterator>::value_type prev = *it1;
         removal_count = 0;
         while(it1 != end)
         {
            while((it1 != end) && ((c != (*it1)) || (prev != c)))
            {
               if (it1 != it2)
               {
                  *it2 = *it1;
               }
               prev = *it1;
               ++it1;
               ++it2;
            }
            while((it1 != end) && (c == (*it1)))
            {
               ++it1;
               ++removal_count;
            }
         }
      }

      template<typename ForwardIterator>
      remove_consecutives_inplace(const typename std::iterator_traits<ForwardIterator>::value_type c[],
                                  const unsigned int c_length,
                                  ForwardIterator begin,
                                  ForwardIterator end,
                                  unsigned int& removal_count)
      {
         if (0 == std::distance(begin,end)) return;
         ForwardIterator it1 = begin;
         ForwardIterator it2 = begin;
         typename std::iterator_traits<ForwardIterator>::value_type prev = *it1;
         removal_count = 0;
         while(it1 != end)
         {
            while((it1 != end) && (!exist_in_list((*it1),c,c_length) || !exist_in_list(prev,c,c_length)))
            {
               if (it1 != it2)
               {
                  *it2 = *it1;
               }
               prev = *it1;
               ++it1;
               ++it2;
            }
            while((it1 != end) && exist_in_list((*it1),c,c_length))
            {
               ++it1;
               ++removal_count;
            }
         }
      }

      remove_consecutives_inplace(const std::string::value_type c, std::string& s)
      {
         if (s.empty()) return;
         unsigned int removal_count = 0;
         remove_consecutives_inplace(c,s.begin(),s.end(),removal_count);
         if (removal_count > 0)
         {
            s.resize(s.size() - removal_count);
         }
      }

      remove_consecutives_inplace(const std::string::value_type c[],
                                  const unsigned int& c_length,
                                  std::string& s)
      {
         if (s.empty()) return;
         unsigned int removal_count = 0;
         remove_consecutives_inplace(c,c_length,s.begin(),s.end(),removal_count);
         if (removal_count > 0)
         {
            s.resize(s.size() - removal_count);
         }
      }

      template<typename ForwardIterator>
      remove_consecutives_inplace(ForwardIterator begin,
                                  ForwardIterator end,
                                  unsigned int& removal_count)
      {
         if (0 == std::distance(begin,end)) return;
         ForwardIterator it1 = (begin + 1);
         ForwardIterator it2 = (begin + 1);
         typename std::iterator_traits<ForwardIterator>::value_type prev = *begin;
         removal_count = 0;
         while(it1 != end)
         {
            while((it1 != end) && (prev != (*it1)))
            {
               if (it1 != it2)
               {
                  *it2 = *it1;
               }
               prev = *it1;
               ++it1;
               ++it2;
            }
            while((it1 != end) && (prev == (*it1)))
            {
               ++it1;
               ++removal_count;
            }
         }
      }

      remove_consecutives_inplace(std::string& s)
      {
         unsigned int removal_count = 0;
         remove_consecutives_inplace(s.begin(),s.end(),removal_count);
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

   template<typename Iterator>
   inline void replace(const typename std::iterator_traits<Iterator>::value_type& c1,
                       const typename std::iterator_traits<Iterator>::value_type& c2,
                       const Iterator data_begin,
                       const Iterator data_end)
   {
      for(Iterator it = begin; it != end; ++it)
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
      if (0 == std::distance(data_begin,data_end)) return false;

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
      bool operator()(const T& d) const { return d == delimiter_; }
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

   template<typename DelimiterPredicate,
            typename Iterator,
            typename OutputIterator>
   inline void split(const Iterator begin,
                     const Iterator end,
                     const DelimiterPredicate& delimiter,
                     OutputIterator& out)
   {
      if (0 == std::distance(begin,end)) return;
      Iterator it1 = begin;
      Iterator prev = begin;
      while(it1 != end)
      {
        if(delimiter(*it1))
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

   template<typename DelimiterPredicate,
            typename Iterator,
            typename OutputIterator>
   inline void split_with_compressed_delimiters(const Iterator begin,
                                                const Iterator end,
                                                const DelimiterPredicate& delimiter,
                                                OutputIterator& out)
   {
      if (0 == std::distance(begin,end)) return;
      Iterator it1 = begin;
      Iterator prev = begin;
      while(it1 != end)
      {
        if(delimiter(*it1))
        {
           if (std::distance(prev,it1) >= 1)
           {
              out = std::make_pair<Iterator,Iterator>(prev,it1);
              ++out;
           }
           do { ++it1; } while((it1 != end) && delimiter(*it1));
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

   template<typename DelimiterPredicate,
            typename OutputIterator>
   inline void split(const std::string& str,
                     const DelimiterPredicate& delimiter,
                     OutputIterator& out)
   {
      split(str.begin(),str.end(),delimiter,out);
   }

   template<typename OutputIterator>
   inline void split(const std::string& str,
                     const std::string::value_type delimiter,
                     OutputIterator& out)
   {
      split(str.begin(),str.end(),single_delimiter_predicate<std::string::value_type>(delimiter),out);
   }


   template<typename DelimiterPredicate,
            typename OutputIterator>
   inline void split_with_compressed_delimiters(const std::string& str,
                                                const DelimiterPredicate& delimiter,
                                                OutputIterator& out)
   {
      split_with_compressed_delimiters(str.begin(),str.end(),delimiter,out);
   }

   template<typename OutputIterator>
   inline void split_with_compressed_delimiters(const std::string& str,
                                                const std::string::value_type delimiter,
                                                OutputIterator& out)
   {
      split_with_compressed_delimiters(str.begin(),str.end(),single_delimiter_predicate<std::string::value_type>(delimiter),out);
   }

   template<typename Iterator, typename DelimiterPredicate>
   inline std::size_t count_tokens(Iterator begin, Iterator end, const DelimiterPredicate& delimiter_)
   {
      if (0 == std::distance(begin,end)) return 0;
      std::size_t count = 0;
      Iterator it1 = begin;
      Iterator prev = begin;
      while(it1 != end)
      {
        if(delimiter_(*it1))
        {
           ++count;
           ++it1;
           prev = it1;
        }
        else
           ++it1;
      }
      if (prev != it1)
      {
         ++count;
      }
      return count;
   }

   template<typename Iterator, typename DelimiterPredicate>
   inline std::size_t count_tokens_with_compressed_delimiters(Iterator begin,
                                                              Iterator end,
                                                              const DelimiterPredicate& delimiter)
   {
      if (0 == std::distance(begin,end)) return 0;
      std::size_t count = 0;
      Iterator it1 = begin;
      Iterator prev = begin;
      while(it1 != end)
      {
         if(delimiter(*it1))
         {
            if (std::distance(prev,it1) >= 1) ++count;
            do { ++it1; } while((it1 != end) && delimiter(*it1));
            prev = it1;
         }
         else
            ++it1;
      }
      if (prev != it1)
      {
         ++count;
      }
      return count;
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

   inline void convert_bin_to_hex(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      const std::size_t symbol_count = 16;
      const unsigned char hex_symbol[symbol_count] = {
                                                      '0','1','2','3','4','5','6','7',
                                                      '8','9','A','B','C','D','E','F'
                                                     };

      for(const unsigned char* it = begin; it != end; ++it)
      {
         *out = hex_symbol[((*it) >> 0x04) & 0x0F]; ++out;
         *out = hex_symbol[ (*it)          & 0x0F]; ++out;
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

   inline void convert_hex_to_bin(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      const std::size_t symbol_count = 256;
      const unsigned char hex_to_bin[symbol_count] = {
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                                      0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                                                     };


      for(const unsigned char* it = begin; it != end;)
      {
         (*out)  = hex_to_bin[(*it)] << 4; ++it;
         (*out) |= hex_to_bin[(*it)]     ; ++it;
         ++out;
      }
   }

   inline void convert_hex_to_bin(const char* begin, const char* end, char* out)
   {
      convert_hex_to_bin(reinterpret_cast<const unsigned char*>(begin),
                         reinterpret_cast<const unsigned char*>(end),
                         reinterpret_cast<unsigned char*>(out));
   }

   inline void convert_hex_to_bin(const std::string& binary_data, std::string& output)
   {
      output.resize(binary_data.size() >> 1);
      convert_hex_to_bin(binary_data.c_str(),binary_data.c_str() + binary_data.size(),const_cast<char*>(output.c_str()));
   }

   inline unsigned int convert_bin_to_base64(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      const std::size_t symbol_count = 64;
      const unsigned char bin_to_base64 [symbol_count]
             = {
                'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                '0','1','2','3','4','5','6','7','8','9','+','/'
               };

      std::size_t length = std::distance(begin,end);
      std::size_t rounds = length / 3;
      const unsigned char* it = begin;
      for(std::size_t i = 0; i < rounds; ++i)
      {
         unsigned int block  = *it << 16; ++it;
                      block |= *it <<  8; ++it;
                      block |= *it      ; ++it;

         *out = bin_to_base64[( block >> 18 ) & 0x3F]; ++out;
         *out = bin_to_base64[( block >> 12 ) & 0x3F]; ++out;
         *out = bin_to_base64[( block >>  6 ) & 0x3F]; ++out;
         *out = bin_to_base64[( block       ) & 0x3F]; ++out;
      }

      if ((rounds = (std::distance(begin,end) % 3)) > 0)
      {
         switch(rounds)
         {
            case 1 : {
                       unsigned int block  = (unsigned char) (*it) << 16;
                       *out = bin_to_base64[( block >> 18 ) & 0x3F]; ++out;
                       *out = bin_to_base64[( block >> 12 ) & 0x3F]; ++out;
                       *out = '='; ++out;
                       *out = '='; ++out;
                     }
                     break;

            case 2 : {
                        unsigned int block  = *it << 16; ++it;
                                     block |= *it <<  8; ++it;
                        *out = bin_to_base64[( block >> 18 ) & 0x3F]; ++out;
                        *out = bin_to_base64[( block >> 12 ) & 0x3F]; ++out;
                        *out = bin_to_base64[( block >>  6 ) & 0x3F]; ++out;
                        *out = '='; ++out;
                     }
                     break;
         }
      }
      return static_cast<unsigned int>((length / 3) * 4) + ((length % 3) > 0 ? 4 : 0);
   }

   unsigned int convert_bin_to_base64(const char* begin, const char* end, char* out)
   {
      return convert_bin_to_base64(reinterpret_cast<const unsigned char*>(begin),
                                   reinterpret_cast<const unsigned char*>(end),
                                   reinterpret_cast<unsigned char*>(out));
   }

   inline void convert_bin_to_base64(const std::string& binary_data, std::string& output)
   {
      output.resize(binary_data.size() << 1);
      std::size_t resize = convert_bin_to_base64(binary_data.c_str(),binary_data.c_str() + binary_data.size(),const_cast<char*>(output.c_str()));
      output.resize(resize);
   }

   inline unsigned int convert_base64_to_bin(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      const std::size_t symbol_count = 256;
      const unsigned char base64_to_bin[symbol_count] = {
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
                                                 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
                                                 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                                                 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                                                 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
                                                 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
                                                 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
                                                 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
                                                 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                                               };


      std::size_t length = std::distance(begin,end);
      std::size_t rounds = length / 4;
      const unsigned char* it = begin;
      for(std::size_t i = 0; i < rounds; ++i)
      {
         unsigned int block  = base64_to_bin[*it] << 18; ++it;
                      block |= base64_to_bin[*it] << 12; ++it;
                      block |= base64_to_bin[*it] <<  6; ++it;
                      block |= base64_to_bin[*it];       ++it;

         *out = static_cast<unsigned char>(( block >> 16 ) & 0xFF); ++out;
         *out = static_cast<unsigned char>(( block >>  8 ) & 0xFF); ++out;
         *out = static_cast<unsigned char>(( block       ) & 0xFF); ++out;
      }

      if ((rounds = (length % 4)) > 0)
      {
         switch(rounds)
         {
            case 2 : {
                        unsigned int block  = base64_to_bin[*it] << 18; ++it;
                                     block |= base64_to_bin[*it] << 12; ++it;
                        *out = static_cast<unsigned char>(( block >> 16 ) & 0xFF); ++out;
                        return static_cast<unsigned int>((3 * length) / 4 + 1);
                     }
                     break;

            case 3 : {
                        unsigned int block  = base64_to_bin[*it] << 18; ++it;
                                     block |= base64_to_bin[*it] << 12; ++it;
                                     block |= base64_to_bin[*it] <<  6; ++it;
                        *out = static_cast<unsigned char>(( block >> 16 ) & 0xFF); ++out;
                        *out = static_cast<unsigned char>(( block >>  8 ) & 0xFF); ++out;
                        return static_cast<unsigned int>((3 * length) / 4 + 2);
                     }
                     break;
         }
      }
      return static_cast<unsigned int>((3 * length) / 4);
   }

   inline unsigned int convert_base64_to_bin(const char* begin, const char* end, char* out)
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

}

#endif
