#pragma once
#include <memory>
#include <functional>
#include <type_traits>

//#define DO_LOG_VALUE_PTR
#ifdef DO_LOG_VALUE_PTR
#include <iostream>
#define LOG_VALUE_PTR(x) std::cout << x << " value_ptr<" << typeid(stored_type).name() << ">" << std::endl;
#else
#define LOG_VALUE_PTR(x)
#endif

namespace stdish
{
	template <typename value_t>
	class value_ptr
	{
	public:
		using stored_type = value_t;
		using pointer_type = std::unique_ptr<stored_type>;
		using this_type = value_ptr<stored_type> const*;
		using copy_func_t = std::function<pointer_type(this_type)>;

		value_ptr()
		{
			LOG_VALUE_PTR("Constructing Empty");
		}

		template<typename derived_t, 
			typename = std::enable_if_t<std::is_base_of<value_t, derived_t>::value> >
		value_ptr(derived_t value)
		{
			static_assert(std::is_base_of<value_t, derived_t>::value, "derived_t must be derived from value_t");
			LOG_VALUE_PTR("Constructing From derived");

			store_ = std::make_unique<derived_t>(std::move(value));
			copy_func_ = [](auto this_) -> pointer_type
			{

				//LOG_VALUE_PTR( "Cloning type " << typeid(derived_t).name() );
				auto base_ptr = this_->get();
				auto derived_ptr = static_cast<derived_t const*>(base_ptr);
				auto cloned_ptr = std::make_unique<derived_t>(*derived_ptr);
				return std::move(cloned_ptr);
			};
		}

		template <>
		explicit value_ptr(stored_type value)
		{
			LOG_VALUE_PTR("Construcing From Base");

			store_ = std::make_unique<stored_type>(std::move(value));
			copy_func_ = [](auto this_) -> pointer_type
			{
				auto& base_value = *this_->get();
				auto cloned_ptr = std::make_unique<stored_type>(std::move(base_value));
				return std::move(cloned_ptr);
			};
		}

		~value_ptr() 
		{
			LOG_VALUE_PTR("Destructing");
		}

		value_ptr(value_ptr<stored_type> const& other)
		{
			LOG_VALUE_PTR("Copy Constructor");
			store_ = other.clone();
			copy_func_ = other.copy_func_;
		}

		value_ptr(value_ptr<stored_type>&& other)
		{
			LOG_VALUE_PTR("Move Constructor");
			store_ = std::move(other.store_);
			copy_func_ = std::move(other.copy_func_);
		}

		value_ptr& operator=(value_ptr<stored_type> const& other)
		{
			LOG_VALUE_PTR("Copy Assignment");
			store_ = other.clone();
			copy_func_ = other.copy_func_;
			return *this;
		}

		value_ptr& operator=(value_ptr<stored_type>&& other)
		{
			LOG_VALUE_PTR("Move Assignment");
			store_ = std::move(other.store_);
			copy_func_ = std::move(other.copy_func_);
			return *this;
		}

		value_t* get() { return store_.get(); }
		value_t const* get() const { return store_.get(); }
		value_t& operator* () { return *get(); }
		value_t const& operator* () const { return *get(); }
		value_t* operator-> () { return get(); }
		value_t const* operator-> () const { return get(); }

		pointer_type clone() const 
		{
			return std::move(copy_func_(this));
		};

		template <typename rhs_t>
		auto operator<=>(rhs_t const& rhs) const 
		{
			return *get() <=> *rhs.get();
		}

	private:
		std::unique_ptr<stored_type> store_;
		copy_func_t copy_func_;
	};
}

