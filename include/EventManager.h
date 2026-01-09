#pragma once
#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>

#include "Event.h"

class EventManager
{
public:
    // event handler functions, including a priority value
    using EventHandler = std::pair<int, std::function<void(const Event&)>>;

    // Comparison functor for event handler functions
    struct EventHandlerComparer
    {
        // takes two EventHandler objects and returns a bool
        bool operator()(const EventHandler& left, const EventHandler& right) const
        {
            // priority values of the handlers
            if (left.first != right.first)
            {
                // Handlers with higher priority values should come before handlers with lower priority values
                return left.first > right.first;
            }

            // If priority values are equal, use target_type() comparison
            return left.second.target_type().before(right.second.target_type());
            // how this works :
            // left.second.target_type().before(right.second.target_type()) is used to compare the std::type_info
            // objects of the stored event handlers based on their names.
            // This comparison determines the order in which the event handlers are stored in the eventHandlers
            // container, with handlers whose types have "lower" names (lexicographically earlier)
            // being stored first.
            // Source : https://en.cppreference.com/w/cpp/types/type_info
            //              &
            //          https://stackoverflow.com/questions/53467813/comparing-two-type-info-from-typeid-operator
            //
            //      For debugging down the line (lexicographically earlier)
            //      : simply means the name of the type comes first in alphabetical order
        }
    };

    // Comparison functor for events based on their enum value
    struct EventComparer
    {
        // Definition of operator() that takes two Event objects and returns a bool
        bool operator()(const Event& left, const Event& right) const
        {
            // Compare the priority levels of the events directly
            return left.GetPriorityLevel() < right.GetPriorityLevel();
        }
    };

private:
    // Map of event types to a set of event handlers, sorted using the custom comparator
    std::unordered_map<uint32_t, std::set<EventHandler, EventHandlerComparer>> m_handlers;

    // Priority queue of events waiting to be processed, sorted by their enum value
    std::priority_queue<Event, std::vector<Event>, EventComparer> m_eventQueue;

public:

    // Add a handler for an event by string
    void AddHandler(const char* eventTypeString, const EventHandler& handler)
    {
        AddHandler(Event::EventType::HashEventString(eventTypeString), handler);
    }

    // Remove a handler for an event by string
    void RemoveHandler(const char* eventTypeString, const EventHandler& handler)
    {
        RemoveHandler(Event::EventType::HashEventString(eventTypeString), handler);
    }

    // Add an event handler for a given event type with a specified priority
    void AddHandler(uint32_t eventHash, const EventHandler& handler)
    {
        // Add the handler to the set of handlers for the specified event type
        m_handlers[eventHash].insert(handler);
    }

    // Remove an event handler for a given event type
    void RemoveHandler(uint32_t eventHash, const EventHandler& handler)
    {
        // Find the set of handlers for the given event type
        if (const auto it = m_handlers.find(eventHash); it != m_handlers.end())
        {
            std::cout << "Removing Handler with priority level of: " << handler.first << "\n";

            auto& handlers = it->second;

            // Remove the specified handler from the set of handlers
            handlers.erase(handler);
        }
    }

    // Add an event to the event queue
    void PushEvent(const Event& event)
    {
        m_eventQueue.push(event);
    }

    // Process all events currently in the event queue
    void ProcessEvents()
    {
        while (!m_eventQueue.empty())
        {
            // Get the next event in the queue
            const Event& event = m_eventQueue.top();

            // Find the set of handlers for the event type of the current event
            if (auto it = m_handlers.find(event.GetType()); it != m_handlers.end())
            {
                // Call each event handler in the set of handlers for the current event type
                const auto& handlers = it->second;
                for (const auto& handler : handlers)
                {
                    handler.second(event);
                }
            }

            // Remove the processed event from the event queue
            m_eventQueue.pop();
        }
    }

};
