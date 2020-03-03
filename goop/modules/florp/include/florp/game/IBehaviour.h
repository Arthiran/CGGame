#pragma once
#include <memory>
#include <vector>
#include <entt.hpp>
#include <type_traits>
#include <typeindex>

namespace florp {
	namespace game {

		struct BehaviourBinding;
		
		/*
		 * Represents a behaviour that can be tied to a single GameObject
		 */
		class IBehaviour {
		public:
			bool    Enabled = true;
			virtual ~IBehaviour() = default;
			
			virtual void OnLoad(entt::entity entity){}
			virtual void OnUnload(entt::entity entity){}
			virtual void Update(entt::entity entity){}
			virtual void FixedUpdate(entt::entity entity){}
			virtual void LateUpdate(entt::entity entity){}
			virtual void RenderGUI(entt::entity entity) {}
			
		protected:
			IBehaviour() = default;
		};

		/*
		 * The component added to an entt entity to connect a behaviour to a specific entity
		 */
		struct BehaviourBinding {
			std::vector<std::shared_ptr<IBehaviour>> Behaviours;

			template <typename T, typename ... TArgs, typename = typename std::enable_if<std::is_base_of<IBehaviour, T>::value>::type>
			static void Bind(entt::registry& registry, entt::entity entity, TArgs&&... args) {
				BehaviourBinding& binding = registry.get_or_assign<BehaviourBinding>(entity);
				const std::shared_ptr<IBehaviour> behaviour = std::make_shared<T>(std::forward<TArgs>(args)...);
				binding.Behaviours.push_back(behaviour);
				behaviour->OnLoad(entity);
			}

			template <typename T, typename = typename std::enable_if<std::is_base_of<IBehaviour, T>::value>::type>
			static bool Has(entt::registry& registry, entt::entity entity) {
				if (registry.has<BehaviourBinding>(entity)) {
					const auto& binding = registry.get<BehaviourBinding>(entity);
					for (const auto& ptr : binding.Behaviours) {
						if (std::type_index(typeid(*ptr.get())) == std::type_index(typeid(T))) {
							return true;
						}
					}
					return false;
				}
				else return false;
			}

			template <typename T, typename = typename std::enable_if<std::is_base_of<IBehaviour, T>::value>::type>
			static std::shared_ptr<T> Get(entt::registry& registry, entt::entity entity) {
				if (registry.has<BehaviourBinding>(entity)) {
					const auto& binding = registry.get<BehaviourBinding>(entity);
					for (const auto& ptr : binding.Behaviours) {
						if (std::type_index(typeid(*ptr.get())) == std::type_index(typeid(T))) {
							return std::dynamic_pointer_cast<T>(ptr);
						}
					}
					return nullptr;
				}
				else return nullptr;
			}
		};
				
	}
}
