#include <Dicom/Dictionary.h>
#include <Dicom/Tag.h>

#include <iostream>

int main()
{
    std::cout << dcmcpp::tagToString(dcmcpp::FileMetaInformationGroupLength) << std::endl;
}
