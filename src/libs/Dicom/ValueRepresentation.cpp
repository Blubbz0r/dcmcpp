#include "ValueRepresentation.h"

namespace dcmcpp
{

std::string vrToString(ValueRepresentation vr)
{
    switch (vr)
    {
    case ValueRepresentation::AE: return "AE";
    case ValueRepresentation::AS: return "AS";
    case ValueRepresentation::AT: return "AT";
    case ValueRepresentation::CS: return "CS";
    case ValueRepresentation::DA: return "DA";
    case ValueRepresentation::DS: return "DS";
    case ValueRepresentation::DT: return "DT";
    case ValueRepresentation::FL: return "FL";
    case ValueRepresentation::FD: return "FD";
    case ValueRepresentation::IS: return "IS";
    case ValueRepresentation::LO: return "LO";
    case ValueRepresentation::LT: return "LT";
    case ValueRepresentation::OB: return "OB";
    case ValueRepresentation::OD: return "OD";
    case ValueRepresentation::OF: return "OF";
    case ValueRepresentation::OL: return "OL";
    case ValueRepresentation::OW: return "OW";
    case ValueRepresentation::PN: return "PN";
    case ValueRepresentation::SH: return "SH";
    case ValueRepresentation::SL: return "SL";
    case ValueRepresentation::SQ: return "SQ";
    case ValueRepresentation::SS: return "SS";
    case ValueRepresentation::ST: return "ST";
    case ValueRepresentation::TM: return "TM";
    case ValueRepresentation::UC: return "UC";
    case ValueRepresentation::UI: return "UI";
    case ValueRepresentation::UL: return "UL";
    case ValueRepresentation::UN: return "UN";
    case ValueRepresentation::UR: return "UR";
    case ValueRepresentation::US: return "US";
    case ValueRepresentation::UT: return "UT";
    }

    // TODO: proper exception?
    throw std::logic_error("Unable to convert ValueRepresentation " + std::to_string(static_cast<int>(vr)) + " to string");
}

}
