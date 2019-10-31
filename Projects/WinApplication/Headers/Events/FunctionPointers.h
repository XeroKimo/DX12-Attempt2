#pragma once
namespace Delegates
{
    namespace Internal
    {
        template <typename ReturnType, typename ...FunctionParameters>
        using FunctionPtr = ReturnType(*)(FunctionParameters...);
    }

    template <class T>
    class FunctionPtr;

    template <typename ReturnType, typename ...FunctionParameters>
    class FunctionPtr<ReturnType(FunctionParameters...)> final
    {
        using TFunctionPtr = FunctionPtr<ReturnType(FunctionParameters...)>;
        using FuncPtr = Internal::FunctionPtr<ReturnType, FunctionParameters...>;
    private:
        FuncPtr m_funcPtr = nullptr;

    public:
        FunctionPtr() = default;
        FunctionPtr(const FuncPtr& other) {  m_funcPtr = other; }
        FunctionPtr(const TFunctionPtr& other) { m_funcPtr = other; }
        bool IsNull() const { return m_funcPtr == nullptr; }
        void operator=(const FuncPtr& func) { m_funcPtr = func; }
        void operator=(const TFunctionPtr& func) { m_funcPtr = func.m_funcPtr; }
        bool operator==(const FuncPtr& func) const { return m_funcPtr == func; }
        bool operator==(const TFunctionPtr& func) const { return m_funcPtr == func.m_funcPtr; }
        bool operator!=(const FuncPtr& func) const { return m_funcPtr != func; }
        bool operator!=(const TFunctionPtr& func) const { return m_funcPtr != func.m_funcPtr; }
        ReturnType operator()(FunctionParameters... params) const { return (*m_funcPtr)(params...); }
        static ReturnType Invoke(const FuncPtr& func, FunctionParameters... params) { return (*func)(params...); }
    };
}