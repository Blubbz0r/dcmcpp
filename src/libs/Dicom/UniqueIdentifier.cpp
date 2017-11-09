#include "UniqueIdentifier.h"

#include "Utils/StringUtils.h"

namespace dcmcpp
{

bool isValid(const UniqueIdentifier& ui)
{
    return !validate(ui).hasErrors();
}

Notification<UiError> validate(const UniqueIdentifier& ui)
{
    Notification<UiError> notification;

    static constexpr auto MaxLength = 64;
    if (ui.size() > MaxLength)
        notification.addError(UiError::MaxSizeExceeded);

    static constexpr auto AllowedCharacters = "0123456789.";
    if (ui.find_first_not_of(AllowedCharacters) != ui.npos)
        notification.addError(UiError::InvalidCharacter);

    static constexpr auto MinComponentCount = 2;
    auto components = StringUtils::split(ui, ".");
    if (components.size() < MinComponentCount)
        notification.addError(UiError::NotEnoughComponents);

    for (const auto& component : components)
    {
        if (StringUtils::startsWith(component, "0") && component.size() > 1)
            notification.addError(UiError::InvalidLeadingZero);

        if (component.empty())
            notification.addError(UiError::EmptyComponent);
    }

    return notification;
}

}
