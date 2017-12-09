* do we want to use external library (e.g. Abseil) for common functions like string split?
  * won't we use boost anyway for asio? then we could also just use all string functionalities from there...
* resource file management (tests) windows and linux
* conanfile.py files in root and conan_package can be merged. we could provide a "DEV" option in conan_package/conanfile.py, which builds with apps, tests and benchmarks and adds appropriate requirements.
  * maybe use [build_requires](http://docs.conan.io/en/latest/mastering/build_requires.html)
* add Doxyfile, generate documentation e.g. on appveyor and publish it as github pages