#
# *******************************************************************
# *                       String Tool Kit Library                   *
# *                                                                 *
# * Author: Arash Partow (2002-2009)                                *
# * URL: http://www.partow.net/programming/strtk/index.html         *
# *                                                                 *
# * Copyright notice:                                               *
# * Free use of the String Tool Kit Library is permitted under the  *
# * guidelines and in accordance with the most current version of   *
# * the Common Public License.                                      *
# * http://www.opensource.org/licenses/cpl1.0.php                   *
# *                                                                 *
# *******************************************************************
#


COMPILER         = -c++
OPTIMIZATION_OPT = -O3
OPTIONS          = -pedantic -ansi -Wall -Werror $(OPTIMIZATION_OPT) -o
REGEX            = -lboost_regex
PTHREAD          = -lpthread
LINKER_OPT       = -L/usr/lib -lstdc++ 


OBJECTS = $(CPP_SRC:.cpp=.o)

%.o: %.hpp %.cpp
	$(COMPILER) $(OPTIONS_LIBS) $*.cpp

BUILD_LIST+=strtk_tokenizer_cmp
BUILD_LIST+=strtk_examples
BUILD_LIST+=strtk_keyvalue_example
BUILD_LIST+=strtk_text_parser_example01
BUILD_LIST+=strtk_text_parser_example02
BUILD_LIST+=strtk_tokenizer_test
BUILD_LIST+=strtk_hexview
BUILD_LIST+=strtk_converters_example
BUILD_LIST+=strtk_tokengrid_example
BUILD_LIST+=strtk_serializer_example
BUILD_LIST+=strtk_randomizer

all: $(BUILD_LIST)

strtk_tokenizer_cmp: strtk_tokenizer_cmp.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_tokenizer_cmp strtk_tokenizer_cmp.cpp $(LINKER_OPT)

strtk_examples: strtk_examples.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_examples strtk_examples.cpp  $(LINKER_OPT) $(REGEX)

strtk_keyvalue_example: strtk_keyvalue_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_keyvalue_example strtk_keyvalue_example.cpp $(LINKER_OPT)

strtk_text_parser_example01: strtk_text_parser_example01.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_text_parser_example01 strtk_text_parser_example01.cpp $(LINKER_OPT)

strtk_text_parser_example02: strtk_text_parser_example02.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_text_parser_example02 strtk_text_parser_example02.cpp $(LINKER_OPT)

strtk_tokenizer_test: strtk_tokenizer_test.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_tokenizer_test strtk_tokenizer_test.cpp $(LINKER_OPT)

strtk_hexview: strtk_hexview.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_hexview strtk_hexview.cpp $(LINKER_OPT)

strtk_converters_example : strtk_converters_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_converters_example strtk_converters_example.cpp $(LINKER_OPT)
	
strtk_tokengrid_example : strtk_tokengrid_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_tokengrid_example strtk_tokengrid_example.cpp $(LINKER_OPT)

strtk_serializer_example : strtk_serializer_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_serializer_example strtk_serializer_example.cpp $(LINKER_OPT)

strtk_randomizer : strtk_randomizer.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_randomizer strtk_randomizer.cpp $(LINKER_OPT)

strip_bin :
	strip -s strtk_tokenizer_cmp
	strip -s strtk_examples
	strip -s strtk_keyvalue_example
	strip -s strtk_text_parser_example01
	strip -s strtk_text_parser_example02
	strip -s strtk_tokenizer_test
	strip -s strtk_hexview
	strip -s strtk_converters_example
	strip -s strtk_tokengrid_example
	strip -s strtk_serializer_example
	strip -s strtk_randomizer

valgrind_check :
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_tokenizer_cmp
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_examples
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_keyvalue_example
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_text_parser_example01
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_text_parser_example02
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_tokenizer_test
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_hexview
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_converters_example
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_tokengrid_example
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_serializer_example
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_randomizer

clean:
	rm -f core *.o *.bak *stackdump *#