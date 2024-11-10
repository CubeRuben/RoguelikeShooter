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

class SIdleState : public SBaseState
{
public:
   
    SIdleState(class UPlayerMovementComponent* NewMovementComponent);

protected:

public:

    EMovementState GetStateType() const override { return EMovementState::Idle; }

    void OnStateEnter() override;

    void Tick(float DeltaTime) override;
};

class SWalkState : public SBaseState
{
public:

    SWalkState(class UPlayerMovementComponent* NewMovementComponent);

protected:

public:

    EMovementState GetStateType() const override { return EMovementState::Walk; }

    void Tick(float DeltaTime) override;
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