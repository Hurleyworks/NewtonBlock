/** ==========================================================================
* 2013 by KjellKod.cc. This is PUBLIC DOMAIN to use at your own risk and comes
* with no warranties. This code is yours to share, use and modify with no
* strings attached and no restrictions or obligations.
* ============================================================================*/

#pragma once

#ifndef STD2_MAKE_UNIQUE_HPP_
#define STD2_MAKE_UNIQUE_HPP_

#include <memory>
#include <utility>
#include <type_traits>

namespace std2 {
	namespace impl_fut_stl {

		template<typename T, typename ... Args>
		std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args) {
			return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
		}

		template<typename T, typename ... Args>
		std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args) {
			static_assert(std::extent<T>::value == 0, "make_unique<T[N]>() is forbidden, please use make_unique<T[]>(),");
			typedef typename std::remove_extent<T>::type U;

			return std::unique_ptr<T>(new U[sizeof...(Args)]{
				std::forward<Args>(args)... });
		}
	}

	template<typename T, typename ... Args>
	std::unique_ptr<T> make_unique(Args&&... args) {
		return impl_fut_stl::make_unique_helper<T>(
			std::is_array<T>(), std::forward<Args>(args)...);
	}
}

#endif


	// A straightforward technique to move around packaged_tasks.
	//  Instances of std::packaged_task are MoveConstructible and MoveAssignable, but
	//  not CopyConstructible or CopyAssignable. To put them in a std container they need
	//  to be wrapped and their internals "moved" when tried to be copied.

	template<typename Moveable>
	struct MoveOnCopy {
		mutable Moveable _move_only;

		explicit MoveOnCopy(Moveable&& m) : _move_only(std::move(m)) {}
		MoveOnCopy(MoveOnCopy const& t) : _move_only(std::move(t._move_only)) {}
		MoveOnCopy(MoveOnCopy&& t) : _move_only(std::move(t._move_only)) {}

		MoveOnCopy& operator=(MoveOnCopy const& other) {
			_move_only = std::move(other._move_only);
			return *this;
		}

		MoveOnCopy& operator=(MoveOnCopy&& other) {
			_move_only = std::move(other._move_only);
			return *this;
		}

		void operator()() { _move_only(); }
		Moveable& get() { return _move_only; }
		Moveable release() { return std::move(_move_only); }
	};

