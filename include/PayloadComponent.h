#pragma once
class Event;

class PayloadComponent
{
public:
    virtual ~PayloadComponent() = default;
    virtual const char* ToString() = 0;
};

class PayloadTest final : public PayloadComponent
{
    Event* m_pOwner = nullptr;
public:
    PayloadTest(Event* pOwner): m_pOwner(pOwner){}  // Add default constructor

    virtual const char* ToString() override;
    virtual ~PayloadTest() override = default;
};

inline const char* PayloadTest::ToString()
{
    return "Event Payload Test\n";
}

