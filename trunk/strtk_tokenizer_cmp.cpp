/*
 *******************************************************************
 *                       String Tool Kit Library                   *
 *                                                                 *
 * String Toolkit Tokenizer vs BOOST Tokenizer                     *
 * Author: Arash Partow - 2003                                     *
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
   Description: This comparison aims to determine the running time
                difference between the BOOST string tokenizer and
                the String Toolkit Library's string tokenizer.
                The comparison involves each tokenizer accepting
                a set of possible delimiters and a string to be
                tokenized. The provided string is tokenized and the
                tokens are subsequently concatenated to each other.
                Running time is measured around the tokenization
                loop, and printed out along with a measure indicating
                the average number of tokens parsed per second.
*/


#include <iostream>
#include <string>
#include <iterator>

#include "strtk.hpp"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>

#ifdef WIN32
 #include <windows.h>
#else
 #include <sys/time.h>
 #include <sys/types.h>
#endif

#ifdef WIN32

class timer
{
public:
   timer()      { QueryPerformanceFrequency(&clock_frequency); }
   void start() { QueryPerformanceCounter(&start_time);        }
   void stop()  { QueryPerformanceCounter(&stop_time);         }
   double time(){ return (1.0 *(stop_time.QuadPart - start_time.QuadPart)) / (1.0 * clock_frequency.QuadPart); }
 private:
  LARGE_INTEGER start_time;
  LARGE_INTEGER stop_time;
  LARGE_INTEGER clock_frequency;
};

#else

class timer
{
public:
   void start() { gettimeofday(&start_time, 0); }
   void stop()  { gettimeofday(&stop_time,  0); }
   double time()
   {
      double diff = (stop_time.tv_sec - start_time.tv_sec) * 1000000.0;
      if (stop_time.tv_usec > start_time.tv_usec)
         diff += (1.0 * (stop_time.tv_usec - start_time.tv_usec));
      else if (stop_time.tv_usec < start_time.tv_usec)
         diff -= (1.0 * (start_time.tv_usec - stop_time.tv_usec));

      return (diff / 1000000.0);
   }
 private:
  struct timeval start_time;
  struct timeval stop_time;
  struct timeval clock_frequency;
};

#endif

static const std::string base = "a|bc?def,ghij;klmno_p|qr?stu,vwxy;z1234_56789 0 A|BC?DEF,GHIJ;KLMNO_P|QR?STU,VWXY;Z1234_56789";
static const std::size_t replicate_count = 1000000;
static const std::string delimiters = "|?,;_ ";

void strtk_tokenizer_timed_test()
{
   printf("[strtk] "); fflush(stdout);
   std::string s = "";
   std::string so = "";
   s.reserve(base.size() * replicate_count);
   so.reserve(s.size());
   for (unsigned int i = 0; i < replicate_count; ++i) s += base;
   strtk::multiple_char_delimiter_predicate predicate(delimiters);
   strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer(s,predicate);
   strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type::iterator it = tokenizer.begin();
   unsigned int token_count = 0;
   timer t;
   t.start();
   while (it != tokenizer.end())
   {
      so += std::string((*it).first,(*it).second);
      ++it;
      ++token_count;
   }
   t.stop();
   printf("Token Count: %d\tTotal time: %8.4fsec\tRate: %8.4ftks/sec\n",
          token_count,
          t.time(),
          token_count / (1.0 * t.time()));
}

void boost_tokenizer_timed_test()
{
   printf("[boost] "); fflush(stdout);
   std::string s = "";
   std::string so  ="";
   s.reserve(base.size() * replicate_count);
   so.reserve(s.size());
   for (unsigned int i = 0; i < replicate_count; ++i) s += base;
   typedef boost::tokenizer<boost::char_separator<char> > tokenizer_type;
   tokenizer_type tokenizer(s,boost::char_separator<char>(delimiters.c_str()));
   tokenizer_type::iterator it = tokenizer.begin();
   unsigned int token_count = 0;
   timer t;
   t.start();
   while (it != tokenizer.end())
   {
      so += *it;
      ++it;
      ++token_count;
   }
   t.stop();
   printf("Token Count: %d\tTotal time: %8.4fsec\tRate: %8.4ftks/sec\n",
          token_count,
          t.time(),
          token_count / (1.0 * t.time()));
}

void strtk_split_timed_test()
{
   printf("[strtk] "); fflush(stdout);
   std::string s = "";
   s.reserve(base.size() * replicate_count);
   for (unsigned int i = 0; i < replicate_count; ++i) s += base;
   std::deque<std::string> token_list;
   timer t;
   t.start();
   strtk::parse(s,delimiters,token_list);
   t.stop();
   printf("Token Count: %d\tTotal time: %8.4fsec\tRate: %8.4ftks/sec\n",
          token_list.size(),
          t.time(),
          token_list.size() / (1.0 * t.time()));
}

void boost_split_timed_test()
{
   printf("[boost] "); fflush(stdout);
   std::string s = "";
   s.reserve(base.size() * replicate_count);
   for (unsigned int i = 0; i < replicate_count; ++i) s += base;
   std::deque<std::string> token_list;
   timer t;
   t.start();
   boost::split(token_list, s, boost::is_any_of(delimiters));
   t.stop();
   printf("Token Count: %d\tTotal time: %8.4fsec\tRate: %8.4ftks/sec\n",
          token_list.size(),
          t.time(),
          token_list.size() / (1.0 * t.time()));
}

void boost_lexical_cast_test_i2s()
{
   printf("[boost] "); fflush(stdout);
   std::string s;
   s.reserve(32);
   const std::size_t max = 10000000;
   std::size_t total_length = 0;
   timer t;
   t.start();
   for(std::size_t i = 0; i < max; ++i)
   {
      s = boost::lexical_cast<std::string>(i);
      total_length += s.size();
   }
   t.stop();
   printf("Numbers: %d\tTotal time: %8.4fsec\tRate: %8.4fnums/sec\n",
          max,
          t.time(),
          max / (1.0 * t.time()));
}

void strtk_lexical_cast_test_i2s()
{
   printf("[strtk] "); fflush(stdout);
   std::string s;
   s.reserve(32);
   const std::size_t max = 10000000;
   std::size_t total_length = 0;
   timer t;
   t.start();
   for(std::size_t i = 0; i < max; ++i)
   {
      strtk::type_to_string(i,s);
      total_length += s.size();
   }
   t.stop();
   printf("Numbers: %d\tTotal time: %8.4fsec\tRate: %8.4fnums/sec\n",
          max,
          t.time(),
          max / (1.0 * t.time()));
}

static const std::string strint_list[] =
             {
                  "917049",   "4931205",   "6768064",   "6884243",   "5647132",   "7371203",   "8629878",   "4941840",   "4543268",   "1075600",
                 "9922547",   "6960207",   "1883152",   "2300759",   "-279294",   "4187292",   "3699841",   "8386395",  "-1441129",   "-887892",
                 "-635422",   "9742573",   "2326186",  "-5903851",   "5648486",   "3057647",   "2980079",   "2957468",   "7929158",   "1925615",
                 "4365041",   "5624958",   "8990205",   "2652177",   "3993588",   "-298316",   "2901599",   "3887387",  "-5202979",   "1196268",
                 "5968501",   "7619928",   "3565643",   "1885272",   "-749485",   "2961381",   "2982579",   "2387454",   "4250081",   "5958205",
                 "4907034",   "2592882",   "3269234",    "549815",   "6256292",   "9721039",   "-595225",   "5587491",   "4596297",  "-3885009",
                 "6518423",   "5149762",   "8834164",   "8085586",   "3233120",   "8166948",   "4172345",   "6735549",   "-934295",   "9481935",
                 "-430406",   "6932717",   "4087292",   "4047263",   "3236400", " -3863050",   "4312079",   "6956261",   "5689446",   "3871332",
                 "4969414",   "9378599",   "7971781",   "5380630",   "5001363",   "1715827",   "6044615",   "9118925",   "9956168",  "-8865496",
                 "5962464",   "7408980",   "6646513",   "-634564",   "4188330",   "9805948",   "5625691",   "7641113",   "4212929",   "7802447",
                 "2174248",   "7449361",   "9896659",    "-25961",   "1706598",   "2412368",  "-4617035",   "6314554",   "2225957",   "7521434",
                "-9530566",   "3914164",   "2394759",   "7157744",   "9919392",   "6406949",   "-744004",   "9899789",   "8380325",  "-1416284",
                 "3402833",   "2150043",   "5191009",   "8979538",   "9565778",   "3750211",   "7304823",   "2829359",   "6544236",   "-615740",
                 "8060564",   "-176752",   "1184717",   "-666343", " -1273292",   "-485827",   "6241066",   "6579411",   "8093119",   "7481306",
                "-4924485",   "7467889",   "9813178",   "7927100",   "3614859",   "7293354",   "9232973",   "4323115",   "1133911",   "9511638",
                 "4443188",   "2289448",   "5639726",   "9073898",   "8540394",   "5389992",   "1397726",    "589230",   "1017086",   "1852330",
                 "9097353",   "6002251",   "-308780",  "-3830169",   "4340467",   "2235284",   "3314444",   "1085967",   "4152107",   "5431117",
                 "-444999",   "2136400",   "6925907",   "6990614",   "3588271",   "8422028",  "-4034772",   "5804039",  "-6740545",   "9381873",
                 "-924923",   "1652367",   "2302616",   "6776663",   "2567821",   "-248935",   "2587688",   "7076742",  "-6461467",   "1562896",
                 "-768116",   "2338768",   "9887307",   "9992184",   "2045182",   "2797589",   "9784597",   "9696554",   "5113329",   "1067216",
               "-76247763",  "58169007",  "29408062",  "85342511",  "42092201", "-95817703",  "-1912517", "-26275135",  "54656606", "-58188878",
                 "3907360", "-23063423",  "59062754",  "83711047",  "95221044",  "34894840", "-38562139", "-82018330",  "14226223", "-10799717",
                 "8529722",  "88961903",  "25608618", "-39988247",  "33228241",  "38598533",  "21161480", "-33723784",   "8873948",  "96505557",
               "-47385048", "-79413272", "-85904404",  "87791158",  "49194195",  "13051222",  "57773302",  "31904423",   "3142966",  "27846156",
                 "7420011", "-72376922", "-68873971",  "23765361",   "4040725", "-22359806",  "85777219",  "10099223", "-90364256", "-40158172",
                "-7948696", "-64344821",  "34404238",  "84037448", "-85084788", "-42078409", "-56550310",  "96898389",   "-595829", "-73166703",
               "-82838342",  "64441808",  "43641062", "-64419642", "-44421934",  "75232413", "-75773725", "-89139509",  "12812089", "-97633526",
                "36090916", "-57706234",  "17804655",   "4189936",  "-4100124",  "38803710", "-39735126", "-62397437",  "75801648",  "51302332",
                "73433906",  "13015224", "-12624818",  "91360377",  "11576319", "-54467535",   "8892431",  "36319780",  "38832042",  "50172572",
                "42423473",  "41078812",  "40445652", "-76722281",  "95092224",  "12075234",  "-4045888", "-74396490", "-57304222", "-21726885",
                "92038121", "-31899682",  "21589254", "-30260046",  "56000244",  "69686659",  "93327838",  "96882881", "-91419389",  "77529147",
                "43288506",   "1192435", "-74095920",  "76756590", "-31184683", "-35716724",   "9451980", "-63168350",  "62864002",  "26283194",
                "37188395",  "29151634",  "99343471", "-69450330", "-55680090", "-64957599",  "47577948",  "47107924",   "2490477",  "48633003",
               "-82740809", "-24122215",  "67301713", "-63649610",  "75499016",  "82746620",  "17052193",   "4602244", "-32721165",  "20837836",
                "64501793",  "53146328",   "5152287",  "-9674493",  "68105580",  "57245637",  "39740229", "-74071854",  "86777268",  "86484437",
               "-86962508",  "12644427", "-62944073",  "59539680",  "43340539",  "30661534",  "20143968", "-68183731", "-48250926",  "42669063",
                "87736852",  "-4444906", "-48094147",  "54774735",  "54571890", "-22473078",  "95053418",    "393654", "-33229960",  "32276798",
               "-48361110",  "44295939", "-79813406",  "11630865",  "38544571",  "70972830",  "-9821748", "-60965384", "-13096675", "-24569041",
                "97233152",  "51028396", "-13796948",  "95437272",  "71352512", "-83233730", "-68517318",  "61832742", "-42667174", "-18002395",
                "-92239407", "12701336", "-63830875",  "41514172",  "-5726049",  "18668677",  "69555144", "-13737009", "-22626233", "-55078143",
             };
static const std::size_t strint_list_size = sizeof(strint_list) / sizeof(std::string);

void boost_lexical_cast_test_s2i()
{
   printf("[boost] "); fflush(stdout);
   const std::size_t rounds = 100000;
   std::size_t total = 0;
   int n = 0;
   timer t;
   t.start();
   for(std::size_t x = 0; x < rounds; ++x)
   {
      for(std::size_t i = 0; i < strint_list_size; ++i)
      {
         n = boost::lexical_cast<int>(strint_list[i]);
         total += n;
      }
   }
   t.stop();
   printf("Numbers: %d\tTotal: %d\tTotal time: %8.4fsec\tRate: %8.4fnums/sec\n",
          rounds * strint_list_size,
          total,
          t.time(),
          (rounds * strint_list_size) / (1.0 * t.time()));
}

void strtk_lexical_cast_test_s2i()
{
   printf("[strtk] "); fflush(stdout);
   const std::size_t rounds = 100000;
   std::size_t total = 0;
   int n = 0;
   timer t;
   t.start();
   for(std::size_t x = 0; x < rounds; ++x)
   {
      for(std::size_t i = 0; i < strint_list_size; ++i)
      {
         strtk::string_to_type_converter(strint_list[i],n);
         total += n;
      }
   }
   t.stop();
   printf("Numbers: %d\tTotal: %d\tTotal time: %8.4fsec\tRate: %8.4fnums/sec\n",
          rounds * strint_list_size,
          total,
          t.time(),
          (rounds * strint_list_size) / (1.0 * t.time()));
}

int main()
{
   std::cout << "Tokenizer Test" << std::endl;
   boost_tokenizer_timed_test();
   strtk_tokenizer_timed_test();
   std::cout << "Split Test" << std::endl;
   boost_split_timed_test();
   strtk_split_timed_test();
   std::cout << "Int To String Test" << std::endl;
   boost_lexical_cast_test_i2s();
   strtk_lexical_cast_test_i2s();
   std::cout << "String To Int Test" << std::endl;
   boost_lexical_cast_test_s2i();
   strtk_lexical_cast_test_s2i();
   return 0;
}
