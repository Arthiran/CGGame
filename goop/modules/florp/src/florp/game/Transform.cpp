
#include "florp/game/Transform.h"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/quaternion.hpp>
#include <GLM/gtx/wrap.hpp>
#include <stdexcept>

#include "florp/game/SceneManager.h"
namespace florp
{
	namespace game
	{		
		Transform::~Transform() {
		}

		void Transform::SetParent(const entt::entity& parent) {
			myParent = parent;
			isLocalDirty = true;
		}

		void Transform::SetScale(const glm::vec3& scale /*= glm::vec3(1.0f)*/) {
			myScale = scale;
			isLocalDirty = true;
		}

		void Transform::LookAt(const glm::vec3& localPos, const glm::vec3& localUp) {
			myLocalRotation = glm::quatLookAt(glm::normalize(localPos - myLocalPosition), localUp);
			isLocalDirty = true;
		}

		void Transform::SetPosition(const glm::vec3& pos, bool local) {
			if (local) {
				myLocalPosition = pos;
				isLocalDirty = true;
			}
			else {
				throw std::logic_error("Not implemented");
			}
		}

		void Transform::SetEulerAngles(const glm::vec3& euler, bool local) {
			if (local) {
				myLocalRotation = glm::quat(glm::radians(euler));
				isLocalDirty = true;
			}
			else {
				throw std::logic_error("Not implemented");
			}
		}

		void Transform::SetRotation(const glm::quat& rot, bool local) {
			if (local) {
				myLocalRotation = rot;
				isLocalDirty = true;
			}
			else {
				throw std::logic_error("Not implemented");
			}
		}

		void Transform::Rotate(const glm::quat& rot) {
			myLocalRotation = rot * myLocalRotation;
			isLocalDirty = true;
		}
		void Transform::Rotate(const glm::vec3& euler) {
			glm::vec3 angleRad = glm::radians(euler);
			glm::quat rotation = glm::quat(angleRad);
			myLocalRotation = myLocalRotation * rotation;
			isLocalDirty = true;
		}

		const glm::mat4& Transform::GetLocalTransform() const {
			if (isLocalDirty) {
				myLocalTransform =
					glm::translate(glm::mat4(1.0f), myLocalPosition) *
					glm::scale(glm::mat4(1.0f), myScale) *
					glm::mat4_cast(myLocalRotation)
					;
				isLocalDirty = false;
			}
			return myLocalTransform;
		}

		const glm::mat4& Transform::GetWorldTransform() const {
			if (__IsChainDirty()) {
				if (myParent != entt::null) {
					Transform& parent = CurrentRegistry().get<Transform>(myParent);
					myWorldTransform = parent.GetWorldTransform() * GetLocalTransform();
				}
				else {
					myWorldTransform = GetLocalTransform();
				}
			}

			return myWorldTransform;
		}

		bool Transform::__IsChainDirty() const {
			return isLocalDirty || ((myParent != entt::null) ? CurrentRegistry().get<Transform>(myParent).__IsChainDirty() : false);
		}
	}
}