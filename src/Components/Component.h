#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>
#include "Core/Hash.h"

#define DECLARE_COMPONENT_TYPE_ID(typeName) \
	componentTypeId GetTypeId() const override { return COMPONENT_TYPE_ID; } \
	static constexpr componentTypeId TypeId() { return COMPONENT_TYPE_ID; } \
	static constexpr componentTypeId COMPONENT_TYPE_ID = hash(#typeName)

class Component
{
public:
	typedef uint64_t componentTypeId;

	Component();
	~Component();

	virtual componentTypeId GetTypeId() const = 0;
	bool IsType(componentTypeId typeId) const { return typeId == GetTypeId(); }

private:

};

#endif