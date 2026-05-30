#include <memory>

template <typename T, class Derived>
class State
{
protected:
    T* m_subject;
public:
    State(T& sub)
        : m_subject{ &sub }
    {}
    virtual ~State() = default;
    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;
    /// <summary>
    /// the main process of the state, to be overwritten and called as needed for the state.
    /// </summary>
    /// <returns>
    /// the new state or null if state didnt switch
    /// </returns>
    virtual std::unique_ptr <Derived> ProcessState() = 0;
};