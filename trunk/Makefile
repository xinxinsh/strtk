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
OPTIONS          = -pedantic -ansi -Wall -lstdc++ $(OPTIMIZATION_OPT) -o
REGEX            = boost_regex-gcc-mt-1_33_1

OBJECTS = $(CPP_SRC:.cpp=.o)

%.o: %.hpp %.cpp
	$(COMPILER) $(OPTIONS_LIBS) $*.cpp

all: stk_tokenizer_cmp strtk_examples strtk_keyvalue_example strtk_text_parser_example01 strtk_text_parser_example02 strtk_tokenizer_test

stk_tokenizer_cmp: stk_tokenizer_cmp.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) stk_tokenizer_cmp stk_tokenizer_cmp.cpp

strtk_examples: strtk_examples.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_examples strtk_examples.cpp -l$(REGEX)

strtk_keyvalue_example: strtk_keyvalue_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_keyvalue_example strtk_keyvalue_example.cpp

strtk_text_parser_example01: strtk_text_parser_example01.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_text_parser_example01 strtk_text_parser_example01.cpp

strtk_text_parser_example02: strtk_text_parser_example02.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_text_parser_example02 strtk_text_parser_example02.cpp

strtk_tokenizer_test: strtk_tokenizer_test.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_tokenizer_test strtk_tokenizer_test.cpp


clean:
	rm -f core *.o *.bak *stackdump *#
