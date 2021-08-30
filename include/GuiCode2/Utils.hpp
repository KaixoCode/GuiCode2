#pragma once
#include "GuiCode2/pch.hpp"

// Get the signature of a member function
template<typename T>
struct MemberSignature {};
template<typename Ret, typename Type, typename... Args>
struct MemberSignature<Ret(Type::*)(Args...) const>
{
	using type = Ret(Args...);
};
template<typename Ret, typename Type, typename... Args>
struct MemberSignature<Ret(Type::*)(Args...)>
{
	using type = Ret(Args...);
};

// Get the signature of a lambda's operator()
template<typename _Fx, typename = void>
struct LambdaSignature {};
template<typename _Fx>
struct LambdaSignature<_Fx, std::void_t<decltype(&_Fx::operator())>>
{
	using type = typename MemberSignature<decltype(&_Fx::operator())>::type;
};

template<typename _Fx>
using LambdaSignatureT = typename LambdaSignature<_Fx>::type;
