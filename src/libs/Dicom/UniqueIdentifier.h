#pragma once

#include "Utils/Notification.h"

#include <string>

namespace dcmcpp
{

using UniqueIdentifier = std::string;

bool isValid(const UniqueIdentifier& ui);

enum class UiError
{
    MaxSizeExceeded,
    InvalidCharacter,
    NotEnoughComponents,
    InvalidLeadingZero,
    EmptyComponent
};

Notification<UiError> validate(const UniqueIdentifier& ui);

}
