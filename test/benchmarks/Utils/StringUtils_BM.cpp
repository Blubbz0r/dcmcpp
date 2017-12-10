#include "dcmcpp/Utils/StringUtils.h"

#include <benchmark/benchmark.h>

namespace dcmcpp
{
namespace StringUtils
{

static void BM_split(benchmark::State& state)
{
    std::string input("This,String,Should,Be,Split");
    while (state.KeepRunning())
        auto test = split(input, ",");
}

BENCHMARK(BM_split);

static void BM_startsWith(benchmark::State& state)
{
    std::string input("@Test");
    while (state.KeepRunning())
        startsWith(input, "@");
}

BENCHMARK(BM_startsWith);

}
}
