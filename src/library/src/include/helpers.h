/**
 * Part of https://github.com/ManelJimeno/bootstrap (C) 2022
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once
#include <memory>

#ifdef _WIN32
#ifdef LIBRARY_EXPORTS
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __declspec(dllimport)
#endif
#else
#define LIBRARY_API
#endif

#if __cplusplus >= 201703L
#define NODISCARD [[nodiscard]] /*NOLINT*/
#else
#define NODISCARD /*NOLINT*/
#endif

/**
 * @brief Disable unsupported copy operations for the class.
 * Place this macro at the public section.
 */
#define DISABLE_COPY(class_name) /*NOLINT*/                                                                            \
    class_name(const class_name& other) = delete;                                                                      \
    class_name& operator=(class_name& other) = delete; /*NOLINT*/

/**
 * @brief Disable unsupported move operations for the class.
 * Place this macro at the public section.
 */
#define DISABLE_MOVE(class_name) /*NOLINT*/                                                                            \
    class_name(const class_name&& other) = delete;                                                                     \
    class_name& operator=(class_name&& other) = delete; /*NOLINT*/

/**
 * @brief Disable unsupported copy and move operations for the class.
 * Place this macro at the public section.
 */
#define DISABLE_COPY_AND_MOVE(class_name) /*NOLINT*/                                                                   \
    DISABLE_COPY(class_name)                                                                                           \
    DISABLE_MOVE(class_name)

// NOLINTNEXTLINE
#define PIMPL(implClass)                                                                                               \
    const implClass* Pimpl() const                                                                                     \
    {                                                                                                                  \
        return m_pImpl.get();                                                                                          \
    }                                                                                                                  \
    implClass* Pimpl() /*NOLINT*/                                                                                      \
    {                                                                                                                  \
        return m_pImpl.get();                                                                                          \
    }                                                                                                                  \
    std::unique_ptr<implClass> m_pImpl;

namespace utils
{

/**
 * @brief Checks if a tuple contains a type
 */
template <typename Type, typename Tuple> struct tupleHasType;
template <typename Type, typename... tupleArgs>
struct tupleHasType<Type, std::tuple<tupleArgs...>> : std::disjunction<std::is_same<Type, tupleArgs>...>
{
};

/**
 * @brief Checks if a type is a std::shared_ptr
 */
template <class T> struct isSharedPtr : std::false_type
{
};
template <class T> struct isSharedPtr<std::shared_ptr<T>> : std::true_type
{
};

} // namespace utils
