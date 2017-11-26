#include "Dicom/File.h"

#include <iostream>

using namespace dcmcpp;

void dumpDicomFile(const File& dicomFile)
{
    std::cout << "# File Meta Information\n\n";
    std::cout << dumpDataElements(dicomFile.metaInfo().dataElements()) << "\n\n";

    std::cout << "# Dataset\n\n";
    std::cout << dumpDataElements(dicomFile.dataset().dataElements()) << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2)
        return -1;

    try
    {
        const auto dicomFile = readDicomFile(argv[1]);
        dumpDicomFile(dicomFile);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }

    return 0;
}
