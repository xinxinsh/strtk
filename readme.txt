C++ String Toolkit Library

[INTRODUCTION]
The C++ String Toolkit  Library (StrTk) consists of  robust, optimized
and portable string processing algorithms for the C++ language.  StrTk
is  designed to  be easy  to use  and integrate  within existing  code
bases. Furthermore the library has  a rich set of features  that makes
light work of any kind of string processing task.


[COPYRIGHT NOTICE]
Free  use  of  the  String  Toolkit  Library  is  permitted  under the
guidelines and  in accordance  with the  most current  version of  the
Common Public License.
http://www.opensource.org/licenses/cpl1.0.php


[DOWNLOADS & UPDATES]
All updates can be found at:
 (1) http://www.partow.net/programming/strtk/index.html
 (2) http://strtk.partow.net

Code repository:
 (1) http://code.google.com/p/strtk/
 (2) svn checkout http://strtk.googlecode.com/svn/trunk/ strtk


[INSTALLATION]
(1) strtk.hpp  should be  placed in  a project  or system include path
(e.g: /usr/include/),  or its  location added  to the  LD_LIBRARY_PATH
environment variable.

(2)  If  the Boost  libraries  (random, regex,  lexical_cast)  are not
available  the  following  defines   should  be  commented  out   from
strtk.hpp:
   (*) ENABLE_LEXICAL_CAST (strtk.hpp:42)
   (*) ENABLE_RANDOM (strtk.hpp:47)
   (*) ENABLE_REGEX (strtk.hpp:54)

(3) It is advisable to have either Boost installed or a TR1  compliant
c++ standard library. Installation of Boost on:
   (*) Win32  : http://www.boostpro.com/download
   (*) Ubuntu : boost packages via apt-get or synaptic
   (*) Fedora : yum -y install boost-devel
   (*) http://www.boost.org/doc/libs/release/more/getting_started/index.html

[COMPILATION]
(1) For a full build: make clean all

(2) For a PGO build: make clean pgo

(3) To strip executable: make strip_bin


[COMPILER COMPATIBILITY]
(*) GNU Compiler Collection (4.3+)
(*) Intel® C++ Compiler (9.x+)
(*) Microsoft Visual Studio C++ Compiler (8.1+)


[FILES]
 strtk.hpp
 strtk_converters_example.cpp
 strtk_examples.cpp
 strtk_hexview.cpp
 strtk_ipv4_parser.cpp
 strtk_keyvalue_example.cpp
 strtk_numstats.cpp
 strtk_parse_test.cpp
 strtk_period_parser.cpp
 strtk_random_line.cpp
 strtk_randomizer.cpp
 strtk_search_trie_example.cpp
 strtk_serializer_example.cpp
 strtk_text_parser_example01.cpp
 strtk_text_parser_example02.cpp
 strtk_tokengrid_example.cpp
 strtk_tokenizer_cmp.cpp
 strtk_tokenizer_test.cpp
 Makefile
 readme.txt
