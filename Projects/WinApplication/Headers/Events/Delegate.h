#pragma once
#include <vector>
#include "FunctionPointers.h"

namespace Delegates
{
    template <class T>
    class Delegate;
    template <class T>
    class Event;

    template <typename ReturnType, typename ...FunctionParameters>
    class Delegate<ReturnType(FunctionParameters...)> final
    {
        using TFunctionWrapper = FunctionPtr<ReturnType(void*, FunctionParameters...)>;
        using TDelegate = Delegate<ReturnType(FunctionParameters...)>;
        using TFunctionPtr = Internal::FunctionPtr<ReturnType, FunctionParameters...>;

        template <class T>
        using TMethodPtr = Internal::MethodPtr<T, ReturnType, FunctionParameters...>;

    private:
        void* m_boundObject = nullptr;
        TFunctionWrapper m_functionWrapper;
    public:

        Delegate() = default;

        Delegate(const TDelegate& other)
        {
            m_boundObject = other.m_boundObject;
            m_functionWrapper = other.m_functionWrapper;
        }

        bool IsNull() const
        {
            return m_functionWrapper.IsNull();
        }

        template<TFunctionPtr func>
        void Bind()
        {
            m_boundObject = nullptr;
            m_functionWrapper = FunctionWrapper<func>;
        }

        template<class T, TMethodPtr<T> function>
        void Bind(T* object)
        {
            m_boundObject = object;
            m_functionWrapper = MethodWrapper<T, function>;
        }

        void Unbind()
        {
            m_boundObject = nullptr;
            m_functionWrapper = nullptr;
        }

        void Swap(TDelegate& other)
        {
            TDelegate temp = other;
            other = *this;
            *this = temp;
        }

        template <TFunctionPtr func>
        static Delegate Generate()
        {
            TDelegate delegate;
            delegate.Bind<func>();
            return delegate;
        }

        template <class T, TMethodPtr<T> func>
        static TDelegate Generate(T* object)
        {
            TDelegate delegate;
            delegate.Bind<T, func>(object);
            return delegate;
        }

        bool operator==(const TDelegate& other) const
        {
            return m_boundObject == other.m_boundObject && m_functionWrapper == other.m_functionWrapper;
        }

        bool operator!=(const TDelegate& other) const
        {
            return m_boundObject != other.m_boundObject && m_functionWrapper != other.m_functionWrapper;
        }

        TDelegate& operator=(const TDelegate& other)
        {
            m_boundObject = other.m_boundObject;
            m_functionWrapper = other.m_functionWrapper;
            return *this;
        }

        ReturnType operator()(FunctionParameters... params)
        {
            return m_functionWrapper(m_boundObject, params...);
        }


    private:
        template <TFunctionPtr Function>
        static ReturnType FunctionWrapper(void* object, FunctionParameters... params)
        {
            return (*Function)(params...);
        }

        template <class T, TMethodPtr<T> Function>
        static ReturnType MethodWrapper(void* object, FunctionParameters... params)
        {
            T* ptr = static_cast<T*>(object);
            return (ptr->*Function)(params...);
        }
    };




    template <typename ReturnType, typename ...FunctionParameters>
    class Event<ReturnType(FunctionParameters...)> final
    {
        using TDelegate = Delegate<ReturnType(FunctionParameters...)>;
        using TFunctionPtr = Internal::FunctionPtr<ReturnType, FunctionParameters...>;

        template <class T>
        using TMethodPtr = Internal::MethodPtr<T, ReturnType, FunctionParameters...>;
    private:
        std::vector<TDelegate> m_delegates;

    public:
        Event() = default;
        Event(const Event& other) = delete;

        void Bind(const TDelegate& other)
        {
            if (other.IsNull())
                return;
            m_delegates.push_back(other);
        }

        template<TFunctionPtr function>
        void Bind()
        {
            m_delegates.push_back(TDelegate());
            m_delegates.back().Bind<function>();
        }

        template<class T, TMethodPtr<T> function>
        void Bind(T* object)
        {
            if (!object)
                return;
            m_delegates.push_back(TDelegate());
            m_delegates.back().Bind<T, function>(object);
        }

        void Unbind(const TDelegate& other)
        {
            if (other.IsNull())
                return;
            int i = 0;
            for (const auto& delegate : m_delegates)
            {
                if (other == delegate)
                {
                    m_delegates[i].Swap(m_delegates[m_delegates.size() - 1]);
                    m_delegates.pop_back();
                    return;
                }
                i++;
            }
        }

        template<TFunctionPtr function>
        void Unbind()
        {
            TDelegate func;
            func.Bind<function>();
            int i = 0;
            for (auto& delegate : m_delegates)
            {
                if (func == delegate)
                {
                    m_delegates[i].swap(m_delegates[m_delegates.size() - 1]);
                    m_delegates.pop_back();
                    return;
                }
                i++;
            }
        }

        template<class T, TMethodPtr<T> function>
        void Unbind(T* object)
        {
            if (object == nullptr)
                return;
            TDelegate func;
            func.Bind<T, function>(object);
            int i = 0;
            for (auto& delegate : m_delegates)
            {
                if (func == delegate)
                {
                    auto it = m_delegates.begin();
                    m_delegates[i].swap(m_delegates[m_delegates.size() - 1]);
                    m_delegates.pop_back();
                    return;
                }
                i++;
            }
        }

        size_t Size() const { return m_delegates.size(); }
        bool Empty() const { return m_delegates.empty(); }

        Event& operator=(const Event& other) = delete;
        void operator+=(const TDelegate& other) { Bind(other); }
        void operator-=(const TDelegate& other) { Unbind(other); }
        void operator()(FunctionParameters... params) { FireEvent(params...); }
        void operator()(std::vector<ReturnType>& returnValues, FunctionParameters... params) { FireEvent(params..., returnValues); }

    private:
        void FireEvent(FunctionParameters... params)
        {
            for (auto& delegate : m_delegates)
            {
                delegate(params...);
            }
        }

        void FireEvent(std::vector<ReturnType>& returnValues, FunctionParameters... params)
        {
            returnValues.reserve(m_delegates.size());
            for (auto& delegate : m_delegates)
            {
                returnValues.push_back(delegate(params...));
            }
        }
    };
}