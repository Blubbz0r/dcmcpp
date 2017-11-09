#pragma once
#include <vector>

namespace dcmcpp
{

/*!
    See https://martinfowler.com/eaaDev/Notification.html
    \param  ErrorType Type of error (e.g. std::string or an enumeration).
            Needs to be move-constructable
*/
template<typename ErrorType>
class Notification
{
public:
    bool hasErrors() const { return !m_errors.empty(); }
    const std::vector<ErrorType>& errors() const { return m_errors; }
    void addError(ErrorType error) { m_errors.emplace_back(std::move(error)); }
    void setErrors(std::vector<ErrorType> errors) { m_errors = std::move(errors); }

private:
    std::vector<ErrorType> m_errors;
};

}
