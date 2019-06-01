/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*   
*   File reviewed on 01/06/2019 by R.M
*       Will need to review again once scaling is added
*       Entity parenting also needs reviewing to decide what we do about positioning and origins
*
**********************************************************************************************/

#ifndef BASEENTITY_H
#define BASEENTITY_H

// Include ngine
#include "../ngine.h"

#include "EntityContainer.h"
#include "Scene.h"

namespace NerdThings::Ngine::Core {
    class Component;

    /*
     * The root class for an entity within a scene
     */
    class NEAPI BaseEntity : public EntityContainer {
        // Private fields

        /*
         * The list of all components.
         * All components are given a name for easy identification.
         */
        std::map<std::string, Component*> _Components;

        /*
         * On draw event reference
         */
        EventHandleRef<EventArgs> _OnDrawRef;

        /*
         * On draw camera event reference
         */
        EventHandleRef<EventArgs> _OnDrawCameraRef;

        /*
         * On update event reference
         */
        EventHandleRef<EventArgs> _OnUpdateRef;

        /*
         * The entity origin
         */
        TVector2 _Origin;

        /*
         * The parent entity
         */
        BaseEntity *_ParentEntity = nullptr;

        /*
         * Parent Scene
         */
        Scene *_ParentScene = nullptr;

        /*
         * The entity position
         */
        TVector2 _Position = TVector2::Zero;

        /*
         * The entity rotation (in radians)
         */
        float _Rotation = 0;

        // TODO: Add logic and draw scaling
        // /*
        //  * The entity scale (Used for rendering and physics)
        //  */
        // float _Scale = 1;

        // Private Methods

        void RemoveEntityParent(BaseEntity *ent_) override;

        void SetEntityParent(BaseEntity *ent_) override;
    public:
        // Public Fields

        /*
         * On draw event
         */
        EventHandler<EventArgs> OnDraw;

        /*
         * On draw event with camera translation
         */
        EventHandler<EventArgs> OnDrawCamera;

        /*
         * On position changed event
         */
        EventHandler<EntityTransformChangedEventArgs> OnTransformChanged;

        /*
         * On update event
         */
        EventHandler<EventArgs> OnUpdate;

        // Public Constructor(s)

        /*
         * Create a BaseEntity.
         */
        BaseEntity(Scene *parentScene_, TVector2 position_);

        // Destructor

        virtual ~BaseEntity();

        // Public Methods

        /*
         * Add a component to the entity.
         * component_ must be derived from class Component.
         * Returns the component if successful and null if not
         */
        template <typename ComponentType>
        ComponentType *AddComponent(const std::string &name_, ComponentType *component_) {
            // Check the name is not taken
            if (HasComponent(name_))
                return nullptr;

            // Cast to component to ensure this is valid
            auto comp = dynamic_cast<Component*>(component_);

            if (comp != nullptr) {
                _Components.insert({name_, comp});
                return component_;
            }

            return nullptr;
        }

        /*
         * Draw code for the entity
         */
        virtual void Draw(EventArgs &e);

        /*
         * Draw code for the entity with camera translation
         */
        virtual void DrawCamera(EventArgs &e);

        /*
         * Get a component by name.
         */
        template <typename ComponentType>
        ComponentType *GetComponent(const std::string &name_) {
            // Try to find the component
            if (HasComponent(name_)) {
                return dynamic_cast<ComponentType*>(_Components.at(name_)); // Will return null if its the wrong type
            }

            return nullptr;
        }

        /*
         * Get all components
         */
        std::vector<Component*> GetComponents();

        /*
         * Get entity origin
         */
        [[nodiscard]] TVector2 GetOrigin() const;

        /*
         * Get the parent entity
         */
        [[nodiscard]] BaseEntity *GetParentEntity() const;

        /*
         * Get the parent entity as.
         * If there is no parent or the parent is not of this type it returns null
         */
        template <typename EntityType>
        [[nodiscard]] EntityType *GetParentEntityAs() const {
            return dynamic_cast<EntityType*>(_ParentEntity);
        }

        /*
         * Get the parent scene
         */
        [[nodiscard]] Scene *GetParentScene() const;

        /*
         * Get the entity position
         */
        [[nodiscard]] TVector2 GetPosition() const;

        /*
         * Get the entity rotation
         */
        [[nodiscard]] float GetRotation() const;

        // /*
        //  * Get the entity scale
        //  */
        // [[nodiscard]] float GetScale() const;

        /*
         * Test if a component exists by a name.
         */
        bool HasComponent(const std::string &name_);

        /*
         * Move an entity
         */
        void MoveBy(TVector2 moveBy_);

        /*
         * Removes the entity from the component.
         * Returns success or fail.
         */
        bool RemoveComponent(const std::string &name_);

        /*
         * Set entity origin
         */
        void SetOrigin(TVector2 origin_);

        /*
         * Set entity position
         */
        void SetPosition(TVector2 position_);

        /*
         * Set entity rotation
         */
        void SetRotation(float rotation_);

        // /*
        //  * Set entity scale
        //  */
        // void SetScale(float scale_);

        /*
         * Subscribe to draw events in the scene with camera translation
         */
        bool SubscribeToCameraDraw();

        /*
         * Subscribe to draw events in the scene
         */
        bool SubscribeToDraw();

        /*
         * Subscribe to update events in the scene
         */
        bool SubscribeToUpdate();

        /*
         * Unsubscribe from update events in the scene with camera translation
         */
        void UnsubscribeFromCameraDraw();

        /*
         * Unsubscribe from update events in the scene
         */
        void UnsubscribeFromDraw();

        /*
         * Unsubscribe from update events in the scene
         */
        void UnsubscribeFromUpdate();

        /*
         * Update the entity
         */
        virtual void Update(EventArgs &e);
    };
}

#endif //BASEENTITY_H
