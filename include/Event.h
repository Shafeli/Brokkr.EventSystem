#pragma once
#include <memory>
#include <vector>

#include "Hash.h"
#include "PayloadComponent.h"



class Event
{
public:

    // Pre-defined priority levels
    inline static constexpr unsigned int kPriorityMin = 0;
    inline static constexpr unsigned int kPriorityLow = 100;
    inline static constexpr unsigned int kPriorityNormal = 500;
    inline static constexpr unsigned int kPriorityHigh = 900;
    inline static constexpr unsigned int kPriorityMax = 429496729;

    class EventType
    {
        inline static constexpr uint32_t s_kHashSeed = 1;

        unsigned int m_priority;
        const char* m_eventTypeName;
        uint32_t m_typeName;

    public:
        EventType(const char* eventTypeName, const unsigned int& priority)
            : m_priority(priority)
            , m_eventTypeName(eventTypeName)
        {
            m_typeName = Hash::Murmur3Hash(eventTypeName, std::strlen(eventTypeName), s_kHashSeed);
        }

        [[nodiscard]] unsigned int GetPriorityLevel() const { return m_priority; }
        [[nodiscard]] uint32_t GetTypeHash() const { return m_typeName; }
        [[nodiscard]] const char* ToString() const { return m_eventTypeName; }

        static uint32_t HashEventString(const char* eventTypeName)
        {
            return Hash::Murmur3Hash(eventTypeName, std::strlen(eventTypeName), s_kHashSeed);
        }
    };

private:

    EventType m_event;
    uint32_t m_type;

    // Components representing different aspects of the event data
    std::vector<std::shared_ptr<PayloadComponent>> m_pComponents;

public:
    Event(const EventType& type)
        :m_event(type)
        , m_type(type.GetTypeHash())
    {}

    Event(const Event& other) = default;
    Event(Event&& other) noexcept = default;
    Event& operator=(const Event& other) = default;
    Event& operator=(Event&& other) noexcept = default;


    ~Event() = default;

    // Get Event Type
    [[nodiscard]] uint32_t GetPriorityLevel() const { return m_event.GetPriorityLevel(); }
    [[nodiscard]] uint32_t GetType() const { return m_type; }
    [[nodiscard]] EventType GetEvent() const { return m_event; }

    // Add a component to the event
    template <typename ComponentType, typename ... Args>
    ComponentType* AddComponent(Args&&... args)
    {
        // Create a instance of the component type
        std::shared_ptr<ComponentType> newComponent = std::make_shared<ComponentType>(this, std::forward<Args>(args)...);

        // Get a raw pointer to the component
        ComponentType* result = newComponent.get();

        // Add the component to the vector
        m_pComponents.emplace_back(std::move(newComponent));

        // Return a pointer
        return result;
    }

    template <typename ComponentType>
    [[nodiscard]] ComponentType* GetComponent() const
    {
        // Iterate through all components in the vector
        for (auto& component : m_pComponents)
        {
            // Attempt to dynamically cast the component to the specified type
            ComponentType* target = dynamic_cast<ComponentType*>(component.get());

            // If the cast is successful
            if (target)
            {
                return target;
            }
        }

        // If no component of type is found
        return nullptr;
    }
};
