//SliderBridge.hpp

#pragma once
#include <memory>
#include <string>

namespace bjoernligan
{
	class SliderBridge
	{
	private:
		SliderBridge(const std::string &p_sLabel);
		SliderBridge(const SliderBridge&);
		SliderBridge& operator=(const SliderBridge&);

	public:
		typedef std::unique_ptr<SliderBridge> Ptr;
		static Ptr Create(const std::string &p_sLabel);

		~SliderBridge();

		const std::string GetLabel() const;
		void SetNewValue(const float &p_fNew);
		bool HasNewValue();
		float GetNewValue();

	private:
		std::string m_sLabel;
		bool m_bNewValue;
		float m_fValue;
	};
}