#pragma once
#include <entt.hpp>
#include "IBehaviour.h"

namespace florp {
	namespace game {

		class Scene {
		public:
			Scene() = default;
			virtual ~Scene() = default;
			
			void Load();
			void Unload();

			/*
			 * Gets this scene's ENTT registry
			 */
			entt::registry& Registry() { return myRegistry; }

			/*
			 * Creates a new entity with the default components
			 */
			entt::entity CreateEntity();

			/*
			 * Adds a behaviour to a given entity
			 * @param T The type of behaviour to add
			 * @param TArgs The type parameters for the arguments to forward to the constructor
			 * @param e The entity to add the behaviour to
			 * @param args The arguments to forward to the behaviour's constructor
			 */
			template <typename T, typename ... TArgs, typename = typename std::enable_if<std::is_base_of<IBehaviour, T>::value>::type>
			void AddBehaviour(entt::entity e, TArgs&&... args) {
				BehaviourBinding::Bind<T>(myRegistry, e, std::forward<TArgs>(args)...);
			}
			template <typename T, typename = typename std::enable_if<std::is_base_of<IBehaviour, T>::value>::type>
			bool HasBehaviour(entt::entity e) {
				return BehaviourBinding::Has<T>(myRegistry, e);
			}
			template <typename T, typename = typename std::enable_if<std::is_base_of<IBehaviour, T>::value>::type>
			std::shared_ptr<T> GetBehaviour(entt::entity e) {
				return BehaviourBinding::Get<T>(myRegistry, e);
			}

			const std::string& GetName() const { return myName; }
			void SetName(const std::string& name) { myName = name; }

		protected:
			entt::registry myRegistry;
			std::string myName;
		};
		
	}
}