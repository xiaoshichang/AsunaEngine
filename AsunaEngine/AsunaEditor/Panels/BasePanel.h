#pragma once


namespace asuna
{

	class BasePanel
	{
	public:
		BasePanel() = default;
		virtual ~BasePanel() = default;

        virtual void Initialize() = 0;
		virtual void Render() = 0;

	protected:
		bool m_Showing{};

	};

}