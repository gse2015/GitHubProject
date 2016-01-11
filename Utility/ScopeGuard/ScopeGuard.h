#ifndef _SCOPEGUARD_H_
#define _SCOPEGUARD_H_

#include <functional>
namespace scopeguard
{
class CScopeGuard
{
public:
	explicit CScopeGuard(std::function<void()> onExitScope)
		: m_onExitScopeFunPtr(onExitScope)
		, m_bDismissed(false)
	{ }

	~CScopeGuard()
	{
		if(!m_bDismissed)
		{
			m_onExitScopeFunPtr();
		}
	}

	void Dismiss()
	{
		m_bDismissed = true;
	}

private:
	std::function<void()> m_onExitScopeFunPtr;
	bool m_bDismissed;

private: // noncopyable
	CScopeGuard(CScopeGuard const&);
	CScopeGuard& operator=(CScopeGuard const&);
};

#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)

#define ON_SCOPE_EXIT(callback) scopeguard::CScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)(callback)

}
#endif