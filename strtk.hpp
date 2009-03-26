/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * Author: Arash Partow - 2002                                     *
 * URL: http://www.partow.net/programming/strtk/index.html         *
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

#include <cstddef>
#include <cctype>
#include <iterator>
#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <algorithm>

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/random.hpp>

namespace strtk
{

   template<typename Tokenizer, class Function>
   unsigned int for_each_token(const std::string& buffer,
                               Tokenizer& tokenizer,
                               Function function)
   {
      unsigned int token_count = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenizer::iterator it = tokenizer.begin();
      while(it != tokenizer.end())
      {
         function(*it);
         ++it;
         ++token_count;
      }
      return token_count;
   }

   template<class Function>
   inline unsigned int for_each_line(std::ifstream& stream, Function function)
   {
      std::string buffer(4096,0x0);
      unsigned int line_count = 0;
      while(std::getline(stream,buffer))
      {
         ++line_count;
         function(buffer);
      }
      return line_count;
   }

   template<class Function>
   inline unsigned int for_each_line(const std::string& file_name, Function function)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line(stream,function);
      else
         return 0;
   }

   template<class Function>
   inline unsigned int for_each_line_conditional(std::ifstream& stream, Function function)
   {
      std::string buffer(4096,0x0);
      unsigned int line_count = 0;
      while(std::getline(stream,buffer))
      {
         ++line_count;
         if(!function(buffer))
         {
            return line_count;
         }
      }
      return line_count;
   }

   template<class Function>
   inline unsigned int for_each_line_conditional(const std::string& file_name, Function function)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line(stream,function);
      else
         return 0;
   }

   template<typename Predicate, typename InputIterator, typename OutputIterator>
   void copy_if(Predicate predicate, InputIterator begin, InputIterator end, OutputIterator out)
   {
      InputIterator it = begin;
      while(it != end)
      {
         if (predicate(*it))
         {
            *(out++)= *(it++);
         }
      }
   }

   template<typename Predicate, typename InputIterator, typename OutputIterator>
   InputIterator copy_while(Predicate predicate, InputIterator begin, InputIterator end, OutputIterator out)
   {
      InputIterator it = begin;
      while(it != end)
      {
         if (!predicate(*it)) break;
         *(out++)= *(it++);
      }
      return it;
   }

   template<typename T>
   struct single_delimiter_predicate
   {
   public:
      typedef T value_type;
      single_delimiter_predicate(const T& d) : delimiter_(d) {}
      bool operator()(const T& d) const { return d == delimiter_; }
   private:
      T delimiter_;
   };

   template<typename T>
   struct multiple_delimiter_predicate
   {
   public:

      typedef T value_type;

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

      bool operator()(const unsigned char& c) const
      {
         return (1 == delimiter_table_[c]);
      }

      bool operator()(const char& c) const
      {
         return operator()(static_cast<unsigned char>(c));
      }

   private:

      template<typename Iterator>
      void setup_delimiter_table(const Iterator begin,const Iterator end)
      {
         for(unsigned int i = 0; i < 0xFF; ++i) delimiter_table_[i] = 0;
         for (Iterator it = begin; it != end; ++it) delimiter_table_[static_cast<unsigned char>(*it)] = 1;
      }

      value_type delimiter_table_[0xFF];
   };

   template<typename Iterator, class Predicate>
   inline unsigned int remove_inplace(Predicate predicate,
                                      Iterator begin,
                                      Iterator end)
   {
      Iterator it1 = begin;
      Iterator it2 = begin;
      unsigned int removal_count = 0;
      while(it1 != end)
      {
         while((it1 != end) && !predicate(*it1))
         {
            if (it1 != it2)
            {
               *it2 = *it1;
            }
            ++it1;
            ++it2;
         }
         while((it1 != end) && predicate(*it1))
         {
            ++it1;
            ++removal_count;
         }
      }
      return removal_count;
   }

   inline void remove_inplace(const std::string::value_type c, std::string& s)
   {
      unsigned int removal_count = remove_inplace(single_delimiter_predicate<std::string::value_type>(c),s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<class Predicate>
   inline void remove_inplace(Predicate predicate,
                              std::string& s)
   {
      unsigned int removal_count = remove_inplace(predicate,s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Iterator, class Predicate>
   inline unsigned int remove_consecutives_inplace(Predicate predicate,
                                                   Iterator begin,
                                                   Iterator end)
   {
      if (0 == std::distance(begin,end)) return 0;
      Iterator it1 = (begin + 1);
      Iterator it2 = (begin + 1);
      typename std::iterator_traits<Iterator>::value_type prev = *begin;
      unsigned int removal_count = 0;
      while(it1 != end)
      {
         while((it1 != end) && (!predicate(*it1) || !predicate(prev)))
         {
            if (it1 != it2)
            {
               *it2 = *it1;
            }
            prev = *it1;
            ++it1;
            ++it2;
         }
         while((it1 != end) && predicate(*it1))
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
      unsigned int removal_count = remove_consecutives_inplace(single_delimiter_predicate<std::string::value_type>(c),s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<class Predicate>
   inline void remove_consecutives_inplace(Predicate predicate, std::string& s)
   {
      if (s.empty()) return;
      unsigned int removal_count = remove_consecutives_inplace(predicate,s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Iterator>
   inline unsigned int remove_consecutives_inplace(Iterator begin, Iterator end)
   {
      if (0 == std::distance(begin,end)) return 0;
      Iterator it1 = (begin + 1);
      Iterator it2 = (begin + 1);
      typename std::iterator_traits<Iterator>::value_type prev = *begin;
      unsigned int removal_count = 0;
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
      return removal_count;
   }

   inline void remove_consecutives_inplace(std::string& s)
   {
      unsigned int removal_count = remove_consecutives_inplace(s.begin(),s.end());

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
         by Jack Handy - 2001 CodeProject (jakkhandy@hotmail.com)
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
         ++p_it;
         ++d_it;
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

   inline bool case_insensitive_match(const std::string::const_iterator begin1, const std::string::const_iterator end1,
                                      const std::string::const_iterator begin2, const std::string::const_iterator end2)
   {
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return false;
      }
      std::string::const_iterator it1 = begin1;
      std::string::const_iterator it2 = begin2;
      while(it1 != end1)
      {
         if (std::toupper(*it1) != std::toupper(*it2))
         {
            return false;
         }
         ++it1;
         ++it2;
      }
      return true;
   }

   inline bool case_insensitive_match(const std::string& s1, const std::string s2)
   {
      return case_insensitive_match(s1.begin(),s1.end(),s2.begin(),s2.end());
   }

   template<typename Iterator>
   inline bool case_insensitive_match(const std::string& s, const Iterator begin, const Iterator end)
   {
      for(const std::string* it = begin; it != end; ++it)
      {
         if (case_insensitive_match(s,*it))
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

         tokenizer_iterator(const iterator begin,
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

         tokenizer_iterator& operator++()
         {
            if (it_ != end_)
            {
               prev_ = it_;
            }

            while (it_ != end_)
            {
               if(predicate_(*it_))
               {
                  curr_tok_begin_ = prev_;
                  curr_tok_end_ = it_;
                  if (compress_delimiters_)
                     while((++it_ != end_) && predicate_(*it_));
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

         tokenizer_iterator operator++(int)
         {
            tokenizer_iterator* tmp = this;
            this->operator++();
            return (*tmp);
         }

         tokenizer_iterator& operator+=(const int inc)
         {
            for(int i = 0; i < inc; ++i) ++(*this);
            return *this;
         }

         T operator*() const
         {
            return std::make_pair<iterator,iterator>(curr_tok_begin_,curr_tok_end_);
         }

         bool operator==(const tokenizer_iterator& it)
         {
            return (it_   == it.it_  ) &&
                   (prev_ == it.prev_) &&
                   (end_  == it.end_ );
         }

         bool operator!=(const tokenizer_iterator& it)
         {
            return !this->operator==(it);
         }

         tokenizer_iterator& operator=(const tokenizer_iterator& it)
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

      tokenizer(const Iterator begin,
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

      tokenizer(const std::string& s,
                const DelimiterPredicate& predicate,
                const bool compress_delimiters = false)
      : predicate_(predicate),
        begin_(s.begin()),
        end_(s.end()),
        begin_itr_(begin_,end_,predicate_,compress_delimiters),
        end_itr_(end_,end_,predicate_,compress_delimiters),
        compress_delimiters_(compress_delimiters)
      {}

      tokenizer& operator=(const tokenizer& t)
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

      void assign(const std::string& s) const
      {
         assign(s.begin(),s.end());
      }

      void assign(const std::string& s)
      {
         assign(s.begin(),s.end());
      }

      void assign(Iterator begin,Iterator end)
      {
        begin_ = begin;
        end_ = end;
        begin_itr_ = iterator(begin_,end_,predicate_,compress_delimiters_);
        end_itr_ = iterator(end_,end_,predicate_,compress_delimiters_);
      }

      const_iterator_ref begin() const
      {
         return begin_itr_;
      }

      const_iterator_ref end() const
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

   template<typename DelimiterPredicate = single_delimiter_predicate<std::string::value_type> >
   struct std_string_tokenizer
   {
      typedef DelimiterPredicate predicate_type;
      typedef tokenizer<std::string::const_iterator,DelimiterPredicate> type;
      typedef std::pair<std::string::const_iterator , std::string::const_iterator> iterator_type;
   };

   template<typename T>
   class range_adapter
   {
   public:

      typedef T value_type;
      typedef T* iterator;
      typedef const iterator const_iterator;

      range_adapter(T* const begin, T* const end)
      : begin_(begin),
        end_(end){}

      range_adapter(T* const begin, const std::size_t length)
      : begin_(begin),
        end_(begin_ + length){}

      iterator begin() { return begin_; }
      iterator end() { return end_; }

   private:
      iterator begin_;
      iterator end_;
   };

   template<class Sequence>
   class range_to_string_back_inserter_iterator : public std::iterator<std::output_iterator_tag, void, void,void, void>
   {
   public:

      explicit range_to_string_back_inserter_iterator(Sequence& sequence)
      : sequence_(sequence) {}

      range_to_string_back_inserter_iterator(const range_to_string_back_inserter_iterator& it)
      : sequence_(it.sequence_) {}

      range_to_string_back_inserter_iterator& operator=(const range_to_string_back_inserter_iterator& it)
      {
         if (this != &it)
         {
            this->sequence_ = it.sequence_;
         }
         return *this;
      }

      range_to_string_back_inserter_iterator& operator=(const std_string_tokenizer<std::string::value_type>::iterator_type& r)
      {
         sequence_.push_back(std::string(r.first,r.second));
         return (*this);
      }

      void operator()(const std_string_tokenizer<std::string::value_type>::iterator_type& r)
      {
         sequence_.push_back(std::string(r.first,r.second));
      }

      range_to_string_back_inserter_iterator& operator*()    { return (*this); }
      range_to_string_back_inserter_iterator& operator++()   { return (*this); }
      range_to_string_back_inserter_iterator operator++(int) { return (*this); }

   private:
      Sequence& sequence_;
   };

   template<class Sequence>
   inline range_to_string_back_inserter_iterator<Sequence> range_to_string_back_inserter(Sequence& sequence_)
   {
      return (range_to_string_back_inserter_iterator<Sequence>(sequence_));
   }

   template<class Sequence>
   class back_inserter_with_valuetype_iterator : public std::iterator<std::output_iterator_tag, typename Sequence::value_type, void,void, void>
   {
   public:

      explicit back_inserter_with_valuetype_iterator(Sequence& sequence)
      : sequence_(sequence) {}

      back_inserter_with_valuetype_iterator(const back_inserter_with_valuetype_iterator& it)
      : sequence_(it.sequence_) {}

      back_inserter_with_valuetype_iterator& operator=(const back_inserter_with_valuetype_iterator& it)
      {
         if (this != &it)
         {
            this->sequence_ = it.sequence_;
         }
         return *this;
      }

      back_inserter_with_valuetype_iterator& operator=(const typename Sequence::value_type& v)
      {
         sequence_.push_back(v);
         return (*this);
      }

      void operator()(const typename Sequence::value_type& v)
      {
         sequence_.push_back(v);
      }

      back_inserter_with_valuetype_iterator& operator*()    { return (*this); }
      back_inserter_with_valuetype_iterator& operator++()   { return (*this); }
      back_inserter_with_valuetype_iterator operator++(int) { return (*this); }

   private:
      Sequence& sequence_;
   };

   template<class Sequence>
   inline back_inserter_with_valuetype_iterator<Sequence> back_inserter_with_valuetype(Sequence& sequence_)
   {
      return (back_inserter_with_valuetype_iterator<Sequence>(sequence_));
   }

   template<typename DelimiterPredicate,
            typename Iterator,
            typename OutputIterator>
   inline std::size_t split(const Iterator begin,
                            const Iterator end,
                            const DelimiterPredicate& delimiter,
                            OutputIterator out,
                            const bool compress_delimiters = false)
   {
      if (0 == std::distance(begin,end)) return 0;
      Iterator it = begin;
      Iterator prev = it;
      std::size_t match_count = 0;
      while(it != end)
      {
        if(delimiter(*it))
        {
           *(out++) = std::make_pair<Iterator,Iterator>(prev,it);
           ++match_count;
           if (!compress_delimiters)
              ++it;
           else
              while(((++it) != end) && delimiter(*it));
           prev = it;
        }
        else
           ++it;
      }
      if ((prev != it) || delimiter(*(it - 1)))
      {
         *(out++) = std::make_pair<Iterator,Iterator>(prev,it);
         ++match_count;
      }
      return match_count;
   }

   template<typename DelimiterPredicate,
            typename OutputIterator>
   inline std::size_t split(const std::string& str,
                            const DelimiterPredicate& delimiter,
                            OutputIterator out,
                            const bool compress_delimiters = false)
   {
      return split(str.begin(),str.end(),delimiter,out,compress_delimiters);
   }

   template<typename OutputIterator>
   inline std::size_t split(const std::string& str,
                            const std::string::value_type delimiter,
                            OutputIterator out,
                            const bool compress_delimiters = false)
   {
      return split(str.begin(),str.end(),single_delimiter_predicate<std::string::value_type>(delimiter),out,compress_delimiters);
   }

   const std::string uri_expression  ("((https?|ftp)\\://((\\[?(\\d{1,3}\\.){3}\\d{1,3}\\]?)|(([-a-zA-Z0-9]+\\.)+[a-zA-Z]{2,4}))(\\:\\d+)?(/[-a-zA-Z0-9._?,+&amp;%$#=~\\\\]+)*/?)");
   const std::string email_expression("([\\w\\-\\.]+)@((\\[([0-9]{1,3}\\.){3}[0-9]{1,3}\\])|(([\\w\\-]+\\.)+)([a-zA-Z]{2,4}))");
   const std::string ip_expression   ("(([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+))");

   template<typename InputIterator, typename OutputIterator>
   std::size_t split(const boost::regex& expression,
                     const InputIterator begin,
                     const InputIterator end,
                     OutputIterator out)
   {
      boost::sregex_iterator it(begin,end,expression);
      boost::sregex_iterator it_end;
      std::string token(4096,0x0);
      std::size_t match_count = 0;
      while(it_end != it)
      {
         token.assign((*it)[0].first,(*it)[0].second);
         *(out++) = token;
         ++it;
         ++match_count;
      }
      return match_count;
   }

   template<typename InputIterator, typename OutputIterator>
   std::size_t split(const std::string& expression,
                     const InputIterator begin,
                     const InputIterator end,
                     OutputIterator out)
   {
      const boost::regex regex_expression(expression);
      return split(regex_expression,begin,end,out);
   }

   template<typename OutputIterator>
   std::size_t split(const std::string& expression,
                     const std::string text,
                     OutputIterator out)
   {
      return split(expression,text.begin(),text.end(),out);
   }

   template<typename OutputIterator>
   std::size_t split(const boost::regex& expression,
                     const std::string& text,
                     OutputIterator out)
   {
      return split(expression,text.begin(),text.end(),out);
   }

   template<const std::size_t offset_list_size>
   class offset_predicate
   {
   public:
      offset_predicate(const int offset_list[], const bool rotate = false)
      : rotate_(rotate)
      {
         current_index_ = 0;
         std::copy(offset_list, offset_list + offset_list_size, offset_list_);
         offset_list_[offset_list_size] = 0;
      }

      bool operator!() const
      {
         return offset_list_size == 0;
      }

      void reset() const
      {
         current_index_ = 0;
      }

      std::size_t size() const
      {
         return offset_list_size;
      }

      int next() const
      {
         int result = offset_list_[current_index_++];
         if (rotate_ && current_index_ >= offset_list_size) current_index_ = 0;
         return result;
      }

   private:
      bool rotate_;
      mutable std::size_t current_index_;
      int offset_list_[offset_list_size + 1];
   };

   template<typename OffsetPredicate,
            typename Iterator,
            typename OutputIterator>
   inline std::size_t offset_splitter(const Iterator begin,
                                      const Iterator end,
                                      const OffsetPredicate& offset,
                                      OutputIterator out)
   {
      std::size_t length = 0;
      if (0 == (length = std::distance(begin,end))) return 0;
      Iterator it = begin;
      Iterator prev = it;
      std::size_t match_count = 0;
      int offset_length = 0;
      std::size_t inc_amnt = 0;
      while ((it != end) && (0 < (offset_length = offset.next())))
      {
         inc_amnt = std::min<std::size_t>(length,offset_length);
         prev = it;
         it +=  inc_amnt;
         length -= inc_amnt;
         *(out++) = std::make_pair<Iterator,Iterator>(prev,it);
         ++match_count;
      }
      return match_count;
   }

   template<typename Iterator, typename DelimiterPredicate>
   inline std::size_t count_tokens(Iterator begin,
                                   Iterator end,
                                   const DelimiterPredicate& delimiter,
                                   const bool compress_delimiters = false)
   {
      if (0 == std::distance(begin,end)) return 0;
      std::size_t token_count = 0;
      Iterator it = begin;
      Iterator prev = begin;
      while(it != end)
      {
        if(delimiter(*it))
        {
           ++token_count;
           if (!compress_delimiters)
              ++it;
           else
              while(((++it) != end) && delimiter(*it));
           prev = it;
        }
        else
           ++it;
      }
      if (prev != it)
      {
         ++token_count;
      }
      return token_count;
   }

   template<typename Iterator>
   inline std::size_t count_consecutive_duplicates(Iterator begin,Iterator end)
   {
      if (std::distance(begin,end) < 2) return 0;
      Iterator prev = begin;
      Iterator it = begin + 1;
      std::size_t count = 0;
      while(it != end)
      {
         if (*prev == *it)
            ++count;
         else
            prev = it;
         ++it;
      }
      return count;
   }

   class token_grid
   {
   public:
      typedef std::deque< std::pair<unsigned char*, unsigned char*> > itr_list_type;
      typedef std::deque<itr_list_type> itr_list_list_type;

      class row_type
      {
      public:
         row_type(const itr_list_type& token_list)
         : token_list_(&token_list) {}

         template<typename T>
         inline T operator[](const unsigned int& index) const
         {
            itr_list_type::value_type curr_range = (*token_list_)[index];
            std::string tmp_str;
            tmp_str.assign(curr_range.first,curr_range.second);
            return boost::lexical_cast<T>(tmp_str);
         }

         template<typename T>
         inline T get(const unsigned int& index) const
         {
            return row_type::operator[]<T>(index);
         }

         std::size_t size() const { return token_list_->size(); }

         template<typename T, typename OutputIterator>
         inline void parse(OutputIterator out) const
         {
            std::string tmp_str;
            itr_list_type::iterator it = token_list_->begin();
            while(token_list_->end() != it)
            {
               itr_list_type::value_type& curr_range = *it;
               tmp_str.assign(curr_range.first,curr_range.second);
               *(out++) = boost::lexical_cast<T>(tmp_str);
               ++it;
            }
         }

         inline std::string as_string() const
         {
            return std::string(token_list_->begin()->first,token_list_->back().second);
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8,
                  typename T9, typename T10 >
         inline void parse(const unsigned int& col1, const unsigned int& col2,
                           const unsigned int& col3, const unsigned int& col4,
                           const unsigned int& col5, const unsigned int& col6,
                           const unsigned int& col7, const unsigned int& col8,
                           const unsigned int& col9, const unsigned int& col10,
                           T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10) const
         {
            std::string tmp_str(1024,0x0);
            process(tmp_str,(*token_list_)[ col1], t1);
            process(tmp_str,(*token_list_)[ col2], t2);
            process(tmp_str,(*token_list_)[ col3], t3);
            process(tmp_str,(*token_list_)[ col4], t4);
            process(tmp_str,(*token_list_)[ col5], t5);
            process(tmp_str,(*token_list_)[ col6], t6);
            process(tmp_str,(*token_list_)[ col7], t7);
            process(tmp_str,(*token_list_)[ col8], t8);
            process(tmp_str,(*token_list_)[ col9], t9);
            process(tmp_str,(*token_list_)[col10],t10);
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8,
                  typename T9 >
         inline void parse(const unsigned int& col1, const unsigned int& col2,
                           const unsigned int& col3, const unsigned int& col4,
                           const unsigned int& col5, const unsigned int& col6,
                           const unsigned int& col7, const unsigned int& col8,
                           const unsigned int& col9,
                           T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9) const
         {
            std::string tmp_str(1024,0x0);
            process(tmp_str,(*token_list_)[ col1], t1);
            process(tmp_str,(*token_list_)[ col2], t2);
            process(tmp_str,(*token_list_)[ col3], t3);
            process(tmp_str,(*token_list_)[ col4], t4);
            process(tmp_str,(*token_list_)[ col5], t5);
            process(tmp_str,(*token_list_)[ col6], t6);
            process(tmp_str,(*token_list_)[ col7], t7);
            process(tmp_str,(*token_list_)[ col8], t8);
            process(tmp_str,(*token_list_)[ col9], t9);
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8>
         inline void parse(const unsigned int& col1, const unsigned int& col2,
                           const unsigned int& col3, const unsigned int& col4,
                           const unsigned int& col5, const unsigned int& col6,
                           const unsigned int& col7, const unsigned int& col8,
                           T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8) const
         {
            std::string tmp_str(1024,0x0);
            process(tmp_str,(*token_list_)[ col1], t1);
            process(tmp_str,(*token_list_)[ col2], t2);
            process(tmp_str,(*token_list_)[ col3], t3);
            process(tmp_str,(*token_list_)[ col4], t4);
            process(tmp_str,(*token_list_)[ col5], t5);
            process(tmp_str,(*token_list_)[ col6], t6);
            process(tmp_str,(*token_list_)[ col7], t7);
            process(tmp_str,(*token_list_)[ col8], t8);
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6, typename T7>
         inline void parse(const unsigned int& col1, const unsigned int& col2,
                           const unsigned int& col3, const unsigned int& col4,
                           const unsigned int& col5, const unsigned int& col6,
                           const unsigned int& col7,
                           T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7) const
         {
            std::string tmp_str(1024,0x0);
            process(tmp_str,(*token_list_)[ col1], t1);
            process(tmp_str,(*token_list_)[ col2], t2);
            process(tmp_str,(*token_list_)[ col3], t3);
            process(tmp_str,(*token_list_)[ col4], t4);
            process(tmp_str,(*token_list_)[ col5], t5);
            process(tmp_str,(*token_list_)[ col6], t6);
            process(tmp_str,(*token_list_)[ col7], t7);
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6>
         inline void parse(const unsigned int& col1, const unsigned int& col2,
                           const unsigned int& col3, const unsigned int& col4,
                           const unsigned int& col5, const unsigned int& col6,
                           T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6) const
         {
            std::string tmp_str(1024,0x0);
            process(tmp_str,(*token_list_)[ col1], t1);
            process(tmp_str,(*token_list_)[ col2], t2);
            process(tmp_str,(*token_list_)[ col3], t3);
            process(tmp_str,(*token_list_)[ col4], t4);
            process(tmp_str,(*token_list_)[ col5], t5);
            process(tmp_str,(*token_list_)[ col6], t6);
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,typename T5>
         inline void parse(const unsigned int& col1, const unsigned int& col2,
                           const unsigned int& col3, const unsigned int& col4, const unsigned int& col5,
                           T1& t1, T2& t2, T3& t3, T4& t4, T5& t5) const
         {
            std::string tmp_str(1024,0x0);
            process(tmp_str,(*token_list_)[ col1], t1);
            process(tmp_str,(*token_list_)[ col2], t2);
            process(tmp_str,(*token_list_)[ col3], t3);
            process(tmp_str,(*token_list_)[ col4], t4);
            process(tmp_str,(*token_list_)[ col5], t5);
         }

         template<typename T1, typename T2,
                  typename T3, typename T4>
         inline void parse(const unsigned int& col1, const unsigned int& col2,
                           const unsigned int& col3, const unsigned int& col4,
                           T1& t1, T2& t2, T3& t3, T4& t4) const
         {
            std::string tmp_str(1024,0x0);
            process(tmp_str,(*token_list_)[ col1], t1);
            process(tmp_str,(*token_list_)[ col2], t2);
            process(tmp_str,(*token_list_)[ col3], t3);
            process(tmp_str,(*token_list_)[ col4], t4);
         }

         template<typename T1, typename T2, typename T3>
         inline void parse(const unsigned int& col1, const unsigned int& col2, const unsigned int& col3,
                           T1& t1, T2& t2, T3& t3) const
         {
            std::string tmp_str(1024,0x0);
            process(tmp_str,(*token_list_)[ col1], t1);
            process(tmp_str,(*token_list_)[ col2], t2);
            process(tmp_str,(*token_list_)[ col3], t3);
         }

         template<typename T1, typename T2>
         inline void parse(const unsigned int& col1, const unsigned int& col2,
                           T1& t1, T2& t2) const
         {
            std::string tmp_str(1024,0x0);
            process(tmp_str,(*token_list_)[ col1], t1);
            process(tmp_str,(*token_list_)[ col2], t2);
         }

         template<typename T1>
         inline void parse(const unsigned int& col, T1& t) const
         {
            std::string tmp_str(1024,0x0);
            process(tmp_str,(*token_list_)[col],t);
         }

      private:

         template<typename T>
         inline void process(std::string& tmp_str, const itr_list_type::value_type& curr_range, T& t) const
         {
            tmp_str.assign(curr_range.first,curr_range.second);
            t = boost::lexical_cast<T>(tmp_str);
         }

      private:
         const itr_list_type* token_list_;
      };

      token_grid(const std::string& file_name,
                 const std::string& delimiters = ",|;\t")
      : file_name_(file_name),
        delimiters_(delimiters),
        buffer_(0),
        buffer_size_(0),
        load_from_file_(true),
        state_(load())
      {}

      token_grid(const unsigned char* input_buffer,
                 const std::size_t& input_buffer_size,
                 const std::string& delimiters = ",|;\t")
      : file_name_(""),
        delimiters_(delimiters),
        buffer_(const_cast<unsigned char*>(input_buffer)),
        buffer_size_(input_buffer_size),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const char* input_buffer,
                 const std::size_t& input_buffer_size,
                 const std::string& delimiters = ",|;\t")
      : file_name_(""),
        delimiters_(delimiters),
        buffer_(reinterpret_cast<unsigned char*>(const_cast<char*>(input_buffer))),
        buffer_size_(input_buffer_size),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const std::string& input_buffer,
                 const std::size_t& input_buffer_size,
                 const std::string& delimiters = ",|;\t")
      : file_name_(""),
        delimiters_(delimiters),
        buffer_(reinterpret_cast<unsigned char*>(const_cast<char*>(input_buffer.c_str()))),
        buffer_size_(input_buffer_size),
        load_from_file_(false),
        state_(load())
      {}

     ~token_grid()
      {
         if ((load_from_file_) && (0 != buffer_)) delete [] buffer_;
      }

      bool operator!()               const { return !state_;            }
      std::string source_file()      const { return file_name_;         }
      std::size_t row_count()        const { return token_list_.size(); }
      std::size_t min_column_count() const { return min_column_count_;  }
      std::size_t max_column_count() const { return max_column_count_;  }

      row_type row(const unsigned int& row_index) const
      {
         return row_type(token_list_[row_index]);
      }

      template<typename OutputIterator>
      void extract_column(const std::size_t& index, OutputIterator out)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         std::string tmp_str(1024,0x00);
         while(token_list_.end() != it)
         {
            process_column(tmp_str,(*it++)[index],out);
         }
      }

      template<typename OutputIterator1, typename OutputIterator2>
      void extract_column(const std::size_t& index1,
                          const std::size_t& index2,
                          OutputIterator1 out1,
                          OutputIterator2 out2)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         std::string tmp_str(1024,0x00);
         while(token_list_.end() != it)
         {
            process_column(tmp_str,(*it)[index1],out1);
            process_column(tmp_str,(*it)[index2],out2);
            ++it;
         }
      }

      template<typename OutputIterator1, typename OutputIterator2, typename OutputIterator3>
      void extract_column(const std::size_t& index1,
                          const std::size_t& index2,
                          const std::size_t& index3,
                          OutputIterator1 out1,
                          OutputIterator2 out2,
                          OutputIterator3 out3)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         std::string tmp_str(1024,0x00);
         while(token_list_.end() != it)
         {
            process_column(tmp_str,(*it)[index1],out1);
            process_column(tmp_str,(*it)[index2],out2);
            process_column(tmp_str,(*it)[index3],out3);
            ++it;
         }
      }

      template<typename OutputIterator1, typename OutputIterator2,
               typename OutputIterator3, typename OutputIterator4>
      void extract_column(const std::size_t& index1,
                          const std::size_t& index2,
                          const std::size_t& index3,
                          const std::size_t& index4,
                          OutputIterator1 out1,
                          OutputIterator2 out2,
                          OutputIterator3 out3,
                          OutputIterator4 out4)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         std::string tmp_str(1024,0x00);
         while(token_list_.end() != it)
         {
            process_column(tmp_str,(*it)[index1],out1);
            process_column(tmp_str,(*it)[index2],out2);
            process_column(tmp_str,(*it)[index3],out3);
            process_column(tmp_str,(*it)[index4],out4);
            ++it;
         }
      }

      template<typename OutputIterator1, typename OutputIterator2,
               typename OutputIterator3, typename OutputIterator4,
               typename OutputIterator5>
      void extract_column(const std::size_t& index1,
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
         std::string tmp_str(1024,0x00);
         while(token_list_.end() != it)
         {
            process_column(tmp_str,(*it)[index1],out1);
            process_column(tmp_str,(*it)[index2],out2);
            process_column(tmp_str,(*it)[index3],out3);
            process_column(tmp_str,(*it)[index4],out4);
            process_column(tmp_str,(*it)[index5],out5);
            ++it;
         }
      }

      void remove_row(const std::size_t& index)
      {
         if (index < token_list_.size()) token_list_.erase(token_list_.begin() + index);
      }

      template<typename Predicate>
      void remove_row_if(Predicate p)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         std::string tmp_str(0xFF,0x00);
         while(token_list_.end() != it)
         {
            if (!it->empty() && p(it->front().first,it->back().second))
            {
               it = token_list_.erase(it);
            }
            else
              ++it;
         }
      }

      void enforce_column_count(const unsigned int& column_count)
      {
         itr_list_list_type::iterator it = token_list_.begin();
         while(token_list_.end() != it)
         {
            if (it->size() != column_count)
            {
               it = token_list_.erase(it);
            }
            else
              ++it;
         }
         min_column_count_ = column_count;
         max_column_count_ = column_count;
      }

      void clear()
      {
         delete[] buffer_;
         buffer_ = 0;
         buffer_size_ = 0;
         token_list_.clear();
         min_column_count_ = 0;
         state_ = false;
         file_name_ = "";
      }

   private:

      token_grid(const token_grid& tg);
      token_grid operator=(const token_grid& tg);

      bool load()
      {
         if (load_from_file_)
         {
            std::ifstream stream(file_name_.c_str(),std::ios::binary);
            if (!stream) return false;
            stream.seekg (0,std::ios::end);
            buffer_size_ = stream.tellg();
            if (0 == buffer_size_) return false;
            stream.seekg (0,std::ios::beg);
            buffer_ = new unsigned char[buffer_size_ + 1]; // an extra char for end iterator;
            stream.read(reinterpret_cast<char*>(buffer_),static_cast<std::streamsize>(buffer_size_));
            stream.close();
         }
         itr_list_type str_list;
         multiple_char_delimiter_predicate text_newline_predicate("\n\r");
         split(buffer_, buffer_ + buffer_size_,text_newline_predicate,std::back_inserter(str_list),true);
         multiple_char_delimiter_predicate token_predicate(delimiters_.begin(),delimiters_.end());

         min_column_count_ = std::numeric_limits<std::size_t>::max();
         max_column_count_ = std::numeric_limits<std::size_t>::min();

         for(itr_list_type::iterator it = str_list.begin(); it != str_list.end(); ++it)
         {
            itr_list_type current_token_list;
            split(it->first,it->second,token_predicate,std::back_inserter(current_token_list));
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
      inline void process_column(std::string& tmp_str, const itr_list_type::value_type& curr_range, OutputIterator out) const
      {
         tmp_str.assign(curr_range.first,curr_range.second);
         *(out++) = boost::lexical_cast<typename std::iterator_traits<OutputIterator>::value_type>(tmp_str);
      }

   private:

      itr_list_list_type token_list_;
      std::string file_name_;
      std::string delimiters_;
      unsigned char* buffer_;
      std::size_t buffer_size_;
      std::size_t min_column_count_;
      std::size_t max_column_count_;
      bool load_from_file_;
      bool state_;
   };

   inline void convert_bin_to_hex(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      static const std::size_t symbol_count = 16;
      static const unsigned char hex_symbol[symbol_count] = {
                                                             '0','1','2','3','4','5','6','7',
                                                             '8','9','A','B','C','D','E','F'
                                                            };

      for(const unsigned char* it = begin; it != end; ++it)
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

   inline void convert_hex_to_bin(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
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


      for(const unsigned char* it = begin; it != end;)
      {
         (*out)  = static_cast<unsigned char>(hex_to_bin[*(it++)] << 4);
         (*out) |= static_cast<unsigned char>(hex_to_bin[*(it++)]     );
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
      static const std::size_t symbol_count = 64;
      static const unsigned char bin_to_base64 [symbol_count]
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
         unsigned int block  = *(it++) << 16;
                      block |= *(it++) <<  8;
                      block |= *(it++)      ;
         *(out++) = bin_to_base64[( block >> 18 ) & 0x3F];
         *(out++) = bin_to_base64[( block >> 12 ) & 0x3F];
         *(out++) = bin_to_base64[( block >>  6 ) & 0x3F];
         *(out++) = bin_to_base64[( block       ) & 0x3F];
      }

      if ((rounds = (std::distance(begin,end) % 3)) > 0)
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
      output.resize(std::max<std::size_t>(4,binary_data.size() << 1));
      std::size_t resize = convert_bin_to_base64(binary_data.c_str(),binary_data.c_str() + binary_data.size(),const_cast<char*>(output.c_str()));
      output.resize(resize);
   }

   inline unsigned int convert_base64_to_bin(const unsigned char* begin, const unsigned char* end, unsigned char* out)
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
      for(std::size_t i = 0; i < rounds; ++i)
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

   void convert_to_printable_chars(unsigned char* begin, unsigned char* end)
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
      while(it != end)
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
      unsigned char* it = begin;
      while(it != end)
      {
         //(*it) = std::toupper((*it), std::locale::classic());
         (*it) = static_cast<unsigned char>(::toupper(static_cast<int>(*it)));
         ++it;
      }
   }

   void convert_to_uppercase(std::string& str)
   {
      convert_to_uppercase(reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str())),
                           reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str() + str.size())));
   }

   void convert_to_lowercase(unsigned char* begin, unsigned char* end)
   {
      unsigned char* it = begin;
      while(it != end)
      {
         //(*it) = std::tolower((*it), std::locale::classic());
         (*it) = static_cast<unsigned char>(::tolower(static_cast<int>(*it)));
         ++it;
      }
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
      if(std::distance(begin1,end1) != std::distance(begin2,end2))
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
      while(it1 != end1)
      {
         *(reinterpret_cast<unsigned short*>(out))  = (interleave_table[*it2] << 1);
         *(reinterpret_cast<unsigned short*>(out)) |=  interleave_table[*it1];
         ++it1;
         ++it2;
         out+=2;
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

   template<> struct interleave_ary<2> { typedef unsigned short type; };
   template<> struct interleave_ary<4> { typedef unsigned int   type; };
   //template<> struct interleave_ary<8> { typedef long long    type; }; Note: atm not supported by ISO C++ (N1811)

   template<std::size_t n>
   inline void create_nway_interleave_table(typename interleave_ary<n>::type table[256])
   {
      typedef typename interleave_ary<n>::type type;
      const type diff = static_cast<type>(n - 1);
      for(type i = static_cast<type>(0); i < static_cast<type>(256); ++i)
      {
         table[i] = 0x00;
         for(type j = static_cast<type>(0); j < static_cast<type>(8); ++j)
         {
            table[i] |= (i & (1 << j)) << (j * diff);
         }
      }
   }

   namespace bitwise_operation { enum type { eAND, eOR, eXOR }; }
   inline void bitwise_transform(const bitwise_operation::type& operation,
                                 const unsigned char* begin1, const unsigned char* end1,
                                 const unsigned char* begin2, const unsigned char* end2,
                                 unsigned char* out)
   {
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return;
      }

      const unsigned char* it1 = begin1;
      const unsigned char* it2 = begin2;

      switch(operation)
      {
         case bitwise_operation::eAND : while(it1 != end1) { *(out++) = *(it1++) & *(it2++); } return;
         case bitwise_operation::eOR  : while(it1 != end1) { *(out++) = *(it1++) | *(it2++); } return;
         case bitwise_operation::eXOR : while(it1 != end1) { *(out++) = *(it1++) ^ *(it2++); } return;
      }
   }

   inline void bitwise_transform(const bitwise_operation::type& operation,
                                 const char* begin1, const char* end1,
                                 const char* begin2, const char* end2,
                                 char* out)
   {
      return bitwise_transform(operation,
                               reinterpret_cast<const unsigned char*>(begin1),
                               reinterpret_cast<const unsigned char*>(end1),
                               reinterpret_cast<const unsigned char*>(begin2),
                               reinterpret_cast<const unsigned char*>(end2),
                               reinterpret_cast<unsigned char*>(out));
   }

   inline void bitwise_transform(const bitwise_operation::type& operation,
                                 const std::string& str1,
                                 const std::string& str2,
                                 std::string& out)
   {
      if (str1.size() != str2.size()) return;
      out.resize(str1.size());
      return bitwise_transform(operation,
                               str1.c_str(),str1.c_str() + str1.size(),
                               str2.c_str(),str2.c_str() + str2.size(),
                               const_cast<char*>(out.c_str()));
   }

   inline std::size_t hamming_distance(const unsigned char* begin1, const unsigned char* end1,
                                       const unsigned char* begin2, const unsigned char* end2)
   {
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return std::numeric_limits<std::size_t>::max();
      }
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
      std::size_t distance = 0;
      const unsigned char* it1 = begin1;
      const unsigned char* it2 = begin2;
      while(it1 != end1)
      {
         distance += high_bits_in_char[((*it1++) ^ (*it2++)) & 0xFF];
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

   namespace hash_details_
   {

      inline void compute_hash(const char data, unsigned int& hash)
      {
         hash ^= ((hash <<  7) ^ data * (hash >> 3));
      }

      inline void compute_hash(const unsigned char data, unsigned int& hash)
      {
         hash ^=  ((hash <<  7) ^ data * (hash >> 3));
      }

      inline void compute_hash(const char data[], unsigned int& hash)
      {
         hash ^=  ((hash <<  7) ^  data[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (data[1] ^ (hash >> 5)));
      }

      inline void compute_hash(const unsigned char data[], unsigned int& hash)
      {
         hash ^=  ((hash <<  7) ^  data[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (data[1] ^ (hash >> 5)));
      }

      inline void compute_hash(const int& data, unsigned int& hash)
      {
         const unsigned char* it = reinterpret_cast<const unsigned char*>(&data);
         hash ^=  ((hash <<  7) ^  it[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[1] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  it[2] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[3] ^ (hash >> 5)));
      }

      inline void compute_hash(const unsigned int& data, unsigned int& hash)
      {
         const unsigned char* it = reinterpret_cast<const unsigned char*>(&data);
         hash ^=  ((hash <<  7) ^  it[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[1] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  it[2] * (hash >> 3));
         hash ^= ~((hash << 11) + (it[3] ^ (hash >> 5)));
      }

      inline void compute_hash(const double& data, unsigned int& hash)
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

      template<typename T>
      inline void compute_hash(const T& data, unsigned int& hash)
      {
         const unsigned char* it = reinterpret_cast<const unsigned char*>(&data);
         for(unsigned int i = 0; i < (sizeof(T) >> 1); ++i)
         {
            hash ^=  ((hash <<  7) ^ (*it++) * (hash >> 3));
            hash ^= ~((hash << 11) + (*it++) ^ (hash >> 5));
         }
      }

      template<typename Iterator, std::size_t block_size>
      inline void compute_block(Iterator buffer_it, std::size_t& length, unsigned int& hash)
      {
        while(length >= block_size)
        {
          for(std::size_t i = 0; i < block_size; ++i, ++buffer_it)
          {
            compute_hash((*buffer_it),hash);
          }
          length -= block_size;
        }
      }

   }

   template<typename Iterator>
   inline void hash(const Iterator buffer_it,
                    std::size_t length,
                    unsigned int& hash_value)
   {
      hash_value = 0xAAAAAAAA;
      hash_details_::compute_block<Iterator,32>(buffer_it,length,hash_value);
      hash_details_::compute_block<Iterator,16>(buffer_it,length,hash_value);
      hash_details_::compute_block<Iterator, 8>(buffer_it,length,hash_value);
      hash_details_::compute_block<Iterator, 4>(buffer_it,length,hash_value);
      hash_details_::compute_block<Iterator, 2>(buffer_it,length,hash_value);

      if (length > 0)
      {
         hash_details_::compute_hash((*buffer_it),hash_value);
      }
   }

   inline unsigned int hash(const std::string& s)
   {
      unsigned int hash_value;
      hash(s.begin(),s.size(),hash_value);
      return hash_value;
   }

   template<typename Sequence>
   struct add_to_sequence
   {
   public:
      add_to_sequence(Sequence& sequence) : sequence_(sequence){}

      template<typename T>
      void operator()(const T& t) const { sequence_.push_back(t); }
      add_to_sequence(const add_to_sequence<Sequence>& ats) : sequence_(ats.sequence_){}
      add_to_sequence<Sequence>& operator=(const add_to_sequence<Sequence>& ats)
      {
         if (this != &ats)
         {
            this->sequence_ = ats.sequence_;
         }
         return *this;
      }

   private:
      Sequence& sequence_;
   };

   template<typename T>
   inline bool convert_str_range(const std::pair<std::string::const_iterator,std::string::const_iterator> range, T& t)
   {
      if (range.first == range.second) return false;
      t = boost::lexical_cast<T>(std::string(range.first,range.second));
      return true;
   }

   template<class OutputPredicate>
   struct filter_empty_range
   {
   public:
      filter_empty_range(OutputPredicate predicate)
      : predicate_(predicate){}

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
      filter_on_wildcard_match(const std::string& match_pattern,OutputPredicate& predicate, bool allow_through_on_match = true)
      : allow_through_on_match_(allow_through_on_match),
        match_pattern_(match_pattern),
        predicate_(predicate){}

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
      :case_insensitive_(case_insensitive),
       allow_through_on_match_(allow_through_on_match),
       begin_(begin),
       end_(end),
       predicate_(predicate){}

      template<typename Iterator>
      inline void operator() (const std::pair<Iterator,Iterator>& range) const
      {
         for(const std::string* i = begin_; i != end_ ; ++i)
         {
            if ((case_insensitive_ &&
               (case_insensitive_match((*i).begin(),(*i).end(),range.first,range.second))) ||
               (!case_insensitive_ && std::equal((*i).begin(),(*i).end(),range.first)))
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
         for(const std::string* i = begin_; i != end_ ; ++i)
         {
            if ((case_insensitive_ &&
               (case_insensitive_match((*i).begin(),(*i).end(),s.begin(),s.end()))) ||
               (!case_insensitive_ && std::equal((*i).begin(),(*i).end(),s.begin())))
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
      bool case_insensitive_;
      bool allow_through_on_match_;
      const std::string* begin_;
      const std::string* end_;
      OutputPredicate& predicate_;
   };

   template<typename MatchPredicate,
            typename Iterator>
   inline void skip_while_matching(Iterator& it,
                                   const Iterator& end,
                                   const MatchPredicate& predicate)
   {
      while (it != end)
      {
         if(predicate(*it))
            ++it;
         else
            break;
      }
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9, typename T10>
   inline std::size_t parse(const std::string& buffer,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4,
                            T5& t5, T6& t6, T7& t7, T8& t8,
                            T9& t9, T10& t10)
   {
      std::size_t token_count = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
       t1 = boost::lexical_cast< T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t2 = boost::lexical_cast< T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t3 = boost::lexical_cast< T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t4 = boost::lexical_cast< T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t5 = boost::lexical_cast< T5>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t6 = boost::lexical_cast< T6>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t7 = boost::lexical_cast< T7>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t8 = boost::lexical_cast< T8>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t9 = boost::lexical_cast< T9>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t10 = boost::lexical_cast<T10>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9>
   inline std::size_t parse(const std::string& buffer,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4,
                            T5& t5, T6& t6, T7& t7, T8& t8,
                            T9& t9)
   {
      std::size_t token_count = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t4 = boost::lexical_cast<T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t5 = boost::lexical_cast<T5>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t6 = boost::lexical_cast<T6>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t7 = boost::lexical_cast<T7>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t8 = boost::lexical_cast<T8>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t9 = boost::lexical_cast<T9>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8>
   inline std::size_t parse(const std::string& buffer,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4,
                            T5& t5, T6& t6, T7& t7, T8& t8)
   {
      std::size_t token_count = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t4 = boost::lexical_cast<T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t5 = boost::lexical_cast<T5>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t6 = boost::lexical_cast<T6>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t7 = boost::lexical_cast<T7>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t8 = boost::lexical_cast<T8>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7>
   inline std::size_t parse(const std::string& buffer,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7)
   {
      std::size_t token_count = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t4 = boost::lexical_cast<T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t5 = boost::lexical_cast<T5>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t6 = boost::lexical_cast<T6>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t7 = boost::lexical_cast<T7>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5,typename T6 >
   inline std::size_t parse(const std::string& buffer,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6)
   {
      std::size_t token_count = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t4 = boost::lexical_cast<T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t5 = boost::lexical_cast<T5>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t6 = boost::lexical_cast<T6>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5 >
   inline std::size_t parse(const std::string& buffer,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4, T5& t5)
   {
      std::size_t token_count = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t4 = boost::lexical_cast<T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t5 = boost::lexical_cast<T5>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4>
   inline std::size_t parse(const std::string& buffer,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4)
   {
      std::size_t token_count = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t4 = boost::lexical_cast<T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3 >
   inline std::size_t parse(const std::string& buffer,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3)
   {
      std::size_t token_count = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template<typename Tokenzier, typename T1, typename T2>
   inline std::size_t parse(const std::string& buffer,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2)
   {
      std::size_t token_count = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template<typename Tokenzier, typename T>
   inline std::size_t parse(const std::string& buffer,
                            Tokenzier& tokenizer,
                            T& t)
   {
      std::size_t token_count = 0;
      tokenizer.assign(buffer.begin(),buffer.end());
      typename Tokenzier::iterator it = tokenizer.begin();
      t = boost::lexical_cast<T>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9, typename T10>
   inline std::size_t parse(const std::string::const_iterator& begin,
                            const std::string::const_iterator& end,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4,
                            T5& t5, T6& t6, T7& t7, T8& t8,
                            T9& t9, T10& t10)
   {
      std::size_t token_count = 0;
      tokenizer.assign(begin,end);
      typename Tokenzier::iterator it = tokenizer.begin();
       t1 = boost::lexical_cast< T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t2 = boost::lexical_cast< T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t3 = boost::lexical_cast< T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t4 = boost::lexical_cast< T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t5 = boost::lexical_cast< T5>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t6 = boost::lexical_cast< T6>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t7 = boost::lexical_cast< T7>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t8 = boost::lexical_cast< T8>(std::string((*it).first,(*it).second)); ++it; ++token_count;
       t9 = boost::lexical_cast< T9>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t10 = boost::lexical_cast<T10>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9>
   inline std::size_t parse(const std::string::const_iterator& begin,
                            const std::string::const_iterator& end,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4,
                            T5& t5, T6& t6, T7& t7, T8& t8,
                            T9& t9)
   {
      std::size_t token_count = 0;
      tokenizer.assign(begin,end);
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t4 = boost::lexical_cast<T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t5 = boost::lexical_cast<T5>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t6 = boost::lexical_cast<T6>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t7 = boost::lexical_cast<T7>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t8 = boost::lexical_cast<T8>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t9 = boost::lexical_cast<T9>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8>
   inline std::size_t parse(const std::string::const_iterator& begin,
                            const std::string::const_iterator& end,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4,
                            T5& t5, T6& t6, T7& t7, T8& t8)
   {
      std::size_t token_count = 0;
      tokenizer.assign(begin,end);
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t4 = boost::lexical_cast<T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t5 = boost::lexical_cast<T5>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t6 = boost::lexical_cast<T6>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t7 = boost::lexical_cast<T7>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t8 = boost::lexical_cast<T8>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7>
   inline std::size_t parse(const std::string::const_iterator& begin,
                            const std::string::const_iterator& end,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7)
   {
      std::size_t token_count = 0;
      tokenizer.assign(begin,end);
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t4 = boost::lexical_cast<T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t5 = boost::lexical_cast<T5>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t6 = boost::lexical_cast<T6>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t7 = boost::lexical_cast<T7>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5,typename T6 >
   inline std::size_t parse(const std::string::const_iterator& begin,
                            const std::string::const_iterator& end,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6)
   {
      std::size_t token_count = 0;
      tokenizer.assign(begin,end);
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t4 = boost::lexical_cast<T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t5 = boost::lexical_cast<T5>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t6 = boost::lexical_cast<T6>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4,
             typename T5 >
   inline std::size_t parse(const std::string::const_iterator& begin,
                            const std::string::const_iterator& end,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4, T5& t5)
   {
      std::size_t token_count = 0;
      tokenizer.assign(begin,end);
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t4 = boost::lexical_cast<T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t5 = boost::lexical_cast<T5>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3, typename T4>
   inline std::size_t parse(const std::string::const_iterator& begin,
                            const std::string::const_iterator& end,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3, T4& t4)
   {
      std::size_t token_count = 0;
      tokenizer.assign(begin,end);
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t4 = boost::lexical_cast<T4>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename Tokenzier,
             typename T1, typename T2, typename T3 >
   inline std::size_t parse(const std::string::const_iterator& begin,
                            const std::string::const_iterator& end,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2, T3& t3)
   {
      std::size_t token_count = 0;
      tokenizer.assign(begin,end);
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t3 = boost::lexical_cast<T3>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template<typename Tokenzier, typename T1, typename T2>
   inline std::size_t parse(const std::string::const_iterator& begin,
                            const std::string::const_iterator& end,
                            Tokenzier& tokenizer,
                            T1& t1, T2& t2)
   {
      std::size_t token_count = 0;
      tokenizer.assign(begin,end);
      typename Tokenzier::iterator it = tokenizer.begin();
      t1 = boost::lexical_cast<T1>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      t2 = boost::lexical_cast<T2>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template<typename Tokenzier, typename T>
   inline std::size_t parse(const std::string::const_iterator& begin,
                            const std::string::const_iterator& end,
                            Tokenzier& tokenizer,
                            T& t)
   {
      std::size_t token_count = 0;
      tokenizer.assign(begin,end);
      typename Tokenzier::iterator it = tokenizer.begin();
      t = boost::lexical_cast<T>(std::string((*it).first,(*it).second)); ++it; ++token_count;
      return token_count;
   }

   template< typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9, typename T10 >
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                         const T9& t9, const T10& t10)
   {
      output += boost::lexical_cast<std::string>(t1); output += delimiter;
      output += boost::lexical_cast<std::string>(t2); output += delimiter;
      output += boost::lexical_cast<std::string>(t3); output += delimiter;
      output += boost::lexical_cast<std::string>(t4); output += delimiter;
      output += boost::lexical_cast<std::string>(t5); output += delimiter;
      output += boost::lexical_cast<std::string>(t6); output += delimiter;
      output += boost::lexical_cast<std::string>(t7); output += delimiter;
      output += boost::lexical_cast<std::string>(t8); output += delimiter;
      output += boost::lexical_cast<std::string>(t9); output += delimiter;
      output += boost::lexical_cast<std::string>(t10);
   }

   template< typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9 >
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                         const T9& t9)
   {
      output += boost::lexical_cast<std::string>(t1); output += delimiter;
      output += boost::lexical_cast<std::string>(t2); output += delimiter;
      output += boost::lexical_cast<std::string>(t3); output += delimiter;
      output += boost::lexical_cast<std::string>(t4); output += delimiter;
      output += boost::lexical_cast<std::string>(t5); output += delimiter;
      output += boost::lexical_cast<std::string>(t6); output += delimiter;
      output += boost::lexical_cast<std::string>(t7); output += delimiter;
      output += boost::lexical_cast<std::string>(t8); output += delimiter;
      output += boost::lexical_cast<std::string>(t9);
   }

   template< typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8)
   {
      output += boost::lexical_cast<std::string>(t1); output += delimiter;
      output += boost::lexical_cast<std::string>(t2); output += delimiter;
      output += boost::lexical_cast<std::string>(t3); output += delimiter;
      output += boost::lexical_cast<std::string>(t4); output += delimiter;
      output += boost::lexical_cast<std::string>(t5); output += delimiter;
      output += boost::lexical_cast<std::string>(t6); output += delimiter;
      output += boost::lexical_cast<std::string>(t7); output += delimiter;
      output += boost::lexical_cast<std::string>(t8);
   }

   template< typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7 >
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7)
   {
      output += boost::lexical_cast<std::string>(t1); output += delimiter;
      output += boost::lexical_cast<std::string>(t2); output += delimiter;
      output += boost::lexical_cast<std::string>(t3); output += delimiter;
      output += boost::lexical_cast<std::string>(t4); output += delimiter;
      output += boost::lexical_cast<std::string>(t5); output += delimiter;
      output += boost::lexical_cast<std::string>(t6); output += delimiter;
      output += boost::lexical_cast<std::string>(t7);
   }


   template< typename T1, typename T2, typename T3, typename T4,
             typename T5,typename T6>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6)
   {
      output += boost::lexical_cast<std::string>(t1); output += delimiter;
      output += boost::lexical_cast<std::string>(t2); output += delimiter;
      output += boost::lexical_cast<std::string>(t3); output += delimiter;
      output += boost::lexical_cast<std::string>(t4); output += delimiter;
      output += boost::lexical_cast<std::string>(t5); output += delimiter;
      output += boost::lexical_cast<std::string>(t6);
   }

   template< typename T1, typename T2, typename T3, typename T4,
             typename T5 >
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5)
   {
      output += boost::lexical_cast<std::string>(t1); output += delimiter;
      output += boost::lexical_cast<std::string>(t2); output += delimiter;
      output += boost::lexical_cast<std::string>(t3); output += delimiter;
      output += boost::lexical_cast<std::string>(t4); output += delimiter;
      output += boost::lexical_cast<std::string>(t5);
   }

   template< typename T1, typename T2, typename T3, typename T4 >
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4)
   {
      output += boost::lexical_cast<std::string>(t1); output += delimiter;
      output += boost::lexical_cast<std::string>(t2); output += delimiter;
      output += boost::lexical_cast<std::string>(t3); output += delimiter;
      output += boost::lexical_cast<std::string>(t4);
   }

   template< typename T1, typename T2, typename T3 >
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3)
   {
      output += boost::lexical_cast<std::string>(t1); output += delimiter;
      output += boost::lexical_cast<std::string>(t2); output += delimiter;
      output += boost::lexical_cast<std::string>(t3);
   }

   template< typename T1, typename T2 >
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2)
   {
      output += boost::lexical_cast<std::string>(t1); output += delimiter;
      output += boost::lexical_cast<std::string>(t2);
   }

   template<typename InputIterator>
   inline void join(std::string& output, const std::string& delimiter,
                    const InputIterator begin,
                    const InputIterator end)
   {
      InputIterator it = begin;
      while(it != end)
      {
         output += boost::lexical_cast<std::string>(*it);
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

   void generate_random_data(unsigned char* data, unsigned int length, unsigned int pre_gen_cnt = 0, unsigned int seed = 0xA5A5A5A5)
   {
      boost::mt19937 rng(static_cast<boost::mt19937::result_type>(seed));
      boost::uniform_int<unsigned int> dist(std::numeric_limits<unsigned int>::min(),std::numeric_limits<unsigned int>::max());
      boost::variate_generator<boost::mt19937&, boost::uniform_int<unsigned int> > rnd(rng,dist);
      if (pre_gen_cnt > 0)
      {
         unsigned int r = 0x00;
         for(unsigned int i = 0; i < pre_gen_cnt; r = rnd(), ++i);
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
      if(length > 0)
      {
         it -= (sizeof(unsigned int) - length);
         x = reinterpret_cast<unsigned int*>(it);
         (*x) = rnd();
      }
   }

   namespace text
   {
      std::string center(const std::size_t& width, const std::string::value_type& pad, const std::string& str)
      {
         if (str.size() >= width) return str;
         std::size_t pad_count = width - str.size();
         std::size_t pad_count_2 = (pad_count >> 1) + (pad_count & 1);
         return std::string(pad_count >> 1,pad) + str + std::string(pad_count_2,pad);
      }

      std::string right_align(const std::size_t& width, const std::string::value_type& pad, const std::string& str)
      {
         if (str.size() >= width) return str;
         return std::string(width - str.size(),pad) + str;
      }

      std::string left_align(const std::size_t& width, const std::string::value_type& pad, const std::string& str)
      {
         if (str.size() >= width) return str;
         return str + std::string(width - str.size(),pad);
      }

      template<typename T>
      std::string center(const std::size_t& width, const std::string::value_type& pad, const T& t)
      {
         return center(width,pad,boost::lexical_cast<std::string>(t));
      }

      template<typename T>
      std::string right_align(const std::size_t& width, const std::string::value_type& pad, const T& t)
      {
         return right_align(width,pad,boost::lexical_cast<std::string>(t));
      }

      template<typename T>
      std::string left_align(const std::size_t& width, const std::string::value_type& pad, const T& t)
      {
         return left_align(width,pad,boost::lexical_cast<std::string>(t));
      }
   }

} // namespace strtk

#endif