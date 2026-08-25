#pragma once
#include <string>
#include <glm/glm.hpp>

class Component; // Forward declaration

struct Sentinel {
	Component* comp{ nullptr };
};

class Component {
public:
	Component() : sentinel{ new Sentinel{this} } {}
	~Component() { delete sentinel; }

	// deleting copy ctors
	Component(const Component&) = delete;
	const Component& operator=(const Component&) = delete;

	// move ctor to update sentinel
	Component(Component&& c) noexcept : sentinel{ c.sentinel } {
		c.sentinel = nullptr;
		sentinel->comp = this;
	}
	const Component& operator=(Component&& c) noexcept {
		sentinel = c.sentinel;
		c.sentinel = nullptr;
		sentinel->comp = this;
		return *this;
	}
	
public:
	Sentinel* GetSentinel() noexcept { return sentinel; }

private:
	Sentinel* sentinel;
};

struct NameComponent : public Component {
	std::string name{};
};


struct PositionComponent : public Component {
	glm::vec3 pos{};
};

