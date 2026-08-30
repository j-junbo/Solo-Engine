#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <iostream>
#include "utils.h"
#include "components.h"


class ComponentStorage {
private:

	class InternalStorage_Base {
	public:
		InternalStorage_Base() = default;
		virtual ~InternalStorage_Base() = default;

		InternalStorage_Base(const InternalStorage_Base&) = delete;
		InternalStorage_Base& operator=(const InternalStorage_Base&) = delete;

		virtual void* getSparse() = 0;				// Gets the sparse set
		virtual void* getDense() = 0;				// Gets the dense set
		virtual void* getIndex() = 0;				// Gets the index set, to find the index in sparse set from dense set
		virtual bool  hasComponent(size_t) = 0;		// Check if an entity has the stored component
		virtual void  removeComponent(size_t) = 0;	// Remove a component for an entity
		virtual void* getComponent(size_t) = 0;		// Get a component from an entity

		// Add component will most likely have to stay as a template function in CompStorage 
		// as it is hard to pass through values into internalStorage
		
	};

	template <typename T>
	class InternalStorage : public InternalStorage_Base {
		std::vector<size_t> storage_sparse{ std::vector<size_t>(10,(size_t)-1) };	// note to change the count later on.
		std::vector<T>		storage_dense;
		std::vector<size_t> dense_index;
	public:

		virtual void* getSparse() override {
			return &storage_sparse;
		}

		virtual void* getDense() override {
			return &storage_dense;
		}

		virtual void* getIndex() override {
			return &dense_index;
		}

		virtual bool  hasComponent(size_t id) override {
			if (id >= storage_sparse.size()) return false;
			return storage_sparse[id] != (size_t)-1;
		}

		virtual void  removeComponent(size_t id) override {

			if (id >= storage_sparse.size()) return;		// Does not exist
			if (storage_sparse[id] == (size_t)-1) return;	// Does not have component

			// Effectively swap the positions of the current last entity
			// with this entity, then pop back.
			storage_sparse[dense_index[storage_dense.size() - 1]] = storage_sparse[id];
			std::swap(storage_dense[storage_sparse[id]], storage_dense[storage_dense.size() - 1]);
			std::swap(dense_index[storage_sparse[id]],	 dense_index[storage_dense.size() - 1]);

			storage_dense.pop_back();
			dense_index.pop_back();
			storage_sparse[id] = (size_t)-1;

		}

		virtual void* getComponent(size_t id) override {
			if (id >= storage_sparse.size()) return{};		// Does not exist
			if (storage_sparse[id] == (size_t)-1) return{};	// Does not have component

			return (void*)&storage_dense[storage_sparse[id]];
		}
	};

	std::unique_ptr<InternalStorage_Base> componentStorage;
	DEBUG_ONLY_(size_t typeID{};)
	DEBUG_ONLY_(std::string typeName{};)

public:
	template <typename T>
	ComponentStorage(T&&) : componentStorage{ new InternalStorage<T> } {
		// Not done together as this will not exist outside of debug mode
		DEBUG_ONLY_(typeID = typeid(T).hash_code();)
		DEBUG_ONLY_(typeName = typeid(T).name();)
	}


	// Returns [Sparse set, Dense set]
	template <typename T>
	std::pair<std::vector<size_t>&, std::vector<T>&> getStorage() {
		DEBUG_ONLY_(
			if (typeID != typeid(T).hash_code()) {
				std::cout << "TypeID Mismatch!!\n";
				std::cout << "Called with type: " << typeid(T).name() << '\n';
				std::cout << "Storing type: " << typeName << '\n';
				throw;
			}
		)

		return { *reinterpret_cast<std::vector<size_t>*>(componentStorage.get()->getSparse()),*reinterpret_cast<std::vector<T>*>(componentStorage.get()->getDense()) };
	}

	std::vector<size_t>& getIndex() {
		return *reinterpret_cast<std::vector<size_t>*>(componentStorage.get()->getIndex());
	}

	template <typename T>
	void addComponent(size_t id, T&& component) {
		auto [sparse, dense] = getStorage<T>();
		auto& index = getIndex();

		if (id >= sparse.size()) {
			sparse.resize(id + 1, (size_t)-1);
		}

		dense.push_back(std::move(component));
		index.push_back(id);
		sparse[id] = dense.size() - 1;
	}

	void removeComponent(size_t id) {
		componentStorage.get()->removeComponent(id);
	}

	bool hasComponent(size_t id) {
		return componentStorage.get()->hasComponent(id);
	}

	template <typename T>
	T& getComponent(size_t id) {
		return *reinterpret_cast<T*>(componentStorage.get()->getComponent(id));
	}

	friend class Registry;	// Only registry can make these changes.
	friend class SceneLayer;
};


class Registry {
private:
	// Private constructor, so cannot be made by anyone except for scene layers
	Registry() = default;

	// Delete the copy assignment and constructors
	Registry(const Registry&) = delete;
	Registry& operator=(const Registry&) = delete;

	friend class SceneLayer;

	std::unordered_map<std::type_index, ComponentStorage> componentMap;

public:

	Registry(Registry&&) = default;
	Registry& operator=(Registry&&) = default;

	// Check if the component is in storage or not
	template<typename T>
	bool hasStoredComponent() {
		return componentMap.find(std::type_index(typeid(T))) != componentMap.end();
	}

	// Returns [Sparse set, Dense set]
	// Does not check if said component exists or not, please do hasComponent beforehand if unsure.
	template<typename T>
	std::pair<std::vector<size_t>&, std::vector<T>&> getComponentStorage() {
		return componentMap.find(std::type_index(typeid(T)))->second.getStorage<T>();
	}

	// Default intializes a ComponentStorage
	template<typename T>
	void loadComponent() {
		componentMap.emplace(std::type_index(typeid(T)), T{});
	}

	// Check if the entity has a component or not
	template<typename T>
	bool hasComponent(size_t id) {
		return componentMap.find(std::type_index(typeid(T)))->second.hasComponent<T>(id);
	}

	// Get a specific entity's component
	template<typename T>
	T& getComponent(size_t id) {
		return componentMap.find(std::type_index(typeid(T)))->second.getComponent<T>(id);
	}

	// Remove a specific entity's component
	template<typename T>
	void removeComponent(size_t id) {
		componentMap.find(std::type_index(typeid(T)))->second.removeComponent(id);
	}

	// Remove a specific entity
	void removeEntity(size_t id) {
		for (auto& pair : componentMap) {
			pair.second.removeComponent(id);
		}
	}

	// Get a list of currently active entities
	std::vector<size_t> getActiveEntities() {
		ComponentStorage& activeStorage = componentMap.find(std::type_index(typeid(ActiveComponent)))->second;
		auto [sparse, dense] = activeStorage.getStorage<ActiveComponent>();

		size_t index{};
		std::vector<size_t> list;
		for (size_t i : sparse) {
			if (i == (size_t)-1) continue;

			if (dense[i].active) list.push_back(index);

			index++;
		}

		return list;
	}

private:

};


