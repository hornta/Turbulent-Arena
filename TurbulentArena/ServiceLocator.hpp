//ServiceLocator.hpp

#pragma once

template<class Service>
class ServiceLocator
{
private:
	ServiceLocator(){};
	ServiceLocator(const ServiceLocator&);
	ServiceLocator& operator=(const ServiceLocator&);

public:
	~ServiceLocator(){};

	static void SetService(Service* p_xService)
	{
		m_xService = p_xService;
	}

	static Service* GetService()
	{
		return m_xService;
	}

private:
	static Service* m_xService;
};

template<class Service>
Service* ServiceLocator<Service>::m_xService = nullptr;