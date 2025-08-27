#include <functional>

template <typename binaryOp, typename T>
struct NeutralEl {
    static const T value;
};

template <typename T>
struct NeutralEl<std::plus<>, T> {
    static const T value = T{0};
};

template <typename T>
struct NeutralEl<std::multiplies<>, T> 
{
    static const T value = T{1};
};

template <typename BinOp, typename T>
constexpr T Neutral_Elem() {
    return NeutralEl<BinOp, T>::value;
};
