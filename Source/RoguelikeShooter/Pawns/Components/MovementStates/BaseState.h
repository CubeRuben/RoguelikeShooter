#pragma once

#include "StateEnum.h"

class SBaseState 
{
public:

    SBaseState(class UPlayerMovementComponent* NewMovementComponent);

protected:

    class UPlayerMovementComponent* MovementComponent;

public:

    virtual EMovementState GetStateType() const = 0;

    virtual void OnStateEnter() {}
    virtual void OnStateExit() {}

    virtual void Tick(float DeltaTime) {}
};

class SBaseGroundedState : public SBaseState
{
public:

    SBaseGroundedState(class UPlayerMovementComponent* NewMovementComponent);

    void OnStateEnter() override;

    void Tick(float DeltaTime) override;

    virtual void UpdateInput() = 0;
    virtual void UpdateVelocity(float DeltaTime) = 0;
};


class SIdleState : public SBaseGroundedState
{
public:
   
    SIdleState(class UPlayerMovementComponent* NewMovementComponent);

    EMovementState GetStateType() const override { return EMovementState::Idle; }

    void UpdateInput() override;
    void UpdateVelocity(float DeltaTime) override;
};

class SWalkState : public SBaseGroundedState
{
public:

    SWalkState(class UPlayerMovementComponent* NewMovementComponent);

    EMovementState GetStateType() const override { return EMovementState::Walk; }

    void UpdateInput() override;
    void UpdateVelocity(float DeltaTime) override;

    virtual float GetTargetSpeed();
};

class SRunState : public SWalkState
{
public:

    SRunState(class UPlayerMovementComponent* NewMovementComponent);

    EMovementState GetStateType() const override { return EMovementState::Run; }

    void UpdateInput() override;

    virtual float GetTargetSpeed() override;
};

class SInAirState : public SBaseState
{
public:

    SInAirState(class UPlayerMovementComponent* NewMovementComponent);

protected:

public:

    EMovementState GetStateType() const override { return EMovementState::InAir; }

    void OnStateExit() override;

    void Tick(float DeltaTime) override;
};