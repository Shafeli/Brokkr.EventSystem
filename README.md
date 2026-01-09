# Brokkr.EventSystem

A lightweight, modular C++ event system designed for flexibility, clarity, and priority-based event handling.  
Handlers and events are sorted independently, allowing precise control over execution order and data flow.

---

## 🚀 How to Use

### Adding a New Event

Adding a new event type is simple:

```cpp
Event PlayerDiedEvent(Event::EventType("PlayerDied", Event::kPriorityNormal));

// or with custom parameters
Event PlayerDiedEvent(Event::EventType("EventNameString", somePriorityLevel));
```

#### Notes
- **Event Name String:** Avoid spaces — use `HelloWorld` instead of `Hello World`.  
- **Priority Levels:** There are predefined constants (e.g., `kPriorityNormal`, `kPriorityMax`),  
  but you can use custom integers as well.

---

### Registering an Event Handler

Handlers are defined as:

```cpp
using EventHandler = std::pair<int, std::function<void(const Event&)>>;
```

To register a handler, you just need:
1. The event name or hash.
2. A priority level for sorting handlers.
3. A functor, lambda, or function pointer for the callback.

```cpp
eventManager.AddHandler("NewEvent", {
    somePriorityLevel,
    [](const Event& event) { /* handle event */ }
});
```

---

### Removing an Event Handler

Handlers **must** be removed manually when they are no longer needed.  
Registered handlers are **not consumed** during event processing.

```cpp
eventManager.RemoveHandler("NewEvent", Handler);
// or using hashed event name
eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerDied"), Handler);
```

You can pre-hash event names or store both the hash and handler for faster registration/removal.

---

### Event Payloads

Payloads act like lightweight components.  
Each payload:
- Requires an **owner pointer** to the event container.
- Is implicitly linked when added via `AddComponent()`.
- Should be **read-only** — preventing unintended state changes before other handlers process the event.

For example, in a collision payload, store **entity IDs** instead of pointers so other systems can safely resolve them.

---

### ⚙️ Priority Rules

- Handlers are sorted **per event** using a `std::set`.  
  → Higher priority values run first.  
- Event priority is **separate** from handler priority.  
  → Changing one doesn’t affect the other.

---

## 🧩 Example

Here’s a simple example showing event registration, processing, and cleanup:

```cpp
void OnPlayerMoved(const Event& event)
{
    std::cout << "PlayerMoved handled — Data payload: "
              << event.GetComponent<PayloadTest>()->ToString() << "\n";
}

void OnPlayerDied(const Event& event)
{
    std::cout << "PlayerDied handled\n";
}

void OnPlayerDiedPriority(const Event& event)
{
    std::cout << "PlayerDied handled with HIGH priority\n";
}

int main()
{
    EventManager eventManager;
    std::string player = "Player variable to capture";

    // Register handlers
    eventManager.AddHandler(Event::EventType::HashEventString("PlayerMoved"), {
        0, [player]([[maybe_unused]] const Event& event) { std::cout << player << "\n"; }
    });

    eventManager.AddHandler(Event::EventType::HashEventString("PlayerMoved"),
        { Event::kPriorityNormal, OnPlayerMoved });

    eventManager.AddHandler("PlayerDied", { Event::kPriorityNormal, OnPlayerDied });
    eventManager.AddHandler(Event::EventType::HashEventString("PlayerDied"),
        { Event::kPriorityMax, OnPlayerDiedPriority });

    // Create events
    const Event PlayerDiedEvent(Event::EventType("PlayerDied", Event::kPriorityNormal));
    Event playerMovedEvent(Event::EventType("PlayerMoved", Event::kPriorityMax));

    if (playerMovedEvent.AddComponent<PayloadTest>())
        std::cout << "Payload component added successfully\n";

    // Push events to queue
    eventManager.PushEvent(PlayerDiedEvent);
    eventManager.PushEvent(playerMovedEvent);

    // Process events
    eventManager.ProcessEvents();

    // Cleanup
    eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerMoved"), {
        0, [player]([[maybe_unused]] const Event& event) { std::cout << player << "\n"; }
    });

    eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerMoved"),
        { Event::kPriorityNormal, OnPlayerMoved });

    eventManager.RemoveHandler(Event::EventType::HashEventString("PlayerDied"),
        { Event::kPriorityNormal, OnPlayerDied });

    eventManager.RemoveHandler("PlayerDied",
        { Event::kPriorityMax, OnPlayerDiedPriority });

    return 0;
}
```

---

## 🧠 Key Takeaways

- Event handlers and event priorities are **completely independent**.  
- Events are **queued and processed**; handlers are **persistent** until removed.  
- Payloads behave like **components**, ensuring clean, safe data flow.  
- Handlers are sorted automatically — higher priority executes first.  

---

## 🔗 Repository

[**GitHub → Brokkr.EventSystem**](https://github.com/Shafeli/Brokkr.EventSystem)
