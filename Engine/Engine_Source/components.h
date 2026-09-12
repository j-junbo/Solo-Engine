// credit vadim
#pragma once
#include <string>
#include <glm/glm.hpp>
#include <rttr/rttr_enable.h>

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
	Component& operator=(Component&& c) noexcept {
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
	
	NameComponent() = default;
	NameComponent(const std::string& name) : name{ name } {}
};

struct ActiveComponent : public Component {
	bool active{};
};

struct TransformComponent : public Component {
	glm::vec3 pos{};
	glm::vec3 scale{};
	float rotation{};	// radians
};

struct CameraComponent : public Component {
	glm::vec3 target{};
	float fieldOfView{};
	float near{};
	float far{};
};

struct LightComponent : public Component {
	glm::vec3 ambient{};
	glm::vec3 diffuse{};
	glm::vec3 specular{};
};

struct TextureComponent : public Component {
	// texture object
	glm::vec2 uv{};
};

// this allows for 65k unique objects.
struct ObjectComponent : public Component {
	unsigned short objID{};
};
