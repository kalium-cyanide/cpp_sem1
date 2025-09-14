#include <complex>
#include <string>
#include <vector>

#define _SILENCE_NONFLOATING_COMPLEX_DEPRECATION_WARNING

template <class t> using matrix = std::vector<std::vector<t>>;

using string_t = std::string;
using color_t = int;

using position_t = std::complex<int>;

#define x real()
#define y imag()
