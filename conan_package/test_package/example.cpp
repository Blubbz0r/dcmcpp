#include <dcmcpp/Dicom/Dictionary.h>
#include <dcmcpp/Dicom/Tag.h>

#include <iostream>

int main()
{
    std::cout << dcmcpp::tagToString(dcmcpp::FileMetaInformationGroupLength) << std::endl;
}
