#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <benchmark/benchmark.h>

static void BM_Boost_trim(benchmark::State& state){
  std::string s("foobar\t");
  while(state.KeepRunning())
    boost::algorithm::trim_right(s);
}

BENCHMARK(BM_Boost_trim);

static void BM_Std_trim(benchmark::State& state){
  std::string s("foobar\t");
  while(state.KeepRunning()){
    int pos = s.find_last_not_of(" \t");
    std::string result = s.substr(0,pos+1);
  }
}

BENCHMARK(BM_Std_trim);

BENCHMARK_MAIN();
