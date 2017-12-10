#pragma once

#include "dcmcpp/Dicom/Dataset.h"
#include "dcmcpp/Dicom/FileMetaInfo.h"

#include <filesystem>

namespace dcmcpp
{

class File
{
public:
    File(FileMetaInfo metaInfo, Dataset dataset);

    const FileMetaInfo& metaInfo() const;
    FileMetaInfo& metaInfo();
    void setMetaInfo(FileMetaInfo metaInfo);

    const Dataset& dataset() const;
    Dataset& dataset();
    void setDataset(Dataset dataset);

private:
    FileMetaInfo m_metaInfo;
    Dataset m_dataset;
};

/*! Returns whether or no the file at the given \a path is a DICOM file. */
bool isDicomFile(const std::experimental::filesystem::path& path);

File readDicomFile(const std::experimental::filesystem::path& path);

}
