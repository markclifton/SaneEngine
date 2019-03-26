#ifndef ECS_H
#define ECS_H

#include "se/utils/dlldefines.h"

#include <unordered_map>
#include <functional>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <type_traits>

#ifndef ECS_TICK_TYPE
#define ECS_TICK_TYPE se::DefaultTickData
#endif

#ifndef ECS_ALLOCATOR_TYPE
#define ECS_ALLOCATOR_TYPE std::allocator<se::Entity>
#endif

#include <typeindex>
#include <typeinfo>
#define ECS_TYPE_IMPLEMENTATION

namespace se
{
	typedef std::type_index TypeIndex;

#define ECS_DEFINE_TYPE(name)

	template<typename T>
	TypeIndex getTypeIndex()
	{
		return std::type_index(typeid(T));
	}

	class World;
	class Entity;

	typedef float DefaultTickData;
	typedef ECS_ALLOCATOR_TYPE Allocator;

	namespace se_internal
	{
		template<typename... Types>
		class EntityComponentView;

		class EntityView;

		struct BaseComponentContainer
		{
		public:
			virtual ~BaseComponentContainer() { }
			virtual void destroy(World* world) = 0;
			virtual void removed(Entity* ent) = 0;
		};

		class BaseEventSubscriber
		{
		public:
			virtual ~BaseEventSubscriber() {};
		};

		template<typename... Types>
		class EntityComponentIterator
		{
		public:
			EntityComponentIterator(World* world, size_t index, bool bIsEnd, bool bIncludePendingDestroy);

			size_t getIndex() const { return index; }
			bool isEnd() const;
			bool includePendingDestroy() const { return bIncludePendingDestroy; }
			World* getWorld() const { return world; }
			Entity* get() const;
			Entity* operator*() const { return get(); }

			bool operator==(const EntityComponentIterator<Types...>& other) const
			{
				if (world != other.world) return false;
				if (isEnd()) return other.isEnd();
				return index == other.index;
			}
			bool operator!=(const EntityComponentIterator<Types...>& other) const { return !(*this == other); }
			EntityComponentIterator<Types...>& operator++();

		private:
			bool bIsEnd{ false };
			size_t index;
			class se::World* world;
			bool bIncludePendingDestroy;
		};

		template<typename... Types>
		class EntityComponentView
		{
		public:
			EntityComponentView(const EntityComponentIterator<Types...>& first, const EntityComponentIterator<Types...>& last);

			const EntityComponentIterator<Types...>& begin() const { return firstItr; }
			const EntityComponentIterator<Types...>& end() const { return lastItr; }

		private:
			EntityComponentIterator<Types...> firstItr;
			EntityComponentIterator<Types...> lastItr;
		};
	}

	template<typename T>
	class ComponentHandle
	{
	public:
		ComponentHandle() = default;
		ComponentHandle(T* component) : component(component) {}

		T* operator->() const { return component; }
		operator bool() const { return isValid(); }
		T& get() { return *component; }
		bool isValid() const { return component != nullptr; }

	private:
		T* component{ nullptr };
	};

	class EntitySystem
	{
	public:
		virtual ~EntitySystem() = default;
		virtual void configure(World* world) {}
		virtual void unconfigure(World* world) {}
		virtual void tick(World* world, ECS_TICK_TYPE data) {}
	};

	template<typename T>
	class EventSubscriber : public se_internal::BaseEventSubscriber
	{
	public:
		virtual ~EventSubscriber() = default;
		virtual void receive(World* world, const T& event) = 0;
	};

	namespace Events
	{
		struct OnEntityCreated
		{
			Entity* entity;
		};

		struct OnEntityDestroyed
		{
			Entity* entity;
		};

		template<typename T>
		struct OnComponentAssigned
		{
			Entity* entity;
			ComponentHandle<T> component;
		};

		template<typename T>
		struct OnComponentRemoved
		{
			Entity* entity;
			ComponentHandle<T> component;
		};
	}

	class World
	{
	public:
		using WorldAllocator = std::allocator_traits<Allocator>::template rebind_alloc<World>;
		using EntityAllocator = std::allocator_traits<Allocator>::template rebind_alloc<Entity>;
		using SystemAllocator = std::allocator_traits<Allocator>::template rebind_alloc<EntitySystem>;
		using EntityPtrAllocator = std::allocator_traits<Allocator>::template rebind_alloc<Entity*>;
		using SystemPtrAllocator = std::allocator_traits<Allocator>::template rebind_alloc<EntitySystem*>;
		using SubscriberPtrAllocator = std::allocator_traits<Allocator>::template rebind_alloc<se_internal::BaseEventSubscriber*>;
		using SubscriberPairAllocator = std::allocator_traits<Allocator>::template rebind_alloc<std::pair<const TypeIndex, std::vector<se_internal::BaseEventSubscriber*, SubscriberPtrAllocator>>>;


		static World* createWorld(Allocator alloc)
		{
			WorldAllocator worldAlloc(alloc);
			World* world = std::allocator_traits<WorldAllocator>::allocate(worldAlloc, 1);
			std::allocator_traits<WorldAllocator>::construct(worldAlloc, world, alloc);

			return world;
		}

		static World* createWorld() { return createWorld(Allocator()); }

		void destroyWorld()
		{
			WorldAllocator alloc(entAlloc);
			std::allocator_traits<WorldAllocator>::destroy(alloc, this);
			std::allocator_traits<WorldAllocator>::deallocate(alloc, this, 1);
		}

		World(Allocator alloc)
			: entAlloc(alloc), systemAlloc(alloc),
			entities({}, EntityPtrAllocator(alloc)),
			systems({}, SystemPtrAllocator(alloc)),
			subscribers({}, 0, std::hash<TypeIndex>(), std::equal_to<TypeIndex>(), SubscriberPtrAllocator(alloc))
		{
		}

		~World();

		Entity* create()
		{
			++lastEntityId;
			Entity* ent = std::allocator_traits<EntityAllocator>::allocate(entAlloc, 1);
			std::allocator_traits<EntityAllocator>::construct(entAlloc, ent, this, lastEntityId);
			entities.push_back(ent);

			emit<Events::OnEntityCreated>({ ent });

			return ent;
		}

		void destroy(Entity* ent, bool immediate = false);

		bool cleanup();

		void reset();

		EntitySystem* registerSystem(EntitySystem* system)
		{
			systems.push_back(system);
			system->configure(this);

			return system;
		}

		void unregisterSystem(EntitySystem* system)
		{
			systems.erase(std::remove(systems.begin(), systems.end(), system), systems.end());
			system->unconfigure(this);
		}

		void enableSystem(EntitySystem* system)
		{
			auto it = std::find(disabledSystems.begin(), disabledSystems.end(), system);
			if (it != disabledSystems.end())
			{
				disabledSystems.erase(it);
				systems.push_back(system);
			}
		}

		void disableSystem(EntitySystem* system)
		{
			auto it = std::find(systems.begin(), systems.end(), system);
			if (it != systems.end())
			{
				systems.erase(it);
				disabledSystems.push_back(system);
			}
		}

		template<typename T>
		void subscribe(EventSubscriber<T>* subscriber)
		{
			auto index = getTypeIndex<T>();
			auto found = subscribers.find(index);
			if (found == subscribers.end())
			{
				std::vector<se_internal::BaseEventSubscriber*, SubscriberPtrAllocator> subList(entAlloc);
				subList.push_back(subscriber);

				subscribers.insert({ index, subList });
			}
			else
			{
				found->second.push_back(subscriber);
			}
		}

		template<typename T>
		void unsubscribe(EventSubscriber<T>* subscriber)
		{
			auto index = getTypeIndex<T>();
			auto found = subscribers.find(index);
			if (found != subscribers.end())
			{
				found->second.erase(std::remove(found->second.begin(), found->second.end(), subscriber), found->second.end());
				if (found->second.size() == 0)
				{
					subscribers.erase(found);
				}
			}
		}

		void unsubscribeAll(void* subscriber)
		{
			for (auto kv : subscribers)
			{
				kv.second.erase(std::remove(kv.second.begin(), kv.second.end(), subscriber), kv.second.end());
				if (kv.second.size() == 0)
				{
					subscribers.erase(kv.first);
				}
			}
		}

		template<typename T>
		void emit(const T& event)
		{
			auto found = subscribers.find(getTypeIndex<T>());
			if (found != subscribers.end())
			{
				for (auto* base : found->second)
				{
					auto* sub = reinterpret_cast<EventSubscriber<T>*>(base);
					sub->receive(this, event);
				}
			}
		}

		template<typename... Types>
		void each(typename std::common_type<std::function<void(Entity*, ComponentHandle<Types>...)>>::type viewFunc, bool bIncludePendingDestroy = false);

		void all(std::function<void(Entity*)> viewFunc, bool bIncludePendingDestroy = false);

		template<typename... Types>
		se_internal::EntityComponentView<Types...> each(bool bIncludePendingDestroy = false)
		{
			se_internal::EntityComponentIterator<Types...> first(this, 0, false, bIncludePendingDestroy);
			se_internal::EntityComponentIterator<Types...> last(this, getCount(), true, bIncludePendingDestroy);
			return se_internal::EntityComponentView<Types...>(first, last);
		}

		se_internal::EntityView all(bool bIncludePendingDestroy = false);

		size_t getCount() const
		{
			return entities.size();
		}

		Entity* getByIndex(size_t idx)
		{
			if (idx >= getCount())
				return nullptr;

			return entities[idx];
		}

		Entity* getById(size_t id) const;

		void tick(ECS_TICK_TYPE data)
		{
			cleanup();
			for (auto* system : systems)
			{
				system->tick(this, data);
			}
		}

		EntityAllocator& getPrimaryAllocator() { return entAlloc; }

	private:
		EntityAllocator entAlloc;
		SystemAllocator systemAlloc;

		std::vector<Entity*, EntityPtrAllocator> entities;
		std::vector<EntitySystem*, SystemPtrAllocator> systems;
		std::vector<EntitySystem*> disabledSystems;
		std::unordered_map<TypeIndex,
			std::vector<se_internal::BaseEventSubscriber*, SubscriberPtrAllocator>,
			std::hash<TypeIndex>,
			std::equal_to<TypeIndex>,
			SubscriberPairAllocator> subscribers;

		size_t lastEntityId = 0;
	};

	class Entity
	{
	public:
		friend class World;

		const static size_t InvalidEntityId = 0;

		Entity(World* world, size_t id) : world(world), id(id) {}

		~Entity() { removeAll(); }

		World* getWorld() const { return world; }

		template<typename T>
		bool has() const
		{
			auto index = getTypeIndex<T>();
			return components.find(index) != components.end();
		}

		template<typename T, typename V, typename... Types>
		bool has() const
		{
			return has<T>() && has<V, Types...>();
		}

		template<typename T, typename... Args>
		ComponentHandle<T> assign(Args&&... args);

		template<typename T>
		bool remove()
		{
			auto found = components.find(getTypeIndex<T>());
			if (found != components.end())
			{
				found->second->removed(this);
				found->second->destroy(world);

				components.erase(found);

				return true;
			}

			return false;
		}

		void removeAll()
		{
			for (auto pair : components)
			{
				pair.second->removed(this);
				pair.second->destroy(world);
			}

			components.clear();
		}

		template<typename T>
		ComponentHandle<T> get();

		template<typename... Types>
		bool with(typename std::common_type<std::function<void(ComponentHandle<Types>...)>>::type view)
		{
			if (!has<Types...>())
				return false;

			view(get<Types>()...);
			return true;
		}

		size_t getEntityId() const { return id; }

		bool isPendingDestroy() const { return bPendingDestroy; }

	private:
		std::unordered_map<TypeIndex, se_internal::BaseComponentContainer*> components;
		World* world;

		size_t id;
		bool bPendingDestroy{ false };
	};

	namespace se_internal
	{
		class EntityIterator
		{
		public:
			EntityIterator(World* world, size_t index, bool bIsEnd, bool bIncludePendingDestroy);

			size_t getIndex() const { return index; }

			bool isEnd() const;

			bool includePendingDestroy() const { return bIncludePendingDestroy; }

			World* getWorld() const { return world; }

			Entity* get() const;

			Entity* operator*() const { return get(); }

			bool operator==(const EntityIterator& other) const
			{
				if (world != other.world) return false;
				if (isEnd()) return other.isEnd();
				return index == other.index;
			}

			bool operator!=(const EntityIterator& other) const { return !(*this == other); }

			EntityIterator& operator++();

		private:
			bool bIsEnd = false;
			size_t index;
			class se::World* world;
			bool bIncludePendingDestroy;
		};

		class EntityView
		{
		public:
			EntityView(const EntityIterator& first, const EntityIterator& last)
				: firstItr(first), lastItr(last)
			{
				if (firstItr.get() == nullptr || (firstItr.get()->isPendingDestroy() && !firstItr.includePendingDestroy()))
				{
					++firstItr;
				}
			}

			const EntityIterator& begin() const { return firstItr; }

			const EntityIterator& end() const { return lastItr; }

		private:
			EntityIterator firstItr;
			EntityIterator lastItr;
		};

		template<typename T>
		struct ComponentContainer : public BaseComponentContainer
		{
			ComponentContainer() {}
			ComponentContainer(const T& data) : data(data) {}

			T data;

		protected:
			virtual void destroy(World* world)
			{
				using ComponentAllocator = std::allocator_traits<World::EntityAllocator>::template rebind_alloc<ComponentContainer<T>>;

				ComponentAllocator alloc(world->getPrimaryAllocator());
				std::allocator_traits<ComponentAllocator>::destroy(alloc, this);
				std::allocator_traits<ComponentAllocator>::deallocate(alloc, this, 1);
			}

			virtual void removed(Entity* ent)
			{
				auto handle = ComponentHandle<T>(&data);
				ent->getWorld()->emit<Events::OnComponentRemoved<T>>({ ent, handle });
			}
		};
	}

	inline World::~World()
	{
		for (auto* ent : entities)
		{
			if (!ent->isPendingDestroy())
			{
				ent->bPendingDestroy = true;
				emit<Events::OnEntityDestroyed>({ ent });
			}

			std::allocator_traits<EntityAllocator>::destroy(entAlloc, ent);
			std::allocator_traits<EntityAllocator>::deallocate(entAlloc, ent, 1);
		}

		for (auto* system : systems)
		{
			system->unconfigure(this);
			std::allocator_traits<SystemAllocator>::destroy(systemAlloc, system);
			std::allocator_traits<SystemAllocator>::deallocate(systemAlloc, system, 1);
		}
	}

	inline void World::destroy(Entity* ent, bool immediate)
	{
		if (ent == nullptr) return;

		if (ent->isPendingDestroy())
		{
			if (immediate)
			{
				entities.erase(std::remove(entities.begin(), entities.end(), ent), entities.end());
				std::allocator_traits<EntityAllocator>::destroy(entAlloc, ent);
				std::allocator_traits<EntityAllocator>::deallocate(entAlloc, ent, 1);
			}

			return;
		}

		ent->bPendingDestroy = true;

		emit<Events::OnEntityDestroyed>({ ent });

		if (immediate)
		{
			entities.erase(std::remove(entities.begin(), entities.end(), ent), entities.end());
			std::allocator_traits<EntityAllocator>::destroy(entAlloc, ent);
			std::allocator_traits<EntityAllocator>::deallocate(entAlloc, ent, 1);
		}
	}

	inline bool World::cleanup()
	{
		size_t count = 0;
		entities.erase(std::remove_if(entities.begin(), entities.end(), [&, this](Entity* ent) {
			if (ent->isPendingDestroy())
			{
				std::allocator_traits<EntityAllocator>::destroy(entAlloc, ent);
				std::allocator_traits<EntityAllocator>::deallocate(entAlloc, ent, 1);
				++count;
				return true;
			}

			return false;
		}), entities.end());

		return count > 0;
	}

	inline void World::reset()
	{
		for (auto* ent : entities)
		{
			if (!ent->isPendingDestroy())
			{
				ent->bPendingDestroy = true;
				emit<Events::OnEntityDestroyed>({ ent });
			}
			std::allocator_traits<EntityAllocator>::destroy(entAlloc, ent);
			std::allocator_traits<EntityAllocator>::deallocate(entAlloc, ent, 1);
		}

		entities.clear();
		lastEntityId = 0;
	}

	inline void World::all(std::function<void(Entity*)> viewFunc, bool bIncludePendingDestroy)
	{
		for (auto* ent : all(bIncludePendingDestroy))
		{
			viewFunc(ent);
		}
	}

	inline se_internal::EntityView World::all(bool bIncludePendingDestroy)
	{
		se_internal::EntityIterator first(this, 0, false, bIncludePendingDestroy);
		se_internal::EntityIterator last(this, getCount(), true, bIncludePendingDestroy);
		return se_internal::EntityView(first, last);
	}

	inline Entity* World::getById(size_t id) const
	{
		if (id == Entity::InvalidEntityId || id > lastEntityId)
			return nullptr;

		for (auto* ent : entities)
		{
			if (ent->getEntityId() == id)
				return ent;
		}

		return nullptr;
	}

	template<typename... Types>
	void World::each(typename std::common_type<std::function<void(Entity*, ComponentHandle<Types>...)>>::type viewFunc, bool bIncludePendingDestroy)
	{
		for (auto* ent : each<Types...>(bIncludePendingDestroy))
		{
			viewFunc(ent, ent->template get<Types>()...);
		}
	}

	template<typename T, typename... Args>
	ComponentHandle<T> Entity::assign(Args&&... args)
	{
		using ComponentAllocator = std::allocator_traits<World::EntityAllocator>::template rebind_alloc<se_internal::ComponentContainer<T>>;

		auto found = components.find(getTypeIndex<T>());
		if (found != components.end())
		{
			se_internal::ComponentContainer<T>* container = reinterpret_cast<se_internal::ComponentContainer<T>*>(found->second);
			container->data = T(args...);

			auto handle = ComponentHandle<T>(&container->data);
			world->emit<Events::OnComponentAssigned<T>>({ this, handle });
			return handle;
		}
		else
		{
			ComponentAllocator alloc(world->getPrimaryAllocator());

			se_internal::ComponentContainer<T>* container = std::allocator_traits<ComponentAllocator>::allocate(alloc, 1);
			std::allocator_traits<ComponentAllocator>::construct(alloc, container, T(args...));

			components.insert({ getTypeIndex<T>(), container });

			auto handle = ComponentHandle<T>(&container->data);
			world->emit<Events::OnComponentAssigned<T>>({ this, handle });
			return handle;
		}
	}

	template<typename T>
	ComponentHandle<T> Entity::get()
	{
		auto found = components.find(getTypeIndex<T>());
		if (found != components.end())
		{
			return ComponentHandle<T>(&reinterpret_cast<se_internal::ComponentContainer<T>*>(found->second)->data);
		}

		return ComponentHandle<T>();
	}

	namespace se_internal
	{
		inline EntityIterator::EntityIterator(class World* world, size_t index, bool bIsEnd, bool bIncludePendingDestroy)
			: bIsEnd(bIsEnd), index(index), world(world), bIncludePendingDestroy(bIncludePendingDestroy)
		{
			if (index >= world->getCount())
				this->bIsEnd = true;
		}

		inline bool EntityIterator::isEnd() const { return bIsEnd || index >= world->getCount(); }

		inline Entity* EntityIterator::get() const
		{
			if (isEnd())
				return nullptr;

			return world->getByIndex(index);
		}

		inline EntityIterator& EntityIterator::operator++()
		{
			++index;
			while (index < world->getCount() && (get() == nullptr || (get()->isPendingDestroy() && !bIncludePendingDestroy)))
			{
				++index;
			}

			if (index >= world->getCount())
				bIsEnd = true;

			return *this;
		}

		template<typename... Types>
		EntityComponentIterator<Types...>::EntityComponentIterator(World* world, size_t index, bool bIsEnd, bool bIncludePendingDestroy)
			: bIsEnd(bIsEnd), index(index), world(world), bIncludePendingDestroy(bIncludePendingDestroy)
		{
			if (index >= world->getCount())
				this->bIsEnd = true;
		}

		template<typename... Types>
		bool EntityComponentIterator<Types...>::isEnd() const { return bIsEnd || index >= world->getCount(); }

		template<typename... Types>
		Entity* EntityComponentIterator<Types...>::get() const
		{
			if (isEnd()) return nullptr;

			return world->getByIndex(index);
		}

		template<typename... Types>
		EntityComponentIterator<Types...>& EntityComponentIterator<Types...>::operator++()
		{
			++index;
			while (index < world->getCount() && (get() == nullptr || !get()->template has<Types...>() || (get()->isPendingDestroy() && !bIncludePendingDestroy)))
			{
				++index;
			}

			if (index >= world->getCount())
				bIsEnd = true;

			return *this;
		}

		template<typename... Types>
		EntityComponentView<Types...>::EntityComponentView(const EntityComponentIterator<Types...>& first, const EntityComponentIterator<Types...>& last)
			: firstItr(first), lastItr(last)
		{
			if (firstItr.get() == nullptr || (firstItr.get()->isPendingDestroy() && !firstItr.includePendingDestroy())
				|| !firstItr.get()->template has<Types...>())
			{
				++firstItr;
			}
		}
	}
}

#endif