* do we want to use external library (e.g. Abseil) for common functions like string split?
  * won't we use boost anyway for asio? then we could also just use all string functionalities from there...
* resource file management (tests) windows and linux
* properly use gtest/mock (HEAD?) from conan (with /std:c++latest)