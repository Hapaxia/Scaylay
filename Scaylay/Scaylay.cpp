//////////////////////////////////////////////////////////////////////////////
//
// Scaylay (https://github.com/Hapaxia/Scaylay)
//
// Copyright(c) 2023 M.J.Silk
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

#include "Scaylay.hpp"

#include <algorithm> // for std::sort

#include <string>
#include <sstream>

namespace scaylay
{

std::stringstream ss;

inline std::string stringFrom(const Vector2& v, const std::string& separator = ", ", const std::size_t precision = 0u)
{
	ss.str(std::string());

	ss.precision(precision);

	ss << std::fixed << "(" << v.x << separator << v.y << ")";

	return ss.str();
}
inline std::string stringFrom(const Property2& v, const std::string& separator = ", ", const std::size_t precision = 0u)
{
	return stringFrom(Vector2{ v.x.value, v.y.value }, separator, precision);
}
inline std::string stringFrom(const Vector2Anchor& v, const std::string& separator = ", ")
{
	return "(" + std::to_string(static_cast<unsigned int>(v.x)) + separator + std::to_string(static_cast<unsigned int>(v.y)) + ")";
}
inline std::string stringFrom(const Vector2Relation& v, const std::string& separator = ", ")
{
	return "(" + std::to_string(static_cast<unsigned int>(v.x)) + separator + std::to_string(static_cast<unsigned int>(v.y)) + ")";
}
std::string Design::getInfo()
{
	if (m_frames.size() == 0u)
		return "";

	std::size_t i{ 0u }; 
	std::string s;
	for (auto& f : m_frames)
	{
		Vector2 difference{ f.end.x.value - f.start.x.value, f.end.y.value - f.start.y.value };
		s += "[" + std::to_string(i++) + "] ";
		s += "prnt:";
		s += std::to_string(f.parentIndex);
		s += " || grp: ";
		s += std::to_string(f.groupId);
		s += " || dth: ";
		s += std::to_string(f.depth);
		s += " || st: ";
		s += stringFrom(f.start, ", ", 2u);
		s += " {rel: ";
		s += stringFrom({ f.start.x.relation, f.start.y.relation }, ", ");
		s += "}";
		s += " {anc: ";
		s += stringFrom({ f.start.x.anchor, f.start.y.anchor }, ", ");
		s += "}";
		s += " || nd: ";
		s += stringFrom(f.end, ", ", 2u);
		s += " {rel: ";
		s += stringFrom({ f.end.x.relation, f.end.y.relation }, ", ");
		s += "}";
		s += " {anc: ";
		s += stringFrom({ f.end.x.anchor, f.end.y.anchor }, ", ");
		s += "}";
		s += " || df: ";
		s += stringFrom(difference, "x", 2u);
		for (std::size_t g{ 0u }; g < getNumberOfGenerics(); ++g)
		{
			s += " || gen[" + std::to_string(g) + "]: ";
			s += std::to_string(f.generics[g].value);
		}
		s += "\n";
	}
	return s;
}



Design::Design()
	: m_frames()
	, m_numOfGenerics{ 0u }
{

}

std::size_t Design::add(
	const Property2 startOffset,
	const bool isConsideredPoint,
	const int parentIndex,
	const int groupId,
	const int depth,
	const Property2 endOffset,
	const std::vector<Property> generics)
{

	m_frames.emplace_back(Frame{
		isConsideredPoint,
		parentIndex,
		groupId,
		depth,
		startOffset,
		endOffset,
		generics });

	if (generics.size() > m_numOfGenerics)
		m_numOfGenerics = generics.size();

	resizeGenerics(m_numOfGenerics);

	return m_frames.size() - 1u;
}

std::size_t Design::addAbsoluteRectangle(const Vector2 position, const Vector2 size)
{
	return add(Property2{ position, { RelationType::Absolute, RelationType::Absolute }, { AnchorPoint::Start, AnchorPoint::Start } },
		false,
		-1,
		0,
		0,
		Property2{ { position.x + size.x, position.y + size.y }, { RelationType::Absolute, RelationType::Absolute }, { AnchorPoint::Start, AnchorPoint::Start } });
}

std::size_t Design::addRelativeRectangle(const std::size_t parentIndex, const Vector2 position, const Vector2 size)
{
	return add(Property2{ position, { RelationType::Relative, RelationType::Relative }, { AnchorPoint::Start, AnchorPoint::Start } },
		false,
		static_cast<int>(parentIndex),
		0,
		0,
		Property2{ { position.x + size.x, position.y + size.y }, { RelationType::Relative, RelationType::Relative }, { AnchorPoint::Start, AnchorPoint::Start } });
}

std::vector<std::size_t> Design::getFramesInGroup(const int groupId) const
{
	std::vector<std::size_t> frames;

	const std::size_t framesSize{ m_frames.size() };
	for (std::size_t i{ 0u }; i < framesSize; ++i)
	{
		if (m_frames[i].groupId == groupId)
			frames.push_back(i);
	}

	return frames;
}

std::vector<std::size_t> Design::getFramesInGroupRange(const int groupIdMin, const int groupIdMax, const bool useInsideRange) const
{
	std::vector<std::size_t> frames;

	const std::size_t framesSize{ m_frames.size() };
	for (std::size_t i{ 0u }; i < framesSize; ++i)
	{
		if (useInsideRange == (m_frames[i].groupId >= groupIdMin) && (m_frames[i].groupId <= groupIdMax))
			frames.push_back(i);
	}

	return frames;
}

std::vector<std::size_t> Design::getFramesInGroups(const std::vector<int>& groupIds) const
{
	std::vector<std::size_t> frames;

	const std::size_t framesSize{ m_frames.size() };
	for (std::size_t i{ 0u }; i < framesSize; ++i)
	{
		for (auto& groupId : groupIds)
		{
			if (m_frames[i].groupId == groupId)
				frames.push_back(i);
		}
	}

	return frames;
}

std::vector<std::size_t> Design::getFramesAtDepth(const int depth) const
{
	std::vector<std::size_t> frames;

	const std::size_t framesSize{ m_frames.size() };
	for (std::size_t i{ 0u }; i < framesSize; ++i)
	{
		if (m_frames[i].depth == depth)
			frames.push_back(i);
	}

	return frames;
}

std::vector<std::size_t> Design::getFramesInDepthRange(const int depthMin, const int depthMax, const bool useInsideRange, const bool sortAscending) const
{
	std::vector<std::size_t> frames;

	const std::size_t framesSize{ m_frames.size() };
	for (std::size_t i{ 0u }; i < framesSize; ++i)
	{
		if (useInsideRange == (m_frames[i].groupId >= depthMin) && (m_frames[i].groupId <= depthMax))
			frames.push_back(i);
	}

	auto depthSortFunction = [&](std::size_t lhs, std::size_t rhs)
	{
		if (m_frames[lhs].depth == m_frames[rhs].depth)
			return sortAscending == (lhs < rhs);
		else
			return m_frames[lhs].depth < m_frames[rhs].depth;
	};

	if (sortAscending)
		std::sort(frames.begin(), frames.end(), depthSortFunction);
	else
		std::sort(frames.rbegin(), frames.rend(), depthSortFunction);

	return frames;
}

std::vector<std::size_t> Design::getFramesToDepth(const int depth, const bool useBelow, const bool sortAscending) const
{
	std::vector<std::size_t> frames;

	const std::size_t framesSize{ m_frames.size() };
	for (std::size_t i{ 0u }; i < framesSize; ++i)
	{
		if ((m_frames[i].depth == depth) || (useBelow == (m_frames[i].depth < depth)))
			frames.push_back(i);
	}

	auto depthSortFunction = [&](std::size_t lhs, std::size_t rhs)
	{
		if (m_frames[lhs].depth == m_frames[rhs].depth)
			return sortAscending == (lhs < rhs);
		else
			return m_frames[lhs].depth < m_frames[rhs].depth;
	};

	if (sortAscending)
		std::sort(frames.begin(), frames.end(), depthSortFunction);
	else
		std::sort(frames.rbegin(), frames.rend(), depthSortFunction);

	return frames;
}

std::vector<std::size_t> Design::getFramesAtAllDepths(const bool sortAscending) const
{
	std::vector<std::size_t> frames(m_frames.size());

	const std::size_t framesSize{ m_frames.size() };
	for (std::size_t i{ 0u }; i < framesSize; ++i)
		frames[i] = i;

	auto depthSortFunction = [&](std::size_t lhs, std::size_t rhs)
	{
		if (m_frames[lhs].depth == m_frames[rhs].depth)
			return sortAscending == (lhs < rhs);
		else
			return m_frames[lhs].depth < m_frames[rhs].depth;
	};

	if (sortAscending)
		std::sort(frames.begin(), frames.end(), depthSortFunction);
	else
		std::sort(frames.rbegin(), frames.rend(), depthSortFunction);

	return frames;
}











// PRIVATE

float Design::priv_unpackComponent(Property property, const ValueType valueType, const int parentIndex, const ComponentType componentType, const std::size_t genericIndex, Property oppositeProperty) const
{
	if ((parentIndex == -1) || ((property.relation == RelationType::Absolute) && (property.anchor != AnchorPoint::Size)))
		return property.value;

	const bool isX{ componentType == ComponentType::X };
	const bool isScaled{ property.relation == RelationType::Scale };

	const Frame& parent{ m_frames[parentIndex] };

	const float parentStart{ priv_unpackComponent(isX ? parent.start.x : parent.start.y, ValueType::Start, parent.parentIndex, componentType, genericIndex) };
	const float parentEnd{ priv_unpackComponent(isX ? parent.end.x : parent.end.y, ValueType::End, parent.parentIndex, componentType, genericIndex) };
	const float parentRange{ parentEnd - parentStart };

	float result{ property.value };

	if (valueType == ValueType::Generic)
	{
		if (isScaled)
			return result * priv_unpackComponent(parent.generics[genericIndex], ValueType::Generic, parent.parentIndex, componentType, genericIndex);
		else
			return result + priv_unpackComponent(parent.generics[genericIndex], ValueType::Generic, parent.parentIndex, componentType, genericIndex);
	}
	else if (isScaled)
		result *= parentRange;

	switch (property.anchor)
	{
	case AnchorPoint::Start:
		return result + parentStart;
	case AnchorPoint::Center:
		return result + ((0.5f * parentStart) + (0.5f * parentEnd));
	case AnchorPoint::End:
		return result + parentEnd;
	case AnchorPoint::Size:
		if (oppositeProperty.anchor == AnchorPoint::Size)
		{
			// if both start and end are size-anchored, us a start anchor for the start offset (end takes precedence)
			if (valueType == ValueType::End)
				oppositeProperty.anchor = AnchorPoint::Start;
			else if (valueType == ValueType::Start)
				property.anchor = AnchorPoint::Start;
		}
		if (property.relation == RelationType::Relative)
			result += parentRange;
		return result + priv_unpackComponent(oppositeProperty, valueType == ValueType::Start ? ValueType::End : ValueType::Start, parentIndex, componentType, genericIndex, property);
	default:
		return result;
	}
}

Vector2 Design::priv_getLocal(const std::size_t index, const ValueType valueType) const
{
	return{ priv_getLocalComponent(index, valueType, ComponentType::X), priv_getLocalComponent(index, valueType, ComponentType::Y) };
}

float Design::priv_getLocalComponent(const std::size_t index, ValueType valueType, const ComponentType componentType, const std::size_t genericIndex) const
{
	const Frame& f{ m_frames[index] };

	const bool isX{ componentType == ComponentType::X };

	if (f.isConsideredPoint && (valueType == ValueType::End))
		valueType = ValueType::Start;
	Property property{ 0.f, RelationType::Absolute, AnchorPoint::Start };
	Property oppositeProperty{ 0.f, RelationType::Absolute, AnchorPoint::Start };
	switch (valueType)
	{
	case ValueType::Start:
		property = isX ? f.start.x : f.start.y;
		oppositeProperty = isX ? f.end.x : f.end.y;
		break;
	case ValueType::End:
		property = isX ? f.end.x : f.end.y;
		oppositeProperty = isX ? f.start.x : f.start.y;
		break;
	case ValueType::Generic:
		property = f.generics[genericIndex];
		break;
	}

	return priv_unpackComponent(property, valueType, f.parentIndex, componentType, genericIndex, oppositeProperty);
}

} // namespace scaylay
