# CMakeList.txt : CMake project for Polynomial, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (PolyMath "PolyApp.cpp" "PolyApp.h" 
						 "Tables/linearTableOnAnArray.h" "Tables/linearListTable.hpp" "Tables/TableInterface" "Tables/OrderedArray.hpp" 
						 "Polynomial/Polynomial.h" "Polynomial/Monom.h"
						 "Containers/Vector.h" "Containers/List.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PolyMath PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
