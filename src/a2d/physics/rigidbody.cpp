//
// Created by selya on 28.01.2019.
//

#include <a2d/physics/rigidbody.hpp>
#include <a2d/physics/physics_collider.hpp>


namespace a2d {

Rigidbody::BodyType Rigidbody::GetType() {
    return (BodyType)body->GetType();
}

float Rigidbody::GetMass() {
    b2MassData mass_data;
    body->GetMassData(&mass_data);
    return mass_data.mass;
}

float Rigidbody::GetInertia() {
    b2MassData mass_data;
    body->GetMassData(&mass_data);
    return mass_data.I;
}

Vector2f Rigidbody::GetCenterOfMass() {
    return Vector2f(body->GetLocalCenter().x, body->GetLocalCenter().y) *= Physics::world_scale_inverted;
}

float Rigidbody::GetGravityScale() {
    return body->GetGravityScale();
}

Vector2f Rigidbody::GetLinearVelocity() {
    return Vector2f(body->GetLinearVelocity().x, body->GetLinearVelocity().y) *= Physics::world_scale_inverted;
}

float Rigidbody::GetAngularVelocity() {
    return body->GetAngularVelocity();
}

float Rigidbody::GetLinearDamping() {
    return body->GetLinearDamping();
}

float Rigidbody::GetAngularDamping() {
    return body->GetAngularDamping();
}

bool Rigidbody::IsFixedRotation() {
    return body->IsFixedRotation();
}

bool Rigidbody::IsActive() {
    return body->IsActive();
}

bool Rigidbody::IsAwake() {
    return body->IsAwake();
}

bool Rigidbody::IsBullet() {
    return body->IsBullet();
}

bool Rigidbody::IsSleepingAllowed() {
    return body->IsSleepingAllowed();
}

void Rigidbody::SetType(a2d::Rigidbody::BodyType body_type) {
    body->SetType((b2BodyType) body_type);
}

void Rigidbody::SetMass(float mass) {
    b2MassData mass_data;
    body->GetMassData(&mass_data);
    mass_data.mass = mass;
    body->SetMassData(&mass_data);
}

void Rigidbody::SetInertia(float inertia) {
    b2MassData mass_data;
    body->GetMassData(&mass_data);
    mass_data.I = inertia;
    body->SetMassData(&mass_data);
}

void Rigidbody::SetCenterOfMass(const Vector2f &center_of_mass) {
    b2MassData mass_data;
    body->GetMassData(&mass_data);
    mass_data.center.Set(center_of_mass.x * Physics::world_scale, center_of_mass.y * Physics::world_scale);
    body->SetMassData(&mass_data);
}

void Rigidbody::SetGravityScale(float gravity_scale) {
    body->SetGravityScale(gravity_scale);
}

void Rigidbody::SetLinearVelocity(const Vector2f &velocity) {
    body->SetLinearVelocity(b2Vec2(velocity.x * Physics::world_scale, velocity.y * Physics::world_scale));
}

void Rigidbody::SetAngularVelocity(float velocity) {
    body->SetAngularVelocity(velocity);
}

void Rigidbody::SetLinearDamping(float damping) {
    body->SetLinearDamping(damping);
}

void Rigidbody::SetAngularDamping(float damping) {
    body->SetAngularDamping(damping);
}

void Rigidbody::SetFixedRotation(bool rotation_fixed) {
    body->SetFixedRotation(rotation_fixed);
}

void Rigidbody::SetActive(bool active) {
    body->SetActive(active);
}

void Rigidbody::SetAwake(bool awake) {
    body->SetAwake(awake);
}

void Rigidbody::SetBullet(bool bullet) {
    body->SetBullet(bullet);
}

void Rigidbody::SetSleepingAllowed(bool sleeping_allowed) {
    body->SetSleepingAllowed(sleeping_allowed);
}

void Rigidbody::ApplyForce(const Vector2f &force, const Vector2f &point, bool wake) {
    body->ApplyForce(b2Vec2(force.x * Physics::world_scale, force.y * Physics::world_scale),
            b2Vec2(point.x * Physics::world_scale, point.y * Physics::world_scale), wake);
}

void Rigidbody::ApplyForceToCenter(const Vector2f &force, bool wake) {
    body->ApplyForceToCenter(b2Vec2(force.x * Physics::world_scale, force.y * Physics::world_scale), wake);
}

void Rigidbody::ApplyLinearImpulse(const Vector2f &impulse, const Vector2f &point, bool wake) {
    body->ApplyLinearImpulse(b2Vec2(impulse.x * Physics::world_scale, impulse.y * Physics::world_scale),
            b2Vec2(point.x * Physics::world_scale, point.y * Physics::world_scale), wake);
}

void Rigidbody::ApplyLinearImpulseToCenter(const Vector2f &impulse, bool wake) {
    body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x * Physics::world_scale, impulse.y * Physics::world_scale), wake);
}

void Rigidbody::ApplyAngularImpulse(float impulse, bool wake) {
    body->ApplyAngularImpulse(impulse, wake);
}

void Rigidbody::ApplyTorque(float torque, bool wake) {
    body->ApplyTorque(torque, wake);
}

void Rigidbody::Initialize() {
    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;
    body_def.position.Set(GetObject2D()->GetPosition().x * Physics::world_scale,
            GetObject2D()->GetPosition().y * Physics::world_scale);
    body_def.angle = GetObject2D()->GetRotation();
    body = Physics::GetWorld().CreateBody(&body_def);
    body->SetUserData(this);
}

void Rigidbody::OnDestroy() {
    auto f = body->GetFixtureList();
    while (f) {
        auto next = f->GetNext();
        ((PhysicsCollider *)f->GetUserData())->DetachFromRigidbody();
        f = next;
    }
    Physics::GetWorld().DestroyBody(body);
}

void Rigidbody::Transform(const Vector2f &position, float rotation) {
    if (internal_transform) return;
    body->SetTransform(b2Vec2(position.x * Physics::world_scale, position.y * Physics::world_scale), rotation);
}

void Rigidbody::PhysicsStep() {
    internal_transform = true;
    GetObject2D()->SetPosition(body->GetPosition().x * Physics::world_scale_inverted,
            body->GetPosition().y * Physics::world_scale_inverted);
    GetObject2D()->SetRotation(body->GetAngle());
    internal_transform = false;
}

}
