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

#ifndef SCAYLAY_SCAYLAY_HPP
#define SCAYLAY_SCAYLAY_HPP

#include "ScaylayTypes.hpp"

#include <vector>
#include <string>

namespace scaylay
{

// Scaylay Design v0.1.0
class Design
{
public:
	std::string getInfo(); // returns a human-readable string with some details of all frames

	Design();

	std::size_t getCount() const { return m_frames.size(); }
	std::size_t add(
		Property2 startOffset = { { 0.f, RelationType::Scale }, { 0.f, RelationType::Scale } },
		bool isConsideredPoint = true,
		int parentIndex = -1,
		Property2 endOffset = { { 0.f, RelationType::Scale }, { 0.f, RelationType::Scale } },
		std::vector<Property> generics = {});
	std::size_t addAbsoluteRectangle(Vector2 position = { 0.f, 0.f }, Vector2 size = { 0.f, 0.f });
	std::size_t addRelativeRectangle(std::size_t parentIndex, Vector2 position = { 0.f, 0.f }, Vector2 size = { 0.f, 0.f });

	std::size_t appendGeneric(float defaultGenericValue = 0.f, RelationType defaultRelationType = RelationType::Relative);
	void resizeGenerics(std::size_t numberOfGenerics); // should trim all frames' generics to match and add extra ones (a default one, probably {0, relative}) if not enough.
	void removeGeneric(std::size_t genericIndex);
	void removeGenerics();

	std::size_t getNumberOfGenerics() const;

	void setParent(std::size_t index, int parentIndex);
	void setStartOffset(std::size_t index, Vector2 offset);
	void setEndOffset(std::size_t index, Vector2 offset);

	void setStartAnchorPoint(std::size_t index, AnchorPoint anchorPoint);
	void setStartOffsetXAnchorPoint(std::size_t index, AnchorPoint anchorPoint);
	void setStartOffsetYAnchorPoint(std::size_t index, AnchorPoint anchorPoint);
	void setEndAnchorPoint(std::size_t index, AnchorPoint anchorPoint);
	void setEndOffsetXAnchorPoint(std::size_t index, AnchorPoint anchorPoint);
	void setEndOffsetYAnchorPoint(std::size_t index, AnchorPoint anchorPoint);

	void setStartOffsetRelationType(std::size_t index, RelationType relationType);
	void setStartOffsetXRelationType(std::size_t index, RelationType relationType);
	void setStartOffsetYRelationType(std::size_t index, RelationType relationType);
	void setEndOffsetRelationType(std::size_t index, RelationType relationType);
	void setEndOffsetXRelationType(std::size_t index, RelationType relationType);
	void setEndOffsetYRelationType(std::size_t index, RelationType relationType);

	void setGeneric(std::size_t index, std::size_t genericIndex, float genericValue);
	void setGenericRelationType(std::size_t index, std::size_t genericIndex, RelationType relationType);

	int getParent(std::size_t index) const;
	Vector2 getStartOffset(std::size_t index) const;
	Vector2 getEndOffset(std::size_t index) const;
	float getGeneric(std::size_t index, std::size_t genericIndex) const;

	AnchorPoint getStartOffsetXAnchorPoint(const std::size_t index) const;
	AnchorPoint getStartOffsetYAnchorPoint(const std::size_t index) const;
	AnchorPoint getEndOffsetXAnchorPoint(const std::size_t index) const;
	AnchorPoint getEndOffsetYAnchorPoint(const std::size_t index) const;
	RelationType getStartOffsetXRelationType(std::size_t index) const;
	RelationType getStartOffsetYRelationType(std::size_t index) const;
	RelationType getEndOffsetXRelationType(std::size_t index) const;
	RelationType getEndOffsetYRelationType(std::size_t index) const;
	RelationType getGenericRelationType(std::size_t index, std::size_t genericIndex) const;

	Vector2 getStartAbsolute(std::size_t index) const;
	Vector2 getEndAbsolute(std::size_t index) const;
	Vector2 getSizeAbsolute(std::size_t index) const;
	float getGenericAbsolute(std::size_t index, std::size_t genericIndex) const;

	Vector2 getPointInFrame(std::size_t index, Vector2 point, RelationType relationType, AnchorPoint anchorPoint) const; // relation and anchor applies to both x and y components equally here
	Vector2 getPointInFrame(std::size_t index, Vector2 point, Vector2Relation relations, Vector2Anchor anchors) const;










private:
	struct Frame
	{
		bool isConsideredPoint;
		int parentIndex;
		Property2 start;
		Property2 end;
		std::vector<Property> generics;
	};

	std::vector<Frame> m_frames;

	std::size_t m_numOfGenerics;

	enum class ComponentType
	{
		X,
		Y,
	};
	enum class ValueType
	{
		Start,
		End,
		Generic,
	};

	float priv_unpackComponent(const Property property, const ValueType valueType, const int parentIndex = -1, const ComponentType componentType = ComponentType::X, const std::size_t genericIndex = 0u, const Property oppositeProperty = Property{}) const;

	Vector2 priv_getLocal(const std::size_t index, const ValueType valueType) const;
	float priv_getLocalComponent(const std::size_t index, const ValueType valueType, const ComponentType componentType = ComponentType::X, const std::size_t genericIndex = 0u) const;

	bool priv_isValidFrameIndex(const int index) const;
	bool priv_isValidFrameIndex(const std::size_t index) const;
};









// PUBLIC
inline void Design::setParent(const std::size_t index, int parentIndex)
{
	if (!priv_isValidFrameIndex(index))
		return;

	if (parentIndex < -1)
		parentIndex = -1;
	m_frames[index].parentIndex = parentIndex;
}

inline void Design::setStartOffset(const std::size_t index, const Vector2 startOffset)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].start = startOffset;
}

inline void Design::setEndOffset(const std::size_t index, const Vector2 endOffset)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].end = endOffset;
}

inline void Design::setStartAnchorPoint(const std::size_t index, const AnchorPoint anchorPoint)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].start = { anchorPoint, anchorPoint };
}

inline void Design::setStartOffsetXAnchorPoint(const std::size_t index, const AnchorPoint anchorPoint)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].start.x.anchor = anchorPoint;
}

inline void Design::setStartOffsetYAnchorPoint(const std::size_t index, const AnchorPoint anchorPoint)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].start.y.anchor = anchorPoint;
}

inline void Design::setEndAnchorPoint(const std::size_t index, const AnchorPoint anchorPoint)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].end = { anchorPoint, anchorPoint };
}

inline void Design::setEndOffsetXAnchorPoint(const std::size_t index, const AnchorPoint anchorPoint)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].end.x.anchor = anchorPoint;
}

inline void Design::setEndOffsetYAnchorPoint(const std::size_t index, const AnchorPoint anchorPoint)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].end.y.anchor = anchorPoint;
}

inline void Design::setStartOffsetRelationType(const std::size_t index, const RelationType relationType)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].start = { relationType, relationType };
}

inline void Design::setStartOffsetXRelationType(const std::size_t index, const RelationType relationType)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].start.x.relation = relationType;
}

inline void Design::setStartOffsetYRelationType(const std::size_t index, const RelationType relationType)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].start.y.relation = relationType;
}

inline void Design::setEndOffsetRelationType(const std::size_t index, const RelationType relationType)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].end = { relationType, relationType };
}

inline void Design::setEndOffsetXRelationType(const std::size_t index, const RelationType relationType)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].end.x.relation = relationType;
}

inline void Design::setEndOffsetYRelationType(const std::size_t index, const RelationType relationType)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].end.y.relation = relationType;
}

inline void Design::setGeneric(const std::size_t index, std::size_t genericIndex, const float genericValue)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].generics[genericIndex].value = genericValue;
}

inline void Design::setGenericRelationType(const std::size_t index, const std::size_t genericIndex, const RelationType relationType)
{
	if (!priv_isValidFrameIndex(index))
		return;

	m_frames[index].generics[genericIndex].relation = relationType;
}

inline int Design::getParent(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return -1;

	return m_frames[index].parentIndex;
}

inline Vector2 Design::getStartOffset(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return m_frames[index].start.getValue2();
}

inline Vector2 Design::getEndOffset(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return m_frames[index].end.getValue2();
}

inline float Design::getGeneric(const std::size_t index, const std::size_t genericIndex) const
{
	if (!priv_isValidFrameIndex(index))
		return 0.f;

	return m_frames[index].generics[genericIndex].value;
}

inline AnchorPoint Design::getStartOffsetXAnchorPoint(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return m_frames[index].start.x.anchor;
}

inline AnchorPoint Design::getStartOffsetYAnchorPoint(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return m_frames[index].start.y.anchor;
}

inline AnchorPoint Design::getEndOffsetXAnchorPoint(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return m_frames[index].end.x.anchor;
}

inline AnchorPoint Design::getEndOffsetYAnchorPoint(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return m_frames[index].end.y.anchor;
}

inline RelationType Design::getStartOffsetXRelationType(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return m_frames[index].start.x.relation;
}

inline RelationType Design::getStartOffsetYRelationType(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return m_frames[index].start.y.relation;
}

inline RelationType Design::getEndOffsetXRelationType(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return m_frames[index].end.x.relation;
}

inline RelationType Design::getEndOffsetYRelationType(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return m_frames[index].end.y.relation;
}

inline RelationType Design::getGenericRelationType(const std::size_t index, const std::size_t genericIndex) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return m_frames[index].generics[genericIndex].relation;
}

inline std::size_t Design::appendGeneric(const float genericValue, const RelationType relationType)
{
	resizeGenerics(m_numOfGenerics); // shouldn't be needed if everything else for generics is working properly. we'll leave it to make sure - for now, at least.

	// could do the resize above with a "+1" and then set the value instead of pushing it back, maybe...

	for (auto& frame : m_frames)
		frame.generics.push_back({ genericValue, relationType });

	return m_numOfGenerics++; // increase m_numOfGenerics but return the value before increasing (this is the index of the newly appended one)
}

inline void Design::resizeGenerics(const std::size_t numberOfGenerics)
{
	m_numOfGenerics = numberOfGenerics;
	for (auto& frame : m_frames)
		frame.generics.resize(m_numOfGenerics, { 0.f, RelationType::Relative }); // default generic of { 0, relative } added if not enough generics in frame
}

inline void Design::removeGeneric(const std::size_t genericIndex)
{
	for (auto& frame : m_frames)
	{
		if (frame.generics.empty())
			continue;
		if (frame.generics.size() > genericIndex)
			frame.generics.erase(frame.generics.begin() + genericIndex);
		if (frame.generics.size() > m_numOfGenerics)
			m_numOfGenerics = frame.generics.size();
	}
	resizeGenerics(m_numOfGenerics);
}

inline void Design::removeGenerics()
{
	m_numOfGenerics = 0u;
	for (auto& frame : m_frames)
		frame.generics.clear();
}

inline std::size_t Design::getNumberOfGenerics() const
{
	return m_numOfGenerics;
}




inline Vector2 Design::getStartAbsolute(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return priv_getLocal(index, ValueType::Start);
}

inline Vector2 Design::getEndAbsolute(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	return priv_getLocal(index, ValueType::End);
}

inline Vector2 Design::getSizeAbsolute(const std::size_t index) const
{
	if (!priv_isValidFrameIndex(index))
		return{};

	Vector2 start{ getStartAbsolute(index) };
	Vector2 end{ getEndAbsolute(index) };
	return{ end.x - start.x, end.y - start.y };
}

inline float Design::getGenericAbsolute(const std::size_t index, const std::size_t genericIndex) const
{
	if (!priv_isValidFrameIndex(index))
		return 0.f;

	return priv_getLocalComponent(index, ValueType::Generic, ComponentType::X, genericIndex);
}

inline Vector2 Design::getPointInFrame(const std::size_t index, const Vector2 point, const RelationType relationType, const AnchorPoint anchorPoint) const
{
	return getPointInFrame(index, point, { relationType, relationType }, { anchorPoint, anchorPoint });
}

inline Vector2 Design::getPointInFrame(const std::size_t index, const Vector2 point, const Vector2Relation relations, const Vector2Anchor anchors) const
{
	return Vector2{
		priv_unpackComponent({ point.x, relations.x, anchors.x }, ValueType::Start, static_cast<int>(index), ComponentType::X),
		priv_unpackComponent({ point.y, relations.y, anchors.y }, ValueType::Start, static_cast<int>(index), ComponentType::Y) };
}







// PRIVATE

inline bool Design::priv_isValidFrameIndex(const int index) const
{
	return (index >= 0) && (static_cast<std::size_t>(index) < m_frames.size());
}

inline bool Design::priv_isValidFrameIndex(const std::size_t index) const
{
	return index < m_frames.size();
}

} // namespace scaylay
#endif // SCAYLAY_SCAYLAY_HPP
