#pragma once

#include <string>
#include <string_view>

namespace dcmcpp
{

/*! DICOM Value Representations (VRs) as defined by PS3.5. */
enum class ValueRepresentation
{
    AE,     /*!< Application Entity */
    AS,     /*!< Age String */
    AT,     /*!< Attribute String */
    CS,     /*!< Code String */
    DA,     /*!< Date */
    DS,     /*!< Decimal String */
    DT,     /*!< Date Time */
    FL,     /*!< Floating Point Single */
    FD,     /*!< Floating Point Double */
    IS,     /*!< Integer String */
    LO,     /*!< Long String */
    LT,     /*!< Long Text */
    OB,     /*!< Other Byte */
    OD,     /*!< Other Double */
    OF,     /*!< Other Float */
    OL,     /*!< Other Long */
    OW,     /*!< Other Word */
    PN,     /*!< Person Name */
    SH,     /*!< Short String */
    SL,     /*!< Signed Long */
    SQ,     /*!< Sequence of Items */
    SS,     /*!< Signed Short */
    ST,     /*!< Short Text */
    TM,     /*!< Time */
    UC,     /*!< Unlimited Characters */
    UI,     /*!< Unique Identifier (UID) */
    UL,     /*!< Unsigned Long */
    UN,     /*!< Unknown */
    UR,     /*!< Universal Resource Identifier or Universal Resource Locator (URI/URL) */
    US,     /*!< Unsigned Short */
    UT,     /*!< Unlimited Text */
    None    /*!< No Value Representation (e.g. Item, ItemDelimitationItem or SequenceDelimitationItem) */
};

std::string vrToString(ValueRepresentation vr);

constexpr ValueRepresentation vrFromString(std::string_view vr)
{
    if (vr.compare("AE") == 0)
        return ValueRepresentation::AE;
    if (vr.compare("AS") == 0)
        return ValueRepresentation::AS;
    if (vr.compare("AT") == 0)
        return ValueRepresentation::AT;
    if (vr.compare("CS") == 0)
        return ValueRepresentation::CS;
    if (vr.compare("DA") == 0)
        return ValueRepresentation::DA;
    if (vr.compare("DS") == 0)
        return ValueRepresentation::DS;
    if (vr.compare("DT") == 0)
        return ValueRepresentation::DT;
    if (vr.compare("FL") == 0)
        return ValueRepresentation::FL;
    if (vr.compare("FD") == 0)
        return ValueRepresentation::FD;
    if (vr.compare("IS") == 0)
        return ValueRepresentation::IS;
    if (vr.compare("LO") == 0)
        return ValueRepresentation::LO;
    if (vr.compare("LT") == 0)
        return ValueRepresentation::LT;
    if (vr.compare("OB") == 0)
        return ValueRepresentation::OB;
    if (vr.compare("OD") == 0)
        return ValueRepresentation::OD;
    if (vr.compare("OF") == 0)
        return ValueRepresentation::OF;
    if (vr.compare("OL") == 0)
        return ValueRepresentation::OL;
    if (vr.compare("OW") == 0)
        return ValueRepresentation::OW;
    if (vr.compare("PN") == 0)
        return ValueRepresentation::PN;
    if (vr.compare("SH") == 0)
        return ValueRepresentation::SH;
    if (vr.compare("SL") == 0)
        return ValueRepresentation::SL;
    if (vr.compare("SQ") == 0)
        return ValueRepresentation::SQ;
    if (vr.compare("SS") == 0)
        return ValueRepresentation::SS;
    if (vr.compare("ST") == 0)
        return ValueRepresentation::ST;
    if (vr.compare("TM") == 0)
        return ValueRepresentation::TM;
    if (vr.compare("UC") == 0)
        return ValueRepresentation::UC;
    if (vr.compare("UI") == 0)
        return ValueRepresentation::UI;
    if (vr.compare("UL") == 0)
        return ValueRepresentation::UL;
    if (vr.compare("UN") == 0)
        return ValueRepresentation::UN;
    if (vr.compare("UR") == 0)
        return ValueRepresentation::UR;
    if (vr.compare("US") == 0)
        return ValueRepresentation::US;
    if (vr.compare("UT") == 0)
        return ValueRepresentation::UT;

    // TODO: is it possible to print the string_view as well?
    throw std::logic_error("Unable to convert to ValueRepresentation"); // TODO: proper exception
}

}
