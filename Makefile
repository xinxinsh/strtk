#
# *******************************************************************
# *                       String Tool Kit Library                   *
# *                                                                 *
# * Author: Arash Partow - 2002                                     *
# * URL: http://www.partow.net/programming/strtk/index.html         *
# *                                                                 *
# * Copyright notice:                                               *
# * Free use of the String Tool Kit Library is permitted under the  *
# * guidelines and in accordance with the most current version of   *
# * the Common Public License.                                      *
# * http://www.opensource.org/licenses/cpl.php                      *
# *                                                                 *
# *******************************************************************
#


COMPILER         = -c++
OPTIMIZATION_OPT = -O3
OPTIONS          = -pedantic -ansi -Wall -lstdc++ $(REGEX) $(OPTIMIZATION_OPT) -o
REGEX            = -L/usr/lib -lboost_regex

OBJECTS = $(CPP_SRC:.cpp=.o)

%.o: %.hpp %.cpp
	$(COMPILER) $(OPTIONS_LIBS) $*.cpp

all: stk_tokenizer_cmp strtk_examples strtk_keyvalue_example strtk_text_parser_example01 strtk_text_parser_example02 strtk_tokenizer_test strtk_hexview strtk_converters_example

stk_tokenizer_cmp: stk_tokenizer_cmp.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) stk_tokenizer_cmp stk_tokenizer_cmp.cpp

strtk_examples: strtk_examples.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_examples strtk_examples.cpp

strtk_keyvalue_example: strtk_keyvalue_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_keyvalue_example strtk_keyvalue_example.cpp

strtk_text_parser_example01: strtk_text_parser_example01.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_text_parser_example01 strtk_text_parser_example01.cpp

strtk_text_parser_example02: strtk_text_parser_example02.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_text_parser_example02 strtk_text_parser_example02.cpp

strtk_tokenizer_test: strtk_tokenizer_test.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_tokenizer_test strtk_tokenizer_test.cpp

strtk_hexview: strtk_hexview.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_hexview strtk_hexview.cpp

strtk_converters_example : strtk_converters_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_converters_example strtk_converters_example.cpp

strip_bin :
	strip -s stk_tokenizer_cmp
	strip -s strtk_examples
	strip -s strtk_keyvalue_example
	strip -s strtk_text_parser_example01
	strip -s strtk_text_parser_example02
	strip -s strtk_tokenizer_test
	strip -s strtk_hexview
	strip -s strtk_converters_example

clean:
	rm -f core *.o *.bak *stackdump *#
