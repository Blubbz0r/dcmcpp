#include "dcmcpp/Dicom/UniqueIdentifier.h"

#include <gmock/gmock.h>

using namespace testing;

namespace dcmcpp
{

TEST(UniqueIdentifierTests, isValid_ValidUI_ReturnsTrue)
{
    EXPECT_TRUE(isValid(UniqueIdentifier{ "1.234.567.890.1122334455" }));
}

TEST(UniqueIdentifierTests, isValid_InvalidUI_ReturnsFalse)
{
    EXPECT_FALSE(isValid(UniqueIdentifier{ "1.234..567.890" }));
}

TEST(UniqueIdentifierTests, validate_ValidUI_HasNoErrors)
{
    EXPECT_FALSE(validate(UniqueIdentifier{ "1.234.567.890.1122334455" }).hasErrors());
}

TEST(UniqueIdentifierTests, validate_LongerThan64Bytes_ReturnsMaxSizeExceededUiError)
{
    auto result = validate(UniqueIdentifier{
        "123456789.123456789.123456789.123456789.123456789.123456789.12345" });

    const auto& errors = result.errors();
    ASSERT_THAT(errors.size(), Eq(1u));
    EXPECT_THAT(errors[0], Eq(UiError::MaxSizeExceeded));
}

TEST(UniqueIdentifierTests, validate_ContainingCharacterOtherThanDigitOrDot_ReturnsInvalidCharacterUiError)
{
    auto result = validate(UniqueIdentifier{ "1.234.567.ABC" });

    const auto& errors = result.errors();
    ASSERT_THAT(errors.size(), Eq(1u));
    EXPECT_THAT(errors[0], Eq(UiError::InvalidCharacter));
}

TEST(UniqueIdentifierTests, validate_OnlyOneComponent_ReturnsNotEnoughComponentsError)
{
    auto result = validate(UniqueIdentifier{ "1234" });

    const auto& errors = result.errors();
    ASSERT_THAT(errors.size(), Eq(1u));
    EXPECT_THAT(errors[0], Eq(UiError::NotEnoughComponents));
}

TEST(UniqueIdentifierTests, validate_LeadingZeroInComponentWithMoreThanOneDigit_ReturnsInvalidLeadingZeroUiError)
{
    auto result = validate(UniqueIdentifier{ "123.0123.234" });

    const auto& errors = result.errors();
    ASSERT_THAT(errors.size(), Eq(1u));
    EXPECT_THAT(errors[0], Eq(UiError::InvalidLeadingZero));
}

TEST(UniqueIdentifierTests, validate_ComponentWithASingleDigitZero_HasNoErrors)
{
    EXPECT_FALSE(validate(UniqueIdentifier{ "123.0.234" }).hasErrors());
}

TEST(UniqueIdentifierTests, validate_EmptyComponent_ReturnsEmptyComponent)
{
    auto result = validate(UniqueIdentifier{ "123..123" });

    const auto& errors = result.errors();
    ASSERT_THAT(errors.size(), Eq(1u));
    EXPECT_THAT(errors[0], Eq(UiError::EmptyComponent));
}

TEST(UniqueIdentifierTests, validate_MultipleErrors_ReturnsAllErrors)
{
    auto result = validate(UniqueIdentifier{ "123..02.1" });
    EXPECT_THAT(result.errors(), UnorderedElementsAre(UiError::InvalidLeadingZero, UiError::EmptyComponent));
}

}
