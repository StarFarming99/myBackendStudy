#pragma once

namespace starfarming
{
	namespace utility
	{
		template<typename T>
		class Singleton
		{
		public:
			static T* getInstance()
			{
				if (m_instance == nullptr)
				{
					m_instance = new T();
				}
				return m_instance;
			}

		private:
			Singleton() {}
			Singleton(const Singleton&) = delete;
			Singleton<T>& operator = (const Singleton<T>&) = delete;
			~Singleton() {}

		private:
			static T* m_instance;
		};
		template <typename T>
		T* Singleton<T>::m_instance = nullptr;
	}
}

