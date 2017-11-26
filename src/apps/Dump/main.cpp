#include "Dicom/File.h"

#include <iostream>

using namespace dcmcpp;

void dumpDataElements(const std::vector<DataElement>& dataElements, int indentationLevel = 0)
{
    std::string indentation;
    for (int i = 0; i < indentationLevel; ++i)
        indentation += "  ";

    // TODO: maybe add a table header?
    for (const auto& dataElement : dataElements)
    {
        std::cout << indentation << tagToString(dataElement.tag) << '\t';
        std::cout << indentation << vrToString(dataElement.valueRepresentation) << '\t';
        // TODO: value length can have different digits possibly requiring multiple \t...
        std::cout << indentation << std::to_string(dataElement.valueLength) << '\t';
        std::cout << indentation << valueToString(dataElement.value) << "\n";

        if (dataElement.valueRepresentation == ValueRepresentation::SQ)
        {
            const auto& sequence = std::get<Sequence>(dataElement.value);
            for (const auto& sequenceItem : sequence.items)
                dumpDataElements(sequenceItem.dataElements, indentationLevel + 1);
        }
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
