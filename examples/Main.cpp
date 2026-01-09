
#include <iostream>
#include <set>
#include "Event.h"
#include "EventManager.h"
#include "PayloadComponent.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              --------------------
//                                How to use system:
//                              --------------------
//
//                              *Adding new Event*
//-------------------------------------------------------------------------------------------------------------
// Add an event type is now pretty easy its just:
//      
//      Event PlayerDiedEvent(Event::EventType("PlayerDied", Event::kPriorityNormal));
//
//      Event PlayerDiedEvent(Event::EventType("Event Name String", some int level of priority));
//
//
//      Event Name String Note: Doesn't really matter other then Best to not Space words out.
//                          HelloWord = Good : Hello World = Bad
//
//      Priority level Note: there are pre-defined levels, but are not required for the system to work 
//
//-------------------------------------------------------------------------------------------------------------
//                              *Event Handler for an Event*
//-------------------------------------------------------------------------------------------------------------
// Type alias for event handler functions, including a priority value is
// " using EventHandler = std::pair<int, std::function<void(const Event&)>> "
//
// All you need to have is the hash or the const char* for the event, then some int level of priority pre pre-defined or not
// and last a functor or function pointer / lambda for a call back 
//
//
//     eventManager.AddHandler("New Event", { priority level num for handlers sorting ,
//     functor or function pointer / lambda for a call back });
//
//                              *Event Handler Removal*
//---------------------------------------------------------------------------------------------------------------
// Caller/ Event Handler/ adding party is responsible for removing handlers from system currently
// This also means once register Handlers are not consumed on Event processing **Very important to note** 
//
// At the end of use of the handler should be removed from the EventManager
//         eventManager.RemoveHandler("New Event", Handler);
//                              or
//         eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerDied"), Handler);
//
// removal can also be done with hashing the string before the removal or the caller can store the hash and
// the handler for fast adding and removal
//----------------------------------------------------------------------------------------------------------------
//
//                              *Event Payloads
//----------------------------------------------------------------------------------------------------------------
// Event payloads are component like system with an interface things to remember when creating custom payloads first
// needs an owner pointer to the event container holding it. In add component function this pointer is the event container and
// implicitly added to the payload component upon adding to the container.
//
// Payloads are meant to be read only so to not have a chance to change event data before another Handler/ listener can do its thing
// this means in a Collision payload the Entity Ids should be returned and not their pointers that way events can get the
// entity pointer from the Object pool with the ids
//
//                          --- Important to remember when using this system---
//      **** EventHandlers are sorted for every event std::set so the higher priority levels are processed first ****
//      **** Events are sorted separate the Handlers priority level has noting to do with the Event priority level  ****
//
//-------------------
// ++Simple Example++ 
//-------------------
//
//        Custom event handler function for PlayerMoved event
//      void OnPlayerMoved(const Event& event)
//      {
//          std::cout << "PlayerMoved event handled Data payload test : " << event.GetComponent<PayloadTest>()->ToString() << "\n";
//      }
//
//       Custom event handler function for PlayerDied event
//      void OnPlayerDied(const Event& event)
//      {
//          std::cout << "PlayerDied event handled  \n";
//      }
//
//       Custom event handler function for PlayerDied event
//      void OnPlayerDiedPriority(const Event& event)
//      {
//          std::cout << "PlayerDied event handled with the High priority for Player death events \n";
//      }
//
//      int main()
//      {
//          // Create an instance of EventManager
//          EventManager eventManager;
//      
//          std::string player = "Player variable to capture";
//      
//          // Register event handlers for events
//          eventManager.AddHandler(Event::EventType::HashEventString("PlayerMoved"), { 0, [player]([[maybe_unused]] const Event& event)
//              {
//                  std::cout << player << "\n";
//              } });
//      
//          eventManager.AddHandler(Event::EventType::HashEventString("PlayerMoved"), { Event::kPriorityNormal, OnPlayerMoved });
//          eventManager.AddHandler("PlayerDied", { Event::kPriorityNormal, OnPlayerDied });
//
//          // Higher priority value for death events
//          eventManager.AddHandler(Event::EventType::HashEventString("PlayerDied"), { Event::kPriorityMax, OnPlayerDiedPriority });  
//      
//          const Event PlayerDiedEvent(Event::EventType("PlayerDied", Event::kPriorityNormal));
//      
//          // Create some events
//          Event playerMovedEvent(Event::EventType("PlayerMoved", Event::kPriorityMax));
//          if (playerMovedEvent.AddComponent<PayloadTest>())
//          {
//              std::cout << "Success the Payload component was added to the event\n";
//          }
//      
//          // Push the events to the event queue
//          eventManager.PushEvent(PlayerDiedEvent);
//          eventManager.PushEvent(playerMovedEvent);
//      
//          // Process the events
//          eventManager.ProcessEvents();
//      
//          // Remove event handlers
//          eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerMoved"), { 0, [player]([[maybe_unused]] const Event& event)
//              {
//                  std::cout << player << "\n";
//              } });
//      
//          eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerMoved"), { Event::kPriorityNormal, OnPlayerMoved });
//          eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerDied"), { Event::kPriorityNormal, OnPlayerDied });
//          eventManager.RemoveHandler("PlayerDied", { Event::kPriorityMax, OnPlayerDiedPriority });
//      
//          return 0;
//      }
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Custom event handler function for PlayerMoved event
void OnPlayerMoved(const Event& event)
{
    std::cout << "PlayerMoved event handled Data payload test : " << event.GetComponent<PayloadTest>()->ToString() << "\n";
}

// Custom event handler function for PlayerDied event
void OnPlayerDied(const Event& event)
{
    std::cout << "PlayerDied event handled  \n";
}

// Custom event handler function for PlayerDied event
void OnPlayerDiedPriority(const Event& event)
{
    std::cout << "PlayerDied event handled with the High priority for Player death events \n";
}

int main()
{
    // Create an instance of EventManager
    EventManager eventManager;

    std::string player = "Player variable to capture";

    // Register event handlers for events
    eventManager.AddHandler(Event::EventType::HashEventString("PlayerMoved"), { 0, [player]([[maybe_unused]] const Event& event)
        {
            std::cout << player << "\n";
        } });

    eventManager.AddHandler(Event::EventType::HashEventString("PlayerMoved"), { Event::kPriorityNormal, OnPlayerMoved });
    eventManager.AddHandler("PlayerDied", { Event::kPriorityNormal, OnPlayerDied });
    eventManager.AddHandler(Event::EventType::HashEventString("PlayerDied"), { Event::kPriorityMax, OnPlayerDiedPriority });  // Higher priority value for death events

    const Event PlayerDiedEvent(Event::EventType("PlayerDied", Event::kPriorityNormal));
    
    // Create some events
    Event playerMovedEvent(Event::EventType("PlayerMoved", Event::kPriorityMax));
    if (playerMovedEvent.AddComponent<PayloadTest>())
    {
        std::cout << "Success the Payload component was added to the event\n";
    }

    // Push the events to the event queue
    eventManager.PushEvent(PlayerDiedEvent);
    eventManager.PushEvent(playerMovedEvent);

    // Process the events
    eventManager.ProcessEvents();

    // Remove event handlers
    eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerMoved"), { 0, [player]([[maybe_unused]] const Event& event)
        {
            std::cout << player << "\n";
        } });

    eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerMoved"), { Event::kPriorityNormal, OnPlayerMoved});
    eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerDied"), { Event::kPriorityNormal, OnPlayerDied });
    eventManager.RemoveHandler("PlayerDied", { Event::kPriorityMax, OnPlayerDiedPriority });

    return 0;
}