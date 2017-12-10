#include "dcmcpp/Dicom/ValueLength.h"
#include "dcmcpp/Dicom/ValueRepresentation.h"
#include "dcmcpp/Dicom/TransferSyntax.h"

namespace dcmcpp
{

bool hasExtendedLengthEncoding(ValueRepresentation vr, std::string_view transferSyntaxUid)
{
    if (isExplicit(transferSyntaxUid))
    {
        switch (vr)
        {
        case ValueRepresentation::OB:
        case ValueRepresentation::OD:
        case ValueRepresentation::OF:
        case ValueRepresentation::OL:
        case ValueRepresentation::OW:
        case ValueRepresentation::SQ:
        case ValueRepresentation::UC:
        case ValueRepresentation::UR:
        case ValueRepresentation::UN:
        case ValueRepresentation::UT:
            return true;

        default:
            return false;
        }
    }
    else
    {
        return true;
    }
}

}
