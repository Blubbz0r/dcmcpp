#include "Dicom/File.h"

#include <iostream>

using namespace dcmcpp;

void dumpDataElements(const std::vector<DataElement>& dataElements)
{
    // TODO: maybe add a table header?
    for (const auto& dataElement : dataElements)
    {
        std::cout << tagToString(dataElement.tag) << '\t';
        std::cout << vrToString(dataElement.valueRepresentation) << '\t';
        // TODO: value length can have different digits possibly requiring multiple \t...
        std::cout << std::to_string(dataElement.valueLength) << '\t';
        std::cout << valueToString(dataElement.value) << "\t\n";
    }
}

void dumpDicomFile(const File& dicomFile)
{
    std::cout << "# File Meta Information\n\n";
    dumpDataElements(dicomFile.metaInfo().dataElements());
    std::cout << "\n\n";

    std::cout << "# Dataset\n\n";
    dumpDataElements(dicomFile.dataset().dataElements());
    std::cout << std::endl;
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
