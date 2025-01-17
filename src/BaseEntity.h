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
#include "ngine.h"

#include "Vector2.h"
#include "EventArgs.h"
#include "EntityContainer.h"
#include "Scene.h"

namespace NerdThings::Ngine {
    class Component;

    /*
     * The root class for an entity within a scene
     */
    class NEAPI BaseEntity : public EntityContainer {
        // Private fields

        /*
         * Whether or not this entity can be culled
         */
        bool _CanCull = false;

        /*
         * The list of all components.
         * All components are given a name for easy identification.
         */
        std::map<std::string, Component*> _Components;

        /*
         * Depth layer
         */
        int _Depth;

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
         * Whether or not we update when paused
         */
        bool _PersistentUpdates = false;

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
         * Whether or not this entity is drawn with the camera
         */
        bool DrawWithCamera = true;

        /*
         * On draw event
         */
        EventHandler<EventArgs> OnDraw;

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
        BaseEntity(Scene *parentScene_, TVector2 position_, int depth_ = 0, bool canCull_ = false);

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
         * This is used to determine if this entity should be culled.
         * This can allow you to hide this if the collision box is not contained instead.
         */
        virtual bool CheckForCulling(TRectangle cullArea_);

        /*
         * Remove from our parent and delete ourselves.
         * NO MORE CALLS MUST BE MADE TO THE ENTITY AFTER THIS.
         */
        void Destroy();

        /*
         * Draw code for the entity
         */
        virtual void Draw();

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
         * Check if this entity can be culled
         */
        bool GetCanCull();

        /*
         * Get all components
         */
        std::vector<Component*> GetComponents();

        /*
         * Get the entity depth
         */
        int GetDepth() const;

        /*
         * Get entity origin
         */
        [[nodiscard]] TVector2 GetOrigin() const;

        /*
         * Get our parent container.
         * This will either be another entity or the scene
         */
        EntityContainer *GetParentContainer() const;

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
         * Test if we update when the scene is paused
         */
        bool GetDoPersistentUpdates();

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
         * Set whether or not this entity can be culled
         */
        void SetCanCull(bool canCull_);

        /*
         * Set the entity depth
         */
        void SetDepth(int depth_);

        /*
         * Set entity origin
         */
        void SetOrigin(TVector2 origin_);

        /*
         * Set whether or not we update when the scene is paused
         */
        void SetDoPersistentUpdates(bool persistentUpdates_);

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
         * Subscribe to update events in the scene
         */
        bool SubscribeToUpdate();

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
