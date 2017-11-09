#include "Dicom/File.h"

#include <benchmark/benchmark.h>

namespace dcmcpp
{

static void BM_isDicomFile(benchmark::State& state)
{
    while (state.KeepRunning())
        isDicomFile(R"(C:\test\dcm)");
}

BENCHMARK(BM_isDicomFile);

}