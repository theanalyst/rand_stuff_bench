// compile-command: g++ -02 -std=c++11 -lbenchmark -o escape
#include <cstring>
#include <string>
#include <benchmark/benchmark.h>
#include <algorithm>
#include <iostream>

using namespace std;

void escape_str_c (const string& s, char esc_char, char special_char, string *dest){
  const char *src = s.c_str();
  char dest_buf[s.size() * 2 + 1];
  char *destp = dest_buf;

  for (size_t i = 0; i < s.size(); i++) {
    char c = src[i];
    if (c == esc_char || c == special_char) {
      *destp++ = esc_char;
    }
    *destp++ = c;
  }
  *destp++ = '\0';
  *dest = dest_buf;
}

void escape_str_cpp(const string& s, char esc_char, char special_char,
                    string& dest){
  dest.resize(1+2*s.size());

  std::transform(begin(s), end(s), begin(dest),
            [&](const char c) -> char {
                   return c == (esc_char || c == special_char) ? esc_char : c;
            });
}

static void BM_C_escape(benchmark::State& state){
  string s("foobarbarbar:foobarbarbar");
  string dest;
  while(state.KeepRunning())
    escape_str_c(s,'\\',':',&dest);
}

static void BM_Cpp_escape(benchmark::State& state){
  string s("foobarbarbar:foobarbarbar");
  string dest;
  while(state.KeepRunning())
    escape_str_cpp(s,'\\',':',dest);
}

BENCHMARK(BM_C_escape);
BENCHMARK(BM_Cpp_escape);

BENCHMARK_MAIN();
