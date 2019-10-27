#pragma once
namespace Delegates
{
    namespace Internal
    {
        template <typename ReturnType, typename ...FunctionParameters>
        using FunctionPtr = ReturnType(*)(FunctionParameters...);

        template <class T, typename ReturnType, typename ...FunctionParameters>
        using MethodPtr = ReturnType(T::*)(FunctionParameters...);
    }

    template <class T>
    class FunctionPtr;

    template <class T, class Func>
    class MethodPtr;

    template <typename ReturnType, typename ...FunctionParameters>
    class FunctionPtr<ReturnType(FunctionParameters...)> final
    {
        using TFunctionPtr = FunctionPtr<ReturnType(FunctionParameters...)>;
        using FuncPtr = Internal::FunctionPtr<ReturnType, FunctionParameters...>;
    private:
        FuncPtr m_funcPtr = nullptr;

    public:
        FunctionPtr() = default;
        FunctionPtr(const FuncPtr& other)
        {
            m_funcPtr = other;
        }

        FunctionPtr(const TFunctionPtr& other)
        {
            m_funcPtr = other;
        }

        bool IsNull() const { return m_funcPtr == nullptr; }

        void operator=(const FuncPtr& func)
        {
            m_funcPtr = func;
        }

        void operator=(const TFunctionPtr& func)
        {
            m_funcPtr = func.m_funcPtr;
        }

        bool operator==(const FuncPtr& func) const
        {
            return m_funcPtr == func;
        }

        bool operator==(const TFunctionPtr& func) const
        {
            return m_funcPtr == func.m_funcPtr;
        }

        bool operator!=(const FuncPtr& func) const
        {
            return m_funcPtr != func;
        }

        bool operator!=(const TFunctionPtr& func) const
        {
            return m_funcPtr != func.m_funcPtr;
        }

        ReturnType operator()(FunctionParameters... params) const
        {
            return (*m_funcPtr)(params...);
        }
    };


    template <class T, typename ReturnType, typename ...FunctionParameters>
    class MethodPtr<T, ReturnType(FunctionParameters...)> final
    {
        using TMethodPtr = FunctionPtr<ReturnType(FunctionParameters...)>;
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