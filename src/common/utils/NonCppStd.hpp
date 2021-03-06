#ifndef NONCPPSTD_HPP
#  define NONCPPSTD_HPP

#  include <cstddef>
#  include <memory>

// **************************************************************
//! \brief Make gcc warms when return code of a function is not
//! checked.
// **************************************************************
#  define WARN_UNUSED __attribute__((warn_unused_result))

// **************************************************************
//! \brief Allows to create literal values of type std::size_t
//! In the same way than U, LL or UL macros. Indeed size_t can
//! be uint32_t or uint64_t depending on the architecture.
//! Example: size_t i = 42_z;
// **************************************************************
constexpr std::size_t operator "" _z (unsigned long long const n)
{
  return static_cast<std::size_t>(n);
}

// **************************************************************
// Enable for C++11 and Visual Studio
// **************************************************************
#  if (defined(_MSC_VER) && _MSC_VER < 1800) || (!defined(_MSC_VER) && __cplusplus <= 201103L)
namespace std
{
  //! \brief std::make_unique is for C++14 enable it for C++11
  template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }
}
#  endif

// **************************************************************
//! \brief Return the number of elements in an array.
//! S for size and T for the type.
// **************************************************************
template<size_t S, typename T>
inline size_t ARRAY_SIZE(T (&)[S])
{
  return S;
}

// **************************************************************
//! \brief One of the most used optimization used in Linux kernel. When
//! working with conditional code (if-else statements), we often know
//! which branch is true and which is not. If compiler knows this
//! information in advance, it can generate most optimized code.
// **************************************************************
#  ifndef likely
#    define likely(x)       __builtin_expect(!!(x),1)
#  endif
#  ifndef unlikely
#    define unlikely(x)     __builtin_expect(!!(x),0)
# endif

#  ifndef NORETURN
#    if __GNUC__ > 2 || defined(__INTEL_COMPILER)
#      define NORETURN __attribute__((__noreturn__))
#    else
#      define NORETURN
#    endif
#  endif

#endif // NONCPPSTD_HPP
