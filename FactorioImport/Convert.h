#pragma once

#include <ParseElements.h>
#include <ParamsCollection.h>
#include <ParseLog.h>

namespace FactorioImport
{
  class Converter
  {
    const ParseLog& pl;
    const AllImport& all;
    ResourceCalculator::ParamsCollection& pc;
  public:
    Converter(const ParseLog& PL, const AllImport& allImport, ResourceCalculator::ParamsCollection& PC);
    int Run();
  };

}
