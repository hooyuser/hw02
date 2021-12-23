#include <tuple>
#include <string_view>

#define Forward(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

template<typename UnknownType, typename ReferenceType>
concept SubtypeOf = std::same_as<std::decay_t<UnknownType>, ReferenceType> || std::derived_from<std::decay_t<UnknownType>, ReferenceType>;

template<typename UnknownType, typename ...ReferenceTypes>
concept AnyOf = (SubtypeOf<UnknownType, ReferenceTypes> || ...);

template<typename UnknownType, typename ...ReferenceTypes>
concept AnyBut = !AnyOf<UnknownType, ReferenceTypes...>;

template<typename UnknownType, typename ReferenceType>
concept ExplicitlyConvertibleTo = requires(UnknownType x) { static_cast<ReferenceType>(Forward(x)); };

template<typename UnknownType, typename ReferenceType>
concept ConstructibleFrom = ExplicitlyConvertibleTo<ReferenceType, std::decay_t<UnknownType>>;

template<typename UnknownType>
concept BuiltinArray = std::is_array_v<std::remove_cvref_t<UnknownType>>;

template<typename UnknownType>
concept Advanceable = requires(UnknownType x) { ++x; };

template<typename UnknownType>
concept Iterable = BuiltinArray<UnknownType> || requires(UnknownType x) {
    { x.begin() }->Advanceable;
    { *x.begin() }->AnyBut<void>;
    { x.begin() != x.end() }->ExplicitlyConvertibleTo<bool>;
};

auto& operator<<(SubtypeOf<std::ostream> auto& Printer, Iterable auto&& Container) requires (
    (requires { Printer << *Container.begin(); } || requires { Printer << Container[0]; }) &&
    requires { { Container }->ConstructibleFrom<const char*>; } == false
) {
    auto [Startpoint, Endpoint] = [&] {
        if constexpr (requires { { Container }->BuiltinArray; })
            return std::tuple{ Container, Container + sizeof(Container) / sizeof(Container[0]) };
        else
            return std::tuple{ Container.begin(), Container.end() };
    }();
    std::string_view separator = "";
    Printer << "[";
    for (; Startpoint != Endpoint; ++Startpoint)
        Printer << std::exchange(separator, ", ") << *Startpoint;
    Printer << "]";
    return Printer;
}

