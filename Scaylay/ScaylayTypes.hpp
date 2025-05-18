//////////////////////////////////////////////////////////////////////////////
//
// Scaylay (https://github.com/Hapaxia/Scaylay)
//
// Copyright(c) 2023-2025 M.J.Silk
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software.If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
// M.J.Silk
// MJSilk2@gmail.com
//
//////////////////////////////////////////////////////////////////////////////

#ifndef SCAYLAY_SCAYLAYTYPES_HPP
#define SCAYLAY_SCAYLAYTYPES_HPP

namespace scaylay
{

template <class T = float>
struct Vector2Base
{
	T x;
	T y;
};
enum class RelationType
{
	Absolute,
	Relative,
	Scale,
};
enum class AnchorPoint
{
	Start,
	Center,
	End,
	Size,
};
using Vector2 = Vector2Base<float>;
using Vector2Relation = Vector2Base<RelationType>;
using Vector2Anchor = Vector2Base<AnchorPoint>;
template <class T = float>
struct PropertyBase
{
	T value;
	RelationType relation;
	AnchorPoint anchor;
};
template <class T = float>
struct Property2Base
{
	PropertyBase<T> x;
	PropertyBase<T> y;

	Property2Base() = default;
	Property2Base(const Vector2Base<T> vector2, const Vector2Relation relation2, const Vector2Anchor anchor2)
	{
		x.value = vector2.x;
		y.value = vector2.y;
		x.relation = relation2.x;
		y.relation = relation2.y;
		x.anchor = anchor2.x;
		y.anchor = anchor2.y;
	}
	Property2Base(const PropertyBase<T> newX, const PropertyBase<T> newY)
	{
		x = newX;
		y = newY;
	}

	Vector2Base<T> getValue2() const { return{ x.value, y.value }; }
	void operator=(const Vector2 vector2) { x.value = vector2.x; y.value = vector2.y; }
	void operator=(const Vector2Relation relation2) { x.relation = relation2.x; y.relation = relation2.y; }
	void operator=(const Vector2Anchor anchor2) { x.anchor = anchor2.x; y.anchor = anchor2.y; }
};
using Property = PropertyBase<float>;
using Property2 = Property2Base<float>;

} // namespace scaylay

#ifndef SCAYLAY_NO_NAMESPACE_SHORTCUT
namespace sc = scaylay; // create shortcut namespace
#endif // SCAYLAY_NO_NAMESPACE_SHORTCUT

#endif // SCAYLAY_SCAYLAYTYPES_HPP
