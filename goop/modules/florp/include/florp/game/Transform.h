#pragma once
#include <memory> // for shared_ptr
#include <GLM/glm.hpp> // for all the GLM types
#include <GLM/gtc/quaternion.hpp> // for the GLM quaternion stuff
#include "entt.hpp" // For the entt parenting stuff

namespace florp {
	namespace game {

		struct Transform {

			typedef std::shared_ptr<Transform> Sptr;

			Transform() :
				myWorldTransform(glm::mat4(1.0f)),
				myLocalTransform(glm::mat4(1.0f)),
				myScale(glm::vec3(1.0f)),
				myLocalPosition(glm::vec3()),
				myLocalRotation(glm::quat(1,0,0,0)),
				isDirty(false), isLocalDirty(false),
				myParent(entt::null) { }
			~Transform();

			void SetParent(const entt::entity& parent);
			const entt::entity& GetParent() { return myParent; }

			void SetScale(const glm::vec3& scale = glm::vec3(1.0f));
			void SetScale(float scale = 1.0f) { SetScale(glm::vec3(scale)); }
			const glm::vec3& GetScale() const { return myScale; }

			void LookAt(const glm::vec3& localPos, const glm::vec3& localUp);

			void SetPosition(const glm::vec3& pos, bool local = true);
			const glm::vec3& GetLocalPosition() const { return myLocalPosition; }

			/*
			 * Sets the Euler rotation for this transform, in degrees
			 * @param euler The rotation for this transformation, in degrees
			 * @param local True if the given rotation is in local space, false for world space
			 */
			void SetEulerAngles(const glm::vec3& euler, bool local = true);
			/*
			 * Sets our quaternion rotation for this transform
			 * @param rot The rotation to set the transform to
			 * @param local True if the rotation is local to this transform, false if it is in world space
			 */
			void SetRotation(const glm::quat& rot, bool local = true);
			/*
			 * Gets the local rotation of this transformation in degrees
			 */
			const glm::vec3& GetLocalEulerAngles() const { return glm::degrees(glm::eulerAngles(myLocalRotation)); }

			/*
			 * Rotates this transformation by the given quaternion
			 * @param rot The quaternion to rotate by
			 */
			void Rotate(const glm::quat& rot);
			/*
			 * Rotates this transformation by the given euler angles, in degrees
			 * @param rot The quaternion to rotate by
			 */
			void Rotate(const glm::vec3& euler);

			const glm::mat4& GetLocalTransform() const;
			const glm::mat4& GetWorldTransform() const;

		protected:
			mutable glm::mat4           myWorldTransform;
			mutable bool                isDirty;
			mutable bool                isLocalDirty;
			mutable glm::mat4           myLocalTransform;

			glm::vec3 myLocalPosition;
			glm::vec3 myScale;
			glm::quat myLocalRotation;

			bool __IsChainDirty() const;

			entt::entity                myParent;
		};
	}
}