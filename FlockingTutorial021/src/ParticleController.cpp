#include <cinder/Rand.h>
#include "ParticleController.h"

// nice little type definition to make for loop code mor readable
typedef list<Particle>::iterator PIt;

ParticleController::ParticleController() {
    mMaxDistance = 300.0f;
    mMaxDistanceSquared = mMaxDistance * mMaxDistance;
    mCenterPullStrength = 0.0001f;
}

void ParticleController::update(const bool &flattenTo2D) {
    for (PIt p = mParticles.begin(); p != mParticles.end(); ++p) {
        p->update(flattenTo2D);
    }
}

void ParticleController::draw(const bool &drawTails) {
    for (PIt p = mParticles.begin(); p != mParticles.end(); ++p) {
        p->draw();
    }

    gl::begin(GL_LINES);
    if (drawTails) {
        for (PIt p = mParticles.begin(); p != mParticles.end(); ++p) {
            p->drawTail();
        }
    }
    gl::end();
}

void ParticleController::addParticles(int amount) {
    int i = 0;
    while (i < amount) {
        vec3 l = Rand::randVec3() * 50.0f;
        vec3 v = Rand::randVec3() * 5.0f;
        mParticles.push_back(Particle(l, v));
        ++i;
    }
}

void ParticleController::applySeparation(float zoneRadius) {
    float zoneRadiusSquared = zoneRadius * zoneRadius;
    for (PIt p1 = mParticles.begin(); p1 != mParticles.end(); ++p1) {
        PIt p2 = p1;
        for (++p2; p2 != mParticles.end(); ++p2) {
            vec3 dir = p1->getPosition() - p2->getPosition();
            float distSqrd = glm::length2(dir);
            if (distSqrd <= zoneRadiusSquared) { // SEPARATION
                float F = (zoneRadiusSquared / distSqrd - 1.0f) * 0.01f;
                dir = glm::normalize(dir);
                dir *= F;
                p1->applyForce(dir); // add force
                p2->applyForce(dir * -1.0f); // substract force
            }
        }
    }
}

void ParticleController::applyCenterPull(const vec3 &center) {
    vec3 dir;
    float distToCenterSquared;
    for (PIt p = mParticles.begin(); p != mParticles.end(); ++p) {
        dir = p->getPosition() - center;
        distToCenterSquared = glm::length2(dir);
        if (distToCenterSquared > mMaxDistanceSquared) {
            dir = glm::normalize(dir);
            p->applyForce(dir * (distToCenterSquared - mMaxDistanceSquared) * mCenterPullStrength * -1.0f);
        }
    }
}















