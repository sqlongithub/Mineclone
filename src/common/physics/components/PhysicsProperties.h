#pragma once

namespace Mineclone {

    struct PhysicsProperties {
        // --------------------------
        // Horizontal movement
        // --------------------------
        float walkSpeed = 4.317f;          // Normal walking speed (blocks/sec)
        float sprintScale = 1.3f;          // Sprint multiplier
        float flySpeed = 10.92f;           // Flying speed (blocks/sec)
        float groundAcceleration = 100.0f; // Ground horizontal acceleration (blocks/sec²)
        float airAcceleration = 20.0f;     // Air horizontal acceleration (blocks/sec²)

        // --------------------------
        // Vertical movement
        // --------------------------
        float jumpVelocity = 8.95f;         // Upward velocity when jumping (blocks/sec)
        float gravity = -32.0f;            // Gravity acceleration (blocks/sec²)
        float terminalVelocity = -78.4f;   // Maximum falling speed (blocks/sec)

        // --------------------------
        // Collision
        // --------------------------
        float stepHeight = 0.6f;           // Maximum step-up height (blocks)

        float groundDragRate = 1.0f;      // Ground friction (~0.546 per tick)
        float airDragRate = 1.9f;          // Air resistance (~0.91 per tick)
        float verticalDragRate = 0.4f;     // Vertical drag (~0.98 per tick)
    };

}