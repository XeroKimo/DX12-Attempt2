#pragma once
#include "FunctionPointers.h"

namespace Delegates
{
    namespace Internal
    {
        template <class T, typename ReturnType, typename ...FunctionParameters>
        using MethodPtr = ReturnType(T::*)(FunctionParameters...);
    }

    template <class T, class Func>
    class MethodPtr;

    template <class T, typename ReturnType, typename ...FunctionParameters>
    class MethodPtr<T, ReturnType(FunctionParameters...)> final
    {
        using TMethodPtr = MethodPtr<T,ReturnType(FunctionParameters...)>;
        using MethPtr = Internal::MethodPtr<T, ReturnType, FunctionParameters...>;
    private:
        MethPtr m_funcPtr = nullptr;

    public:
        MethodPtr() = default;
        MethodPtr(const MethPtr& other)
        {
            m_funcPtr = other;
        }

        MethodPtr(const TMethodPtr& other)
        {
            m_funcPtr = other;
        }

        bool IsNull() { return m_funcPtr == nullptr; }

        void operator=(const MethPtr& func)
        {
            m_funcPtr = func;
        }

        void operator=(const TMethodPtr& func)
        {
            m_funcPtr = func.m_funcPtr;
        }

        bool operator==(const MethPtr& func) const
        {
            return m_funcPtr == func;
        }

        bool operator==(const TMethodPtr& func) const
        {
            return m_funcPtr == func.m_funcPtr;
        }

        bool operator!=(const MethPtr& func) const
        {
            return m_funcPtr != func;
        }

        bool operator!=(const TMethodPtr& func) const
        {
            return m_funcPtr != func.m_funcPtr;
        }

        ReturnType operator()(T* const object, FunctionParameters... params) const
        {
            return (object->*m_funcPtr)(params...);
        }
    };
}